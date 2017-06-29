// droidcarV3.cpp : Defines the entry point for the console application. Made for 2017 droid racing competition
//
#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <numeric>


#include "fuzzyColor.h"


using namespace cv;
using namespace std;


struct Directions {
	int speed = 0;
	int angle = 0;

	int prevSpeed = 0;
	int prevAngle = 0;
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
int processImage(Mat& frame);
void laneDetect(Mat& input, Directions& D);
int processDirections(Directions& D);
vector<Point> findPoints(vector<vector<Point> > contours, Point midPoint);
Point intersection(vector<Point> contour, Point midPoint);
vector<Point>  contourBounds(vector<vector<Point> >& contours, Mat& input);

FuzzyColor fuzzyColor;
Size frameSize;

int main(int argc, char** argv)
{
	VideoCapture cap;
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.

	
	// to open the default web camera
	if (!cap.open(0)){
	cout << "Camera Is not able to be Opened, is it connected?" << endl;
	return 0;
	cin.ignore();
	}
	
	/*
	// to open video file
	if (!cap.open("Video_1.mp4")) {
		cout << "video file Is not able to be Opened, does it exist?" << endl;
		cin.ignore();
		return 0;
	}
	*/
	//process the camera input
	
	processVideo(cap);
	

	//testing call

	// the camera will be closed automatically upon exit
	// cap.close();
	return 0;
}

//process input
int processVideo(VideoCapture& camera) {
	for (;;)
	{
		Mat frame; // matrix container for image frame
		camera >> frame; //grab the next frame of video in sequence
		if (frame.empty()) break; // end of video stream
		processImage(frame);

								  //do stuff with the image,

	/*	Mat edges;
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges); // display the frame for the user to view

		imshow("this is you, smile! :)", frame); // display the frame for the user to view
		*/
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
int processImage(Mat& frame) {

	//while (true) {
		//frame = imread("screenshots/light.png");
		frameSize = frame.size();
		//resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
		//circle(frame, Point(frame.cols / 2, frame.rows/2), 5, Scalar(0, 0, 0),5);

		imshow("frame", frame);
		Mat edges, frame_thresholdBlue, frame_thresholdYellow, frame_hsv;
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(2, 2));

		Mat fuzzyImg, fuzzyEdges;
		fuzzyColor.processFuzzyColor(frame, fuzzyImg);
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
		imshow("edges", edges); // display the frame for the user to view
		
		Mat bitwiseImg;
		bitwise_or(frame_thresholdBlue, frame_thresholdYellow, bitwiseImg);
		erode(bitwiseImg, bitwiseImg, element, Point(1, 1), 1);
		dilate(bitwiseImg, bitwiseImg, element, Point(1, 1), 2);
		imshow("bluandYel", bitwiseImg);
		bitwise_and(bitwiseImg, edges, bitwiseImg);


		//fuzzy
		bitwise_and(fuzzyEdges, edges, fuzzyEdges);
		imshow("fuzzyEdges", fuzzyEdges);

/*
		std::vector<std::vector<Point> > contours;
		std::vector<Vec4i> hierarchy;
		findContours(bitwiseImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		Mat contoursImg;
		RNG rng(12345);
		vector<double> lengths;
		for (int i = 0; i < contours.size(); i++)
		{
			lengths.push_back(arcLength(contours[i], false));
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(frame, contours, i, color);

		}
*/
		Directions D;
		laneDetect(fuzzyEdges, D);



		imshow("contours", frame);
		imshow("combined", bitwiseImg);


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
void laneDetect(Mat& input, Directions& D) {
	Mat circleImg(frameSize, CV_THRESH_BINARY);
	int st = input.cols;
	//split the image up into sections
	Mat img1, img2, img3, img4;
	
	Rect Ra1(0, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Ra2(frameSize.width / 4, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Rb1(0, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);
	Rect Rb2(frameSize.width / 4, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);

	Rect Rc1((frameSize.width / 4) * 3, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Rc2(frameSize.width / 2, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Rd1((frameSize.width / 4) * 3, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);
	Rect Rd2(frameSize.width / 2, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);


	rectangle(circleImg, Ra1, Scalar(255, 255, 255));
	rectangle(circleImg, Ra2, Scalar(255, 255, 255));
	rectangle(circleImg, Rb1, Scalar(255, 255, 255));
	rectangle(circleImg, Rb2, Scalar(255, 255, 255));
	rectangle(circleImg, Rc1, Scalar(255, 255, 255));
	rectangle(circleImg, Rc2, Scalar(255, 255, 255));
	rectangle(circleImg, Rd1, Scalar(255, 255, 255));
	rectangle(circleImg, Rd2, Scalar(255, 255, 255));
	
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	findContours(input, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > intersections;
	
	vector<Point> circles = contourBounds(contours, input);
	for (int i = 0; i < circles.size(); i++)
	{
		circle(circleImg, circles[i], 1, Scalar(255, 255, 255), 2);
	}


	
	for (int i = input.rows; i > input.rows/2; i=i-1) {
		circle(circleImg, Point(input.cols / 2, i), 1, Scalar(100, 100, 100), 1);
	//	intersections.push_back(findPoints(contours, Point(input.cols / 2, i)));
	}
	
	/*for (int i = 0; i < intersections.size(); i++) {
		for (int j = 0; j < intersections[i].size(); j++) {
			circle(input, intersections[i][j], 2, Scalar(255, 255, 255), 5);
		}
	}*/
	imshow("lanedetection", circleImg);
	
	D.angle = 75;
	D.speed = 100;

}

/* Process the determined directions and send them to the motor
	use the motor serial classes made by duncan.

*/
int processDirections(Directions& D) {


	D.angle = 75;
	D.speed = 100;

	return 0;
}

/*
Point findPoint(vector<vector<Point> > contours, Point midPoint) {
	Point selectedPoint(-1, -1);
	for (int i = 0; i < contours.size(); i++) {
		//cout<< "loop Internal: "<<i<<endl; 
		for (int j = 0; j < contours[i].size(); j++) {
			if (selectedPoint.x == -1) {
				selectedPoint = contours[i][j];
			}
			//find point that is closest to lane section midpoint	
			if (abs(contours[i][j].y - midPoint.y) < abs(selectedPoint.y - midPoint.y)) {
				//point is closer
				//cout <<"i:" << i << " blu:" << contoursBlu[i][j] << endl;
	
	selectedPoint = contours[i][j];
			}
		}
	}
	return selectedPoint;
}
*/
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
	/*
	for (int i = 0; i < contours.size(); i++) {
		//check if contour contains the y value
		//Point Cstart = contours[i].begin();
		//if (midPoint.y >= contours[i].begin() && midPoint.y <= contours[i].end()))
		for (int j = 0; j < contours[i].size(); j++)
		{
			if (direction == 0) {
				if (abs(contours[i][j].y - midPoint.y) < abs( - midPoint.y)) {
					//point is closer
					//cout <<"i:" << i << " blu:" << contoursBlu[i][j] << endl;
					//selectedPoint = contours[i][j];
				}
			}


		}
	}
	*/	
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
			}else if (contours[i][j].y >= lyBound.y) {
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

vector<Point>  contourBounds(vector<vector<Point> >& contours, Mat& input) {
	Mat singleContour(frameSize, CV_8U);
	int Ca1 = 0, Ca2 = 0, Cb1 = 0, Cb2 = 0, Cc1 = 0, Cc2 = 0, Cd1 = 0, Cd2 = 0;
	Rect Ra1(0, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Ra2(frameSize.width / 4, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Rb1(0, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);
	Rect Rb2(frameSize.width / 4, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);

	Rect Rc1((frameSize.width / 4) * 3, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Rc2(frameSize.width / 2, frameSize.height / 2, frameSize.width / 4, frameSize.height / 4);
	Rect Rd1((frameSize.width / 4) * 3, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);
	Rect Rd2(frameSize.width / 2, (frameSize.height / 4) * 3, frameSize.width / 4, frameSize.height / 4);

	vector<Point> circles;
	int spacing = 10;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++)
		{
			
			if (contours[i][j].y % spacing == 0 && contours[i][j].y > input.rows /2 ) {
				//double Rsqr = leastSqrRegression(contours[i]);
				//if (Rsqr >= 0.9)
					//drawContours(singleContour, contours, i, Scalar(255, 255, 255), 1);
				if (Ra1.contains(contours[i][j]))
					Ca1++;
				else if (Ra2.contains(contours[i][j]))
					Ca2++;
				else if (Rb1.contains(contours[i][j]))
					Cb1++;
				else if (Rb2.contains(contours[i][j]))
					Cb2++;
				else if (Rc1.contains(contours[i][j]))
					Cc1++;
				else if (Rc2.contains(contours[i][j]))
					Cc2++;
				else if (Rd1.contains(contours[i][j]))
					Cd1++;
				else if (Rd2.contains(contours[i][j]))
					Cd2++;
				circles.push_back(contours[i][j]);
			}
		}
	}
	
	// Adding weights to counts
	int wa1 = 1;
	int wa2 = 3;
	int wb1 = 2;
	int wb2 = 5;
	int wc1 = 1;
	int wc2 = 3;
	int wd1 = 2;
	int wd2 = 5;


	// Weighted quads
	 Ca1 *= wa1;
	 Ca2 *= wa2;
	 Cb1 *= wb1;
	 Cb2 *= wb2;
	 Cc1 *= wc1;
	 Cc2 *= wc2;
	 Cd1 *= wd1;
	 Cd2 *= wd2;

	// Average the left/right
	double left = (Ca1 + Ca2 + Cb1 + Cb2) / 4;
	double right = (Cc1 + Cc2 + Cd1 + Cd2) / 4;
	cout << left << " " << right << " " << left-right<<endl;
	//	imshow("singleContour", singleContour);
	cout << Ca1 << " " << Ca2 << " " << Cb1 << " " << Cb2 << " " << Cc1 << " " << Cc2 << " " << Cd1 << " " << Cd2 << " " << endl;

	return circles;
}
double slope( vector<double>& x,  vector<double>& y) {
	if (x.size() != y.size()) {
		throw exception("...");
	}
	double n = x.size();

	// Get average of x and y
	double avgX = accumulate(x.begin(), x.end(), 0.0) / n;
	double avgY = accumulate(y.begin(), y.end(), 0.0) / n;

	// Assume num and denom
	double numerator = 0.0;
	double denominator = 0.0;

	// Apply regression
	for (int i = 0; i<n; ++i) {
		numerator += (x[i] - avgX) * (y[i] - avgY);
		denominator += (x[i] - avgX) * (x[i] - avgX);
	}

	if (denominator == 0) {
		throw exception("...");
	}

	// Square means (fitness)


	// Return the gradient
	return numerator / denominator;
}

void linearRegression(vector<vector<Point> >& contours) {
	for (size_t i = 0; i < contours.size(); i++){ // Go through each contour
		vector<double> x, y;

		for (size_t j = 0; j < contours[i].size(); j++) { // Going through each point on the contour
			x.push_back(contours[i][j].x);
			y.push_back(contours[i][j].y);
		}
		slope(x, y);
	}
}

double leastSqrRegression(vector<Point> xyCollection){
	vector<double> x, y;
	int dataSize = xyCollection.size();
	for (size_t j = 0; j < xyCollection.size(); j++) { // Going through each point on the contour
		x.push_back(xyCollection[j].x);
		y.push_back(xyCollection[j].y);
	}
	
	double SUMx = 0;     //sum of x values
	double SUMy = 0;     //sum of y values
	double SUMxy = 0;    //sum of x * y
	double SUMxx = 0;    //sum of x^2
	double SUMres = 0;   //sum of squared residue
	double res = 0;      //residue squared
	double slope = 0;    //slope of regression line
	double y_intercept = 0; //y intercept of regression line
	double SUM_Yres = 0; //sum of squared of the discrepancies
	double AVGy = 0;     //mean of y
	double AVGx = 0;     //mean of x
	double Yres = 0;     //squared of the discrepancies
	double Rsqr = 0;     //coefficient of determination
						//calculate various sums 
	
	
	for (int i = 0; i < xyCollection.size(); i++)
	{
		//sum of x
		SUMx += x[i];
		//sum of y
		SUMy += y[i];
		//sum of squared x*y
		SUMxy += x[i] * y[i];
		//sum of squared x
		SUMxx += x[i] * x[i];
	}

	//calculate the means of x and y
	AVGy = SUMy / dataSize;
	AVGx = SUMx / dataSize;

	//slope or a1
	slope = (dataSize * SUMxy - SUMx * SUMy) / (dataSize * SUMxx - SUMx*SUMx);

	//y itercept or a0
	y_intercept = AVGy - slope * AVGx;

	/*printf("x mean(AVGx) = %0.5E\n", AVGx);
	printf("y mean(AVGy) = %0.5E\n", AVGy);

	printf("\n");
	printf("The linear equation that best fits the given data:\n");
	printf("       y = %2.8lfx + %2.8f\n", slope, y_intercept);
	printf("------------------------------------------------------------\n");
	printf("   Original (x,y)   (y_i - y_avg)^2     (y_i - a_o - a_1*x_i)^2\n");
	printf("------------------------------------------------------------\n");
*/
	//calculate squared residues, their sum etc.
	for (int i = 0; i < dataSize; i++)
	{
		//current (y_i - a0 - a1 * x_i)^2
		Yres = pow((xyCollection[i].y - y_intercept - (slope * xyCollection[i].x)), 2);

		//sum of (y_i - a0 - a1 * x_i)^2
		SUM_Yres += Yres;

		//current residue squared (y_i - AVGy)^2
		res = pow(xyCollection[i].y - AVGy, 2);

		//sum of squared residues
		SUMres += res;

		//printf("   (%0.2f %0.2f)      %0.5E         %0.5E\n",	xyCollection[i].x, xyCollection[i].y, res, Yres);
	}

	//calculate r^2 coefficient of determination
	Rsqr = (SUMres - SUM_Yres) / SUMres;

	//printf("--------------------------------------------------\n");
	//printf("Sum of (y_i - y_avg)^2 = %0.5E\t\n", SUMres);
	//printf("Sum of (y_i - a_o - a_1*x_i)^2 = %0.5E\t\n", SUM_Yres);
	//printf("Standard deviation(St) = %0.5E\n", sqrt(SUMres / (dataSize - 1)));
	//printf("Standard error of the estimate(Sr) = %0.5E\t\n", sqrt(SUM_Yres / (dataSize - 2)));
	//printf("Coefficent of determination(r^2) = %0.5E\t\n", (SUMres - SUM_Yres) / SUMres);
	//printf("Correlation coefficient(r) = %0.5E\t\n", sqrt(Rsqr));
	
	return Rsqr;
}

