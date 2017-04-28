//to do:
//https://books.google.com.au/books?id=LPm3DQAAQBAJ&pg=PA27&lpg=PA27&dq=opencv+trackbar+videocapture&source=bl&ots=2uPnXiddAf&sig=r4dUz5gfGJfuNmat8FaJZoG59gk&hl=en&sa=X&ved=0ahUKEwjf0_T5ierRAhVJI5QKHfA_CIMQ6AEIVTAJ#v=onepage&q=opencv%20trackbar%20videocapture&f=false

////////////////////////////////////////////////////////////////////
//
// Author: n.h.reyes@massey.ac.nz
// Requirements: DLLs
//           opencv_ffmpeg2413.dll - required by VideoCapture for
//                                   reading video files
////////////////////////////////////////////////////////////////////
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "graphics.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

const float PI=3.14;


enum OperationMode{STATIC_FILE_MODE, VIDEO_MODE, LIVE_MODE};
enum ClassifierMode{RGB_CLASSIFIER, RG_CLASSIFIER, HSI_CLASSIFIER};

RNG rng(12345);

unsigned LookUpTable[16777216];  //-- Look Up Table

bool LUT_MODE;
bool RESCALE_IMAGE;
OperationMode opMode;
ClassifierMode classifier;

Scalar laneColor;// = Scalar(255,0,0); //blue
int g_run=1;
int g_dontset=0;
// #include "opencv2/video/video.hpp"

#include <iostream>
// #include <windows.h> //required by the BGI graphics library
// #include "stdafx.h"

using namespace cv;
using namespace std;


// void intersection(Point linePoint, int offset, Mat& img, Point& returnLP, Point& returnRP);
void intersection(Point midPoint, int offset, Mat& img, Point& returnLP, Point& returnRP);

Mat img;
Mat destImg;
Mat greyImg;
Mat rescaledImg;
int threshval = 100;

//---
//RGB CLASSIFIER
int red_max;
int green_max;
int blue_max;
int red_min;
int green_min;
int blue_min;
//---
//RG CLASSIFIER
int t_min_contrast_angle, t_max_contrast_angle;
int t_min_angle, t_max_angle, t_min_radius, t_max_radius;
float min_contrast_angle, max_contrast_angle;
float min_angle, max_angle, min_radius, max_radius;
int RedContrast, RedContrastLevel;
int GreenContrast, GreenContrastLevel;
int BlueContrast, BlueContrastLevel;
//---
int t_min_contrast_angle2, t_max_contrast_angle2;
int t_min_angle2, t_max_angle2, t_min_radius2, t_max_radius2;
float min_contrast_angle2, max_contrast_angle2;
float min_angle2, max_angle2, min_radius2, max_radius2;
int RedContrast2, RedContrastLevel2;
int GreenContrast2, GreenContrastLevel2;
int BlueContrast2, BlueContrastLevel2;
//---
int t_min_contrast_angle3, t_max_contrast_angle3;
int t_min_angle3, t_max_angle3, t_min_radius3, t_max_radius3;
float min_contrast_angle3, max_contrast_angle3;
float min_angle3, max_angle3, min_radius3, max_radius3;
int RedContrast3, RedContrastLevel3;
int GreenContrast3, GreenContrastLevel3;
int BlueContrast3, BlueContrastLevel3;
//---
//---
int t_min_contrast_angle4, t_max_contrast_angle4;
int t_min_angle4, t_max_angle4, t_min_radius4, t_max_radius4;
float min_contrast_angle4, max_contrast_angle4;
float min_angle4, max_angle4, min_radius4, max_radius4;
int RedContrast4, RedContrastLevel4;
int GreenContrast4, GreenContrastLevel4;
int BlueContrast4, BlueContrastLevel4;
//---

struct ClassifierDescription{
     float min_angle; float max_angle; float min_radius; float max_radius;
     float min_contrast_angle; float max_contrast_angle;
     int RedContrast; int RedContrastLevel;
     int GreenContrast; int GreenContrastLevel;
     int BlueContrast; int BlueContrastLevel;
     Vec3b colour;
     string name;
};

vector<ClassifierDescription> targetColours;

void colourClassify_RGB(int min_red, int min_green, int min_blue,
                    int thresh_red, int thresh_green, int thresh_blue, Mat &sourceImg, Mat &destImg);
// void colourClassify_rg(float min_angle, float max_angle, float min_radius, float max_radius,
//                        Mat &sourceImg, Mat &destImg);
void colourClassify_rg(float min_angle, float max_angle, float min_radius, float max_radius,
                       float min_contrast_angle, float max_contrast_angle,
                       int RedContrast, int RedContrastLevel,
                       int GreenContrast, int GreenContrastLevel,
                       int BlueContrast, int BlueContrastLevel,
                       Mat &sourceImg, Mat &destImg);


//////////////////////////////////////////////////////////////////
void colourClassify_RGB(int min_red, int min_green, int min_blue,
                    int thresh_red, int thresh_green, int thresh_blue, Mat &sourceImg, Mat &destImg){
    // thresh_red=200;
    // green_max = 60;
    // blue_max = 57;

    int MINX, MAXX, MINY, MAXY;

    MINX = 0;MAXX=sourceImg.cols;
    MINY = 0;MAXY=sourceImg.rows;
    // destImg.release();
    destImg = Mat::zeros(img.size(), CV_8UC3);
    
    for(int x=MINX; x < MAXX; x++){
        for(int y=MINY; y < MAXY; y++){

            Vec3b intensity = sourceImg.at<Vec3b>(y, x);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];
            Vec3b newColour;// = destImg.at<Vec3b>(y, x);
            
            //if((red >= 0 && red <= 128) && (green >= 25 && green <= 100) && (blue >= 60 && blue <= 200)){
            if((red >= min_red && red <= thresh_red) && (green >= min_green && green <= thresh_green) && (blue >= min_blue && blue <= thresh_blue)){
                newColour.val[0] = 0;
                newColour.val[1] = 0;
                newColour.val[2] = 255;
                destImg.at<Vec3b>(Point(x,y)) = newColour;
            } else {
                newColour.val[0] = 0;
                newColour.val[1] = 0;
                newColour.val[2] = 0;
                // newColour.val[0] = blue;
                // newColour.val[1] = green;
                // newColour.val[2] = red;
                destImg.at<Vec3b>(Point(x,y)) = newColour;
            }

        }
    }
    // imshow( "Results", destImg );

}
//---
inline float GetAngle(float ChR, float ChG)
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
inline int FuzzyEnhanceX(int x, float Threshold, short N)
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


inline int FuzzyDegradeX(int x, float Threshold, short N)
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
//---
void initLookUpTable(){
  cout << "init LUT" << endl;
  long  index_lut;
  for(int blue=0; blue < 255; blue++){    
    for(int green=0; green < 255; green++){
        for(int red=0; red < 255; red++){
            index_lut = ((red<<16) + (green<<8) + blue);
            LookUpTable[index_lut] = (unsigned)0; //no colour
            // cout << "index_lut[" <<  index_lut << ", R=" << red << ", G=" << green << ", B=" << blue << endl;
         
        }
    }
  }
}
//---
void generateLookUpTable_rg(float min_angle, float max_angle, float min_radius, float max_radius,
                       float min_contrast_angle, float max_contrast_angle,
                       int RedContrast, int RedContrastLevel,
                       int GreenContrast, int GreenContrastLevel,
                       int BlueContrast, int BlueContrastLevel, uchar assignedColour){
    
  long  index_lut;

  cout << "generating LUT." << endl;

  uchar red, green, blue;

  for(int b=0; b < 255; b++){    
    for(int g=0; g < 255; g++){
        for(int r=0; r < 255; r++){

            red = (uchar)r;
            green = (uchar)g;
            blue = (uchar)b;
            index_lut = ((r<<16) + (g<<8) + b);
            // cout << "index_lut[" <<  index_lut << ", R=" << r << ", G=" << g << ", B=" << b << endl;
            // Vec3b intensity = sourceImg.at<Vec3b>(y, x);
            // uchar blue = intensity.val[0];
            // uchar green = intensity.val[1];
            // uchar red = intensity.val[2];

            float chR, chG;
            float chRGB = (float(red) + float(green) + float(blue));
            chR = float(red)/chRGB;
            chG = float(green)/chRGB;

            float radius = float(pow(double(chR - 0.333),2.0)) + float(pow(double(chG - 0.333),2.0));
            float angle = GetAngle(chR, chG);

            //==== RG Color Model ============================
            if(angle >= min_contrast_angle && angle <= max_contrast_angle)
            {
                /// Contrast Operations ////////////////////////////////// 
                //0 - degrade, 1 - enhance, 2 - retain
                if(RedContrast==1) 
                  red = FuzzyEnhanceX(red,0.5,short(RedContrastLevel));
                else if(RedContrast==0.0)
                  red = FuzzyDegradeX(red,0.5,short(RedContrastLevel));


                if(GreenContrast==1) 
                  green = FuzzyEnhanceX(green,0.5,short(GreenContrastLevel));
                else if(GreenContrast==0.0)
                  green = FuzzyDegradeX(green,0.5,short(GreenContrastLevel));


                if(BlueContrast==1) 
                  blue = FuzzyEnhanceX(blue,0.5,short(BlueContrastLevel));
                else if(BlueContrast==0.0)
                  blue = FuzzyDegradeX(blue,0.5,short(BlueContrastLevel));
                
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
                    if(angle >= min_angle && angle <= max_angle && radius >= min_radius && radius <= max_radius){
                        LookUpTable[index_lut] = (unsigned)assignedColour; 
                        
                    } else {
                       
                    }
                    
                }
                
         
        } //End for loop - red
    } //End for loop - green
  } //End for loop - blue
  cout << "done." << endl;  
}
//---


