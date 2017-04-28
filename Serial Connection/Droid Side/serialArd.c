/* www.chrisheydrick.com
     
   June 23 2012
   CanonicalArduinoRead write a byte to an Arduino, and then
   receives a serially transmitted string in response.
   The call/response Arduino sketch is here:
   https://gist.github.com/2980344
   Arduino sketch details at www.chrisheydrick.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <boost/asio.hpp>
#define DEBUG 1


  
int main(int argc, char *argv[])
{
  int fd, n, i;
  char buf[256] = "";
  struct termios toptions;

  /* open serial port */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  printf("fd opened as %i\n", fd);
  
  /* wait for the Arduino to reboot */
  usleep(3500000);
  
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
	toptions.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	toptions.c_iflag = IGNPAR;
	toptions.c_oflag = 0;
	toptions.c_lflag = 0;  
  
  tcflush(fd,TCIFLUSH);
  /* commit the serial port settings */
  tcsetattr(fd, TCSANOW, &toptions);
	int j = 0;
	unsigned char buf2[] ={'0','2','0',',','1','5','0','\n'};
for(;;){
	j++;
	buf2[0]= '0';
	if (j > 100)	{
	j=0;
	buf2[0]= '1';
	}
	//printf("loop count %d\n",j);
  //String str;
  memset(&buf, '\0',sizeof buf);
  //getline(cin,str);
  //cout << "input: "<< str<<" \n";
	
  /* Send byte to trigger Arduino to send string back */
  int out = write(fd, buf2,sizeof buf2);
  //printf("data written: %d\n",out);
  usleep(200);
  /* Receive string from Arduino */
  n = read(fd, buf, sizeof buf);
  /* insert terminating zero in the string */
 

  printf("output: %s\n", buf);
 
}
 
  return 0;
}
