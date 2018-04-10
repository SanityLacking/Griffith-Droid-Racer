#include "fuzzyColor.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "colorClassifierFactory.h"


using namespace std;
using namespace cv;

bool RESCALE_IMAGE = true;
Mat rescaledImg, destImg;
Scalar laneColor;

struct ClassifierDescription{
     float min_angle; float max_angle; float min_radius; float max_radius;
     float min_contrast_angle; float max_contrast_angle;
     int redContrast; int redContrastLevel;
     int greenContrast; int greenContrastLevel;
     int BlueContrast; int BlueContrastLevel;
     Vec3b colour;
     string name;
};


vector<ClassifierDescription> targetColours;
ClassifierDescription c;
const float PI=3.14;
		 
ColorClassifier *(bCl) = ColorClassifierFactory::createClassifier("blue");
ColorClassifier *(yCL) = ColorClassifierFactory::createClassifier("yellow");
ColorClassifier *(gCL) = ColorClassifierFactory::createClassifier("green");

ColorClassifier &blueCL = *bCl;
ColorClassifier &yellowCL = *yCL;
ColorClassifier &greenCL = *gCL;

//Constructor
 FuzzyColor::FuzzyColor(){
 
 }
//function declarations.

void FuzzyColor::processFuzzyColor(Mat& img){
	img.copyTo(rescaledImg);
	resize(img, rescaledImg, Size(), 0.25, 0.25, INTER_LINEAR);
	destImg = Mat::zeros(rescaledImg.size(), CV_8UC3);
//	cout << "fuzzy Color"<<endl;
 	  	  	 
  	multi_colourClassify_rg_3(blueCL,yellowCL,greenCL,rescaledImg, destImg);

	laneDetection();
}



