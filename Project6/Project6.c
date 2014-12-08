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
#define CLOCK_OUTPUT_PORT DAQ_START+8
#define ACK_PORT DAQ_START+9
#define DATA_OUTPUT_PORT_START DAQ_START+10
#define NUM_DATA_LINES 5
#define PORT_LENGTH 1
#define LOW 0
#define HIGH 1
#define CLOCK_PER_MICROS 20
#define INPUT_PER_MICROS 1000//as to be AT LEAST 16 times slower than clock
#define INPUT_LENGTH 16
#define VFS 5

//source variables
static uintptr_t clock_handle;
static uintptr_t data_handles[NUM_DATA_LINES];
static uintptr_t ack_handle;
int ackReceived = 0;
int inputVoltage = 0;
int dataBitIndex = 0;

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
			inputVoltage = convertData(readData, VFS);
		else
			inputVoltage = 0;//=(

		printf("%d\n", inputVoltage);
	}
}

void Output(int output, int length, uintptr_t handles[])
{
	int i, k;
	int parityCalc;

	//output data
	for(i = 0; i < length; i++)
	{
		if(i == (length - 1))
		{
			if(parityCalc % 2)
				out8( handles[i], HIGH );//odd
			else
				out8( handles[i], LOW );//even
		}
		else if(i == (length - 2))//if it is the last data bit, we want to send the sign
		{
			if(output >= 0)
				out8( handles[i], LOW );
			else
				out8( handles[i], HIGH );// negative values output positive

		}
		else if(output >> i & 1)
		{
			out8( handles[i], HIGH );
			parityCalc++;
		}
		else
			out8( handles[i], LOW );
	}
}

void ClockDataOutput(int threadID)
{
	struct _pulse pulse;
	int i;

	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	clock_handle = mmap_device_io(PORT_LENGTH, CLOCK_OUTPUT_PORT);
	for(i = 0; i < NUM_DATA_LINES; i++)
	{
		data_handles[i] = mmap_device_io(PORT_LENGTH, DATA_OUTPUT_PORT_START + i);
	}

	while(1)
	{
		MsgReceive(clock_interrupt.chid, &pulse, sizeof(pulse), NULL);
		ackReceived = 0;
		out8( clock_handle, HIGH );

		Output(inputVoltage, NUM_DATA_LINES, data_handles);

		usleep(CLOCK_PER_MICROS/2);//output high for half the clock period
		out8( clock_handle, LOW );
	}
}

void Ack(int threadID)
{
	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	ack_handle = mmap_device_io(PORT_LENGTH, ACK_PORT);

	while(1)
	{
		// Listen for a HIGH event
		while ((in8(ack_handle) & 1) == LOW){}

		// Listen for a HIGH event
		while ((in8(ack_handle) & 1) == HIGH){}

		ackReceived = 1;
		if(dataBitIndex < (INPUT_LENGTH - 1))
			dataBitIndex++;
		else
			printf("Too many data bits were sent!");
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
	pthread_create( &AckThread, &threadAttributes, (void *)Ack, (void *)2);
}

int main(int argc, char *argv[]) {
	//init
	CreateInterrupt(&clock_interrupt, CLOCK_PER_MICROS, 0);
	CreateInterrupt(&input_interrupt, INPUT_PER_MICROS, 0);

	CreateThreads();

	startInterrupt(&clock_interrupt);
	startInterrupt(&input_interrupt);

	pthread_join( ClockDataThread, 0 );
	pthread_join( InputThread, 0 );
	pthread_join( AckThread, 0 );

	return 0;
}
