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
#define PERIODMICROS  ((int)20000)
#define PWMPRCLKVALUE ((int)4)
#define PWMSCLAVALUE  ((int)5)
#define PORT_LENGTH 1
#define LOW 0x00
#define HIGH 0xFF
#define INIT_BIT 0x00

#define BASE_ADDR 0x280
#define DAQ_A BASE_ADDR+8
#define DAQ_B BASE_ADDR+9
#define DAQ_CTRL BASE_ADDR+11

//Threads
pthread_t IOThread;
pthread_t ServoExecThread;
pthread_t Servo1PWMThread;
pthread_t Servo2PWMThread;

//Instructions/recipes
Instruction recipe1[] = {
  INSTRUCTION(MOV, 0),
  INSTRUCTION(WAIT, 0),
  INSTRUCTION(MOV, 1),
  INSTRUCTION(MOV, 2),
  INSTRUCTION(LOOP_START, 3),
  INSTRUCTION(MOV, 3),
  INSTRUCTION(MOV, 4),
  INSTRUCTION(MOV, 5),
  INSTRUCTION(END_LOOP, 0),
  INSTRUCTION(MOV, 3),
  INSTRUCTION(RECIPE_END,0)
};

Instruction recipe2[] = {
  INSTRUCTION(MOV, 0),
  INSTRUCTION(WAIT, 2),
  INSTRUCTION(MOV, 3),
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
Servo servos[] = {
	{1,3,0,0,0,0,INIT, recipe1},
	{2,3,0,0,0,0,INIT, recipe2}
};

//interrupts
Interrupt instructionInterrupt;
Interrupt pwm1Interrupt;
Interrupt pwm2Interrupt;

//Engine
Engine engine = {2, servos};//2 servos

//Data Output Handles
uintptr_t ctrl_handle;
uintptr_t data_handle_servo1;
uintptr_t data_handle_servo2;

void InitCtrl()
{
	/* Get a handle to the parallel port's Control register */
	ctrl_handle = mmap_device_io( PORT_LENGTH, DAQ_CTRL );

	/* Initialize the parallel port */
	out8( ctrl_handle, INIT_BIT );
}

void UserIO(int threadID)
{
	char inputs [2] = {0,0};
	char anInput;
	int index = 0;

	printf("\n>");

	while(1)
	{
		anInput = getchar();

		if(anInput == 'x' || anInput == 'X')
		{
			printf("\n>");
			index = 0;
		}
		else if(anInput == 13 && index >= 2)
		{
			sendCommand(&servos[0], inputs[0]);
			sendCommand(&servos[1], inputs[1]);
			index = 0;
			printf("\n>");
		}
		else if(anInput == 13)
		{
			printf("\n>");
			index = 0;
		}
		else if((anInput >= 97 && anInput <= 122) ||
				(anInput >= 65 && anInput <= 90))
		{
			if(index < 2)
				inputs[index++] = anInput;
		}
	}
}

void ExecuteInstructions(int threadID)
{
	struct _pulse pulse;
	Interrupt * interrupt = &instructionInterrupt;

	while(1)
	{
		MsgReceive(interrupt->chid, &pulse, sizeof(pulse), NULL);
		tick(engine);
	}
}

void pwmThread(Servo * servo)
{
	struct _pulse pulse;
	Interrupt * pwmInterrupt = (servo->id == 1)? &pwm1Interrupt : &pwm2Interrupt;

	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	uintptr_t data_handle = (servo->id == 1)? mmap_device_io( PORT_LENGTH, DAQ_A ) : mmap_device_io( PORT_LENGTH, DAQ_B );

	int positionToMicroSecHigh[] = {500, 760, 1020, 1280, 1540, 1800};

	while(1)
	{
		MsgReceive(pwmInterrupt->chid, &pulse, sizeof(pulse), NULL);
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
	parameters.sched_priority-- ;									// lower the priority
	pthread_attr_setschedparam(&threadAttributes, &parameters) ;	// set up the pthread_attr struct with the updated priority


	pthread_create( &IOThread, &threadAttributes, (void *)UserIO, (void *)0 );
	pthread_create( &ServoExecThread, &threadAttributes, (void *)ExecuteInstructions, (void *)1 );
	pthread_create( &Servo1PWMThread, &threadAttributes, (void *)pwmThread, &servos[0]);
	pthread_create( &Servo2PWMThread, &threadAttributes, (void *)pwmThread, &servos[1]);
}

int main(void)
{
	//create interrupts
	CreateInterrupt(&pwm1Interrupt, PERIODMICROS, 0);//20ms
	CreateInterrupt(&pwm2Interrupt, PERIODMICROS, 0);//20ms
	CreateInterrupt(&instructionInterrupt, 0, 1);//1sec

	//printf("chid = %d, period = %d\n", instructionInterrupt.chid, instructionInterrupt.period);
	//printf("chid = %d, period = %d\n", pwm1Interrupt.chid, pwm1Interrupt.period);
	//printf("chid = %d, period = %d\n", pwm2Interrupt.chid, pwm2Interrupt.period);

    //Create Threads
    CreateThreads();

    //Init CTRL Register
    InitCtrl();

    //start the interrupts
    startInterrupt(&instructionInterrupt);
    startInterrupt(&pwm1Interrupt);
    startInterrupt(&pwm2Interrupt);

    pthread_exit(0);
}


