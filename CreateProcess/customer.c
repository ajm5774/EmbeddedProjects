<<<<<<< HEAD
/*
 * customer.c
 *
 *  Created on: Oct 27, 2014
 *      Author: ajm5774
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "TellerInterface.h"
#include "customer.h"

extern pthread_t customerThread;

extern conQueue cq;
extern Customer * custArray[1000];

//Method to create customers
static void CustomersThread(int * arg)
{
	Customer *cust;
	int id = 0;
	int sleepMinsUS;//in usec
	int maxSleepTimeMS = 4 * SIMULATION_MINUTE_MSEC;
	int minSleepTimeMS = SIMULATION_MINUTE_MSEC;

	//loop until theres no more time in the day
	while(getSimTimeMins() < MINUTES_PER_DAY)
	{
		//create customer
		cust = malloc( sizeof(Customer) );
		cust->id = id;
		cust->timeStart = getSimTimeMilliMins();
		cust->timeWaiting = 0;
		cust->timeWithTeller = 0;
		cust->timeEnd = 0;
		cust->behind = 0;

		//printf("Customer %d arrived at %ld.\n", cust->id, cust->timeStart);
		printf("Customer %d arrived at %dh %dm.\n", cust->id, getHour(cust->timeStart), getMinute(cust->timeStart));

		//Add customer to the list of all customers (for metrics)
		custArray[id++] = cust;

		//Add customer to the queue
		enqueue(&cq, cust);

		//wait 1-4 sim minutes before creating another customer
		sleepMinsUS = ((rand() % (maxSleepTimeMS - minSleepTimeMS)) + minSleepTimeMS) * 1000;
		usleep(sleepMinsUS);
	}
	printf("Customer creation thread stopped \n");
}

void StartCustomerThread()
{
	pthread_attr_t threadAttributes ;
	int policy ;
	struct sched_param parameters ;

	pthread_attr_init(&threadAttributes) ;		// initialize thread attributes structure -- must do before any other activity on this struct
	pthread_getschedparam(pthread_self(), &policy, &parameters) ;	// get this main thread's scheduler parameters
	printf( "Customer Creation thread running at priority %d\n", parameters.sched_priority) ;
	parameters.sched_priority-- ;									// lower the priority
	pthread_attr_setschedparam(&threadAttributes, &parameters) ;	// set up the pthread_attr struct with the updated priority

	//start customer creation thread
	pthread_create( &customerThread, &threadAttributes, (void *)CustomersThread, 0) ;
}


=======
/*
 * customer.c
 *
 *  Created on: Oct 27, 2014
 *      Author: ajm5774
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "TellerInterface.h"
#include "customer.h"

extern pthread_t customerThread;

extern conQueue cq;
extern Customer * custArray[1000];

//Method to create customers
static void CustomersThread(int * arg)
{
	Customer *cust;
	int id = 0;
	int sleepMinsUS;//in usec
	int maxSleepTimeMS = 4 * SIMULATION_MINUTE_MSEC;
	int minSleepTimeMS = SIMULATION_MINUTE_MSEC;

	//loop until theres no more time in the day
	while(getSimTimeMins() < MINUTES_PER_DAY)
	{
		//create customer
		cust = malloc( sizeof(Customer) );
		cust->id = id;
		cust->timeStart = getSimTimeMilliMins();
		cust->timeWaiting = 0;
		cust->timeWithTeller = 0;
		cust->timeEnd = 0;
		cust->behind = 0;

		//printf("Customer %d arrived at %ld.\n", cust->id, cust->timeStart);
		printf("Customer %d arrived at %dh %dm.\n", cust->id, getHour(cust->timeStart), getMinute(cust->timeStart));

		//Add customer to the list of all customers (for metrics)
		custArray[id++] = cust;

		//Add customer to the queue
		enqueue(&cq, cust);

		//wait 1-4 sim minutes before creating another customer
		sleepMinsUS = ((rand() % (maxSleepTimeMS - minSleepTimeMS)) + minSleepTimeMS) * 1000;
		usleep(sleepMinsUS);
	}
	printf("Customer creation thread stopped \n");
}

void StartCustomerThread()
{
	pthread_attr_t threadAttributes ;
	int policy ;
	struct sched_param parameters ;

	pthread_attr_init(&threadAttributes) ;		// initialize thread attributes structure -- must do before any other activity on this struct
	pthread_getschedparam(pthread_self(), &policy, &parameters) ;	// get this main thread's scheduler parameters
	printf( "Customer Creation thread running at priority %d\n", parameters.sched_priority) ;
	parameters.sched_priority-- ;									// lower the priority
	pthread_attr_setschedparam(&threadAttributes, &parameters) ;	// set up the pthread_attr struct with the updated priority

	//start customer creation thread
	pthread_create( &customerThread, &threadAttributes, (void *)CustomersThread, 0) ;
}


>>>>>>> 7a45798c5ed9dba6cef3f6cc29f864e3972417fd
