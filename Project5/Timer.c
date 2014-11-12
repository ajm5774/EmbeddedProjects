#include <stdio.h>
#include <time.h>

#include "Timer.h"

struct timespec getTime()
{
	if( clock_gettime( CLOCK_REALTIME, &currentTime) == -1 ) {
	  perror( "clock gettime" );
	}

	return currentTime;
}

void StartTimer(){}
/*{
	//declare variables
	int pid;
	int pulse_id = 0 ;
	timer_t timer_id;
	struct sigevent event;
	struct itimerspec timer;
	struct _clockperiod clkper;
	int privity_err;
	uintptr_t ctrl_handle;
	uintptr_t data_handle;
	int count = 0 ;
	float cpu_freq;
	time_t start;

	//Get the CPU frequency in order to do precise time calculations.
	cpu_freq = SYSPAGE_ENTRY( qtime )->cycles_per_sec;

	//Set our priority to the maximum, so we won’t get disrupted by anything other than interrupts.
	struct sched_param param;
	int ret;
	param.sched_priority = sched_get_priority_max( SCHED_RR );
	ret = sched_setscheduler( 0, SCHED_RR, &param);
	assert ( ret != -1 );	// if returns a -1 for failure we stop with error

	//set the clock from 10ns to 1ms ticks
	clkper.nsec = 100000;
	clkper.fract = 0;
	ClockPeriod ( CLOCK_REALTIME, &clkper, NULL, 0 ); // 1ms

	//Create a channel to receive timer events on.
	channel_id = ChannelCreate( 0 );
	assert ( channel_id != -1 );			// if returns a -1 for failure we stop with error

	//Set up the timer and timer event.
	event.sigev_notify = SIGEV_PULSE;		// most basic message we can send -- just a pulse number
	event.sigev_coid = ConnectAttach ( ND_LOCAL_NODE, 0, channel_id, 0, 0 );  // Get ID that allows me to communicate on the channel
	assert ( event.sigev_coid != -1 );		// stop with error if cannot attach to channel
	event.sigev_priority = getprio(0);
	event.sigev_code = 1023;				// arbitrary number assigned to this pulse
	event.sigev_value.sival_ptr = (void*)pulse_id;		// ?? TBD

	// Now create the timer and get back the timer_id value for the timer we created.
	if ( timer_create( CLOCK_REALTIME, &event, &timer_id ) == -1 )	// CLOCK_REALTIME available in all POSIX systems
	{
		perror ( "can’t create timer" );
		exit( EXIT_FAILURE );
	}

	//Change the timer request to alter the behavior.
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_nsec = 500000;		// interrupt at .5 ms.
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 500000;	// keep interrupting every .5 ms.

	//Start the timer.
	if ( timer_settime( timer_id, 0, &timer, NULL ) == -1 )
	{
		perror("Can’t start timer.\n");
		exit( EXIT_FAILURE );
	}

	//Give this thread root permissions to access the hardware
	privity_err = ThreadCtl( _NTO_TCTL_IO, NULL );
	if ( privity_err == -1 )
	{
		fprintf( stderr, "can't get root permissions\n" );
		return -1;
	}

	//Get a handle to the parallel port's Control register
	ctrl_handle = mmap_device_io( PORT_LENGTH, CTRL_ADDRESS );

	//Initialize the parallel port
	out8( ctrl_handle, INIT_BIT );

	//Get a handle to the parallel port's Data register
	data_handle = mmap_device_io( PORT_LENGTH, DATA_ADDRESS );

}*/



