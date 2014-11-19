/*
 * servo.c
 * @author mlw2224
 *
 * The functionality for the Servo.
 */

#include "servo.h"

int positionToPWM[6] = {5, 9, 13, 17, 21, 25};

void execute(Servo *servo, Instruction command) {
	enum State state = servo->state;

	switch (state) {
		case INIT:
			servo->state = RUN;
		case RUN:
			/* Process the current instruction */
			process(servo, command);
			break;
		case PAUSE:
			break;
		case ERROR:
			break;
		case END:
			break;
	}
}

void pause(Servo *servo) {
	/* Switch to paused state if recipe not ended/error */
	if (servo->state != ERROR && servo->state != END) {
		servo->state = PAUSE;
  	setStatus(servo, RecipePaused);
	}
}

void cont(Servo *servo) {
	/* Switch to running state if recipe not ended/error */
	if (servo->state != ERROR && servo->state != END) {
		servo->state = RUN;
	  setStatus(servo, RecipeRunning);
	}
}

void right(Servo *servo) {
	/*
	 * Move servo right one position if recipe is paused & not
	 * at the extreme right position.
	 */
	if (servo->state == PAUSE && servo->position > 0) {
		servo->position--;
		mov(servo, servo->position);
	}
}

void left(Servo *servo) {
	/*
	 * Move servo left one position if recipe is paused & not
	 * at the extreme left position.
	 */
	if (servo->state == PAUSE && servo->position < 5) {
		servo->position++;
		mov(servo, servo->position);
	}
}

void nop(Servo *servo) {
	/* Do Nothing */
}

void restart(Servo *servo) {
	/* Reset the PC to 0 & start execution */
	servo->state = INIT;
	servo->pc = 0;
	servo->isWaiting = 0;
	servo->waitsRemaining = 0;
	servo->loopsRemaining = 0;
	setStatus(servo, Reset);
}

void process(Servo *servo, Instruction b) {
	enum opcode operation = OPCODE(b);
	int parameter = PARAM(b);

	switch(operation) {
		case MOV:
			mov(servo, parameter);
			break;
		case WAIT:
			wait(servo, parameter);
			break;
		case LOOP_START:
			loop_start(servo, parameter);
			process(servo, servo->recipe[servo->pc]);
			break;
		case END_LOOP:
			end_loop(servo);
			process(servo, servo->recipe[servo->pc]);
			break;
		case RECIPE_END:
			recipe_end(servo);
			break;
		default: /* Opcode not defined */
			break;
	}
}

void mov(Servo *servo, int parameter) {
	if (parameter >= 0 && parameter <= 5) { /* Valid input */
		servo->position = parameter;
    * servo->pwmDuty = positionToPWM[parameter];
		/* Increment the PC */
		servo->pc++;

	} else { /* Out of bounds */
		servo->state = ERROR;
		setStatus(servo, RecipeCommandError);
	}
}

void wait(Servo *servo, int parameter) {
	if (parameter >= 0 && parameter <= 31) { /* Valid input */
		if (servo->isWaiting) { /* is servo currently waiting? */
			servo->waitsRemaining--;
		}
		else { /* Begin Waiting */
			servo->waitsRemaining = parameter;
			servo->isWaiting = 1;
		}
		if (servo->waitsRemaining == 0) { /* No wait cycles left */
			servo->isWaiting = 0;
			servo->pc++;
		}
	} else { /* Out of bounds */
		servo->state = ERROR;
		setStatus(servo, RecipeCommandError);
	}
}

void loop_start(Servo *servo, int parameter) {
	if (parameter >= 0 && parameter <= 31) { /* Valid input */
		if (servo->loopsRemaining) { /* Already looping */
			servo->state = ERROR;
		  setStatus(servo, NestedLoopError);
		} else { /* Start the loop */
			servo->loopsRemaining = parameter;
			/* Increment the PC */
		  servo->pc++;
		}

	} else { /* Out of bounds */
		servo->state = ERROR;
		setStatus(servo, RecipeCommandError);
	}
}

void end_loop(Servo *servo) {
	if (servo->loopsRemaining) { /* Jump back to start of loop */
		while ( OPCODE(servo->recipe[servo->pc]) != LOOP_START )
			servo->pc--;
		servo->loopsRemaining--;
	}

	/* Increment the PC */
	servo->pc++;
}

void recipe_end(Servo *servo) {
	servo->state = END;
	setStatus(servo, EndOfRecipe);
}

void setStatus(Servo *servo, enum Status status) {
     switch(status) {
      case RecipeCommandError:
        //set LEDs to 0 x x x
        *servo->leds &= LED_3_ON;
      break;
      case NestedLoopError:
        //set LEDs to x 0 x x
        *servo->leds &= LED_2_ON;
      break;
      case EndOfRecipe:
        //set LEDs to x x 0 x
        *servo->leds &= LED_1_ON;
      break;
      case RecipePaused:
        //set LEDs to x x x 0
        *servo->leds &= LED_0_ON;
      break;
      case RecipeRunning:
        //set LEDs to x x x 1
        *servo->leds |= LED_0_OFF;
        break;
      case Reset:
        //set LEDs to 1 1 1 1
        *servo->leds |= LED_3_OFF | LED_2_OFF | LED_1_OFF | LED_0_OFF;
        break;
     }

}

