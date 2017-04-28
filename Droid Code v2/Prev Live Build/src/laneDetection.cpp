
#include "laneDetection.h"


#include <opencv/highgui.h>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cmath>

using namespace cv;
using namespace std;

Mat detected_edges;
int lowThreshold = 50; // x to 100
int	kernel_size = 3;
int ratio = 2;
Mat element2 = getStructuringElement(MORPH_RECT,
	Size(3, 3),
	Point(2, 2));
	
	//trackbar variables
	int cannyCount = 150;
	int cannyCount2 = 120;
	int rho = 1;
	int hAngle = 1;
	//int h1 = 50;
	//int h2 = 50;
	//int h3 = 10;
	
	
	int h1 = 17;
	int h2 = 28;
	int h3 = 25;
	
	
Point findPoint (vector<vector<Point> > contours, Point midPoint){
	Point selectedPoint(-1,-1);
	for (int i=0; i < contours.size(); i++){		
		//cout<< "loop Internal: "<<i<<endl; 
		for (int j=0; j < contours[i].size(); j++){
			if (selectedPoint.x == -1){					
				selectedPoint = contours[i][j];
			}
			//find point that is closest to lane section midpoint	
			if (abs(contours[i][j].y - midPoint.y ) < abs(selectedPoint.y - midPoint.y)){
				//point is closer
				//cout <<"i:" << i << " blu:" << contoursBlu[i][j] << endl;
				selectedPoint = contours[i][j];
			}									
		}
	}	
	return selectedPoint;
}

//takes the result from HoughP and compares horizontially each line to a mid point to find the line closest.
void intersection(vector<Vec4i> lines, Point midPoint, int offset, Mat& img, Point& returnLP, Point& returnRP){
	vector<Point> points;
	Point selectedPoint(-1,-1), intersection;
	bool inter(false);

	for(size_t i = 0;i<lines.size();i++){
		Vec4i l = lines[i];
		l[1] += offset;
		l[3] += offset;
		double A = l[3] - l[1];
		double B = l[0] - l[2];
		double C = (l[0]*A) + (l[1]*B);
		double mA = midPoint.y - midPoint.y; // Y value doesn't change
		double mB = 1 - img.cols;
		double mC = (0 * mA) + (midPoint.y * mB);

		double det = (A*mB) - (mA*B);
		if (det == 0){
		 //lines are paralell.
		}else {
			int x = ((mB * C) - (B*mC))/det;
			int y = ((A*mC) - (mA*C))/det;
			//make sure the intersection is within the line segments.			
			if ((min(l[0],l[2]) <= x && x <= max(l[0],l[2])) && (min(l[1],l[3]) <= y && y <= max(l[1],l[3]))){
			//	cout << "Point Found! " << Point(x,y) <<endl;
				points.push_back (Point(x,y));
				circle(img ,Point(x,y) ,4, Scalar(0,0,0),2);
			}
		}
	}
	
	for (int i=0; i < points.size(); i++){		
		//loop through all points, find points that are closest to the midPoint.
		if (points[i].x < midPoint.x && abs(points[i].x - midPoint.x ) < abs(returnLP.x - midPoint.x)){//lefthandside
			returnLP = points[i];
		}else if (points[i].x > midPoint.x && abs(points[i].x - midPoint.x ) < abs(returnRP.x - midPoint.x)){//righthandside
			returnRP = points[i];
	}
	
	
	
		/*//cout<< "loop Internal: "<<i<<endl; 
		for (int j=0; j < points[i].size(); j++){
			
			if (selectedPoint.x == -1){					
				selectedPoint = points[i][j];
			}
			//find point that is closest to lane section midpoint	
			if (abs(contours[i][j].y - midPoint.y ) < abs(selectedPoint.y - midPoint.y)){
				//point is closer
				//cout <<"i:" << i << " blu:" << contoursBlu[i][j] << endl;
				selectedPoint = contours[i][j];
			}									
		}
		*/
	}	
	
}

	
vector<Point> detectLane(vector<vector<Point> > contours, Point midPoint){
		vector<Point> output;
		Point selectedPoint1 = findPoint(contours, midPoint);
		Point selectedPoint2 = findPoint(contours, Point(midPoint.x,midPoint.y-30));
		Point selectedPoint3 = findPoint(contours, Point(midPoint.x,midPoint.y-60));
		
}

void on_trackbar(int, void*){
	
	
}



