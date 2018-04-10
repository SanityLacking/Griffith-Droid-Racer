#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char serialPortFilename[] = "/dev/ttyACM0";

int main()
{
    char readBuffer[1000];
    int numBytesRead;

    FILE *serPort = fopen("/dev/ttyACM0", "r+");

	if (serPort == NULL)
	{
		printf("ERROR");	
		return 0;
	}

	printf("%s",serialPortFilename);
	printf(":\n");
	int i = 75;
	//unsigned char* output ="120";
//	usleep(35000);
	printf("begin\n");
	while(1)
	{	
	char readBuffer[64];
		//fwrite(&output,sizeof(char),sizeof(output),serPort);
		
		
		
		fgets(readBuffer, sizeof(char),serPort);
		if(sizeof(readBuffer) != 0)
		{
			printf("%s",readBuffer);
		}
	}
	return 0;
}
