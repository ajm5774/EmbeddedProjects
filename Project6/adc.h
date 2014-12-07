/*
 * adc.h
 *
 * Header file for ADC.
 */
#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define BASE 0x280
#define COMMAND_REGISTER 0x280
#define ADC_CHANNEL_REGISTER (BASE+2)
#define ADC_SETTINGS_REGISTER (BASE+3)

#define HIGH 1
#define LOW 0
#define PORT_LENGTH 1


// Initialize the ADC
void initADC();

// Perform an A/D conversion on the current channel
void startADC();

// Wait for the conversion to finish
int checkStatus();

// Read the data from the board
int16_t readData();

#endif