// an attempt to use CHEVP algorthim to determine the vanishing point.
void lineDetection(Mat input, vector<Point>& laneL, vector<Point>& laneR){
Mat workImg;
//run canny edge mape and edge orientation map
	cvtColor(input, workImg, COLOR_BGR2GRAY);  
	Mat detectedEdges;
	
	Canny(workImg, workImg, cannyCount, cannyCount2, 3);
	//cout << cannyCount <<endl;
	//cannyCount = cannyCount % 150;
	//cannyCount += 10;
	
	
	imshow("canny",workImg);

	Mat img1, img2, img3, img4, img5;
	img1 = workImg(Rect(0,0,workImg.cols,200));
	img2 = workImg(Rect(0,200,workImg.cols,200));
	img5 = workImg(Rect(0,400,workImg.cols,100));
	img3 = workImg(Rect(0,500,workImg.cols,100));
	img4 = workImg(Rect(0,600,workImg.cols,120));
//	img5 = workImg(Rect(0,200,workImg.cols,workImg.rows));
	//run hough transform to find line segement.
	
//	imshow("roi",img1);
//	imshow("roi2",img2);
//	imshow("roi3",img3);
//	imshow("roi4",img4);

	createTrackbar("canny1","canny", &cannyCount, 150, on_trackbar);
	createTrackbar("canny2","canny", &cannyCount2, 150, on_trackbar);
	float downAngle = 30*CV_PI/180;
	float upAngle = 60*CV_PI/180;
	Point midPoint;
	int offset;
	Point pointL(-1,-1), pointR(-1,-1);
	
	vector<Vec4i> lines;
	HoughLinesP(img4, lines, rho,(CV_PI/180)*hAngle,h1,h2,h3);
	for(size_t i = 0;i<lines.size();i++){
		Vec4i l = lines[i];
			line(input,Point(l[0],l[1]+600), Point(l[2],l[3]+600),Scalar(0,0,255),3,CV_AA);
	}
	midPoint = Point(700,650);
	offset = 600;
	circle(input,midPoint,4, Scalar(0,0,255),3);
	intersection(lines, midPoint, offset, input, pointL, pointR);
	laneL.push_back(pointL);
	laneR.push_back(pointR);
	line(input, Point(0,midPoint.y),Point(input.cols,midPoint.y),Scalar(255,255,255),3,CV_AA);
	
	
	//reset variables.
	lines.clear();
	pointL =Point(-1,-1);
	pointR =Point(-1,-1);
	midPoint = Point(700,550);
	offset = 500;
	
	HoughLinesP(img3, lines, rho,(CV_PI/180)*hAngle,h1,h2,h3);
	for(size_t i = 0;i<lines.size();i++){
		Vec4i l = lines[i];	
		line(input,Point(l[0],l[1]+offset), Point(l[2],l[3]+offset),Scalar(0,0,255),3,CV_AA);
	}
	
	
	circle(input,midPoint,4, Scalar(0,0,255),3);
	intersection(lines, midPoint, offset, input, pointL, pointR);
	laneL.push_back(pointL);
	laneR.push_back(pointR);
	line(input, Point(0,midPoint.y),Point(input.cols,midPoint.y),Scalar(255,255,255),3,CV_AA);
	
	//reset variables.
	lines.clear();
	pointL =Point(-1,-1);
	pointR =Point(-1,-1);
	midPoint = Point(700,250);
	offset = 200;
	/*
	HoughLinesP(img2, lines, rho,(CV_PI/180)*hAngle,h1,h2,h3);
	for(size_t i = 0;i<lines.size();i++){
		Vec4i l = lines[i];
			line(input,Point(l[0],l[1]+offset), Point(l[2],l[3]+offset),Scalar(0,0,255),3,CV_AA);
	}
	
	
	circle(input,midPoint,4, Scalar(0,0,255),3);
	intersection(lines, midPoint, offset, input, pointL, pointR);
	laneL.push_back(pointL);
	laneR.push_back(pointR);
	line(input, Point(0,midPoint.y),Point(input.cols,midPoint.y),Scalar(255,255,255),3,CV_AA);
	*/
	//reset variables
	lines.clear();
	pointL =Point(-1,-1);
	pointR =Point(-1,-1);
	midPoint = Point(700,450);
	offset = 400;
	
	HoughLinesP(img5, lines, rho,(CV_PI/180)*hAngle,h1,h2,h3);
	for(size_t i = 0;i<lines.size();i++){
		Vec4i l = lines[i];
			line(input,Point(l[0],l[1]+offset), Point(l[2],l[3]+offset),Scalar(0,0,255),3,CV_AA);
	}
	
	circle(input,midPoint,4, Scalar(0,0,255),3);
	intersection(lines, midPoint, offset, input, pointL, pointR);
	laneL.push_back(pointL);
	laneR.push_back(pointR);
	line(input, Point(0,midPoint.y),Point(input.cols,midPoint.y),Scalar(255,255,255),3,CV_AA);
	
	
	//show output
	resize(input,input,Size(input.cols/1.5, input.rows/1.5));
	imshow("hough", input);
    createTrackbar("rho","hough", &rho, 100, on_trackbar);
	createTrackbar("Angle","hough", &hAngle, 150, on_trackbar);
	createTrackbar("threshold","hough", &h1, 150, on_trackbar);
	createTrackbar("minLin","hough", &h2, 200, on_trackbar);
	createTrackbar("maxLin","hough", &h3, 200, on_trackbar);
}













