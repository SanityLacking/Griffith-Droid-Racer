// droidcarV3.cpp : Defines the entry point for the console application. Made for 2017 droid racing competition
//
#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>


// Maths!
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#define _USE_MATH_DEFINES // for C  
#include <math.h>  


//socket file
#define CV_H
#include <iostream>
#include <conio.h>
#include "UDPClient.h"
#include <sstream>
//#include "socketReceive.hpp"

#include "fuzzyColor.h"



using namespace cv;
using namespace std;

//socket
char *addr = "127.0.0.1";
int UDPMAX = 65507;
int port = 5000;

char *returnAddr = "192.168.0.109";
int returnPort = 5001;


// Debug
bool debug = true;

// Ryoma's Functions
int convertTurningAngle(double angle);
int setSpeed(int angle);
double turningRadius(double angle);
double turningAngle(int left, int right);

// Ryoma's Variables
int refreshRate = 1;							// How many frames until next decision?

UDPClient *returnClient = new UDPClient(returnAddr, returnPort);

struct Directions {
	int speed = 0;
	int angle = 0;

	int prevSpeed = 0;
	int prevAngle = 0;
};

struct Sectors {
	int fCount = 0;
	double left = 0;
	double right = 0;
	double dLeft = 0;
	double dRight = 0;
};


class SingleContour {
public:
	vector<Point> contour;
	Point lyBound, hyBound, lxBound, hxBound;
	SingleContour::SingleContour() {
		lyBound, hyBound, lxBound, hxBound = Point(-1, -1);
	}
	SingleContour::SingleContour(vector<Point> C, Point lyB, Point hyB, Point lxB, Point hxB) {
		contour = C;
		lyBound = lyB;
		hyBound = hyB;
		lxBound = lxB;
		hxBound = hxB;
	}
};



double leastSqrRegression(vector<Point> xyCollection);
vector<SingleContour>  outerBounds(vector<vector<Point> >& contours);
int processVideo(VideoCapture& Camera);
int processVideo(Sectors& S);
//int processVideo(SocketReceive &socket);
int processImage(Mat& frame, Sectors& S);
void laneDetect(Mat& input, Directions& D, Sectors& S);
int processDirections(Directions& D);
vector<Point> findPoints(vector<vector<Point> > contours, Point midPoint);
Point intersection(vector<Point> contour, Point midPoint);
vector<Point>  contourBounds(vector<vector<Point> >& contours, Mat& input, double& left, double& right);
void emitDirections(Directions& d);
int angleConversion(int input);
FuzzyColor fuzzyColor;
Size frameSize;

int main(int argc, char** argv)
{
	VideoCapture cap;
	Sectors S;
	processVideo(S);
	return 0;
}
int processVideo(Sectors& S) {
	cout << "=== VIDEO RECEIVER ===" << endl;

	//setup UDP client for recieving
	UDPClient *client = new UDPClient(port);
	char *buff = (char*)malloc(UDPMAX);



	vector<uchar> videoBuffer;

	while (1) {
		//read data
		int result = client->receiveData(buff, UDPMAX);
		if (result < 0) {
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
		processImage(jpegimage, S);


		waitKey(1);
	}
	return 0;
}
int processVideo(VideoCapture& camera, Sectors& S) {
	for (;;)
	{
		Mat frame; // matrix container for image frame
		camera >> frame; //grab the next frame of video in sequence
		if (frame.empty()) break; // end of video stream
		processImage(frame, S);
		if (waitKey(1) == 27) break; // stop capturing by pressing ESC 
	}
	return 0;
}



/*
Image processing per frame
Processing steps
Reduce frame size
Cut out Static Region of interest if nessecary
change colour if nessecary
perform errode/dilate/blur
perform edge detection method: sobel, canny or other?
map between lines

*/
int processImage(Mat& frame, Sectors& S) {

	//while (true) {
	//frame = imread("screenshots/light.png");
	frameSize = frame.size();
	//resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
	//circle(frame, Point(frame.cols / 2, frame.rows/2), 5, Scalar(0, 0, 0),5);

	if (debug)
		imshow("frame", frame);
	Mat edges, frame_thresholdBlue, frame_thresholdYellow, frame_hsv;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(2, 2));

	Mat fuzzyImg, fuzzyEdges;
	fuzzyColor.processFuzzyColor(frame, fuzzyImg);
	if (debug)
		imshow("fuzzyColor", fuzzyImg);
	cvtColor(fuzzyImg, fuzzyEdges, COLOR_BGR2GRAY);
	blur(fuzzyEdges, fuzzyEdges, Size(3, 3));
	dilate(fuzzyEdges, fuzzyEdges, element, Point(1, 1), 2);


	cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
	cvtColor(frame, edges, COLOR_BGR2GRAY);


	inRange(frame_hsv, Scalar(92, 60, 40), Scalar(124, 256, 256), frame_thresholdBlue);
	erode(frame_thresholdBlue, frame_thresholdBlue, element, Point(1, 1), 1);
	inRange(frame_hsv, Scalar(20, 90, 160), Scalar(60, 256, 256), frame_thresholdYellow);

	//imshow("thresholdBlue", frame_thresholdBlue); // display the frame for the user to view

	//	imshow("thresholdYellow", frame_thresholdYellow); // display the frame for the user to view

	GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
	Canny(edges, edges, 0, 30, 3);
	if (debug)
		imshow("edges", edges); // display the frame for the user to view

	Mat bitwiseImg;
	bitwise_or(frame_thresholdBlue, frame_thresholdYellow, bitwiseImg);
	erode(bitwiseImg, bitwiseImg, element, Point(1, 1), 1);
	dilate(bitwiseImg, bitwiseImg, element, Point(1, 1), 2);
	if (debug)
		imshow("bluandYel", bitwiseImg);
	bitwise_and(bitwiseImg, edges, bitwiseImg);


	//fuzzy
	bitwise_and(fuzzyEdges, edges, fuzzyEdges);
	if (debug)
		imshow("fuzzyEdges", fuzzyEdges);
	Directions D;
	laneDetect(fuzzyEdges, D, S);
	emitDirections(D);


	if (debug) {
		imshow("contours", frame);
		imshow("combined", bitwiseImg);
	}


	//if (waitKey(1) == 27) break; // stop capturing by pressing ESC 
	//}

	return 0;
}


