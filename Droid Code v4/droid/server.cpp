/**
 * OpenCV video streaming over TCP/IP
 * Server: Captures video from a webcam and send it to a client
 * by Isaac Maia
 * g++ -pthread server.cpp -o server `pkg-config opencv --cflags --libs`
 */

/*#include <opencv2/opencv.hpp>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <string>

using namespace cv;

void *display(void *);

int capDev = 0;

VideoCapture cap(capDev); // open the default camera



int main(int argc, char** argv)
{

    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
    int                 localSocket,
                        remoteSocket,
                        port = 4099;

    struct  sockaddr_in localAddr,
                        remoteAddr;
    pthread_t thread_id;


    int addrLen = sizeof(struct sockaddr_in);


    if ( (argc > 1) && (strcmp(argv[1],"-h") == 0) ) {
          std::cerr << "usage: ./cv_video_srv [port] [capture device]\n" <<
                       "port           : socket port (4097 default)\n" <<
                       "capture device : (0 default)\n" << std::endl;

          exit(1);
    }

    if (argc == 2) port = atoi(argv[1]);

    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
         perror("socket() call failed!!");
    }

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );

    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
         perror("Can't bind() socket");
         exit(1);
    }

    //Listening
    listen(localSocket , 3);

    std::cout <<  "Waiting for connections...\n"
              <<  "Server Port:" << port << std::endl;

    //accept connection from an incoming client
    while(1){
    //if (remoteSocket < 0) {
    //    perror("accept failed!");
    //    exit(1);
    //}

     remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
      //std::cout << remoteSocket<< "32"<< std::endl;
    if (remoteSocket < 0) {
        perror("accept failed!");
        exit(1);
    }
    std::cout << "Connection accepted" << std::endl;
     pthread_create(&thread_id,NULL,display,&remoteSocket);

     //pthread_join(thread_id,NULL);

    }
    //pthread_join(thread_id,NULL);
    //close(remoteSocket);

    return 0;
}

std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void *display(void *ptr){
    int socket = *(int *)ptr;
    //OpenCV Code
    //----------------------------------------------------------

    Mat img, imgGray;
    int bytes = 0;
    int key;

    //std::cout << "Image Size:" << imgSize << std::endl;

    while(1) {

            /* get a frame from camera
                cap >> img;
                resize(img, img, Size(1080, 720));
                int imgSize = img.total() * img.elemSize();

                //send processed image
                if ((bytes = send(socket, img.data, imgSize, 0)) < 0){
                     std::cerr << "bytes = " << bytes << std::endl;
                     break;
                }
    }

}
*/