#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "Timer.h"
#include "SensorInterface.h"
#include "Report.h"

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
			distance = (int) roundf(mDistance);

			printMeasure(distance);
		}

		// Clear the character
		getchar();

		// Print the results
		printReport();
	}
}
