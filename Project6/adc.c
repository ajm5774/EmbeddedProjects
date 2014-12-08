/*
 * adc.c
 *
 * Definition of adc functions.
 */

#include "adc.h"

static int numRegisters = 4;
static uintptr_t command_handle[numRegisters];

void initADC() {
	// Make handle to ADC
	int i;

	for (i = 0; i < numRegisters; i++) {
		command_handle[i] = mmap_device_io( PORT_LENGTH, COMMAND_REGISTER + i );
	}

	// Select the input channel
	out8( command_handle[2], 0xF0 ); // 15 bits

	// Select the input range
	out8( command_handle[3], 0x01 ); // +-5V

	// Wait for analog input circuit to settle
	usleep(10);
}

void startADC() {
	out8( command_handle[0], 0x80 );
}

int checkStatus() {
	int i;
	for (i = 0; i < 10000; i++) {
		if (!in8( command_handle[3] ) & 0x80)
			return 0;
	}

	return -1; // Conversion didn't complete
}

int16_t readData() {
	uint8_t LSB, MSB;

	LSB = in8 ( command_handle[0] );
	MSB = in8 ( command_handle[1] );

	return (MSB * 256) + LSB;
}

int convertData(int16_t data, int vfs) {
	return data / 32768 * vfs;
}
