

#include <stdio.h>
#include <string.h>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <iostream>


#include <unistd.h> // sleep()
#include <stdio.h>  // fopen(), fclose(), fprintf(), perror()
#include <stdlib.h> // exit() and EXIT_FAILURE
#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <iostream>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
using namespace std;
char readBuffer[1024];
int numBytesRead;
int fd, n, i;
  char buf[64] = "temp text";
  struct termios toptions;
int main(){

/* open serial port */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  printf("fd opened as %i\n", fd);
  
  /* wait for the Arduino to reboot */
  usleep(3500000);
  
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions, B9600);
  cfsetospeed(&toptions, B9600);
  /* 8 bits, no parity, no stop bits */
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  /* Canonical mode */
  toptions.c_lflag |= ICANON;
  /* commit the serial port settings */
  tcsetattr(fd, TCSANOW, &toptions);
  

 char data[] = {'f','b','r'};  //Random data we want to send
    FILE *file;
    if( NULL == (file = fopen("/dev/ttyACM0","w") ) )  //Opening device file
    { // then fopen failed
        perror("fopen failed for ttyACM0" );
        exit( EXIT_FAILURE );
    }
int getnum;
  usleep(3500000);
	if (file == NULL){
		printf("Error connecting to the Arduino\n");
		cerr << strerror(errno);
	return 0;
	}

	while(true){
		memset(readBuffer, 0,1024);
		fread(readBuffer, sizeof(char),1024,file);
		if(sizeof(readBuffer)!=0){
			printf("%s\n",readBuffer);
		}
	}	
fclose(file);	
return 0;
	
}

