/*
 * TellerSemaphore.h
 *
 *  Created on: Oct 14, 2014
 *      Author: llk
 */

#ifndef TIMER_H_
#define TIMER_H_

#define NUMBER_OF_TELLERS 3

struct timespec getTime();
struct timespec getDifference(struct timespec ts1, struct timespec ts2);
void StartTimer();

#endif /* TELLERINTERFACE_H_ */
