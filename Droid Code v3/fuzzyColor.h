#ifndef FUZZYCOLOR_H
#define FUZZYCOLOR_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "colorClassifier.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

class FuzzyColor{

public:
	FuzzyColor();

	void processFuzzyColor(Mat& frame, Mat& outputImg);
	void processSingleFuzzyColor(Mat& img, Mat& outputImg, String color);
private:

	void multi_colourClassify_rg_3(ColorClassifier color1,ColorClassifier color2,ColorClassifier color3,Mat &sourceImg, Mat &destImg);
	void single_colourClassify_rg(ColorClassifier color1, Mat &sourceImg, Mat &destImg);
	void laneDetection();

	 float GetAngle(float ChR, float ChG);
	 int FuzzyEnhanceX(int x, float Threshold, short N);
	 int FuzzyDegradeX(int x, float Threshold, short N);
	void intersection(Point midPoint, int offset, Mat& img, Point& returnLP, Point& returnRP);
};



#endif
