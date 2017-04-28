
//test

//standard include
#include <stdio.h>
#include <string.h>
#include <chrono>


//ZED include
#include <zed/Mat.hpp>
#include <zed/Camera.hpp>
#include <zed/utils/GlobalDefine.hpp>

//OpenCV include
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


//additional files
#include "colorDetection.h"
#include "laneDetection.h"
#include "jetsonGPIO.h"
#include "zedGPU.h"

// Cuda functions include
#include "kernel.cuh"




using namespace cv;
using namespace std;


int setupDroid();
int receiveInput();
int processInput();
//int decisionProcess(vector<Point> objects);
//int statusCheck();
void emitDirections();
int controlLoop();
void test();
int pAngle, pSpeed;
void findMidPoint(Point laneL, Point laneR, Point& midPoint,Point& lastMidPoint);
//globals
Point lastPointL(0,0), lastPointR(0,0);
Point midPoint(700,550), midPoint1(700,550), midPoint2(700,550), midPoint3(700,550);
Point lastMidPoint(700,550), lastMidPoint1(700,550), lastMidPoint2(700,550), lastMidPoint3(700,550);

Scalar midPointColor =(0,255,0);
int offsetL = 100, offsetR = 100;
int tolerance = 100;
int lastDirection=0;
int currentDirection=1;
int dcount=0;
int counter = 0;
int laneCounter =0;

jetsonGPIO serialStop = gpio160 ;
jetsonGPIO serialForward = gpio161 ;
jetsonGPIO serialReverse = gpio162 ;
jetsonGPIO serialLeft = gpio163 ;
jetsonGPIO serialRight = gpio164 ;
jetsonGPIO serialHardLeft = gpio165 ;
jetsonGPIO serialHardRight = gpio166 ;

char key = ' ';
    


//main Loop

int setupDroid(){
	
	gpioExport(serialStop);
	gpioExport(serialForward);
	gpioExport(serialLeft);
	gpioExport(serialRight);
	gpioExport(serialHardRight);
	gpioExport(serialHardLeft);
	gpioExport(serialReverse);
	
	gpioSetDirection(serialStop,outputPin) ;
	gpioSetDirection(serialForward,outputPin) ;
	gpioSetDirection(serialLeft,outputPin) ;
	gpioSetDirection(serialRight,outputPin) ;
	gpioSetDirection(serialHardRight,outputPin) ;
	gpioSetDirection(serialHardLeft,outputPin) ;
	gpioSetDirection(serialReverse,outputPin) ;
	
	return 1;	
}


int main(int argc, char **argv) {
   	
	
  cout << "program start" <<endl;

	//setup
	setupDroid();
	//controlLoop
	controlLoop();

}

