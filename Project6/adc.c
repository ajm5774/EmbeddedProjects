/*
 * adc.c
 *
 * Definition of adc functions.
 */
#include <stdio.h>
#include <hw/inout.h>
#include <unistd.h>
#include <sys/mman.h>     /* for mmap_device_io() */
#include <stdint.h>
#include <sys/neutrino.h>
#include "adc.h"

static uintptr_t command_handle[NUM_REGISTERS];

void initADC() {
	// Make handle to ADC
	int i;

	/* Give this thread root permissions to access the hardware */
	ThreadCtl( _NTO_TCTL_IO, NULL );

	for (i = 0; i < NUM_REGISTERS; i++) {
		command_handle[i] = mmap_device_io( PORT_LENGTH, COMMAND_REGISTER + i );
	}

	// Select the input channel
	out8( command_handle[2], 0x22 ); // 15 bits
	//out8( command_handle[2], (HIGH_CHANNEL << 4) | LOW_CHANNEL );

	// Select the input range
	out8( command_handle[3], 0x02 ); // +-2.5V

	// Wait for analog input circuit to settle
	//monitor the WAIT bit at Base + 3 bit 5
	//when it is 1, the circuit is actively settling on the input signal.
	//When it is 0, the board is ready to perform A/D conversions
	while(in8( command_handle[3] ) & 0x20){}
}

void startADC() {
	out8( command_handle[0], 0x80 );
}

int checkStatus() {
	int i;
	for (i = 0; i < 10000; i++) {
		if (!(in8( command_handle[3] ) & 0x80))
			return 0;
	}

	return -1; // Conversion didn't complete
}

int16_t readData() {
	char LSB, MSB;

	LSB = in8 ( command_handle[0] );
	MSB = in8 ( command_handle[1] );

	return MSB * 256 + LSB;
}

int convertData(int16_t data) {
	return data * VFS / 32768;
}
