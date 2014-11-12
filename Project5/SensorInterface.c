/*
 * SensorInterface.c
 *
 *  Created on: Nov 5, 2014
 *      Author: ajm5774
 */
#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>

#include "SensorInterface.h"

static uintptr_t ctrl_handle;
static uintptr_t trigger_handle;
static uintptr_t echo_handle;

static float _cmDivisor = 27.6233;
static float _inDivisor = 70.1633;
static uint64_t cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;

static float connvert(long microsec, bool metric)
{
	microsec / 29 / 2;
	if(metric) 
		return microsec / _cmDivisor / 2.0;  // CM
	// microsec / 74 / 2;
	else 
		return microsec / _inDivisor / 2.0;  // IN
}

float measureDistance(void)
{
	uint64_t nCycles;
	long distance;

	// Send a pulse
	pulse(PULSE_DURATION);

	// Record the echo time
	nCycles = echo();

	// Convert cycles to microseconds
	distance = (nCycles / cps) * 1000 * 1000;

	// Convert to inches
	return convert(distance, false);
}

void pulse(int usec)
{
	// Clear the output
	out8(trigger_handle, LOW);
	usleep(CLEAR_DURATION);

	// Set the output high
	out8(trigger_handle, HIGH);

	// Wait for the pulse width
	usleep(duration);

	// Set the output to low
	out8(trigger_handle, LOW);
}

uint64_t echo()
{
	uint64_t startCycle, endCycle;
	// Listen for a HIGH event
	while (in8(echo_handle) == LOW);

	// HIGH event received, Start recording time
	startCycle = ClockCycles();

	while (in8(echo_handle) == HIGH);

	// Echo pulse ended. Record end time
	endCycle = ClockCycles();

	return endCycle - startCycle;
}

void init()
{
	// Get a handle to each device register
	ctrl_handle = mmap_device_io(PORT_LENGTH, SONAR_CTRL);
	trigger_handle = mmap_device_io(PORT_LENGTH, SONAR_PORTA);
	echo_handle = mmap_device_io(PORT_LENGTH, SONAR_PORTB);

	// Set the direction of the ports
	out8(ctrl_handle, DIRA(OUTPUT) | DIRB(INPUT));
}
