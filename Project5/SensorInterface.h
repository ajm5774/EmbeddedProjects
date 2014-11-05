/*
 * SensorInterface.h
 *
 *  Created on: Nov 5, 2014
 *      Author: ajm5774
 */

#ifndef SENSORINTERFACE_H_
#define SENSORINTERFACE_H_

//equivalent of a ping
int MeasureDistance(void);
void SendOutput();
void GetInput();
void StartMeasuring(void);
void EndMeasuring(void);

#endif /* SENSORINTERFACE_H_ */
