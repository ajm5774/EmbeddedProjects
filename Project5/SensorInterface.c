#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

#include "SensorInterface.h"

static uintptr_t ctrl_handle;
static uintptr_t trigger_handle;
static uintptr_t echo_handle;

static uint64_t max_echo_cycles;
static uint64_t cps;

static float _cmDivisor = 27.6233;
static float _inDivisor = 74.64;

static uint64_t start_cycles;
static uint64_t end_cycles;


static float convert(double microsec, int isMetric)
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
	double distance;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;

	// Determine the maximum cycles an echo can be
	max_echo_cycles = MAX_ECHO_TIME * cps ;

	// Send a pulse
	pulse(PULSE_DURATION);

	// Record the echo time
	nCycles = echo();

	// Convert cycles to microseconds
	distance = (double) (nCycles * 1000 * 1000 / cps);

	// Convert to inches
	return boundDistance(convert(distance, 0));
}

float boundDistance(float distance)
{
	if(distance < .67)//.67in = 17mm which is the min
		return (float)0.0;
	else if(distance > 118.11)//118.11in = 3m which is the max
		return (float)0.0;

	return distance;
}

void pulse(int usec)
{
	// Set the output high
	out8(trigger_handle, HIGH);

	// Wait for the pulse width
	usleep(usec);

	// Set the output to low
	out8(trigger_handle, LOW);

	//mark the time that the trigger pulse ends
	start_cycles = ClockCycles();
}

uint64_t echo()
{
	uint64_t listen_start = ClockCycles();//number of cycles at the start of the echo
	uint64_t cycles = 0;//cycles since start of echo

	// Listen for a HIGH event
	while ((in8(echo_handle) & 1) == LOW && cycles < max_echo_cycles)
		cycles = ClockCycles() - listen_start;

	// Listen for a LOW event
	while ((in8(echo_handle) & 1) == HIGH && cycles < max_echo_cycles)
		cycles = ClockCycles() - listen_start;

	//check to see if it timed out
	if(cycles > max_echo_cycles)
		return 0;

	// mark the time that the echo pulse ends
	end_cycles = ClockCycles();

	//return difference between trigger and echo pulse ends
	return end_cycles - start_cycles;
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
	out8(ctrl_handle, 0x02);

	// Clear the output
	out8(trigger_handle, LOW);
	usleep(CLEAR_DURATION);
}
