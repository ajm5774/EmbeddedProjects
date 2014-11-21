/******************************************************************************
 * Project 2b
 *
 * Description:
 *
 *
 *
 * Author:
 *  Andrew Mueller (9/08/2014)
 *  Matt Warner (09/08/2014)
 *
 *****************************************************************************/
// system includes
#include <stdio.h>      /* Standard I/O Library */
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/mman.h>     /* for mmap_device_io() */
#include <stdint.h>       /* for uintptr_t */
#include <hw/inout.h>
#include <unistd.h>
#include "timer.h"
#include "engine.h"
//-----------------------------------------------------------------------------

// Definitions
#define PERIODMILLIS  ((int)20)
#define PWMPRCLKVALUE ((int)4)
#define PWMSCLAVALUE  ((int)5)
#define DATA_ADDRESS 0x378
#define CTRL_ADDRESS 0x37a
#define PORT_LENGTH 1
#define LOW 0x00
#define HIGH 0xFF
#define INIT_BIT 0x04

//Threads
pthread_t * IOThread;
pthread_t * ServoExecThread;
pthread_t * Servo1PWMThread;
pthread_t * Servo2PWMThread;

//Instructions/recipes
Instruction recipe1[] = {
  INSTRUCTION(MOV, 0),
  INSTRUCTION(WAIT, 0),
  INSTRUCTION(MOV, 1),
  INSTRUCTION(MOV, 2),
  INSTRUCTION(LOOP_START, 3),
  INSTRUCTION(MOV, 3),
  INSTRUCTION(LOOP_START, 3),
  INSTRUCTION(MOV, 4),
  INSTRUCTION(MOV, 5),
  INSTRUCTION(END_LOOP, 0),
  INSTRUCTION(MOV, 3),
  INSTRUCTION(RECIPE_END,0)
};

Instruction recipe2[] = {
  INSTRUCTION(MOV, 0),
  INSTRUCTION(WAIT, 2),
  INSTRUCTION(MOV, 16),
  INSTRUCTION(MOV, 0),
  INSTRUCTION(MOV, 3),
  INSTRUCTION(LOOP_START, 3),
  INSTRUCTION(MOV, 1),
  INSTRUCTION(MOV, 4),
  INSTRUCTION(END_LOOP, 0),
  INSTRUCTION(MOV, 0),
  INSTRUCTION(RECIPE_END,0)
};

//servos
Servo * servos[] = {
	{1,3,0,0,0,0,INIT, recipe1},
	{2,3,0,0,0,0,INIT, recipe2}
};

//interrupts
Interrupt * instructionInterrupt;
Interrupt * pwmInterrupt;

//Engine
Engine engine = {2, &servos};//2 servos

void UserIO(int threadID)
{

}

void ExecuteInstructions(Interrupt * interrupt)
{
	struct _pulse pulse;

	while(1)
	{
		MsgReceive(interrupt->chid, &pulse, sizeof(pulse), NULL);
		tick(engine);
	}
}

void pwmThread(Interrupt * interrupt, Servo * servo)
{
	struct _pulse pulse;
	uintptr_t ctrl_handle;
	uintptr_t data_handle;
	int positionToMicroSecHigh[] = {5, 9, 13, 17, 21, 25};


	/* Get a handle to the parallel port's Control register */
	ctrl_handle = mmap_device_io( PORT_LENGTH, CTRL_ADDRESS );

	/* Initialize the parallel port */
	out8( ctrl_handle, INIT_BIT );

	/* Get a handle to the parallel port's Data register */
	data_handle = mmap_device_io( PORT_LENGTH, DATA_ADDRESS );

	while(1)
	{
		MsgReceive(interrupt->chid, &pulse, sizeof(pulse), NULL);
		out8( data_handle, HIGH );
		usleep(positionToMicroSecHigh[servo->position]);
		out8( data_handle, LOW );
	}
}

void CreateThreads()
{
	pthread_attr_t threadAttributes ;
	int policy ;
	struct sched_param parameters ;

	pthread_attr_init(&threadAttributes) ;		// initialize thread attributes structure -- must do before any other activity on this struct
	pthread_getschedparam(pthread_self(), &policy, &parameters) ;	// get this main thread's scheduler parameters
	printf( "Main Teller Server thread running at priority %d\n", parameters.sched_priority) ;
	parameters.sched_priority-- ;									// lower the priority
	pthread_attr_setschedparam(&threadAttributes, &parameters) ;	// set up the pthread_attr struct with the updated priority


	pthread_create( IOThread, &threadAttributes, (void *)UserIO, 1 );
	pthread_create( ServoExecThread, &threadAttributes, (void *)ExecuteInstructions, instructionInterrupt );
	pthread_create( Servo1PWMThread, &threadAttributes, (void *)pwmThread, pwmInterrupt);
	pthread_create( Servo2PWMThread, &threadAttributes, (void *)pwmThread, pwmInterrupt);
}

void main(void)
{
	//create interrupts
	instructionInterrupt = CreateInterrupt(1000000);//1sec
	pwmInterrupt = CreateInterrupt(PERIODMILLIS * 1000);//20ms

    //Create Threads
    CreateThreads();

    //start the interrupts
    //startInterrupt(instructionInterrupt);
    startInterrupt(pwmInterrupt);
}


