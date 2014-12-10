#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>       /* for uintptr_t */
#include <hw/inout.h>     /* for in*() and out*() functions */
#include <sys/neutrino.h> /* for ThreadCtl() */
#include <sys/mman.h>     /* for mmap_device_io() */
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <assert.h>
#include "timer.h"

void CreateInterrupt(Interrupt *ret, long periodMicros, long sec)
{
	//declare variables
	struct sigevent event;
	struct itimerspec timer;
	struct _clockperiod clkper;
	struct sched_param param;
	timer_t timer_id;

	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	int chid = ChannelCreate( 0 ); //create event channel

	/* Set our priority to the maximum, so we won’t get disrupted by anything other than interrupts. */
	param.sched_priority = sched_get_priority_max( SCHED_RR );

	//set the clock from 10ns to 1microsecond ticks
	clkper.nsec = 100000;
	clkper.fract = 0;
	ClockPeriod ( CLOCK_REALTIME, &clkper, NULL, 0 ); // 1ms

	//Set up the pulse width modulation event
	event.sigev_notify = SIGEV_PULSE;		// most basic message we can send -- just a pulse number
	event.sigev_coid = ConnectAttach ( ND_LOCAL_NODE, 0, chid, 0, 0 );  // Get ID that allows me to communicate on the channel
	assert ( event.sigev_coid != -1 );		// stop with error if cannot attach to channel
	event.sigev_priority = getprio(0);
	event.sigev_code = 1023;				// arbitrary number assigned to this pulse
	event.sigev_value.sival_ptr = (void*)0;		// ?? TBD

	// Now create the timer and get back the timer_id value for the timer we created.
	if ( timer_create( CLOCK_REALTIME, &event, &timer_id ) == -1)
	{
		perror ( "can’t create timer" );
		exit( EXIT_FAILURE );
	}

	/* Change the timer request to alter the behavior. */
	timer.it_value.tv_sec = sec;
	timer.it_value.tv_nsec = periodMicros * 1000;
	timer.it_interval.tv_sec = sec;
	timer.it_interval.tv_nsec = periodMicros * 1000;


	//ret = (Interrupt){timer_id, timer, chid, periodMicros};
	ret->timer_id = timer_id;
	ret->timer = timer;
	ret->chid = chid;
	ret->period = periodMicros;
}

void startInterrupt(Interrupt * interrupt)
{
	if(timer_settime( interrupt->timer_id, 0, &interrupt->timer, NULL ) == -1)
	{
		perror("Can’t start timer.\n");
		exit( EXIT_FAILURE );
	}
}

void stopTimer()
{

}

