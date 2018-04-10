#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int processFrame(VideoCapture& Camera);


int main(int argc, char** argv)
{
	VideoCapture cap;
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.

	/*
	// to open the default web camera
	if (!cap.open(0)){
		cout << "Camera Is not able to be Opened, is it connected?" << endl;
		return 0;
		cin.ignore();
		}
	*/

	// to open video file
	if (!cap.open(0)){
		cout << "video file Is not able to be Opened, does it exist?" << endl;
		cin.ignore();
		return 0;
	}

	//process the camera input
	processFrame(cap);
	// the camera will be closed automatically upon exit
	// cap.close();
	return 0;
}

//process input
int processFrame(VideoCapture& camera){
	for (;;)
	{
		Mat frame; // matrix container for image frame
		camera >> frame; //grab the next frame of video in sequence
		resize(frame, frame, Size(1080, 720));
		imshow("this is you, smile! :)", frame); // display the frame for the user to view

		if (waitKey(1) == 27) break; // stop capturing by pressing ESC
	}
	return 0;
}
