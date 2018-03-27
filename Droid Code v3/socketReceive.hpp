#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#define CV_H
#include <iostream>
#include <conio.h>
#include "UDPClient.h"
using namespace std;
using namespace cv;

class SocketReceive{
public:
	char *addr = "127.0.0.1";
	int UDPMAX = 65507;
	int port = 5000;

	SocketReceive::SocketReceive();
	int SocketReceive::connect();
	Mat SocketReceive::getFrame();
private:
	UDPClient *client;
	char *buff;
	vector<uchar> videoBuffer;
};

SocketReceive::SocketReceive(){
	UDPClient *client = new UDPClient(port);
	char *buff = (char*)malloc(UDPMAX);

};

int SocketReceive::connect(){
	cout << "=== VIDEO RECEIVER ===" << endl;

	//setup UDP client
	client = new UDPClient(port);
	char *buff = (char*)malloc(UDPMAX);
	
	return 0;
}

Mat SocketReceive::getFrame(){
	int result = client->receiveData(buff, UDPMAX);
	if (result < 0){
		cout << "Failed to receive frame." << endl;
	}
	cout << "Got a frame of size " << result << endl;
	videoBuffer.resize(result);
	memcpy((char*)(&videoBuffer[0]), buff, result);

	Mat jpegimage = imdecode(Mat(videoBuffer), CV_LOAD_IMAGE_COLOR);
	IplImage img = jpegimage;
	return jpegimage;
}

