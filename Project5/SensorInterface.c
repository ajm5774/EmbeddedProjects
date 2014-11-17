#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <stdint.h>

#include "SensorInterface.h"

static uintptr_t ctrl_handle;
static uintptr_t trigger_handle;
static uintptr_t echo_handle;

static float _cmDivisor = 27.6233;
static float _inDivisor = 70.1633;

static float convert(long microsec, int isMetric)
{
	// microsec / 29 / 2;
	if(isMetric)
		return microsec / _cmDivisor / 2.0;  // CM
	// microsec / 74 / 2;
	else
		return microsec / _inDivisor / 2.0;  // IN
}

float measureDistance(void)
{
	uint64_t nCycles;
	long distance;
	uint64_t cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;

	// Send a pulse
	pulse(PULSE_DURATION);

	// Record the echo time
	nCycles = echo();
	printf("cycles: %ld, cps: %ld\n", nCycles, cps);

	// Convert cycles to microseconds
	distance = (nCycles / cps) * 1000 * 1000;

	// Convert to inches
	return convert(distance, 0);
}

void pulse(int usec)
{
	// Clear the output
	out8(trigger_handle, LOW);
	usleep(CLEAR_DURATION);

	// Set the output high
	out8(trigger_handle, HIGH);

	// Wait for the pulse width
	usleep(usec);

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

	while (in8(echo_handle) > LOW);

	// Echo pulse ended. Record end time
	endCycle = ClockCycles();

	return endCycle - startCycle;
}

void init(int triggerPort, int echoPort)
{
	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	// Get a handle to each device register
	ctrl_handle = mmap_device_io(PORT_LENGTH, SONAR_CTRL);
	trigger_handle = mmap_device_io(PORT_LENGTH, triggerPort);
	echo_handle = mmap_device_io(PORT_LENGTH, echoPort);

	// Set the direction of the ports
	//out8(ctrl_handle, DIRA(OUTPUT) | DIRB(INPUT));
	out8(ctrl_handle, 18);
}