int controlLoop(){
	bool run = true;
	sl::zed::Camera *zed;
    zed = new sl::zed::Camera(sl::zed::HD720);
	// init computation mode of the zed
    sl::zed:: ERRCODE err = zed->init(sl::zed::MODE::PERFORMANCE, -1, true); 
	// ERRCODE display
	cout << sl::zed::errcode2str(err) << endl;
	// Quit if an error occurred
	if (err != sl::zed::SUCCESS) {
		delete zed;
 		return 1;
	}		 
	zedGPU* zg =new zedGPU(zed);
	midPoint = Point(zg->width/2 - 50 ,zg->height/2 - 100);
	while(run){
		currentDirection = 0;
		Mat output,colorLines, colorHSV, imgROI;
		cv::Mat depthImg(zg->height, zg->width, CV_8UC4);
		cv::Mat colorImg(zg->height, zg->width, CV_8UC4);
		cv::Mat grayImg(colorImg);
		// display the result
		zg->getFrame(zed, depthImg, colorImg);
		
		cv::imshow("Depth", depthImg);
		cvtColor(colorImg, colorHSV, COLOR_BGR2HSV);
		
		//addWeighted(cutImage(colorHSV,"blu"),1,cutImage(colorHSV,"yel"),1,0.0,workImg);
		vector<Point> laneL, laneR;
		lineDetection(colorImg ,laneL, laneR);
		cout << "midPoint" << endl;
		
		
		cout << "left" << laneL << endl;
		cout << "right" << laneR << endl;
		
		//generatePath(laneL, laneR, colorImg);
		
		findMidPoint(laneL[0], laneR[0], midPoint1,lastMidPoint1);
		findMidPoint(laneL[1], laneR[1], midPoint2,lastMidPoint2);
		findMidPoint(laneL[2], laneR[2], midPoint3,lastMidPoint3);
		
		
		midPoint = Point((midPoint1.x+midPoint2.x+midPoint3.x)/3,550);		
				
		if (midPoint.x < zg->width/2 - tolerance){
			//			turn left		
			currentDirection = 3;
			midPointColor = (0,0,200);
		}else if (midPoint.x > zg->width/2 + tolerance){
			//			turn right		
			currentDirection = 4;
			midPointColor = (255,0,0);
		}else{
			//			turn foward
			currentDirection = 1;
			midPointColor = (0,255,0);
		}
		
		
		
		
		circle(colorImg, midPoint, 8, midPointColor,8);		
        line(colorImg, Point(zg->width/2 - tolerance,0),Point (zg->width/2 - tolerance,zg->height), Scalar(0,0,0),2);
        line(colorImg, Point(zg->width/2 + tolerance,0),Point (zg->width/2 + tolerance,zg->height), Scalar(0,0,0),2);
        line(colorImg, Point(zg->width/2, 0),Point (zg->width/2,zg->height), Scalar(0,0,0),2);
       
       
		
		emitDirections();
		lastMidPoint = midPoint;
//		resize(colorImg,colorImg,Size(zg->width/4, zg->height/4));
		cv::imshow("Color", colorImg);   		
	
		/*

//		cvtColor(colorImg, colorHSV, COLOR_BGR2HSV);  
//		Rect ROI(0,100,colorImg.cols,colorImg.rows - 100);
//		imgROI = colorImg(ROI);		
		vector<vector<Point> > contoursBlu = colorDetection(colorHSV, "blu");
		vector<vector<Point> > contoursYel = colorDetection(colorHSV, "yel");  

	//	selectedPointL = Point(abs(selectedPointL1.x + selectedPointL2.x) / 2, abs(selectedPointL1.y + selectedPointL2.y) / 2);

		Point selectedPointL = findPoint(contoursBlu, midPoint);
		Point selectedPointL2 = findPoint(contoursBlu, Point(midPoint.x,midPoint.y-30));
		Point selectedPointL3 = findPoint(contoursBlu, Point(midPoint.x,midPoint.y-60));	
		
		Point selectedPointR = findPoint(contoursYel, midPoint);
		Point selectedPointR2 = findPoint(contoursYel, Point(midPoint.x,midPoint.y-30));
		Point selectedPointR3 = findPoint(contoursYel, Point(midPoint.x,midPoint.y-60));

		//selectedPointR = Point(abs(selectedPointR1.x + selectedPointR2.x) / 2, abs(selectedPointR1.y + selectedPointR2.y) / 2);

			//generate a midpoint that keeps the prior distance between the found point and the prior midpoint.		
		if (selectedPointL.x == -1 && selectedPointR.x != -1) //cant find left, can find right
			midPoint = Point(selectedPointR.x - abs(selectedPointR.x - midPoint.x), selectedPointR.y);
		else if (selectedPointR.x == -1 && selectedPointL.x != -1) //cant find right, can find left
			midPoint = Point(selectedPointL.x + abs(selectedPointL.x - midPoint.x), selectedPointL.y);
		else if (selectedPointL.x != -1 && selectedPointR.x != -1) //can find both
			midPoint = Point(abs(selectedPointL.x +selectedPointR.x)/2,abs(selectedPointL.y +selectedPointR.y)/2);
		/*
			go through all contours, sort them by y value. find lowest point. use that as the starting point for the curve.
			find point +20 x above the previous point and with a y value within +-20 of the previous point.
			draw line between the two points. use the mid point of that line as the marker, generate point 50 across from that point as the mid line of the lane. Determine if we are within tolerances of the mid point. generate directions based on the results.
			draw line between these points.
		*/
		/*
		
        //imshow("output",redLane);
        circle(colorImg, selectedPointL, 8, Scalar(255,0,0),8);
        circle(colorImg, selectedPointR, 8, Scalar(0,0,255),8);
      	circle(colorImg, midPoint, 8, midPointColor,8);
        //imshow("outputCanny",colorImg); 
        //perform obstacle detection
        
        //decide on a direction based on dectection method results.
		*/               
        key = cv::waitKey(20);
        switch (key)// handle the pressed key
        {
            case 'q': // close the program
            case 'Q':
                run = false;
                gpioUnexport(serialStop);
				gpioUnexport(serialForward);
				gpioUnexport(serialLeft);
				gpioUnexport(serialRight);
				gpioUnexport(serialHardRight);     // unexport the LED
				gpioUnexport(serialHardLeft);      // unexport the push button
				gpioUnexport(serialReverse); 
                break;
            default:
                break;
        }      
    }	
		
				
	return 0;
}

void findMidPoint(Point laneL, Point laneR, Point& midPoint,Point& lastMidPoint){		
		if (laneL.x != -1 && laneR.x != -1) //can find both
			 midPoint = Point(abs(laneL.x + laneR.x) / 2, abs(laneL.y + laneR.y) / 2);
		else if (laneL.x == -1 && laneR.x != -1) {//cant find left, can find right	
			 midPoint = Point(abs(laneR.x - abs(laneR.x - lastMidPoint.x)), laneR.y);					
		}else if (laneL.x != -1 && laneR.x == -1) {//can find left, can't find right	
			 midPoint1 = Point(abs(laneL.x + abs(laneL.x - lastMidPoint.x)), laneL.y);			
		}
}

