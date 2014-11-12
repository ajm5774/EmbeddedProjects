#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Timer.h"
#include "SensorInterface.h"
#include "Report.h"

int main(int argc, char *argv[]) {

	char input;
	float mDistance;

	init(SONAR_PORTA, SONAR_PORTB);

	/*while(1)
	{
		StartTimer();

		//wait for first character to start measuring
		getchar();

		while(!kbhit())
		{
			//MsgReceivePulse ( channel_id, &pulse, sizeof( pulse ), NULL );
			usleep(100000);//100ms
			measureDistance();
		}
		printReport();
	}*/



	while(1)
	{
		usleep(100000);//100ms
		mDistance = measureDistance();
		printMeasure(mDistance);
	}
}

/*int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0)
    {
        return r;
    } else {
        return c;
    }
}*/
