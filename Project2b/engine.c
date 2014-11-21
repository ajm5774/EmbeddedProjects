/*
 * engine.c
 * @author mlw2224
 *
 * The program source for the engine operations.
 */

#include "engine.h"


void tick(Engine engine) {
	int i;
	int pc;
	Servo *servo;

	for (i = 0; i < engine.numServos; i++) {
		//Execute the next instruction on the servo.
		servo = &engine.servos[i];
		pc = servo->pc;

		execute(servo, servo->recipe[pc]);
  }
}

void sendCommand(Servo *servo, unsigned char command) {
	switch (command) {
		case 'P':
		case 'p':
			/* Pause */
			pauseExec(servo);
			break;
		case 'C':
		case 'c':
			/* Continue */
			cont(servo);
			break;
		case 'R':
		case 'r':
			/* Move Right */
			right(servo);
			break;
		case 'L':
		case 'l':
			/* Move Left */
			left(servo);
			break;
		case 'N':
		case 'n':
			/* No-Op */
			nop(servo);
			break;
		case 'B':
		case 'b':
			/* Restart */
			restart(servo);
			break;
	}
}
