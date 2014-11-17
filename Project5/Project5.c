#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "Timer.h"
#include "SensorInterface.h"
#include "Report.h"

//int roundNum(float number)
//{
	//return (int)(number - floor(number) >= 0.5) ? ceil(number) : floor(number);
//}

int main(int argc, char *argv[]) {

	float mDistance;
	int distance;

	printf("Opening ports...");
	// Open sensor ports
	init(SONAR_PORTA, SONAR_PORTB);
	printf("done.\n");

	while(1)
	{
		// Reset high/low measurements
		clearMeasurements();

		printf("Press button to start.\n");
		// Block until input
		getchar();

		// Loop until the button is pressed again
		while (tcischars(fileno (stdin)) < 1) {
			mDistance = measureDistance();

			// Round result & print
			//distance = roundNum(mDistance);
			//distance = (int) mDistance;

			//printMeasure(mDistance);
			printf("Distance: %f\n", mDistance);
		}

		// Clear the character
		getchar();

		// Print the results
		printReport();
	}
}