//---
void colourClassify_rg(float min_angle, float max_angle, float min_radius, float max_radius,
                       float min_contrast_angle, float max_contrast_angle,
                       int RedContrast, int RedContrastLevel,
                       int GreenContrast, int GreenContrastLevel,
                       int BlueContrast, int BlueContrastLevel,
                       Mat &sourceImg, Mat &destImg){
    
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
            if(angle >= min_contrast_angle && angle <= max_contrast_angle)
            {
                /// Contrast Operations ////////////////////////////////// 
                //0 - degrade, 1 - enhance, 2 - retain
                if(RedContrast==1) 
                  red = FuzzyEnhanceX(red,0.5,short(RedContrastLevel));
                else if(RedContrast==0.0)
                  red = FuzzyDegradeX(red,0.5,short(RedContrastLevel));


                if(GreenContrast==1) 
                  green = FuzzyEnhanceX(green,0.5,short(GreenContrastLevel));
                else if(GreenContrast==0.0)
                  green = FuzzyDegradeX(green,0.5,short(GreenContrastLevel));


                if(BlueContrast==1) 
                  blue = FuzzyEnhanceX(blue,0.5,short(BlueContrastLevel));
                else if(BlueContrast==0.0)
                  blue = FuzzyDegradeX(blue,0.5,short(BlueContrastLevel));
                
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
                    if(angle >= min_angle && angle <= max_angle && radius >= min_radius && radius <= max_radius){
                        // LookUpTable[index_lut] = Color; 
                        // newColour.val[0] = 0;
                        // newColour.val[1] = 0; //green
                        // newColour.val[2] = 255; // red
                        
                        newColour.val[0] = 0;
                        newColour.val[1] = 242; //green
                        newColour.val[2] = 255; // red
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
                    }
                    destImg.at<Vec3b>(Point(x,y)) = newColour;
                }
         

        } //End for loop - y
    } //End for loop - x
    
}


//---
void multi_colourClassify_rg(vector<ClassifierDescription> classifier, Mat &sourceImg, Mat &destImg){
    
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

            for(int c=0; c < classifier.size(); c++){ 

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
                //---
                Vec3b newColour;
                newColour.val[0] = 0;
                newColour.val[1] = 0;
                newColour.val[2] = 0;

                //==== RG Color Model ============================
                if(angle >= classifier[c].min_contrast_angle && angle <= classifier[c].max_contrast_angle)
                {
                    /// Contrast Operations ////////////////////////////////// 
                    //0 - degrade, 1 - enhance, 2 - retain
                    if(classifier[c].RedContrast==1) 
                      red = FuzzyEnhanceX(red,0.5,short(classifier[c].RedContrastLevel));
                    else if(classifier[c].RedContrast==0.0)
                      red = FuzzyDegradeX(red,0.5,short(classifier[c].RedContrastLevel));


                    if(classifier[c].GreenContrast==1) 
                      green = FuzzyEnhanceX(green,0.5,short(classifier[c].GreenContrastLevel));
                    else if(classifier[c].GreenContrast==0.0)
                      green = FuzzyDegradeX(green,0.5,short(classifier[c].GreenContrastLevel));


                    if(classifier[c].BlueContrast==1) 
                      blue = FuzzyEnhanceX(blue,0.5,short(classifier[c].BlueContrastLevel));
                    else if(classifier[c].BlueContrast==0.0)
                      blue = FuzzyDegradeX(blue,0.5,short(classifier[c].BlueContrastLevel));
                    
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
                        
                        //if(y >= Ly && y <= Uy && u >= Lu && u <= Uu && v >= Lv && v <= Uv) 
                        if(angle >= classifier[c].min_angle && angle <= classifier[c].max_angle && radius >= classifier[c].min_radius && radius <= classifier[c].max_radius){
                            newColour = classifier[c].colour; 
                            // newColour.val[0] = 0;
                            // newColour.val[1] = 242; //green
                            // newColour.val[2] = 255; // red
                            destImg.at<Vec3b>(Point(x,y)) = newColour;
                        } else {
                            
                            // newColour.val[0] = 0;
                            // newColour.val[1] = 0;
                            // newColour.val[2] = 0;
                            // destImg.at<Vec3b>(Point(x,y)) = newColour;
                        }
                        
                    }
                //---
            } //classifier loop

        } //End for loop - y
    } //End for loop - x
    
}


