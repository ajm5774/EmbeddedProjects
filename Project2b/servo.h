/*
 * servo.h
 * @author mlw2224
 *
 * Header file for the Servos.
 */
#ifndef SERVO_H
#define SERVO_H

#define OPCODE(b) 	(((b) >> 5) & 0x07)
#define PARAM(b)	((b) & 0x1F)
#define INSTRUCTION(opcode, param) ((opcode << 5) | param)

// LED masks
#define LED_3_OFF (1<<7)
#define LED_2_OFF (1<<6)
#define LED_1_OFF (1<<5)
#define LED_0_OFF (1<<4)

#define LED_3_ON (0<<7 | LED_2_OFF | LED_1_OFF | LED_0_OFF)
#define LED_2_ON (0<<6 | LED_3_OFF | LED_1_OFF | LED_0_OFF)
#define LED_1_ON (0<<5 | LED_3_OFF | LED_2_OFF | LED_0_OFF)
#define LED_0_ON (0<<4 | LED_3_OFF | LED_2_OFF | LED_1_OFF)

/*
 * An Instruction is a single byte composed of:
 * Opcode - 3 bits
 * Parameters - 5 bits
 */
typedef unsigned char Instruction;

enum opcode {
	MOV		= 1,	/* 001 */
	WAIT		= 2,	/* 010 */
	LOOP_START 	= 4,	/* 100 */
	END_LOOP 	= 5,	/* 101 */
	RECIPE_END 	= 0	/* 000 */
};

enum State {
	INIT,
	RUN,
	PAUSE,
	ERROR,
	END
};

enum Status {
   RecipeCommandError,
   NestedLoopError,
   EndOfRecipe,
   RecipePaused,
   RecipeRunning,
   Reset
};

typedef struct {
  int   id;
	int		position;	/* Current Servo Position */
	int		pc;		/* Current position in recipe */
	int 		loopsRemaining;	/* Number of loops left */
	int		isWaiting : 1;	/* Is the servo waiting? */
	int 		waitsRemaining;	/* Number of waits left */
	enum State	state;		/* Current state of the servo */
	unsigned char *   pwmDuty;
	unsigned char *   pwmPer;
	unsigned char *   leds;
	Instruction *recipe;
} Servo;

/**
 * Run the next operation.
 * @param servo The servo to run the command on.
 */
void execute(Servo *servo, Instruction command);

/**
 * Pause Recipe Execution
 *
 * Not operative after recipe end or error.
 */
void pause(Servo *servo);

/**
 * Continue Recipe Execution
 *
 * Not operative after recipe end or error.
 */
void cont(Servo *servo);

/**
 * Move 1 position to the right if possible.
 *
 * Not operative if recipe isn't paused or at extreme right position.
 */
void right(Servo *servo);

/**
 * Move 1 position to the left if possible.
 *
 * Not operative if recipe isn't paused or at extreme left position.
 */
void left(Servo *servo);

/**
 * No-op no new override entered for selected servo.
 */
void nop(Servo *servo);

/**
 * Restart the recipe.
 *
 * Starts the recipe's execution immediately.
 */
void restart(Servo *servo);

/*
 * Process the Instruction & call the appropriate operation.
 */
void process(Servo *servo, Instruction b);

/*
 * @param parameter The target position number
 *
 * An out of range parameter value produces an error.
 */
void mov(Servo *servo, int parameter);

/*
 * @param parameter The number of 1/10 seconds to delay before attempting
 * 			to execute next recipe command.
 *
 * The actual delay will be 1/10 second more than the parameter value..
 */
void wait(Servo *servo, int parameter);

/*
 * @param parameter The number of additional times to execute the following
 * 			recipe block.
 *
 * A parameter value of "n" will execute the block once but will repeat it "n"
 * more times.
 */
void loop_start(Servo *servo, int parameter);

/*
 * Marks the end of a recipe loop block.
 */
void end_loop(Servo *servo);

/*
 * End the recipe.
 */
void recipe_end(Servo *servo);

/*
 * Set the LED status
 */
void setStatus(Servo *servo, enum Status status);

#endif
