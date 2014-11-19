/*
 * timer.h
 *
 *  Created on: Nov 19, 2014
 *      Author: ajm5774
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef struct{
	timer_t timer_id;
	struct itimerspec timer;
	int chid;
	int period;
} Interrupt;

#endif /* TIMER_H_ */
