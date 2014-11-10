/*
 * Report.c
 *
 *  Created on: Nov 5, 2014
 *      Author: ajm5774
 */

void processMeasure(int distance)
{
	if(distance < minDistance)
		minDistance = distance;

	if (distance > maxDistance)
		maxDistance = distance;
}
void printReport()
{
	printf("--------------Report-------------");
	printf("Minimum Distance: %d \n", minDistance);
	printf("Maximum Distance: %d \n", maxDistance);
}
void clearScreen()
{

}
void printMeasure()
{
	clearScreen();
	printf();
}
