

#include <opencv/highgui.h>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

vector<vector<Point>> colorDetection(Mat input, String color);

Mat cutImage(Mat input, String color);
