#include <stdlib.h>
#include <stdio.h>

#define DAQ_START 0x280
#define CLOCK_OUTPUT_PORT DAQ_START+7
#define DATA_OUTPUT_PORT DAQ_START+8
#define ACK_PORT DAQ_START+8
#define PORT_LENGTH 1
#define LOW 0
#define HIGH 1
#define CLOCK_PER_MICROS 20

static uintptr_t clock_handle;
static uintptr_t data_handle;
static uintptr_t ack_handle;
int ackReceived = 0;

//interrupts
Interrupt clock_interrupt;

//Threads
pthread_t ClockDataThread;
pthread_t InputThread;
pthread_t AckThread;

int main(int argc, char *argv[]) {
	//init
	CreateThreads();
	CreateInterrupt(&clock_interrupt, CLOCK_PER_MICROS, 0);
	startInterrupt(&clock_interrupt);

	while(true)
	{
		theInput = input();
		output(theInput);
		ack();
	}
	return EXIT_SUCCESS;
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

	pthread_create( &ClockDataThread, &threadAttributes, (void *)ClockDataOutput, &servos[0]);
	pthread_create( &InputThread, &threadAttributes, (void *)Input, &servos[1]);
	pthread_create( &AckThread, &threadAttributes, (void *)Ack, (void *)1 );
}

float Input()
{

}

void ClockDataOutput()
{
	struct _pulse pulse;
	int dataOutputIndex = 0;
	clock_handle = mmap_device_io(PORT_LENGTH, CLOCK_OUTPUT_PORT);
	data_handle = mmap_device_io(PORT_LENGTH, DATA_OUTPUT_PORT);


	while(true)
	{
		MsgReceive(clock_interrupt->chid, &pulse, sizeof(pulse), NULL);
		out8( clock_handle, HIGH );
		usleep(CLOCK_PER_MICROS/2);
		out8( clock_handle, LOW );
	}
}

void Ack()
{
	ack_handle = mmap_device_io(PORT_LENGTH, ACK_PORT);

	while(true)
	{
		// Listen for a HIGH event
		while ((in8(echo_handle) & 1) == LOW){}

		// Listen for a HIGH event
		while ((in8(echo_handle) & 1) == HIGH){}

		ackReceived = true;
	}
}
