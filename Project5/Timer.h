#include <time.h>

#ifndef TIMER_H_
#define TIMER_H_

#define NUMBER_OF_TELLERS 3

int channel_id;
struct timespec currentTime;

void StartTimer();
struct timespec getTime();

#endif /* TELLERINTERFACE_H_ */