int factorial(int n){
	return (n == 1 || n== 0) ? 1: factorial(n-1)*n;
}

int choose(int a, int b){
	return factorial(a) / (factorial(b) * factorial(a-b));
}
 
Point bezier(vector<Point>& pts, double t){
	Point p;
	size_t len = pts.size();
	for (size_t i = 0; i < len; i++){
		p +=pts[i] * choose(len -1, i) * pow(1-t, len - 1 - i) * pow(t,i);
	}
	return p;
}


//take the vector of lane points and generate a midLine
void generatePath(vector<Point> laneL, vector<Point> laneR, Mat& input){
/*
	//generate a bezir curve for each lane
	
	//left lane
	for( size_t i =0; i < laneL.size(); i++){
		//remove any unfound markers
		if (laneL[i].x == -1)
			laneL.erase(laneL.begin()+i);
	}
	
	
	
	//define the begining and the end of the lanel as the hard control points.
	
	
		
	
	
	//right lane
	if (laneL[0].x == -1 && laneR[0].x != -1) //cant find left, can find right
		midPoint = Point(selectedPointR.x - abs(selectedPointR.x - midPoint.x), selectedPointR.y);
	else if (selectedPointR.x == -1 && selectedPointL.x != -1) //cant find right, can find left
		midPoint = Point(selectedPointL.x + abs(selectedPointL.x - midPoint.x), selectedPointL.y);
	else if (selectedPointL.x != -1 && selectedPointR.x != -1) //can find both
		midPoint = Point(abs(selectedPointL.x +selectedPointR.x)/2,abs(selectedPointL.y +selectedPointR.y)/2);
		
		
	Point midPoint = Point(abs(laneL[0].x + laneR[0].x) / 2, abs(laneL[0].y + laneR[0].y) / 2);

	
	if (midPoint.x < zg->width/2 - tolerance){
		//			turn left		
		currentDirection = 3;
		midPointColor = (0,0,200);

	}else if (midPoint.x > zg->width/2 + tolerance){
		//			turn right		
		currentDirection = 4;
		midPointColor = (255,0,0);
	}else{
		//			turn foward
		currentDirection = 1;
		midPointColor = (0,255,0);
	}
	
	circle(colorImg, midPoint, 8, midPointColor,8);		
    line(colorImg, Point(zg->width/2 - tolerance,0),Point (zg->width/2 - tolerance,zg->height), Scalar(0,0,0),2);
    line(colorImg, Point(zg->width/2 + tolerance,0),Point (zg->width/2 + tolerance,zg->height), Scalar(0,0,0),2);
    line(colorImg, Point(zg->width/2, 0),Point (zg->width/2,zg->height), Scalar(0,0,0),2);
   
   



	currentDirection = 1;
	*/
}


void emitDirections(){
	
	gpioSetValue(serialStop, off);
	gpioSetValue(serialForward, off);
	gpioSetValue(serialLeft, off);
	gpioSetValue(serialRight, off);
	gpioSetValue(serialHardRight, off);
	gpioSetValue(serialHardLeft, off);
	gpioSetValue(serialReverse, off);	
	if (currentDirection == lastDirection && dcount >=100){
		//do nothing
		dcount++;
	}else {
		dcount = 0;
	//send the new command
		if (currentDirection==0){//stop
			//send high to serial port:
			cout << "same command" << endl;
			gpioSetValue(serialForward, off);
			gpioSetValue(serialReverse, off);
			gpioSetValue(serialLeft, off);
			gpioSetValue(serialRight, off);
			gpioSetValue(serialStop, on);		
	
		}
		else if (currentDirection==1){//forward
			//send high to serial port:
							gpioSetValue(serialForward, on);
							cout << "serialForward" << endl;
		}
		else if (currentDirection==2){//reverse
			//send high to serial port:
							gpioSetValue(serialReverse, on);
							cout << "serialReverse" << endl;
		}			
		else if (currentDirection==3){//left
			//send high to serial port:
							gpioSetValue(serialHardLeft, on);
							cout << "serialLeft" << endl;
		}
		else if (currentDirection==4){//right
			//send high to serial port:
							gpioSetValue(serialHardRight, on);
							cout << "serialRight" << endl;
		}
		/*
		else if (currentDirection==5){//Hardleft
			//send high to serial port:
							gpioSetValue(serialStop, off);
							cout << "Hardleft" >> endl;
		}
		else if (currentDirection==6){//HardRight
			//send high to serial port:
							gpioSetValue(serialStop, off);
							cout << "HardRight" >> endl;
		}
		*/
		else{//stop
			//send high to serial port:
							gpioSetValue(serialStop, on);
							cout << "serialStop" << endl;
		}		
	}
	lastDirection = currentDirection;
}