//---
void multi_colourClassify_rg_3(float min_angle, float max_angle, float min_radius, float max_radius,
                       float min_contrast_angle, float max_contrast_angle,
                       int RedContrast, int RedContrastLevel,
                       int GreenContrast, int GreenContrastLevel,
                       int BlueContrast, int BlueContrastLevel,

                       float min_angle2, float max_angle2, float min_radius2, float max_radius2,
                       float min_contrast_angle2, float max_contrast_angle2,
                       int RedContrast2, int RedContrastLevel2,
                       int GreenContrast2, int GreenContrastLevel2,
                       int BlueContrast2, int BlueContrastLevel2,

                       float min_angle3, float max_angle3, float min_radius3, float max_radius3,
                       float min_contrast_angle3, float max_contrast_angle3,
                       int RedContrast3, int RedContrastLevel3,
                       int GreenContrast3, int GreenContrastLevel3,
                       int BlueContrast3, int BlueContrastLevel3, 

                       Mat &sourceImg, Mat &destImg){
    
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
                if(angle >= min_contrast_angle2 && angle <= max_contrast_angle2)
                {
                    /// Contrast Operations ////////////////////////////////// 
                    //0 - degrade, 1 - enhance, 2 - retain
                    if(RedContrast2==1) 
                      red = FuzzyEnhanceX(red,0.5,short(RedContrastLevel2));
                    else if(RedContrast2==0.0)
                      red = FuzzyDegradeX(red,0.5,short(RedContrastLevel2));


                    if(GreenContrast2==1) 
                      green = FuzzyEnhanceX(green,0.5,short(GreenContrastLevel2));
                    else if(GreenContrast2==0.0)
                      green = FuzzyDegradeX(green,0.5,short(GreenContrastLevel2));


                    if(BlueContrast2==1) 
                      blue = FuzzyEnhanceX(blue,0.5,short(BlueContrastLevel2));
                    else if(BlueContrast2==0.0)
                      blue = FuzzyDegradeX(blue,0.5,short(BlueContrastLevel2));
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
                    if(angle >= min_angle2 && angle <= max_angle2 && radius >= min_radius2 && radius <= max_radius2){
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
            if(angle >= min_contrast_angle && angle <= max_contrast_angle)
            {
                /// Contrast Operations ////////////////////////////////// 
                //0 - degrade, 1 - enhance, 2 - retain
                if(RedContrast==1) 
                  red = FuzzyEnhanceX(red,0.5,short(RedContrastLevel));
                else if(RedContrast==0.0)
                  red = FuzzyDegradeX(red,0.5,short(RedContrastLevel));


                if(GreenContrast==1) 
                  green = FuzzyEnhanceX(green,0.5,short(GreenContrastLevel));
                else if(GreenContrast==0.0)
                  green = FuzzyDegradeX(green,0.5,short(GreenContrastLevel));


                if(BlueContrast==1) 
                  blue = FuzzyEnhanceX(blue,0.5,short(BlueContrastLevel));
                else if(BlueContrast==0.0)
                  blue = FuzzyDegradeX(blue,0.5,short(BlueContrastLevel));
                
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
                    if(angle >= min_angle && angle <= max_angle && radius >= min_radius && radius <= max_radius){
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
            
                if(angle >= min_contrast_angle3 && angle <= max_contrast_angle3)
                {
                    /// Contrast Operations ////////////////////////////////// 
                    //0 - degrade, 1 - enhance, 2 - retain
                    if(RedContrast3==1) 
                      red = FuzzyEnhanceX(red,0.5,short(RedContrastLevel3));
                    else if(RedContrast3==0.0)
                      red = FuzzyDegradeX(red,0.5,short(RedContrastLevel3));


                    if(GreenContrast3==1) 
                      green = FuzzyEnhanceX(green,0.5,short(GreenContrastLevel3));
                    else if(GreenContrast3==0.0)
                      green = FuzzyDegradeX(green,0.5,short(GreenContrastLevel3));


                    if(BlueContrast3==1) 
                      blue = FuzzyEnhanceX(blue,0.5,short(BlueContrastLevel3));
                    else if(BlueContrast3==0.0)
                      blue = FuzzyDegradeX(blue,0.5,short(BlueContrastLevel3));
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
                    if(angle >= min_angle3 && angle <= max_angle3 && radius >= min_radius3 && radius <= max_radius3){
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
                
            
            //---  
            



            //////////////////////////////////////   

                
                
         

        } //End for loop - y
    } //End for loop - x
    
}

//---

//---
void multi_colourClassify_rg(float min_angle, float max_angle, float min_radius, float max_radius,
                       float min_contrast_angle, float max_contrast_angle,
                       int RedContrast, int RedContrastLevel,
                       int GreenContrast, int GreenContrastLevel,
                       int BlueContrast, int BlueContrastLevel,

                       float min_angle2, float max_angle2, float min_radius2, float max_radius2,
                       float min_contrast_angle2, float max_contrast_angle2,
                       int RedContrast2, int RedContrastLevel2,
                       int GreenContrast2, int GreenContrastLevel2,
                       int BlueContrast2, int BlueContrastLevel2,

                       Mat &sourceImg, Mat &destImg){
    
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
                if(angle >= min_contrast_angle2 && angle <= max_contrast_angle2)
                {
                    /// Contrast Operations ////////////////////////////////// 
                    //0 - degrade, 1 - enhance, 2 - retain
                    if(RedContrast2==1) 
                      red = FuzzyEnhanceX(red,0.5,short(RedContrastLevel2));
                    else if(RedContrast2==0.0)
                      red = FuzzyDegradeX(red,0.5,short(RedContrastLevel2));


                    if(GreenContrast2==1) 
                      green = FuzzyEnhanceX(green,0.5,short(GreenContrastLevel2));
                    else if(GreenContrast2==0.0)
                      green = FuzzyDegradeX(green,0.5,short(GreenContrastLevel2));


                    if(BlueContrast2==1) 
                      blue = FuzzyEnhanceX(blue,0.5,short(BlueContrastLevel2));
                    else if(BlueContrast2==0.0)
                      blue = FuzzyDegradeX(blue,0.5,short(BlueContrastLevel2));
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
                    if(angle >= min_angle2 && angle <= max_angle2 && radius >= min_radius2 && radius <= max_radius2){
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

            //Target #1
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
            if(angle >= min_contrast_angle && angle <= max_contrast_angle)
            {
                /// Contrast Operations ////////////////////////////////// 
                //0 - degrade, 1 - enhance, 2 - retain
                if(RedContrast==1) 
                  red = FuzzyEnhanceX(red,0.5,short(RedContrastLevel));
                else if(RedContrast==0.0)
                  red = FuzzyDegradeX(red,0.5,short(RedContrastLevel));


                if(GreenContrast==1) 
                  green = FuzzyEnhanceX(green,0.5,short(GreenContrastLevel));
                else if(GreenContrast==0.0)
                  green = FuzzyDegradeX(green,0.5,short(GreenContrastLevel));


                if(BlueContrast==1) 
                  blue = FuzzyEnhanceX(blue,0.5,short(BlueContrastLevel));
                else if(BlueContrast==0.0)
                  blue = FuzzyDegradeX(blue,0.5,short(BlueContrastLevel));
                
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
                    if(angle >= min_angle && angle <= max_angle && radius >= min_radius && radius <= max_radius){
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

            



            //////////////////////////////////////   

                
                
         

        } //End for loop - y
    } //End for loop - x
    
}

//---
void colourClassify_rg_fast(unsigned targetColour,
                            Mat &sourceImg, Mat &destImg){
    
    int MINX, MAXX, MINY, MAXY;

    MINX = 0;MAXX=sourceImg.cols;
    MINY = 0;MAXY=sourceImg.rows;

   
    // greyImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
    destImg = Mat::zeros(img.size(), CV_8UC3);
    
    for(int x=MINX; x < MAXX; x++){
        for(int y=MINY; y < MAXY; y++){

            Vec3b intensity = sourceImg.at<Vec3b>(y, x);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];
            Vec3b newColour;
            long index_lut = ((red<<16) + (green<<8) + blue);
            if(LookUpTable[index_lut] == targetColour){
                newColour.val[0] = 0;
                newColour.val[1] = 0; //green
                newColour.val[2] = 255; // red
                
            } else {
                newColour.val[0] = 0;
                newColour.val[1] = 0;
                newColour.val[2] = 0;
            }
            destImg.at<Vec3b>(Point(x,y)) = newColour;

        } //End for loop - y
    } //End for loop - x
    
}
//---
//---
static void on_trackbar(int, void*)
{
   red_min = getTrackbarPos("red min", "Control"); 
   green_min = getTrackbarPos("green min", "Control"); 
   blue_min = getTrackbarPos("blue min", "Control"); 

   red_max = getTrackbarPos("red max", "Control"); 
   green_max = getTrackbarPos("green max", "Control"); 
   blue_max = getTrackbarPos("blue max", "Control"); 
   //---
   if(classifier == RGB_CLASSIFIER)
       colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, img, destImg);
   else if(classifier = RG_CLASSIFIER){
       colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
                         min_contrast_angle, max_contrast_angle,
                         RedContrast, RedContrastLevel,
                         GreenContrast, GreenContrastLevel,
                         BlueContrast, BlueContrastLevel,img, destImg);
       
   }
   //---
   imshow( "Results", destImg );
   greyImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
   cvtColor( destImg, greyImg, CV_BGR2GRAY );
   threshold( greyImg, greyImg, 5, 255, CV_THRESH_BINARY );
   imshow("Binary", greyImg);
   
}

//---
static void on_trackbar_rg(int, void*)
{
   t_min_radius = getTrackbarPos("min radius", "Control"); 
   min_radius = (float) t_min_radius / 10000;

   t_max_radius = getTrackbarPos("max radius", "Control"); 
   max_radius = (float) t_max_radius / 10000;

   t_min_angle = getTrackbarPos("min angle", "Control"); 
   min_angle = (float) t_min_angle;

   t_max_angle = getTrackbarPos("max angle", "Control"); 
   max_angle = (float) t_max_angle;
   //---
   t_min_contrast_angle = getTrackbarPos("min contrast angle", "Control"); 
   min_contrast_angle = (float) t_min_contrast_angle;

   t_max_contrast_angle = getTrackbarPos("max contrast angle", "Control"); 
   max_contrast_angle = (float) t_max_contrast_angle;
   //---
   RedContrast = getTrackbarPos("red contrast", "Control"); 
   GreenContrast = getTrackbarPos("green contrast", "Control"); 
   BlueContrast = getTrackbarPos("blue contrast", "Control"); 
   //---
   RedContrastLevel = getTrackbarPos("red contrast level", "Control"); 
   GreenContrastLevel = getTrackbarPos("green contrast level", "Control"); 
   BlueContrastLevel = getTrackbarPos("blue contrast level", "Control"); 

   //---
   

   //---
   // min_radius=0.0393;
   // max_radius=1.0;
   // min_angle=149.6880;
   // max_angle=152.6040;
   // min_contrast_angle=147.9960;
   // max_contrast_angle=153.1080;
   // RedContrast=0;
   // GreenContrast=1;
   // BlueContrast=0;

   //---
   if(classifier == RGB_CLASSIFIER){
       if(RESCALE_IMAGE){
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, rescaledImg, destImg);
       } else {
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, img, destImg);
       
       } 
   } else if(classifier = RG_CLASSIFIER){

       if(RESCALE_IMAGE){


          if(LUT_MODE){
               colourClassify_rg_fast(4,rescaledImg, destImg);
          }else {    
               //for CALIBRATION 
               colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
                                 min_contrast_angle, max_contrast_angle,
                                 RedContrast, RedContrastLevel,
                                 GreenContrast, GreenContrastLevel,
                                 BlueContrast, BlueContrastLevel,rescaledImg, destImg);

               // multi_colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
               //                   min_contrast_angle, max_contrast_angle,
               //                   RedContrast, RedContrastLevel,
               //                   GreenContrast, GreenContrastLevel,
               //                   BlueContrast, BlueContrastLevel,
               //                   min_angle2, max_angle2, min_radius2, max_radius2, 
               //                   min_contrast_angle2, max_contrast_angle2,
               //                   RedContrast2, RedContrastLevel2,
               //                   GreenContrast2, GreenContrastLevel2,
               //                   BlueContrast2, BlueContrastLevel2,
               //                   rescaledImg, destImg);


          }
       
           
       } else {

            if(LUT_MODE){
               colourClassify_rg_fast(4,img, destImg);
            }else {    
               //for CALIBRATION 
               colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
                                 min_contrast_angle, max_contrast_angle,
                                 RedContrast, RedContrastLevel,
                                 GreenContrast, GreenContrastLevel,
                                 BlueContrast, BlueContrastLevel,img, destImg);

            }

          
       }
// 
       
   }
    if(opMode == STATIC_FILE_MODE)
       imshow( "Results", destImg );
    
    if(RESCALE_IMAGE){
        greyImg = Mat::zeros(rescaledImg.rows, rescaledImg.cols, CV_8UC3);
        //---
        cvtColor( destImg, greyImg, CV_BGR2GRAY );

        threshold( greyImg, greyImg, 5, 255, CV_THRESH_BINARY );
        // imshow("Binary", greyImg); //not necessary
        //---
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        // Mat drawing = Mat::zeros(destImg.rows, destImg.cols, CV_8UC3);
        findContours( greyImg, contours, hierarchy,
                      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
        // findContours( greyImg, contours, hierarchy,
        //               CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
        Mat drawing = Mat::zeros(destImg.size(), CV_8UC3);
        // int idx = 0;
        // for( ; idx >= 0; idx = hierarchy[idx][0] )
        // {          
        //     cout << "contours[" << idx << "]=" << contours[idx].size() << endl;
        //     if(contours[idx].size() >= 25 && contours[idx].size() <= 55){
        //        Scalar color( rand()&255, rand()&255, rand()&255 );
        //        drawContours( drawing, contours, idx, color, CV_FILLED, 8, hierarchy );
            
        //     }
        // }
        for(int i=0; i < contours.size();i++){
           // Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
           // Scalar color = Scalar(0,242,255);
           Scalar color = laneColor;
           drawContours(drawing, contours, i, color, CV_FILLED /* 2 */, 8, hierarchy,0, Point());
        }

        namedWindow("Contours",CV_WINDOW_AUTOSIZE);
        imshow("Contours",drawing);

    } else {
       greyImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
    }

   
   //---
   
}


