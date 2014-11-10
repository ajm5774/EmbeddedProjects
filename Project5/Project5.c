#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	char input;

	while(true)
	{
		StartTimer();

		//wait for first character to start measuring
		getchar();

		while(getCharNonB())
		{
			MsgReceivePulse ( channel_id, &pulse, sizeof( pulse ), NULL );
			measureDistance();
		}
		printReport();
	}
}

unsigned char getCharNonB()
{
    char r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return 0;
    } else {
        return c;
    }
}
