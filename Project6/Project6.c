#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/mman.h>     /* for mmap_device_io() */
#include <stdint.h>       /* for uintptr_t */
#include <hw/inout.h>
#include <unistd.h>
#include "timer.h"
#include "adc.h"

//defines
#define DAQ_START 0x280
#define DATA_OUTPUT_PORT DAQ_START+8
#define DATA_INPUT_PORT DAQ_START+9
#define DATA_CTRL_PORT DAQ_START+11
#define NUM_DATA_LINES 5
#define PORT_LENGTH 1
#define LOW 0
#define HIGH 1
#define CLOCK_PER_MICROS 2000
#define INPUT_PER_MICROS 20000//as to be AT LEAST 16 times slower than clock
#define INPUT_LENGTH 16


//source variables
static uintptr_t input_handle;
static uintptr_t output_handle;
static uintptr_t io_ctrl_handle;

int ackReceived = 0;
int inputVoltage = 0;

//interrupts
Interrupt clock_interrupt;
Interrupt input_interrupt;

//Threads
pthread_t ClockDataThread;
pthread_t InputThread;
pthread_t AckThread;


void Input(int threadID)
{
	struct _pulse pulse;

	initADC();

	while(1)
	{
		MsgReceive(input_interrupt.chid, &pulse, sizeof(pulse), NULL);

		//read ADC voltage input
		startADC();
		if(checkStatus() == 0)
			inputVoltage = convertData(readData());
		else
			inputVoltage = 0;//=(

		//printf("%d\n", inputVoltage);
	}
}




int calcParity(int output, int length)
{
	int i;
	int parityCalc = 0;

	for(i = 0; i < length; i++)
	{
		if(output >> i & 1)
			parityCalc++;
	}

	if(parityCalc % 2 == 0)
		return 0;
	else
		return 1;
}

//lines 0-4 = data
//line 5 = parity
//line 6 = Clock
//line 7 = Ack
void ClockDataOutput(int threadID)
{
	struct _pulse pulse;
	uint8_t output = 0;

	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	output_handle = mmap_device_io(PORT_LENGTH, DATA_OUTPUT_PORT);

	while(1)
	{
		MsgReceive(clock_interrupt.chid, &pulse, sizeof(pulse), NULL);

		if(ackReceived)
		{
			output = (HIGH << (NUM_DATA_LINES + 1) )
					|(calcParity(inputVoltage, NUM_DATA_LINES) << NUM_DATA_LINES)
					|(inputVoltage & 0x1f);
			out8( output_handle, output);
			ackReceived = 0;
		}
		else
		{
			output = (HIGH << (NUM_DATA_LINES + 1));
			out8( output_handle, output);
		}

		MsgReceive(clock_interrupt.chid, &pulse, sizeof(pulse), NULL);

		output = output & 0xbf;
		out8( output_handle, output );
	}
}

void Ack(int threadID)
{
	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	while(1)
	{
		// Listen for a HIGH event
		while ((in8(input_handle) & 0x80) == LOW){}

		// Listen for a LOW event
		while ((in8(input_handle) & 0x80)){}

		ackReceived = 1;
	}
}

void CreateThreads()
{
	pthread_attr_t threadAttributes ;
	int policy ;
	struct sched_param parameters ;

	pthread_attr_init(&threadAttributes) ;		// initialize thread attributes structure -- must do before any other activity on this struct
	pthread_getschedparam(pthread_self(), &policy, &parameters) ;	// get this main thread's scheduler parameters
	parameters.sched_priority-- ;// lower the priority
	pthread_attr_setschedparam(&threadAttributes, &parameters) ;	// set up the pthread_attr struct with the updated priority

	pthread_create( &ClockDataThread, &threadAttributes, (void *)ClockDataOutput, (void *)0);
	pthread_create( &InputThread, &threadAttributes, (void *)Input, (void *)1);
	//pthread_create( &AckThread, &threadAttributes, (void *)Ack, (void *)2);
}

void initIO() {
	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	io_ctrl_handle = mmap_device_io(PORT_LENGTH, DATA_CTRL_PORT);

	out8( io_ctrl_handle, 0x00);
}

int main(int argc, char *argv[])
{
	//init
	printf("Starting...\n");
	initIO();

	CreateInterrupt(&clock_interrupt, CLOCK_PER_MICROS, 0);
	CreateInterrupt(&input_interrupt, INPUT_PER_MICROS, 0);

	CreateThreads();

	startInterrupt(&clock_interrupt);
	startInterrupt(&input_interrupt);

	pthread_join( ClockDataThread, 0 );
	pthread_join( InputThread, 0 );
	pthread_join( AckThread, 0 );

	printf("done!\n");
	return 0;
}
