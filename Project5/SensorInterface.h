#include <inttypes.h>

#ifndef SENSORINTERFACE_H_
#define SENSORINTERFACE_H_

#define PORT_LENGTH 1

#define SONAR_ADDRESS 0x280
#define SONAR_PORTA SONAR_ADDRESS+8
#define SONAR_PORTB SONAR_ADDRESS+9
#define SONAR_PORTC SONAR_ADDRESS+10
#define SONAR_CTRL SONAR_ADDRESS+11

#define DIRA(bit) (bit << 4)
#define DIRB(bit) (bit << 1)

#define OUTPUT 0
#define INPUT 1

#define LOW 0
#define HIGH 1

#define CLEAR_DURATION (2) // 2us
#define PULSE_DURATION (10) // 10us
#define MAX_ECHO_TIME (18.0/1000.0) // 18ms

// Record the total distance
float measureDistance(void);

float boundDistance(float distance);

// Send a single pulse on the TRIGGER line
void pulse(int usec);

// Measure the time on the ECHO line
uint64_t echo();

// Initialize the ports
void init(int triggerPort, int echoPort);

#endif /* SENSORINTERFACE_H_ */
