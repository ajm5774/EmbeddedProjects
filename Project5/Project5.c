#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include "Timer.h"
#include "SensorInterface.h"
#include "Report.h"

int main(int argc, char *argv[]) {

	char input;
	float mDistance;
	int distance;

	// Open sensor ports
	init(SONAR_PORTA, SONAR_PORTB);

	while(1)
	{

		// Reset high/low measurements
		clearMeasurements();

		// Block until input
		getchar();

		// Loop until the button is pressed again
		while (tcischars(stdin) < 1) {
			mDistance = measureDistance();

			// Round result & print
			distance = (int) round(mDistance);

			printMeasure(mDistance);
		}

		// Clear the character
		getchar();

		// Print the results
		printReport();
	}
}
