
#include "colorDetection.h"
#include <opencv/highgui.h>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

// inner lane is blue
Scalar bluMin(92, 60, 40);
Scalar bluMax(124, 256, 256);

// outter lane is yellow
Scalar yelMin(20, 90, 160);
Scalar yelMax(60, 256, 256);

//obstacles are purple
Scalar purMin(92, 0, 0);
Scalar purMax(124, 256, 256);

// competitors are red
Scalar redMin(0, 150, 110);
Scalar redMax(10, 255, 255);

Scalar redMin2(160, 150, 110);
Scalar redMax2(175, 255, 255);

//Scalar greMin(40, 104.55, 84);
//Scalar greMax(67, 255, 255);

Scalar greMin(40, 90, 20);
Scalar greMax(90, 255, 255);


Scalar drawColor(255, 0, 0);
Mat element = getStructuringElement(MORPH_RECT,
	Size(3, 3),
	Point(2, 2));


vector<vector<Point>> colorDetection(Mat input, String color){
	Scalar hsvMin, hsvMax;
	// poor man's version of a switch statement.
	if (color.compare("blu")==0){
		hsvMin = bluMin;
		hsvMax = bluMax;
		drawColor= Scalar(255,0,0);

	}
	else if (color.compare("yel") == 0){
		hsvMin = yelMin;
		hsvMax = yelMax;
		drawColor = Scalar(0, 255, 255);
	}
	else if (color.compare("pur") == 0){
		hsvMin = purMin;
		hsvMax = purMax;
		drawColor = Scalar(255, 0, 0);
	}
	else if (color.compare("red") == 0){
		hsvMin = redMin;
		hsvMax = redMax;
		drawColor = Scalar(0, 0, 255);
	}
	else if (color.compare("gre") == 0){
		hsvMin = greMin;
		hsvMax = greMax;
		drawColor = Scalar(0, 0, 255);
	}
	else {//default case
		hsvMin = bluMin;
		hsvMax = bluMax;
		drawColor = Scalar(0, 0, 255);
	}

	inRange(input, hsvMin, hsvMax, input);
	cv::erode(input, input, element, Point(-1, -1), 2);
	cv::dilate(input, input, element, Point(-1, -1), 1);
	vector<vector<Point> > contours0, contours;
	//Mat imgContour = input.clone();
	Mat imgContour(input);

	cv::findContours(imgContour, contours0, RETR_TREE, CHAIN_APPROX_NONE);
	for (size_t j = 0; j < contours0.size(); j++){
		double area = contourArea(contours0[j]);
		if (area < 20){
			contours0.erase(contours0.begin() + j);
		}
		else{
			//cout << "area: " << area << endl;
		}
	}
	contours.resize(contours0.size());
	for (size_t k = 0; k < contours0.size(); k++){
		double area = contourArea(contours0[k]);
		if (area > 10){
			approxPolyDP(Mat(contours0[k]), contours[k], 3, true);
		}
	}
	//drawContours(input, contours, -1, Scalar(128, 255, 255), 3, LINE_4);
	return contours;
}
 
Mat cutImage(Mat input, String color){
	Scalar hsvMin, hsvMax;
	
	// poor man's version of a switch statement.
	if (color.compare("blu") == 0){
		hsvMin = bluMin;
		hsvMax = bluMax;
		drawColor = Scalar(255, 0, 0);

	}
	else if (color.compare("yel") == 0){
		hsvMin = yelMin;
		hsvMax = yelMax;
		drawColor = Scalar(0, 255, 255);
	}
	else if (color.compare("pur") == 0){
		hsvMin = purMin;
		hsvMax = purMax;
		drawColor = Scalar(255, 0, 0);
	}
	else if (color.compare("red") == 0){
		hsvMin = redMin;
		hsvMax = redMax;
		drawColor = Scalar(0, 0, 255);
	}
	else if (color.compare("gre") == 0){
		hsvMin = greMin;
		hsvMax = greMax;
		drawColor = Scalar(0, 0, 255);
	}
	else {//default case
		hsvMin = bluMin;
		hsvMax = bluMax;
		drawColor = Scalar(0, 0, 255);
	}
	Mat output;
	inRange(input, hsvMin, hsvMax, output);
	cv::erode(output, output, element, Point(-1, -1), 1);
	//cv::dilate(output, output, element, Point(-1, -1), 1);
	//vector<vector<Point> > contours0, contours;
	//Mat imgContour = input.clone();
	return output;
}