//---
static void on_trackbar_multi_target_rg2(int, void*)
{
   //---
   if(classifier == RGB_CLASSIFIER){
       if(RESCALE_IMAGE){
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, rescaledImg, destImg);
       } else {
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, img, destImg);
       
       } 
   } else if(classifier = RG_CLASSIFIER){

       if(RESCALE_IMAGE){


          if(LUT_MODE){
               colourClassify_rg_fast(4,rescaledImg, destImg);
          }else {    
               //for CALIBRATION            

               // multi_colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
               //                   min_contrast_angle, max_contrast_angle,
               //                   RedContrast, RedContrastLevel,
               //                   GreenContrast, GreenContrastLevel,
               //                   BlueContrast, BlueContrastLevel,
               //                   min_angle2, max_angle2, min_radius2, max_radius2, 
               //                   min_contrast_angle2, max_contrast_angle2,
               //                   RedContrast2, RedContrastLevel2,
               //                   GreenContrast2, GreenContrastLevel2,
               //                   BlueContrast2, BlueContrastLevel2,
               //                   rescaledImg, destImg);

               //---

              multi_colourClassify_rg_3(min_angle, max_angle, min_radius, max_radius, 
                                   min_contrast_angle, max_contrast_angle,
                                   RedContrast, RedContrastLevel,
                                   GreenContrast, GreenContrastLevel,
                                   BlueContrast, BlueContrastLevel,
                                   min_angle2, max_angle2, min_radius2, max_radius2, 
                                   min_contrast_angle2, max_contrast_angle2,
                                   RedContrast2, RedContrastLevel2,
                                   GreenContrast2, GreenContrastLevel2,
                                   BlueContrast2, BlueContrastLevel2,
                                   min_angle4, max_angle4, min_radius4, max_radius4, 
                                   min_contrast_angle4, max_contrast_angle4,
                                   RedContrast4, RedContrastLevel4,
                                   GreenContrast4, GreenContrastLevel4,
                                   BlueContrast4, BlueContrastLevel4,
                                   rescaledImg, destImg);

               // multi_colourClassify_rg(targetColours, rescaledImg, destImg);


          }
       
           
       } else {

            if(LUT_MODE){
               colourClassify_rg_fast(4,img, destImg);
            }else {    
               //for CALIBRATION 
               colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
                                 min_contrast_angle, max_contrast_angle,
                                 RedContrast, RedContrastLevel,
                                 GreenContrast, GreenContrastLevel,
                                 BlueContrast, BlueContrastLevel,img, destImg);

            }

          
       }
// 
       
   }
    if(opMode == STATIC_FILE_MODE)
       imshow( "Results", destImg );
    
    if(RESCALE_IMAGE){
        imshow( "Results", destImg ); //colour classification results


        greyImg = Mat::zeros(rescaledImg.rows, rescaledImg.cols, CV_8UC3);
        //---
        cvtColor( destImg, greyImg, CV_BGR2GRAY );

        threshold( greyImg, greyImg, 5, 255, CV_THRESH_BINARY );
        // imshow("Binary", greyImg); //not necessary
        //---
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        findContours( greyImg, contours, hierarchy,
                      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
     
        Mat drawing = Mat::zeros(destImg.size(), CV_8UC3);
      
        for(int i=0; i < contours.size();i++){
           // Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));         
           Scalar color = laneColor;
           drawContours(drawing, contours, i, color, CV_FILLED /* 2 */, 8, hierarchy,0, Point());
        }

        Rect boundRect;

        // boundRect = boundingRect( Mat(contours_poly[i]) );

        namedWindow("Contours",CV_WINDOW_AUTOSIZE);
        imshow("Contours",drawing);

    } else {
       greyImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
    }

   
   //---
   
}



//---
static void on_trackbar_multi_target_rg_waypoint(int, void*)
{
   //---
   if(classifier == RGB_CLASSIFIER){
       if(RESCALE_IMAGE){
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, rescaledImg, destImg);
       } else {
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, img, destImg);
       
       } 
   } else if(classifier = RG_CLASSIFIER){

       if(RESCALE_IMAGE){


          if(LUT_MODE){
               colourClassify_rg_fast(4,rescaledImg, destImg);
          }else {    
               //for CALIBRATION            

               // multi_colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
               //                   min_contrast_angle, max_contrast_angle,
               //                   RedContrast, RedContrastLevel,
               //                   GreenContrast, GreenContrastLevel,
               //                   BlueContrast, BlueContrastLevel,
               //                   min_angle2, max_angle2, min_radius2, max_radius2, 
               //                   min_contrast_angle2, max_contrast_angle2,
               //                   RedContrast2, RedContrastLevel2,
               //                   GreenContrast2, GreenContrastLevel2,
               //                   BlueContrast2, BlueContrastLevel2,
               //                   rescaledImg, destImg);

               //---

              multi_colourClassify_rg_3(min_angle, max_angle, min_radius, max_radius, 
                                   min_contrast_angle, max_contrast_angle,
                                   RedContrast, RedContrastLevel,
                                   GreenContrast, GreenContrastLevel,
                                   BlueContrast, BlueContrastLevel,
                                   min_angle2, max_angle2, min_radius2, max_radius2, 
                                   min_contrast_angle2, max_contrast_angle2,
                                   RedContrast2, RedContrastLevel2,
                                   GreenContrast2, GreenContrastLevel2,
                                   BlueContrast2, BlueContrastLevel2,
                                   min_angle4, max_angle4, min_radius4, max_radius4, 
                                   min_contrast_angle4, max_contrast_angle4,
                                   RedContrast4, RedContrastLevel4,
                                   GreenContrast4, GreenContrastLevel4,
                                   BlueContrast4, BlueContrastLevel4,
                                   rescaledImg, destImg);

               // multi_colourClassify_rg(targetColours, rescaledImg, destImg);


          }
       
           
       } else {

            if(LUT_MODE){
               colourClassify_rg_fast(4,img, destImg);
            }else {    
               //for CALIBRATION 
               colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
                                 min_contrast_angle, max_contrast_angle,
                                 RedContrast, RedContrastLevel,
                                 GreenContrast, GreenContrastLevel,
                                 BlueContrast, BlueContrastLevel,img, destImg);

            }

          
       }
