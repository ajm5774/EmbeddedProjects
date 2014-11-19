Interrupt * CreateInterrupt(int periodMicros)
{
	//declare variables
	struct sigevent event;
	struct itimerspec timer;
	struct _clockperiod clkper;
	struct sched_param param;
	timer_t timer_id;
	int chid = ChannelCreate( 0 ); //create event channel

	/* Set our priority to the maximum, so we won’t get disrupted by anything other than interrupts. */
	param.sched_priority = sched_get_priority_max( SCHED_RR );

	//set the clock from 10ns to 1microsecond ticks
	clkper.nsec = 100;
	clkper.fract = 0;
	ClockPeriod ( CLOCK_REALTIME, &clkper, NULL, 0 ); // 1ms

	//Set up the pulse width modulation event
	event.sigev_notify = SIGEV_PULSE;		// most basic message we can send -- just a pulse number
	event.sigev_coid = ConnectAttach ( ND_LOCAL_NODE, 0, chid, 0, 0 );  // Get ID that allows me to communicate on the channel
	assert ( event.sigev_coid != -1 );		// stop with error if cannot attach to channel
	event.sigev_priority = getprio(0);
	event.sigev_code = 1023;				// arbitrary number assigned to this pulse

	// Now create the timer and get back the timer_id value for the timer we created.
	timer_create( CLOCK_REALTIME, &event, &timer_id );

	/* Change the timer request to alter the behavior. */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_nsec = periodMicros;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = periodMicros;

	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	return &Interrupt(timer_id, timer, chid, periodMicros);
}

void startInterrupt(Interrupt * interrupt)
{
	/* Start the timer. */
	if ( timer_settime( interrupt->timer_id, 0, &interrupt->timer, NULL ) == -1 )
	{
		perror("Can’t start timer.\n");
		exit( EXIT_FAILURE );
	}
}

void stopTimer()
{

}

