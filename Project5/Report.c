#include <stdio.h>
#include <limits.h>
#include "Report.h"

static int _maxDistance = INT_MAX;
static int _minDistance = 0;

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
	_maxDistance = INT_MAX;
	_minDistance = 0;
}
void printMeasure(int distance)
{
	printf("%c Distance: %d in", (char)0x15, 1);
	processMeasure(distance);

}