// 
       
   }
    if(opMode == STATIC_FILE_MODE)
       imshow( "Results", destImg );
    
    if(RESCALE_IMAGE){      
        //line intersection, etc.
        //---
        Mat outputFrame;
        outputFrame = Mat::zeros(rescaledImg.rows, rescaledImg.cols, CV_8UC3);
        
        //convert to grayscale
        cvtColor(destImg,outputFrame,CV_BGR2GRAY);
        //---
        //Filter blobs
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        findContours( outputFrame, contours, hierarchy,
                      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
     
        Mat drawing = Mat::zeros(outputFrame.size(), CV_8UC3);
      
        for(int i=0; i < contours.size();i++){
           if((contours[i].size() > 20) && (contours[i].size() < 1000)){
              double area = contourArea(contours[i]);
              //http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/hull/hull.html
              // double hull = convexHull(contours[i]);
              // if(area > 10 && area < 350){
              // Scalar color = Scalar(255,0,0);   
              // vector<Point> approx;
              // approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.01, true);
              
              //calculate aspect ratio
              //http://docs.opencv.org/3.1.0/d1/d32/tutorial_py_contour_properties.html
              Rect r = boundingRect(contours[i]);
              double aspectRatio = r.width/r.height;

              if(aspectRatio > 0.3){

              // if((arcLength(approx,true) > 4 && area < 2000)){
              // if( area > 30 && area < 100){
             // Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));         
                Scalar color = Scalar(255,0,0);
                drawContours(drawing, contours, i, color, CV_FILLED /* 2 */, 8, hierarchy,0, Point());
              }
            }
           
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
        Point midPoint = Point(100,130);
        for(;;){
           intersection(midPoint, 0, drawing, LeftP,RightP);
           trials++;
           if(trials > 30){ break;}
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
        //circle(rescaledImg,RightP,3, Scalar(255,255,255));



        //---

        // greyImg = Mat::zeros(rescaledImg.rows, rescaledImg.cols, CV_8UC3);
        // //---
        // cvtColor( destImg, greyImg, CV_BGR2GRAY );

        // threshold( greyImg, greyImg, 5, 255, CV_THRESH_BINARY );
        // // imshow("Binary", greyImg); //not necessary
        // //---
        // vector<vector<Point> > contours;
        // vector<Vec4i> hierarchy;

        // findContours( greyImg, contours, hierarchy,
        //               CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
     
        // Mat drawing = Mat::zeros(destImg.size(), CV_8UC3);
      
        // for(int i=0; i < contours.size();i++){
        //    // Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));         
        //    Scalar color = laneColor;
        //    drawContours(drawing, contours, i, color, CV_FILLED /* 2 */, 8, hierarchy,0, Point());
        // }

        // Rect boundRect;


        // namedWindow("Contours",CV_WINDOW_AUTOSIZE);
        // imshow("Contours",drawing);

    } else {
       greyImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
    }

   
   //---
   
}


//---
static void on_trackbar_multi_target_rg(int, void*)
{
   


   //---
   if(classifier == RGB_CLASSIFIER){
       if(RESCALE_IMAGE){
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, rescaledImg, destImg);
       } else {
          colourClassify_RGB(red_min, green_min, blue_min, red_max, green_max, blue_max, img, destImg);
       
       } 
   } else if(classifier = RG_CLASSIFIER){

       if(RESCALE_IMAGE){


          if(LUT_MODE){
               colourClassify_rg_fast(4,rescaledImg, destImg);
          }else {    
               //for CALIBRATION            

               multi_colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
                                 min_contrast_angle, max_contrast_angle,
                                 RedContrast, RedContrastLevel,
                                 GreenContrast, GreenContrastLevel,
                                 BlueContrast, BlueContrastLevel,
                                 min_angle2, max_angle2, min_radius2, max_radius2, 
                                 min_contrast_angle2, max_contrast_angle2,
                                 RedContrast2, RedContrastLevel2,
                                 GreenContrast2, GreenContrastLevel2,
                                 BlueContrast2, BlueContrastLevel2,
                                 rescaledImg, destImg);


          }
       
           
       } else {

            if(LUT_MODE){
               colourClassify_rg_fast(4,img, destImg);
            }else {    
               //for CALIBRATION 
               colourClassify_rg(min_angle, max_angle, min_radius, max_radius, 
                                 min_contrast_angle, max_contrast_angle,
                                 RedContrast, RedContrastLevel,
                                 GreenContrast, GreenContrastLevel,
                                 BlueContrast, BlueContrastLevel,img, destImg);

            }

          
       }
// 
       
   }
    if(opMode == STATIC_FILE_MODE)
       imshow( "Results", destImg );
    
    if(RESCALE_IMAGE){
        greyImg = Mat::zeros(rescaledImg.rows, rescaledImg.cols, CV_8UC3);
        //---
        cvtColor( destImg, greyImg, CV_BGR2GRAY );

        threshold( greyImg, greyImg, 5, 255, CV_THRESH_BINARY );
        // imshow("Binary", greyImg); //not necessary
        //---
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        // Mat drawing = Mat::zeros(destImg.rows, destImg.cols, CV_8UC3);
        findContours( greyImg, contours, hierarchy,
                      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
        // findContours( greyImg, contours, hierarchy,
        //               CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
        Mat drawing = Mat::zeros(destImg.size(), CV_8UC3);
        // int idx = 0;
        // for( ; idx >= 0; idx = hierarchy[idx][0] )
        // {          
        //     cout << "contours[" << idx << "]=" << contours[idx].size() << endl;
        //     if(contours[idx].size() >= 25 && contours[idx].size() <= 55){
        //        Scalar color( rand()&255, rand()&255, rand()&255 );
        //        drawContours( drawing, contours, idx, color, CV_FILLED, 8, hierarchy );
            
        //     }
        // }
        for(int i=0; i < contours.size();i++){
           // Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
           // Scalar color = Scalar(0,242,255);
           Scalar color = laneColor;
           drawContours(drawing, contours, i, color, CV_FILLED /* 2 */, 8, hierarchy,0, Point());
        }

        namedWindow("Contours",CV_WINDOW_AUTOSIZE);
        imshow("Contours",drawing);

    } else {
       greyImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
    }

   
   //---
   
}


static void help()
{
    cout << "\n This program demonstrates colour classification techniques\n"
             "Usage: \n"
             "  main <video/static> <file_name> >\n";             
}

const char* keys =
{
    "{1| |video.avi|sample video}"
};
void colourDescriptors(){
   //---
    //RGB
    //ORANGE
    red_min = 142; 
    green_min = 0;
    blue_min = 0;

    red_max = 253; 
    green_max = 100;
    blue_max = 60;
    //---


    //Blue Lane v.2.0
    t_min_angle2 = 175;
    t_max_angle2 = 226;
    t_min_radius2 = 31;
    t_max_radius2 = 523;
    t_min_contrast_angle2 = 142; t_max_contrast_angle2 = 251;
    RedContrast2 =2; RedContrastLevel2=1;
    GreenContrast2 = 2; GreenContrastLevel2=1;
    BlueContrast2 = 1; BlueContrastLevel2=1;
    //---
    //---
    min_radius2 = (float) t_min_radius2 / 10000;
    max_radius2 = (float) t_max_radius2 / 10000;
 
    min_angle2 = (float) t_min_angle2;
    max_angle2 = (float) t_max_angle2;
   
    min_contrast_angle2 = (float) t_min_contrast_angle2;
    max_contrast_angle2 = (float) t_max_contrast_angle2;
    //---
    //---
    //Yellow Lane 2
    t_min_angle = 18;
    t_max_angle = 49;
    t_min_radius = 31;//351;
    t_max_radius = 10000;
    t_min_contrast_angle = 42; t_max_contrast_angle = 55;
    //0-degrade, 1-enhance, 2-retain
    RedContrast =2; RedContrastLevel=0;
    GreenContrast = 2; GreenContrastLevel=3;//2;
    BlueContrast = 2; BlueContrastLevel=3;//2;
    //---
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;

    laneColor = Scalar(0,242,255); //yellow
    // //--- 
    //---

  // ORANGE RG v3
    t_min_angle = 323;
    t_max_angle = 339;
    t_min_radius = 1937;
    t_max_radius = 10000;
  
    t_min_contrast_angle = 320; t_max_contrast_angle = 360;
    RedContrast =1; RedContrastLevel=2;
    GreenContrast = 1; GreenContrastLevel=0;
    BlueContrast = 2; BlueContrastLevel=1;
    //---
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;
    //---

     //Initialisation
    initLookUpTable();
    generateLookUpTable_rg(min_angle, max_angle, min_radius, max_radius, 
                         min_contrast_angle, max_contrast_angle,
                         RedContrast, RedContrastLevel,
                         GreenContrast, GreenContrastLevel,
                         BlueContrast, BlueContrastLevel, 4);
    //---

    //  // ORANGE RG v2
    // t_min_angle = 316;
    // t_max_angle = 340;
    // t_min_radius = 1067;
    // max_radius = 1.0;
    // t_min_contrast_angle = 287; t_max_contrast_angle = 354;
    // RedContrast =1; RedContrastLevel=3;
    // GreenContrast = 1; GreenContrastLevel=1;
    // BlueContrast = 0; BlueContrastLevel=1;
}

void find_rg_blue(){
   //Blue Lane v.2.0
    t_min_angle = 175;
    t_max_angle = 226;
    t_min_radius = 31;
    t_max_radius = 523;
    t_min_contrast_angle = 142; t_max_contrast_angle = 251;
    RedContrast =2; RedContrastLevel=1;
    GreenContrast = 2; GreenContrastLevel=1;
    BlueContrast = 1; BlueContrastLevel=1;
    //---
    //---
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;
    //---
        setTrackbarPos("min radius", "Control", t_min_radius);
        setTrackbarPos("max radius", "Control", t_max_radius);
        setTrackbarPos("min angle", "Control", t_min_angle);
        setTrackbarPos("max angle", "Control", t_max_angle);
        setTrackbarPos("min contrast angle", "Control", t_min_contrast_angle);
        setTrackbarPos("max contrast angle", "Control", t_max_contrast_angle);
        setTrackbarPos("red contrast", "Control", RedContrast);
        setTrackbarPos("green contrast", "Control", GreenContrast);
        setTrackbarPos("blue contrast", "Control", BlueContrast);
        setTrackbarPos("red contrast level", "Control", RedContrastLevel);
        setTrackbarPos("green contrast level", "Control", GreenContrastLevel);
        setTrackbarPos("blue contrast level", "Control", BlueContrastLevel);
        imshow("Control",WINDOW_NORMAL);
}

void find_rg_yellow(){
  //Yellow Lane 2
     //Yellow Lane 2
    t_min_angle = 18;
    t_max_angle = 49;
    t_min_radius = 31;//351;
    t_max_radius = 10000;
    t_min_contrast_angle = 42; t_max_contrast_angle = 55;
    //0-degrade, 1-enhance, 2-retain
    RedContrast =2; RedContrastLevel=0;
    GreenContrast = 2; GreenContrastLevel=3;//2;
    BlueContrast = 2; BlueContrastLevel=3;//2;
    //---
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;
    //---
    //---
        setTrackbarPos("min radius", "Control", t_min_radius);
        setTrackbarPos("max radius", "Control", t_max_radius);
        setTrackbarPos("min angle", "Control", t_min_angle);
        setTrackbarPos("max angle", "Control", t_max_angle);
        setTrackbarPos("min contrast angle", "Control", t_min_contrast_angle);
        setTrackbarPos("max contrast angle", "Control", t_max_contrast_angle);
        setTrackbarPos("red contrast", "Control", RedContrast);
        setTrackbarPos("green contrast", "Control", GreenContrast);
        setTrackbarPos("blue contrast", "Control", BlueContrast);
        setTrackbarPos("red contrast level", "Control", RedContrastLevel);
        setTrackbarPos("green contrast level", "Control", GreenContrastLevel);
        setTrackbarPos("blue contrast level", "Control", BlueContrastLevel);
        imshow("Control",WINDOW_NORMAL);
}

void find_rg_sky(){
  //Yellow Lane 2
     //Yellow Lane 2
    t_min_angle = 157;
    t_max_angle = 191;
    t_min_radius = 31;//351;
    t_max_radius = 523;
    t_min_contrast_angle = 142; t_max_contrast_angle = 251;
    //0-degrade, 1-enhance, 2-retain
    RedContrast = 2; RedContrastLevel=1;
    GreenContrast = 1; GreenContrastLevel=1;//2;
    BlueContrast = 1; BlueContrastLevel=1;//2;
    //---
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;
    //---
    //---
        setTrackbarPos("min radius", "Control", t_min_radius);
        setTrackbarPos("max radius", "Control", t_max_radius);
        setTrackbarPos("min angle", "Control", t_min_angle);
        setTrackbarPos("max angle", "Control", t_max_angle);
        setTrackbarPos("min contrast angle", "Control", t_min_contrast_angle);
        setTrackbarPos("max contrast angle", "Control", t_max_contrast_angle);
        setTrackbarPos("red contrast", "Control", RedContrast);
        setTrackbarPos("green contrast", "Control", GreenContrast);
        setTrackbarPos("blue contrast", "Control", BlueContrast);
        setTrackbarPos("red contrast level", "Control", RedContrastLevel);
        setTrackbarPos("green contrast level", "Control", GreenContrastLevel);
        setTrackbarPos("blue contrast level", "Control", BlueContrastLevel);
        imshow("Control",WINDOW_NORMAL);
}

//takes the result from HoughP and compares horizontially each line to a mid point to find the line closest.

// new version that works with Mat input not lines
// find the intersection point on the horizontal line of midpoint in the 
void intersection(Point midPoint, int offset, Mat& img, Point& returnLP, Point& returnRP){
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

  cout << "output::" <<points.size()<<endl;
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

//---
//main <video/static> <rgb/rg> <filename>
int main( int argc, const char** argv )
{
    //---
    RESCALE_IMAGE=true;
    LUT_MODE=false;
    bool KEY_PRESSED=false;
    //---
    //RGB
    //ORANGE
    red_min = 142; 
    green_min = 0;
    blue_min = 0;

    red_max = 253; 
    green_max = 100;
    blue_max = 60;
    // //---

    ClassifierDescription c;

    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //---
    //Sky colour
    // t_min_angle3 = 157;
    // t_max_angle3 = 191;
    // t_min_radius3 = 31;//351;
    // t_max_radius3 = 523;
    // t_min_contrast_angle3 = 142; t_max_contrast_angle3 = 251;
    // //0-degrade, 1-enhance, 2-retain
    // RedContrast3 = 2; RedContrastLevel3=1;
    // GreenContrast3 = 1; GreenContrastLevel3=1;//2;
    // BlueContrast3 = 1; BlueContrastLevel3=1;//2;
    // //---
    // min_radius3 = (float) t_min_radius3 / 10000;
    // max_radius3 = (float) t_max_radius3 / 10000;
 
    // min_angle3 = (float) t_min_angle3;
    // max_angle3 = (float) t_max_angle3;
   
    // min_contrast_angle3 = (float) t_min_contrast_angle3;
    // max_contrast_angle3 = (float) t_max_contrast_angle3;

    // laneColor = Scalar(128,128,128); //grey

    // //---
    // //------------------------------------------------------
    // // colourDescriptors c;
    // c.min_angle = min_angle3;
    // c.max_angle = max_angle3;
    // c.min_radius = min_radius3;
    // c.max_radius = max_radius3;
    // c.min_contrast_angle = min_contrast_angle3;
    // c.max_contrast_angle = max_contrast_angle3;
    // c.colour.val[0] = 128; //blue
    // c.colour.val[1] = 128; //green
    // c.colour.val[2] = 128; //red            
    // targetColours.push_back(c);
    //------------------------------------------------------
    //---
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

    //---
    //SHADOW colour
    // t_min_angle4 = 174;//189;
    // t_max_angle4 = 205;///202;
    // t_min_radius4 = 0;//31;//351;
    // t_max_radius4 = 1723;////246;
    // t_min_contrast_angle4 = 144;//21; 
    // t_max_contrast_angle4 = 222;//195;
    // //0-degrade, 1-enhance, 2-retain
    // RedContrast4 = 1; RedContrastLevel4=2;
    // GreenContrast4 = 1; GreenContrastLevel4=1;//2;
    // BlueContrast4 = 0; BlueContrastLevel4=1;//2;
    // //---
    // min_radius4 = (float) t_min_radius4 / 10000;
    // max_radius4 = (float) t_max_radius4 / 10000;
 
    // min_angle4 = (float) t_min_angle4;
    // max_angle4 = (float) t_max_angle4;
   
    // min_contrast_angle4 = (float) t_min_contrast_angle4;
    // max_contrast_angle4 = (float) t_max_contrast_angle4;

    // laneColor = Scalar(164,73,163); //purple

    // //---
    // //------------------------------------------------------
    // // colourDescriptors c;
    // c.min_angle = min_angle4;
    // c.max_angle = max_angle4;
    // c.min_radius = min_radius4;
    // c.max_radius = max_radius4;
    // c.min_contrast_angle = min_contrast_angle4;
    // c.max_contrast_angle = max_contrast_angle4;
    // c.colour.val[0] = 164; //blue
    // c.colour.val[1] = 73; //green
    // c.colour.val[2] = 163; //red            
    // targetColours.push_back(c);
    //------------------------------------------------------
    //---


    

    //---
/// Contrast Operations ////////////////////////////////// 
                //0 - degrade, 1 - enhance, 2 - retain

  // ORANGE RG v3
    // t_min_angle = 323;
    // t_max_angle = 339;
    // t_min_radius = 1937;
    // t_max_radius = 10000;
    // t_min_contrast_angle = 320; t_max_contrast_angle = 360;
    // RedContrast =1; RedContrastLevel=2;
    // GreenContrast = 1; GreenContrastLevel=0;
    // BlueContrast = 2; BlueContrastLevel=1;
    // //---
    // min_angle = 323.0;
    // max_angle = 339.0;
    // min_radius = 1937/10000;
    // max_radius = 10000/10000;
    // min_contrast_angle = 320.0; max_contrast_angle = 360.0;
    // RedContrast =1; RedContrastLevel=2;
    // GreenContrast = 1; GreenContrastLevel=0;
    // BlueContrast = 2; BlueContrastLevel=1;
    //---
 

 

    //Blue Lane v.3.0 with shadow presence
    // t_min_angle2 = 199;
    // t_max_angle2 = 217;
    // t_min_radius2 = 246;
    // t_max_radius2 = 400;
    // t_min_contrast_angle2 = 34; t_max_contrast_angle2 = 147;
    // RedContrast2 =2; RedContrastLevel2=0;
    // GreenContrast2 = 2; GreenContrastLevel2=1;
    // BlueContrast2 = 1; BlueContrastLevel2=1;
    //---
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    // //Blue Lane v.2.0
    t_min_angle2 = 175;
    t_max_angle2 = 226;
    t_min_radius2 = 31;
    t_max_radius2 = 523;
    t_min_contrast_angle2 = 142; t_max_contrast_angle2 = 251;
    RedContrast2 =2; RedContrastLevel2=1;
    GreenContrast2 = 2; GreenContrastLevel2=1;
    BlueContrast2 = 1; BlueContrastLevel2=1;
 //    //---
    //---
    min_radius2 = (float) t_min_radius2 / 10000;
    max_radius2 = (float) t_max_radius2 / 10000;
 
    min_angle2 = (float) t_min_angle2;
    max_angle2 = (float) t_max_angle2;
   
    min_contrast_angle2 = (float) t_min_contrast_angle2;
    max_contrast_angle2 = (float) t_max_contrast_angle2;
    //---
    //------------------------------------------------------
    // colourDescriptors c;
    c.min_angle = min_angle2;
    c.max_angle = max_angle2;
    c.min_radius = min_radius2;
    c.max_radius = max_radius2;
    c.min_contrast_angle = min_contrast_angle2;
    c.max_contrast_angle = max_contrast_angle2;
    c.colour.val[0] = 255; //blue
    c.colour.val[1] = 0; //green
    c.colour.val[2] = 0; //red            
    targetColours.push_back(c);
    //------------------------------------------------------

    
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //  //Yellow Lane 2
    t_min_angle = 18;
    t_max_angle = 49;
    t_min_radius = 31;//351;
    t_max_radius = 10000;
    t_min_contrast_angle = 42; t_max_contrast_angle = 55;
    //0-degrade, 1-enhance, 2-retain
    RedContrast =2; RedContrastLevel=0;
    GreenContrast = 2; GreenContrastLevel=3;//2;
    BlueContrast = 2; BlueContrastLevel=3;//2;
    //---
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;

    laneColor = Scalar(0,242,255); //yellow
    // //---

    //------------------------------------------------------
    // colourDescriptors c;
    c.min_angle = min_angle;
    c.max_angle = max_angle;
    c.min_radius = min_radius;
    c.max_radius = max_radius;
    c.min_contrast_angle = min_contrast_angle;
    c.max_contrast_angle = max_contrast_angle;
    c.colour.val[0] = 0; //blue
    c.colour.val[1] = 242; //green
    c.colour.val[2] = 255; //red            
    targetColours.push_back(c);
    //------------------------------------------------------
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //---
    //GRASS colour
    t_min_angle4 = 58;
    t_max_angle4 = 119;
    t_min_radius4 = 246;
    t_max_radius4 = 1000;
    t_min_contrast_angle4 = 42; t_max_contrast_angle4 = 103;
    //0-degrade, 1-enhance, 2-retain
    RedContrast4 = 2; RedContrastLevel4=1;
    GreenContrast4 = 2; GreenContrastLevel4=1;//2;
    BlueContrast4 = 1; BlueContrastLevel4=3;//2;
    //---
    min_radius4 = (float) t_min_radius4 / 10000;
    max_radius4 = (float) t_max_radius4 / 10000;
 
    min_angle4 = (float) t_min_angle4;
    max_angle4 = (float) t_max_angle4;
   
    min_contrast_angle4 = (float) t_min_contrast_angle4;
    max_contrast_angle4 = (float) t_max_contrast_angle4;

    laneColor = Scalar(0,255,0); //grey

    //---
    //------------------------------------------------------
    // colourDescriptors c;
    c.min_angle = min_angle4;
    c.max_angle = max_angle4;
    c.min_radius = min_radius4;
    c.max_radius = max_radius4;
    c.min_contrast_angle = min_contrast_angle4;
    c.max_contrast_angle = max_contrast_angle4;
    c.colour.val[0] = 0; //blue
    c.colour.val[1] = 255; //green
    c.colour.val[2] = 0; //red            
    targetColours.push_back(c);
    //------------------------------------------------------
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



    //  //Yellow Lane 3
    // t_min_angle = 18;
    // t_max_angle = 49;
    // t_min_radius = 31;//351;
    // t_max_radius = 10000;
    // t_min_contrast_angle = 42; t_max_contrast_angle = 55;
    // //0-degrade, 1-enhance, 2-retain
    // RedContrast =1; RedContrastLevel=1;
    // GreenContrast = 1; GreenContrastLevel=3;//2;
    // BlueContrast = 2; BlueContrastLevel=3;//2;
    // //---
    // min_radius = (float) t_min_radius / 10000;
    // max_radius = (float) t_max_radius / 10000;
 
    // min_angle = (float) t_min_angle;
    // max_angle = (float) t_max_angle;
   
    // min_contrast_angle = (float) t_min_contrast_angle;
    // max_contrast_angle = (float) t_max_contrast_angle;

    // laneColor = Scalar(0,242,255); //yellow
    // //---

    //Yellow Lane 4 - shadow
    // t_min_angle = 13;
    // t_max_angle = 115;
    // t_min_radius = 31;//351;
    // t_max_radius = 892;
    // t_min_contrast_angle = 27; t_max_contrast_angle = 71;
    // //0-degrade, 1-enhance, 2-retain
    // RedContrast =0; RedContrastLevel=1;
    // GreenContrast = 2; GreenContrastLevel=1;//2;
    // BlueContrast = 1; BlueContrastLevel=2;//2;
    // //---
    // min_radius = (float) t_min_radius / 10000;
    // max_radius = (float) t_max_radius / 10000;
 
    // min_angle = (float) t_min_angle;
    // max_angle = (float) t_max_angle;
   
    // min_contrast_angle = (float) t_min_contrast_angle;
    // max_contrast_angle = (float) t_max_contrast_angle;

    // laneColor = Scalar(0,242,255); //yellow
    //---

    //Initialisation
    // initLookUpTable();
    // generateLookUpTable_rg(min_angle, max_angle, min_radius, max_radius, 
    //                      min_contrast_angle, max_contrast_angle,
    //                      RedContrast, RedContrastLevel,
    //                      GreenContrast, GreenContrastLevel,
    //                      BlueContrast, BlueContrastLevel, 4);
    //---
    cout << "targetColours = " << targetColours.size() << endl;

    VideoCapture cap;
/////////////////////////////////////
// IMAGE FROM FILE    
    // help();
    CommandLineParser parser(argc, argv, keys);
    //---
    if(argc < 3){
        cout << "Error: incomplete parameters" << endl;
        cout << "syntax: " << "main <video/static/live> <rgb/rg> <filename>" << endl;

        exit(-1);
    }
    //---
    string classifierStr(argv[2]);
    std::transform(classifierStr.begin(), classifierStr.end(), classifierStr.begin(), ::tolower);
    if((classifierStr.compare("rgb")==0)){
            classifier = RGB_CLASSIFIER;
            // classifier = RG_CLASSIFIER;    
            cout << "classifier = RGB_CLASSIFIER" << endl;
    } else if((classifierStr.compare("rg")==0)){
            classifier = RG_CLASSIFIER;            
            cout << "classifier = RG_CLASSIFIER" << endl;
    }
    //---
    string opModeStr(argv[1]);
    std::transform(opModeStr.begin(), opModeStr.end(), opModeStr.begin(), ::tolower);
    if((opModeStr.compare("static")==0) || (opModeStr.compare("s")==0)){
            opMode = STATIC_FILE_MODE;        
            cout << "Mode = STATIC_FILE_MODE" << endl;
    }
    if((opModeStr.compare("video")==0) || (opModeStr.compare("v")==0)){
            opMode = VIDEO_MODE;        
            cout << "Mode = VIDEO_MODE" << endl;
            if(argc != 4){
                cout << "incomplete parameters" << endl;
                exit(-1);
            }
    }
    if((opModeStr.compare("live")==0) || (opModeStr.compare("l")==0)){
            opMode = LIVE_MODE;        
            cout << "Mode = LIVE_MODE" << endl;
            if(argc != 3){
                cout << "incorrect parameters" << endl;
                exit(-1);
            }
    }

    //---
    if(opMode == STATIC_FILE_MODE){

        string inputImage(argv[3]);        
        img = imread(inputImage.c_str(), CV_LOAD_IMAGE_COLOR); //colour
        
        if(img.empty())
        {
            cout << "Could not read input image file: " << inputImage << endl;
            return -1;
        } else {
            cout << "Opening input image file: " << inputImage << endl;
        }  
       

    } else if(opMode == VIDEO_MODE){
        string inputVideo(argv[3]); 
        ////////////////////////////////////
        //VIDEO FROM FILE
        
        if (!cap.open(inputVideo.c_str())){
            cout << "video file Is not able to be Opened, does it exist?" << endl;
            cin.ignore();
            return 0;
        }
    }  else if(opMode == LIVE_MODE){
        ////////////////////////////////////
        //VIDEO FROM FILE
        
        if (!cap.open(0)) {
            cout << "came failed to open, does it exist?" << endl;
            cin.ignore();
            return 0;
        }
    }
    //---
    namedWindow("Source_Image", 1 );
    
    namedWindow("Control",WINDOW_NORMAL);
    namedWindow("Control2",WINDOW_NORMAL);
    /// Contrast Operations ////////////////////////////////// 
    //0 - degrade, 1 - enhance, 2 - retain
    
    //---
    //RGB classifier
    if(classifier == RGB_CLASSIFIER){
        namedWindow("Results",1);
        createTrackbar("red min", "Control", &red_min, 255, on_trackbar);
        createTrackbar("red max", "Control", &red_max, 255, on_trackbar);
        createTrackbar("green min", "Control", &green_min, 255, on_trackbar);    
        createTrackbar("green max", "Control", &green_max, 255, on_trackbar);
        createTrackbar("blue min", "Control", &blue_min, 255, on_trackbar);
        createTrackbar("blue max", "Control", &blue_max, 255, on_trackbar);
    } else if(classifier == RG_CLASSIFIER){     
        //---
        //RG classifier - Yellow colour
        createTrackbar("min radius", "Control", &t_min_radius, 10000, on_trackbar_rg);
        createTrackbar("max radius", "Control", &t_max_radius, 10000, on_trackbar_rg);
        createTrackbar("min angle", "Control", &t_min_angle, 360, on_trackbar_rg);
        createTrackbar("max angle", "Control", &t_max_angle, 360, on_trackbar_rg);
        createTrackbar("min contrast angle", "Control", &t_min_contrast_angle, 360, on_trackbar_rg);    
        createTrackbar("max contrast angle", "Control", &t_max_contrast_angle, 360, on_trackbar_rg);
        createTrackbar("red contrast", "Control", &RedContrast, 2, on_trackbar_rg);
        createTrackbar("green contrast", "Control", &GreenContrast, 2, on_trackbar_rg);
        createTrackbar("blue contrast", "Control", &BlueContrast, 2, on_trackbar_rg);
        createTrackbar("red contrast level", "Control", &RedContrastLevel, 6, on_trackbar_rg);
        createTrackbar("green contrast level", "Control", &GreenContrastLevel, 6, on_trackbar_rg);
        createTrackbar("blue contrast level", "Control", &BlueContrastLevel, 6, on_trackbar_rg);
        //---
        //RG classifier - Blue colour
        // createTrackbar("min radius2", "Control2", &t_min_radius2, 10000, on_trackbar_rg);
        // createTrackbar("max radius2", "Control2", &t_max_radius2, 10000, on_trackbar_rg);
        // createTrackbar("min angle2", "Control2", &t_min_angle2, 360, on_trackbar_rg);
        // createTrackbar("max angle2", "Control2", &t_max_angle2, 360, on_trackbar_rg);
        // createTrackbar("min contrast angle2", "Control2", &t_min_contrast_angle2, 360, on_trackbar_rg);    
        // createTrackbar("max contrast angle2", "Control2", &t_max_contrast_angle2, 360, on_trackbar_rg);
        // createTrackbar("red contrast2", "Control2", &RedContrast2, 2, on_trackbar_rg);
        // createTrackbar("green contrast2", "Control2", &GreenContrast2, 2, on_trackbar_rg);
        // createTrackbar("blue contrast2", "Control2", &BlueContrast2, 2, on_trackbar_rg);
        // createTrackbar("red contrast level2", "Control2", &RedContrastLevel2, 6, on_trackbar_rg);
        // createTrackbar("green contrast level2", "Control2", &GreenContrastLevel2, 6, on_trackbar_rg);
        // createTrackbar("blue contrast level2", "Control2", &BlueContrastLevel2, 6, on_trackbar_rg);


        // //---
    }
    //---
   

///////////////////////////////////////////
    if(opMode == VIDEO_MODE || opMode == LIVE_MODE){
        
        long counter=0;
         
        cout << "Total frames = " << cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;
        cout << "Frame dimensions: (width vs. height) " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << " by " << cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        while(cap.isOpened())
        { 
                // cap.set(CV_CAP_PROP_POS_FRAMES, 1312);
                cap >> img; //grab the next frame of video in sequence
                if (img.empty()) {//
                    break;
                }
                // specify fx and fy and let the function compute the destination image size.
                resize(img, rescaledImg, Size(), 0.25, 0.25, INTER_LINEAR);
                
                destImg = Mat::zeros(rescaledImg.size(), CV_8UC3);

                if(classifier == RGB_CLASSIFIER){
                   on_trackbar(red_max, 0);
                } else if(classifier == RG_CLASSIFIER){
                   // on_trackbar_rg(red_max, 0);
                   // on_trackbar_multi_target_rg2(red_max, 0);
                   on_trackbar_multi_target_rg_waypoint(red_max, 0);
                }
               
                //---
                counter=cap.get(CV_CAP_PROP_POS_FRAMES);
                if(counter >= /*cap.get(CV_CAP_PROP_FRAME_COUNT)*/ 3333) {
                   cap.set(CV_CAP_PROP_POS_FRAMES, 1);
                }

                char text[255]; 
                sprintf(text, "Frame %d", (int)counter);

                putText(rescaledImg, text, Point2f(50,50), FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,255,255));
                //---
                // imshow( "Source_Image", img ); //not necessary
                imshow( "Rescaled_Image", rescaledImg );
                
                // if(waitKey(30) >= 0) break;
                char key = (char) waitKey(10);
                if(key == 'y'){ //single step
                  find_rg_yellow();
                  laneColor = Scalar(0,242,255); //yellow
                }
                if(key == 'b'){ //single step
                  find_rg_blue();
                  laneColor = Scalar(255,0,0); //blue
                }
                if(key == 's'){ //single step
                  find_rg_sky();
                  laneColor = Scalar(175,20,0); //blue
                }
                if(key == 'r'){ //reset
                  cap.set(CV_CAP_PROP_POS_FRAMES, 1);  
                }
                
                
                if(key == 'q'){
                  break; //quit
                }

                
        }
        cap.release();
        
    } else if(opMode == STATIC_FILE_MODE){

        imshow( "Source_Image", img );
        destImg = Mat::zeros(img.size(), CV_8UC3);
        if(classifier == RGB_CLASSIFIER){
           on_trackbar(red_max, 0);
        } else if(classifier == RG_CLASSIFIER){
           on_trackbar_rg(red_max, 0);
        }
        waitKey(0);
    }
    //---
    cout << "---finished.---" << endl;
    return 0;
}