void FuzzyColor::multi_colourClassify_rg_3(ColorClassifier color1,ColorClassifier color2,ColorClassifier color3,Mat &sourceImg, Mat &destImg){
    
    int MINX, MAXX, MINY, MAXY;

    MINX = 0;MAXX=sourceImg.cols;
    MINY = 0;MAXY=sourceImg.rows;

   
    // greyImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
    if(RESCALE_IMAGE)
       destImg = Mat::zeros(rescaledImg.size(), CV_8UC3);
    else 
       destImg = Mat::zeros(sourceImg.size(), CV_8UC3); 
    //---
    for(int x=MINX; x < MAXX; x++){
        for(int y=MINY; y < MAXY; y++){

            Vec3b intensity = sourceImg.at<Vec3b>(y, x);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];

            float chR, chG;
            float chRGB = (float(red) + float(green) + float(blue));
            chR = float(red)/chRGB;
            chG = float(green)/chRGB;

            float radius = float(pow(double(chR - 0.333),2.0)) + float(pow(double(chG - 0.333),2.0));
            float angle = GetAngle(chR, chG);

            //==== RG Color Model ============================

            

            //Target #2 - Blue
            //==== RG Color Model ============================
                if(angle >= color2.min_contrast_angle && angle <= color2.max_contrast_angle)
                {
                    /// Contrast Operations ////////////////////////////////// 
                    //0 - degrade, 1 - enhance, 2 - retain
                    if(color2.redContrast==1) 
                      red = FuzzyEnhanceX(red,0.5,short(color2.redContrastLevel));
                    else if(color2.redContrast==0.0)
                      red = FuzzyDegradeX(red,0.5,short(color2.redContrastLevel));


                    if(color2.greenContrast==1) 
                      green = FuzzyEnhanceX(green,0.5,short(color2.greenContrastLevel));
                    else if(color2.greenContrast==0.0)
                      green = FuzzyDegradeX(green,0.5,short(color2.greenContrastLevel));


                    if(color2.blueContrast==1) 
                      blue = FuzzyEnhanceX(blue,0.5,short(color2.blueContrastLevel));
                    else if(color2.blueContrast==0.0)
                      blue = FuzzyDegradeX(blue,0.5,short(color2.blueContrastLevel));
                }
                //---

                chRGB = (float(red) + float(green) + float(blue));
                if(chRGB > 0)
                {
                    chR = float(red)/chRGB;
                    chG = float(green)/chRGB;

                    radius = float(pow(double(chR - 0.333),2)) + float(pow(double(chG - 0.333),2));
                    angle = GetAngle(chR, chG);           

                    //////////////////////////////
                    Vec3b newColour;
                    //if(y >= Ly && y <= Uy && u >= Lu && u <= Uu && v >= Lv && v <= Uv) 
                    if(angle >= color2.min_angle && angle <= color2.max_angle && radius >= color2.min_radius && radius <= color2.max_radius){
                        newColour.val[0] = 255;
                        newColour.val[1] = 0; //green
                        newColour.val[2] = 0; // red
                        destImg.at<Vec3b>(Point(x,y)) = newColour;
                        continue;
                    } else {
                   
                        newColour.val[0] = 0;
                        newColour.val[1] = 0;
                        newColour.val[2] = 0;
                        destImg.at<Vec3b>(Point(x,y)) = newColour;
                    }
                    

                }
                
            

            //---
            //Target #1 - Yellow
            intensity = sourceImg.at<Vec3b>(y, x);
            blue = intensity.val[0];
            green = intensity.val[1];
            red = intensity.val[2];

            
            chRGB = (float(red) + float(green) + float(blue));
            chR = float(red)/chRGB;
            chG = float(green)/chRGB;

            radius = float(pow(double(chR - 0.333),2.0)) + float(pow(double(chG - 0.333),2.0));
            angle = GetAngle(chR, chG);

            //==== RG Color Model ============================
            if(angle >= color1.min_contrast_angle && angle <= color1.max_contrast_angle)
            {
                /// Contrast Operations ////////////////////////////////// 
                //0 - degrade, 1 - enhance, 2 - retain
                if(color1.redContrast==1) 
                  red = FuzzyEnhanceX(red,0.5,short(color1.redContrastLevel));
                else if(color1.redContrast==0.0)
                  red = FuzzyDegradeX(red,0.5,short(color1.redContrastLevel));


                if(color1.greenContrast==1) 
                  green = FuzzyEnhanceX(green,0.5,short(color1.greenContrastLevel));
                else if(color1.greenContrast==0.0)
                  green = FuzzyDegradeX(green,0.5,short(color1.greenContrastLevel));


                if(color1.blueContrast==1) 
                  blue = FuzzyEnhanceX(blue,0.5,short(color1.blueContrastLevel));
                else if(color1.blueContrast==0.0)
                  blue = FuzzyDegradeX(blue,0.5,short(color1.blueContrastLevel));
                
            }
            //---
            /// End of Contrast Operations //////////////////////////////////   

                
                chRGB = (float(red) + float(green) + float(blue));
                if(chRGB > 0)
                {
                    chR = float(red)/chRGB;
                    chG = float(green)/chRGB;

                    radius = float(pow(double(chR - 0.333),2)) + float(pow(double(chG - 0.333),2));
                    angle = GetAngle(chR, chG);           

                    //////////////////////////////
                    Vec3b newColour;
                    //if(y >= Ly && y <= Uy && u >= Lu && u <= Uu && v >= Lv && v <= Uv) 
                    if(angle >= color1.min_angle && angle <= color1.max_angle && radius >= color1.min_radius && radius <= color1.max_radius){
                        // LookUpTable[index_lut] = Color; 
                        // newColour.val[0] = 0;
                        // newColour.val[1] = 0; //green
                        // newColour.val[2] = 255; // red
                        
                        newColour.val[0] = 0;
                        newColour.val[1] = 242; //green
                        newColour.val[2] = 255; // red
                        destImg.at<Vec3b>(Point(x,y)) = newColour;
                    } else {
                        // newColour.val[0] = 255;
                        // newColour.val[1] = 255;
                        // newColour.val[2] = 255;
                        // newColour.val[0] = blue;
                        // newColour.val[1] = green;
                        // newColour.val[2] = red;
                        newColour.val[0] = 0;
                        newColour.val[1] = 0;
                        newColour.val[2] = 0;
                        destImg.at<Vec3b>(Point(x,y)) = newColour;
                    }
                    
                }
            //---


            
            //Target #3 - Grass
            
                if(angle >= color3.min_contrast_angle && angle <= color3.max_contrast_angle)
                {
                    /// Contrast Operations ////////////////////////////////// 
                    //0 - degrade, 1 - enhance, 2 - retain
                    if(color3.redContrast==1) 
                      red = FuzzyEnhanceX(red,0.5,short(color3.redContrastLevel));
                    else if(color3.redContrast==0.0)
                      red = FuzzyDegradeX(red,0.5,short(color3.redContrastLevel));


                    if(color3.greenContrast==1) 
                      green = FuzzyEnhanceX(green,0.5,short(color3.greenContrastLevel));
                    else if(color3.greenContrast==0.0)
                      green = FuzzyDegradeX(green,0.5,short(color3.greenContrastLevel));


                    if(color1.redContrast==1) 
                      blue = FuzzyEnhanceX(blue,0.5,short(color3.blueContrastLevel));
                    else if(color3.blueContrast==0.0)
                      blue = FuzzyDegradeX(blue,0.5,short(color3.blueContrastLevel));
                }
                //---

                chRGB = (float(red) + float(green) + float(blue));
                if(chRGB > 0)
                {
                    chR = float(red)/chRGB;
                    chG = float(green)/chRGB;

                    radius = float(pow(double(chR - 0.333),2)) + float(pow(double(chG - 0.333),2));
                    angle = GetAngle(chR, chG);           

                    //////////////////////////////
                    Vec3b newColour;
                    //if(y >= Ly && y <= Uy && u >= Lu && u <= Uu && v >= Lv && v <= Uv) 
                    if(angle >= color3.min_angle && angle <= color3.max_angle && radius >= color3.min_radius && radius <= color3.max_radius){
                        newColour.val[0] = 0;
                        newColour.val[1] = 255; //green
                        newColour.val[2] = 0; // red
                        destImg.at<Vec3b>(Point(x,y)) = newColour;
                        continue;
                    } else {                   
                        // newColour.val[0] = 0;
                        // newColour.val[1] = 0;
                        // newColour.val[2] = 0;
                        // destImg.at<Vec3b>(Point(x,y)) = newColour;
                    }                    
                }                                   
        } //End for loop - y
    } //End for loop - x
    
}


