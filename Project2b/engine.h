/*
 * engine.h
 * @author mlw2224
 *
 * Header file for the Instructions.
 */
#ifndef ENGINE_H
#define ENGINE_H

#include "servo.h"

typedef struct {
	int		numServos;	/* The number of servos. */
	Servo		*servos;	/* Pointer to the servos. */
} Engine;

/*
 * Perform an engine "tick" by running the next operation on each servo.
 */
void tick(Engine engine);

/*
 * Send a user command to the given servo.
 */
void sendCommand(Servo *servo, unsigned char command);

#endif
