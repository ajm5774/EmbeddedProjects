/*
 * adc.c
 *
 * Definition of adc functions.
 */

#include "adc.h"

static uintptr_t command_handle;
static uintptr_t adc_channel_handle;
static uintptr_t adc_settings_handle;

void initADC() {
	// Make handle to ADC
	command_handle = mmap_device_io(PORT_LENGTH, COMMAND_REGISTER);
	adc_channel_handle = mmap_device_io(PORT_LENGTH, ADC_CHANNEL_REGISTER);
	adc_settings_handle = mmap_device_io(PORT_LENGTH, ADC_SETTINGS_REGISTER);

	// Select the input channel
	out8( adc_channel_handle, 0xF0 ); // 15 bits

	// Select the input range
	out8( adc_settings_handle, 0x01 ); // +-5V

	// Wait for analog input circuit to settle
	usleep(10);
}

void startADC() {
	out8( command_handle, 0x80 );
}

int checkStatus() {
	int i;
	for (i = 0; i < 10000; i++) {
		if (!in8( adc_settings_handle ) & 0x80)
			return 0;
	}

	return -1; // Conversion didn't complete
}

int16_t readData() {
	uint8_t LSB, MSB;

	LSB = in8 ( command_handle );
	MSB = in8 ( command_handle+1 );

	return (MSG * 256) + LSB;
}
