#include "Report.h"

int _maxDistance = 0;
int _minDistance = 0;

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
void clearScreen()
{

}
void printMeasure(int distance)
{
	//clear;
	printf("Distance: %f in. \n", distance);
}
