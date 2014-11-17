#include <stdio.h>
#include <limits.h>
#include "Report.h"

static int _maxDistance = 0;
static int _minDistance = INT_MAX;

void processMeasure(int distance)
{
	if(distance < _minDistance)
		_minDistance = distance;

	if (distance > _maxDistance)
		_maxDistance = distance;
}
void printReport()
{
	printf("--------------Report-------------");
	printf("Minimum Distance: %d \n", _minDistance);
	printf("Maximum Distance: %d \n", _maxDistance);
}
void clearMeasurements()
{
	_maxDistance = 0;
	_minDistance = INT_MAX;
}
void printMeasure(int distance)
{
	printf("Distance: %d in\r", distance);
	processMeasure(distance);

}
