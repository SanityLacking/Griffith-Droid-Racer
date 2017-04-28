// droidRacer.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"


#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "time.h"

using namespace cv;
using namespace std;

double PointDist(Point a, Point b);
Mat processImage(Mat frame);

double H = 0;
double S = 0;
double V = 0;
const int alphaMax = 255;
const int betaMax = 255;

const int alpha_slider_max = 255;
const int beta_slider_max = 100;
const int theta_slider_max = 100;
int alpha_slider;
int beta_slider;
int theta_slider;


double alpha;
double beta;
double theta;


void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider ;
	beta = (double)beta_slider;
	theta = (double)theta_slider ;
}
int displayCamera(VideoCapture camera){
	if (!camera.isOpened())  // check if we succeeded
		return -1;
	Mat edges;
	namedWindow("output", 1);
	createTrackbar("hue", "output", &alpha_slider, alpha_slider_max, on_trackbar);
	createTrackbar("sat", "output", &beta_slider, beta_slider_max, on_trackbar);
	createTrackbar("value", "output", &theta_slider, theta_slider_max, on_trackbar);


	// fps counter begin
	time_t start, end;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end

	for (;;)
	{
		if (counter == 0){
			time(&start);
		}
		// fps counter end
		Mat frame;
		camera >> frame; // get a new frame from camera
		//flip(frame, frame, 0);

		/// Create Trackbars
		
		
		cout << alpha << " " << beta << " " << theta << endl;
		

		time(&end);
		counter++;
		sec = difftime(end, start);
		fps = counter / sec;
		if (counter > 30)

			// overflow protection
			if (counter == (INT_MAX - 1000))
				counter = 0;
		// fps counter end
		putText(frame, "fps: " + to_string(fps), Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);


		Mat outputFrame = processImage(frame);
		imshow("input", frame);
		imshow("output", outputFrame);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}



Mat processImage(Mat frame){
	// need to threshold the image for each colour that i need to detect.
	Mat frameHSV;
	
	cvtColor(frame,frameHSV,CV_BGR2HSV);
	inRange(frameHSV, Scalar(alpha,beta, theta), Scalar(255, 255, 255), frameHSV);

	return frameHSV;
}


int _tmain(int argc, _TCHAR* argv[])
{
	
	//Mat image = getInput();
	VideoCapture camera(0);
	if (!camera.isOpened()){  // check if we succeeded
		cout << "camera not opened" << endl;
		//return -1;
	}
	displayCamera(camera);

	cout << "Program End";
	getchar();

	return 0;
}

// simple equation to detemine the distance between two 2d points.
double PointDist(Point a, Point b){
	return sqrt((a.x - b.x) ^ 2 + (a.y - b.y) ^ 2);
}

