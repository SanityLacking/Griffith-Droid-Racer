

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string>
#include <iostream> 

static int ARDUINO; 
using namespace std;
void setupArduino(){

  static std::string base = "/dev/ttyACM";

  for( int i = 0; i < 4; i++ ){

    //std::string port = base + std::to_string(i);
    char port[12];
    sprintf(port, "/dev/ttyACM%d", i);
    ARDUINO = open(port, O_RDWR | O_NOCTTY);

    if(ARDUINO < 0)
    {
      printf("Could not open serial port, tryng the next one");
      continue;
    }
    
     break;     
  }

  /* wait for the Arduino to reboot */
  usleep(350);
  
  char buf[2];
  struct termios toptions;

  /* get current serial port settings */
  tcgetattr(ARDUINO, &toptions);
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
  tcsetattr(ARDUINO, TCSANOW, &toptions);


}

int readArduino()
{
       int n;
       char buf[2];

  	/* Receive string from Arduino */
  	n = read(ARDUINO, buf, 2);

	int val = atoi(&buf[0]);

	return val;
}

int main(int argc, char *argv[])
{
  
  setupArduino();
 
  while(true)
  {
         char buf[2];
         int n = read(ARDUINO, buf, 2);
//        int val = readArduino();
  	printf("Arduino says: %i\n", n);
  }

  
  return 0;
}
