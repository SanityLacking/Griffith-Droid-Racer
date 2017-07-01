// UDPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#define CV_H
#include <iostream>
#include <conio.h>
#include "UDPClient.h"
using namespace std;
using namespace cv;

char *addr = "127.0.0.1";
int UDPMAX = 65507;
int port = 5000;

int main(){
	cout << "=== VIDEO RECEIVER ===" << endl;

	//setup UDP client
	UDPClient *client = new UDPClient(port);
	char *buff = (char*)malloc(UDPMAX);

	//setup openCV
	cvNamedWindow("UDP Video Receiver", CV_WINDOW_AUTOSIZE);
	vector<uchar> videoBuffer;

	while (1){
		//read data
		int result = client->receiveData(buff, UDPMAX);
		if (result < 0){
			cout << "Failed to receive frame." << endl;
			continue;
		}
		cout << "Got a frame of size " << result << endl;

		videoBuffer.resize(result);
		memcpy((char*)(&videoBuffer[0]), buff, result);

		//reconstruct jpeg and display it
		Mat jpegimage = imdecode(Mat(videoBuffer), CV_LOAD_IMAGE_COLOR);
		IplImage img = jpegimage;
		cvShowImage("UDP Video Receiver", &img);

		cvWaitKey(5);
	}
}