/* perform the lane detection on the now processed frame
output the driving instructions to the Struct

1. divide the image up into static sections
2. define spline along the center point
3. find intersection points to the lanes on both sides of the spline in each section
4. use a weighted system to determine the influence of the spline points in each ROI section. the further the section is from the car the less weight it has.
5. determine a direction set to drive the car.

*/

void laneDetect(Mat& input, Directions& D, Sectors& S) {
	Mat circleImg(frameSize, CV_8UC3);
	circleImg.setTo(Scalar(0, 0, 0));						// Set bg of image
	int st = input.cols;
	//split the image up into sections
	Mat img1, img2, img3, img4;

	// First row of rectangles
	Rect Ra1((frameSize.width / 8) * 0, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Ra2((frameSize.width / 8) * 1, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Ra3((frameSize.width / 8) * 2, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Ra4((frameSize.width / 8) * 3, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb4((frameSize.width / 8) * 4, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb3((frameSize.width / 8) * 5, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb2((frameSize.width / 8) * 6, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb1((frameSize.width / 8) * 7, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));

	// Second row of rectangles
	Rect Rc1(((frameSize.width / 8)) * 0, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rc2(((frameSize.width / 8)) * 1, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rc3(((frameSize.width / 8)) * 2, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rc4(((frameSize.width / 8)) * 3, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd4(((frameSize.width / 8)) * 4, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd3(((frameSize.width / 8)) * 5, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd2(((frameSize.width / 8)) * 6, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd1(((frameSize.width / 8)) * 7, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));


	rectangle(circleImg, Ra1, Scalar(255, 255, 255));
	rectangle(circleImg, Ra2, Scalar(255, 255, 255));
	rectangle(circleImg, Ra3, Scalar(255, 255, 255));
	rectangle(circleImg, Ra4, Scalar(255, 255, 255));
	rectangle(circleImg, Rb1, Scalar(255, 255, 255));
	rectangle(circleImg, Rb2, Scalar(255, 255, 255));
	rectangle(circleImg, Rb3, Scalar(255, 255, 255));
	rectangle(circleImg, Rb4, Scalar(255, 255, 255));
	rectangle(circleImg, Rc1, Scalar(255, 255, 255));
	rectangle(circleImg, Rc2, Scalar(255, 255, 255));
	rectangle(circleImg, Rc3, Scalar(255, 255, 255));
	rectangle(circleImg, Rc4, Scalar(255, 255, 255));
	rectangle(circleImg, Rd1, Scalar(255, 255, 255));
	rectangle(circleImg, Rd2, Scalar(255, 255, 255));
	rectangle(circleImg, Rd3, Scalar(255, 255, 255));
	rectangle(circleImg, Rd4, Scalar(255, 255, 255));

	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	findContours(input, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > intersections;

	double left = 0;
	double right = 0;
	vector<Point> circles = contourBounds(contours, input, left, right);
	for (int i = 0; i < circles.size(); i++)
	{
		circle(circleImg, circles[i], 1, Scalar(255, 255, 255), 2);
	}



	for (int i = input.rows; i > input.rows / 2; i = i - 1) {
		circle(circleImg, Point(input.cols / 2, i), 1, Scalar(100, 100, 100), 1);
		//	intersections.push_back(findPoints(contours, Point(input.cols / 2, i)));
	}

	// 75 Deg = straight. 
	
	/*
	if (S.fCount < refreshRate) {
		S.fCount++;
		// Simplify by using linear weights
		S.left += left * S.fCount;
		S.right += right * S.fCount;
	}
	else {
		// Update directions
		S.dLeft = S.left / S.fCount;
		S.dRight = S.right / S.fCount;

		// Reset fCount
		S.fCount = 0;
		S.left = 0;
		S.right = 0;
	}
	

	D.angle = turningAngle(S.dLeft, S.dRight);
	D.speed = setSpeed(D.angle);

	*/
	D.angle = turningAngle(left, right);
	D.speed = 100;
	int radius = turningRadius(D.angle);
	double scale = 14.2222;						// Magic number!
	radius *= scale;

	cout << "Left: " << left << " || Right: " << right << endl;
	/*
	cout << "S.fCount: " << S.fCount << endl;
	cout << "D.angle: " << D.angle << endl;
	cout << "D.speed: " << D.speed << endl;
	cout << "raidus: " << radius << endl;
	//*/
	if (D.angle == 75) {
		// Straight Line
		line(circleImg, Point(input.cols / 2, 0), Point(input.cols / 2, input.rows), Scalar(0, 0, 255), 1);
	}
	else {
		int centreX = 0;
		if (D.angle > 75) {
			centreX = (input.cols / 2) + radius;
		}
		else if (D.angle < 75) {
			centreX = (input.cols / 2) - radius;
		}
		circle(circleImg, Point(centreX, input.rows), radius, Scalar(0, 0, 255), 1);
	}

	// Add some text to img
	double fontSize = 0.5;
	int padding = 10;
	ostringstream oss;
	oss << "Decision Rate: " << refreshRate;
	String text = oss.str();

	Size textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, fontSize, 1, false);
	Point textOrg(0, textSize.height);
	putText(circleImg, text, textOrg, FONT_HERSHEY_SIMPLEX, fontSize, Scalar(255, 255, 255));

	ostringstream oss2;
	oss2 << "Angle: " << D.angle;
	text = oss2.str();
	textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, fontSize, 1, false);
	Point textOrg2(0, textSize.height * 2 + padding);
	putText(circleImg, text, textOrg2, FONT_HERSHEY_SIMPLEX, fontSize, Scalar(255, 255, 255));


	// Draw an image
	imshow("lanedetection", circleImg);
}


// This function converts the relative angle to 20-130 angles
int convertTurningAngle(double angle) {
	int result = 75;			// Centre is at 75 degrees
	int min = 20;				// Furthest left the car can turn
	int max = 130;				// Furthest right the car can turn
	if (angle < result) {
		result -= abs(angle);	// Left turn
		if (result <= min)
			result = min;
	}
	else {
		result += abs(angle);	// Right turn
		if (result >= max)
			result = max;
	}

	cout << "convertTurningAngle(): " << result << endl;
	return result;
}
/*
turningAngle() will calculate how much we need to turn left or right according to the dot density from filters
Negative angle = left turn
Positive angle = right turn
*/
double turningAngle(int left, int right) {
	double result = 0.0;

	// Something simple...
	if (left == 0 && right == 0){
		// Go straight
		result = 75;
	} else if (left == 0 && right > 0){
		result = 75 - abs(left - right);
		cout << "Turn left: " << result << endl;
	} else {
		result = 75 + abs(left - right); 
	}
	/*
	if (left != 0 && right != 0) {
		cout << "Left: " << left << " || Right: " << right << endl;
		result = left - right;
	}
	else {
		// Go straight
		result = 75;
	}
	*/
	result = convertTurningAngle(result);
	return result;
}

double turningRadius(double angle) {
	double result = 0.0;
	angle = abs(angle - 75);
	int length = 34;				// Wheelbase of the car

	if (angle == 0) {
		// Straight
		result = 0.0;
	}
	else {
		result = length / (2 * sin(angle * M_PI / 180));	// Because radians... (-_-)
	}

	return result;
}

int setSpeed(int angle) {
	int min = 100;					// Minimum speed the vehicle can go
	int max = 133;					// Maximum speed the vehicle can go
	int range = abs(max - min);		// The speed range
	int straight = 75;				// The angle for driving straight


	double percentAngle = abs((double)straight - angle) / straight;
	double speed = percentAngle * range;
	int result = max - speed;
	return result;
}

/* Process the determined directions and send them to the motor
use the motor serial classes made by duncan.

*/
int processDirections(Directions& D) {


	D.angle = 75;
	D.speed = 100;

	return 0;
}

/* direction 0 both directions, 1 left, 2 right.
*/
vector<Point> findPoints(vector<vector<Point> > contours, Point midPoint) {
	int direction = 0;
	vector<Point> selectedPoints;
	vector<SingleContour> newContours = outerBounds(contours);
	for (int i = 0; i < newContours.size(); i++)
	{
		if (midPoint.y <= newContours[i].hyBound.y && midPoint.y >= newContours[i].lyBound.y) {
			selectedPoints.push_back(intersection(newContours[i].contour, midPoint));
		}

	}
	return selectedPoints;
}

/*
generate outer bounds for contours
*/
vector<SingleContour>  outerBounds(vector<vector<Point> >& contours) {
	vector<SingleContour>  newContours;
	for (int i = 0; i < contours.size(); i++) {
		Point lyBound(-1, -1);
		Point hyBound(-1, -1);
		Point lxBound(-1, -1);
		Point hxBound(-1, -1);
		for (int j = 0; j < contours[i].size(); j++)
		{
			if (lyBound.y == -1) {
				lyBound = contours[i][j];
			}
			else if (contours[i][j].y >= lyBound.y) {
				lyBound = contours[i][j];
			}
			if (hyBound.y == -1) {
				hyBound = contours[i][j];
			}
			else if (contours[i][j].y <= hyBound.y) {
				hyBound = contours[i][j];
			}

			if (lxBound.x == -1) {
				lxBound = contours[i][j];
			}
			else if (contours[i][j].x <= lxBound.x) {
				lxBound = contours[i][j];
			}
			if (hxBound.x == -1) {
				hxBound = contours[i][j];
			}
			else if (contours[i][j].x >= hxBound.x) {
				hxBound = contours[i][j];
			}
		}
		SingleContour singleContour(contours[i], lyBound, hyBound, lxBound, hxBound);
		newContours.push_back(singleContour);
	}
	return newContours;
}


Point intersection(vector<Point> contour, Point midPoint) {
	Point output(-1, -1);
	if (abs(output.y - contour.front().y) >= abs(output.y - contour.back().y)) {//if closer to start, start there
		for (int i = 0; i < contour.size(); i++)
		{
			if (contour[i].y == midPoint.y) {
				output = contour[i];
			}
		}
	}
	else {// if closer to end, start there
		for (int i = contour.size(); i >= 0; i--)
		{
			if (contour[i].y == midPoint.y) {
				output = contour[i];
			}
		}
	}
	return output;
}
vector<Point>  contourBounds(vector<vector<Point> >& contours, Mat& input, double& left, double& right) {
	Mat singleContour(frameSize, CV_8U);
	int Ca1 = 0,
		Ca2 = 0,
		Ca3 = 0,
		Ca4 = 0,
		Cb1 = 0,
		Cb2 = 0,
		Cb3 = 0,
		Cb4 = 0,
		Cc1 = 0,
		Cc2 = 0,
		Cc3 = 0,
		Cc4 = 0,
		Cd1 = 0,
		Cd2 = 0,
		Cd3 = 0,
		Cd4 = 0;

	// First row of rectangles
	Rect Ra1((frameSize.width / 8) * 0, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Ra2((frameSize.width / 8) * 1, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Ra3((frameSize.width / 8) * 2, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Ra4((frameSize.width / 8) * 3, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb4((frameSize.width / 8) * 4, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb3((frameSize.width / 8) * 5, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb2((frameSize.width / 8) * 6, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rb1((frameSize.width / 8) * 7, (frameSize.height / 4) * 2, (frameSize.width / 8), (frameSize.height / 4));

	// Second row of rectangles
	Rect Rc1((frameSize.width / 8) * 0, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rc2((frameSize.width / 8) * 1, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rc3((frameSize.width / 8) * 2, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rc4((frameSize.width / 8) * 3, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd4((frameSize.width / 8) * 4, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd3((frameSize.width / 8) * 5, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd2((frameSize.width / 8) * 6, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));
	Rect Rd1((frameSize.width / 8) * 7, (frameSize.height / 4) * 3, (frameSize.width / 8), (frameSize.height / 4));


	vector<Point> circles;
	int spacing = 10;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++)
		{
			if (contours[i][j].y % spacing == 0 && contours[i][j].y > input.rows / 2) {
				//double Rsqr = leastSqrRegression(contours[i]);
				//if (Rsqr >= 0.9)
				//drawContours(singleContour, contours, i, Scalar(255, 255, 255), 1);
				if (Ra1.contains(contours[i][j]))
					Ca1++;
				else if (Ra2.contains(contours[i][j]))
					Ca2++;
				else if (Ra3.contains(contours[i][j]))
					Ca3++;
				else if (Ra4.contains(contours[i][j]))
					Ca4++;
				else if (Rb1.contains(contours[i][j]))
					Cb1++;
				else if (Rb2.contains(contours[i][j]))
					Cb2++;
				else if (Rb3.contains(contours[i][j]))
					Cb3++;
				else if (Rb4.contains(contours[i][j]))
					Cb4++;
				else if (Rc1.contains(contours[i][j]))
					Cc1++;
				else if (Rc2.contains(contours[i][j]))
					Cc2++;
				else if (Rc3.contains(contours[i][j]))
					Cc3++;
				else if (Rc4.contains(contours[i][j]))
					Cc4++;
				else if (Rd1.contains(contours[i][j]))
					Cd1++;
				else if (Rd2.contains(contours[i][j]))
					Cd2++;
				else if (Rd3.contains(contours[i][j]))
					Cd3++;
				else if (Rd3.contains(contours[i][j]))
					Cd3++;
				circles.push_back(contours[i][j]);
			}
		}
	}

	// Initialising weights to 0
	int wa1 = 0, wa2 = 0, wa3 = 0, wa4 = 0, wb1 = 0, wb2 = 0, wb3 = 0, wb4 = 0, wc1 = 0, wc2 = 0, wc3 = 0, wc4 = 0, wd1 = 0, wd2 = 0, wd3 = 0, wd4 = 0;


	// Choose weighting scheme
	int scheme = 1;
	switch (scheme) {
	case 1:
		wa1 = 1;
		wa2 = 2;
		wa3 = 3;
		wa4 = 5;
		wb4 = 5;
		wb3 = 3;
		wb2 = 2;
		wb1 = 1;
		wc1 = 2;
		wc2 = 3;
		wc3 = 0;
		wc4 = 0;
		wd4 = 0;
		wd3 = 0;
		wd2 = 3;
		wd1 = 2;
		break;
	default:
		wa1 = 1;
		wa2 = 1;
		wa3 = 3;
		wa4 = 3;
		wb4 = 3;
		wb3 = 3;
		wb2 = 1;
		wb1 = 1;
		wc1 = 2;
		wc2 = 2;
		wc3 = 5;
		wc4 = 5;
		wd4 = 5;
		wd3 = 5;
		wd2 = 2;
		wd1 = 2;
	}
	// Weighted Sectors
	Ca1 *= wa1;
	Ca2 *= wa2;
	Ca3 *= wa3;
	Ca4 *= wa4;
	Cb1 *= wb1;
	Cb2 *= wb2;
	Cb3 *= wb3;
	Cb4 *= wb4;
	Cc1 *= wc1;
	Cc2 *= wc2;
	Cc3 *= wc3;
	Cc4 *= wc4;
	Cd1 *= wd1;
	Cd2 *= wd2;
	Cd3 *= wd3;
	Cd4 *= wd4;

	// Average the left/right
	left = (Ca1 + Ca2 + Ca3 + Ca4 + Cc1 + Cc2 + Cc3 + Cc4) / 8;
	right = (Cb1 + Cb2 + Cb3 + Cb4 + Cd1 + Cd2 + Cd3 + Cd4) / 8;
	cout << left << " " << right << " " << left - right << endl;
	//	imshow("singleContour", singleContour);
	cout << Ca1 << " " << Ca2 << " " << Cb1 << " " << Cb2 << " " << Cc1 << " " << Cc2 << " " << Cd1 << " " << Cd2 << " " << endl;

	return circles;
}

/*
send the directions determined by the processor back to the pi.
The returnClient is defined at the start of the program.
TODO add the client it is supposed to use to this function to make it more independant.
*/
void emitDirections(Directions& D) {
	std::ostringstream oss;
	int angle = angleConversion(D.angle);
	int speed = 100; // D.speed
	oss << angle << "," << speed;
	string message = oss.str();
	std::vector<uchar> buff(message.c_str(), message.c_str() + message.length() + 1);
	int returnResult = returnClient->sendData((char*)(&buff[0]), buff.size());
	if (returnResult < 0)
		cout << "Failed to send." << endl;
	else
		cout << message << endl;

}
int angleConversion(int input) {
	int result;

	int temp = input - 75;
	result = 75 - temp;

	return result;
}