void FuzzyColor::laneDetection(){
 //line intersection, etc.
        //---
        Mat outputFrame;
        outputFrame = Mat::zeros(rescaledImg.rows, rescaledImg.cols, CV_8UC3);
        cout<< "laneDetect"<<endl;
        //convert to grayscale
        cvtColor(destImg,outputFrame,CV_BGR2GRAY);
                cout<< "laneDetect2"<<endl;
        //---
        //Filter blobs
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        findContours( outputFrame, contours, hierarchy,
                      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
     
        Mat drawing = Mat::zeros(outputFrame.size(), CV_8UC3);
      
        for(int i=0; i < contours.size();i++){
           //if((contours[i].size() > 20) && (contours[i].size() < 1000)){
              double area = contourArea(contours[i]);
              Rect r = boundingRect(contours[i]);
              double aspectRatio = r.width/r.height;

              //if(aspectRatio > 0.3){

                Scalar color = Scalar(255,0,0);
                drawContours(drawing, contours, i, color, CV_FILLED /* 2 */, 8, hierarchy,0, Point());
              //}
            //}
           
        }
                 
        //End - Filter blobs  
        //---
        Point RightP, LeftP;
        Point laneCenter(-1,-1);
        RightP = laneCenter;
        LeftP = laneCenter;
        Point steeringDirection(-1,-1);

        static Point prevLeftP(-1,-1);
        static Point prevRightP(-1,-1);
        static Point prevsteeringDirection(-1,-1);

        //---
        // Point midPoint = Point(100,111); //Point(100,125);
        // intersection(midPoint, 0, drawing, LeftP,RightP);

        //---
        //Search each horizontal line from the bottom up, to find both yellow and
        //blue lanes.
        int trials=0;

        Point midPoint = Point(outputFrame.cols/2,outputFrame.rows);
        for(;;){
           intersection(midPoint, 0, drawing, LeftP,RightP);
           trials++;
           if(trials > 160){ break;}
           if(LeftP.x != -1 && LeftP.y != -1 && RightP.x != -1 && RightP.y != -1){ 
               break;
           } else {
               midPoint.y = midPoint.y - 1;
           }

        }                 



        cout << "LeftP::x=" << LeftP.x << ",y=" << LeftP.y << endl;
        cout << "RightP::x=" << RightP.x << ",y=" << RightP.y << endl;

        if(LeftP.x != -1)
           circle(rescaledImg,LeftP,3, Scalar(0,0,255));
        if(RightP.x != -1)
           circle(rescaledImg,RightP,3, Scalar(0,0,255));

        if(LeftP.x != -1)
           circle(destImg,LeftP,5, Scalar(0,0,255));
         
        if(RightP.x != -1) 
           circle(destImg,RightP,5, Scalar(0,0,255));

        
        if(LeftP.x == -1 && prevLeftP.x != -1){
          LeftP.x = prevLeftP.x;
          LeftP.y = prevLeftP.y;

        }
        if(RightP.x == -1 && prevRightP.x != -1){
          RightP.x = prevRightP.x;
          RightP.y = prevRightP.y;
        }

        double laneWidth = (double)fabs(RightP.x - LeftP.x);

        if(LeftP.x != -1 && LeftP.y != -1 && RightP.x != -1 && RightP.y != -1 && laneWidth > 20.0){
           line(destImg, LeftP, RightP,Scalar(0,0,255));
           steeringDirection.x = LeftP.x + ((RightP.x - LeftP.x)/2); 
           steeringDirection.y = midPoint.y;

           //to do: check distance from grass (known object), if grass is detected in the scene
           //add codes here 

           circle(destImg, steeringDirection, 5, Scalar(255,255,255));
           prevsteeringDirection = steeringDirection;
           prevLeftP = LeftP;
           prevRightP = RightP;
          
        } else if(LeftP.x == -1 || LeftP.y == -1 || RightP.x == -1 || RightP.y == -1 || laneWidth < 20.0){
           circle(destImg, prevsteeringDirection, 5, Scalar(255,255,255));
        }
                  

        if(LeftP.x != -1)
           circle(drawing,LeftP,5, Scalar(0,0,255));
         
        if(RightP.x != -1) 
           circle(drawing,RightP,5, Scalar(0,0,255));

        

        imshow( "Results", destImg ); //colour classification results
        imshow("Contours",drawing);
}

//---
 float FuzzyColor::GetAngle(float ChR, float ChG)
{
    float Angle=0.0;
    
    Angle = float(atan(double((ChG-0.333)/(ChR-0.333))));

    //Adjust angle
    if(ChR < 0.333 && ChG > 0.333)  //2nd Q
    {
        Angle = Angle + PI; 
    }
    else if(ChR < 0.333 && ChG < 0.333) //3rd Q
    {
        Angle = Angle + PI; 
    }
    else if(ChR > 0.333 && ChG < 0.333) //4th Q
    {
        Angle = Angle + (2*PI); 
    }
    
    return (Angle*180/PI);
}
//---
 int FuzzyColor::FuzzyEnhanceX(int x, float Threshold, short N)
{
   float Nx;
   float Ox;
//   int Tmp;
   int i;

   Nx = float(x) / 255.0f;
   
   for(i=1;i <= N;i++)
   {
       if(Nx >= Threshold)
         Ox = 1 - (2 * (float) (pow((1 - Nx),2)));
       else
         Ox = 2 * (float) pow(Nx,2);

       Nx = Ox;
   }

   return (int)(Ox * 255);
}


 int FuzzyColor::FuzzyDegradeX(int x, float Threshold, short N)
{
   float Nx;
   float Ox;
//   int Tmp;
   int i;

   Nx = float(x) / 255.0f;
   
   for(i=1;i <= N;i++)
   {
       if(Nx >= Threshold)
         Ox = 0.5 + (2 * (float) (pow((Nx-0.5),2)));
       else
         Ox = (1 - (2 * (float) (pow(1-(Nx+0.5),2))))-0.5;

       Nx = Ox;
   }

   return (int)(Ox * 255);
}


// new version that works with Mat input not lines
// find the intersection point on the horizontal line of midpoint in the 
void FuzzyColor::intersection(Point midPoint, int offset, Mat& img, Point& returnLP, Point& returnRP){
    vector<Point> points;

    
    // int Y = midPoint.y;

    // for(int i = 0; i < img.cols; i++){
    //   if (img.at<double>(i,Y) == 0){
    //     points.push_back(Point(i,Y));

    //   }
    // }

    // //---

    // int Y = midPoint.y;

    // for(int i = 0; i < img.cols; i++){
    //   Scalar colour = img.at<uchar>(Y,i);
    //   // Grayscale
    //   if (colour.val[0] != 0){
    //      points.push_back(Point(i,Y));
    //   }
     
    // }


    //---

    int Y = midPoint.y;

    for(int i = 0; i < img.cols; i++){
      Vec3b colour = img.at<Vec3b>(Y, i);
     
      if ((colour.val[0] == 255) ) {
         points.push_back(Point(i,Y));
      }
     
    }

//  cout << "output::" <<points.size()<<endl;
  // find a center point
  for (int i=0; i < points.size(); i++){    
    //loop through all points, find points that are closest to the midPoint.
    if (points[i].x < midPoint.x && abs(points[i].x - midPoint.x ) < abs(returnLP.x - midPoint.x)){//lefthandside
      returnLP = points[i];
    } else if (points[i].x > midPoint.x && abs(points[i].x - midPoint.x ) < abs(returnRP.x - midPoint.x)){//righthandside
      returnRP = points[i];
    }
  }


}

