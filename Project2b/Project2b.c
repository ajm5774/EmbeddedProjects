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

#include "engine.h"
//-----------------------------------------------------------------------------

// Definitions
#define PERIODMILLIS  ((UINT16)20)
#define PWMPRCLKVALUE ((UINT16)4)
#define PWMSCLAVALUE  ((UINT16)5)
#define DATA_ADDRESS 0x378
#define CTRL_ADDRESS 0x37a
#define PORT_LENGTH 1
#define LOW 0x00
#define HIGH 0xFF

//Threads
pthread_t * IOThread;
pthread_t * ServoExecThread;
pthread_t * Servo1PWMThread;
pthread_t * Servo2PWMthread;

//servos
Servo servos[2];

//interrupts
Interrupt * instructionInterrupt;
Interrupt * pwmInterrupt;

/*INT32 clkafreq;
int flag = 0;
UINT8 userInput1 = 0;
UINT8 userInput2 = 0;
int flagHasUserInput = 0;
char newLine = '\n';
int userInputIndex = 0;


void ConfigurePer(UINT16 periodMillis, Servo *servo )
{
   * servo->pwmPer = clkafreq *  periodMillis / 1000;
}
*/

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

void UserIO(int threadID)
{

}

void ExecuteInstructions(Interrupt * interrupt)
{
	struct _pulse pulse;

	while(1)
	{
		rcvid = MsgReceive(interrupt->chid, &pulse, sizeof(pulse), NULL);
		tick(engine);
	}
}

void pwmThread(Interrupt * interrupt, Servo * servo)
{
	struct _pulse pulse;

	/* Get a handle to the parallel port's Control register */
	ctrl_handle = mmap_device_io( PORT_LENGTH, CTRL_ADDRESS );

	/* Initialise the parallel port */
	out8( ctrl_handle, INIT_BIT );

	/* Get a handle to the parallel port's Data register */
	data_handle = mmap_device_io( PORT_LENGTH, DATA_ADDRESS );

	while(1)
	{
		rcvid = MsgReceive(interrupt->chid, &pulse, sizeof(pulse), NULL);
		out8( data_handle, HIGH );
		usleep(* servo->pwmDuty);
		out8( data_handle, LOW );
	}
}

void main(void)
{
   UINT16 userInput;
   UINT8 test;
   int i;
   UINT8 ch = 0;

   const int  numServos = 2;


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

   /* Initialize the servos */
   servos = {
    {1,3,0,0,0,0,INIT,&PWMDTY0, &PWMPER0, &PORTB, recipe1},
    {2,3,0,0,0,0,INIT,&PWMDTY1, &PWMPER1, &PORTB, recipe2}
   };

   instructionInterrupt = CreateInterrupt(1000000);//1sec
   pwmInterrupt = CreateInterrupt(PERIODMILLIS * 1000);//20ms

   /* Initialize the engine */
   Engine engine = {numServos, servos};

   clkafreq = BUS_CLK_FREQ / (PWMSCLAVALUE * 2);
   clkafreq = clkafreq / (1 << PWMPRCLKVALUE);

   //start these before starting the timer to fulfill recipe
   (void)InitializeSerialPort();
   //(void)InitPositionToPWM(PERIODMILLIS);

   (void)InitializePwm();
   (void)ConfigurePer(PERIODMILLIS, &servos[0]);
   (void)ConfigurePer(PERIODMILLIS, &servos[1]);
   (void)InitializeLEDs();
   (void)InitializeTimer();//start processing Instructions

   //Output new line and command prompt initially
   TERMIO_PutChar(newLine);
   TERMIO_PutChar('>');


   for (;;){
      GetUserCommand();
      if(flagHasUserInput == 1)
      {
        sendCommand(&servos[0], userInput1);
        sendCommand(&servos[1], userInput2);

        flagHasUserInput = 0;
        userInputIndex = 0;
      }

      if(flag == 1) {
        tick(engine);
        flag = 0;
      }
   }
}
