/*
 * SensorInterface.h
 *
 *  Created on: Nov 5, 2014
 *      Author: ajm5774
 */

#ifndef SENSORINTERFACE_H_
#define SENSORINTERFACE_H_

//equivalent of a ping
int measureDistance(void);
void pulse(int duration);
int echo();

#endif /* SENSORINTERFACE_H_ */
