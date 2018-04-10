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

				////all this could be made into classes for each
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
//
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
ClassifierDescription c;
	const float PI=3.14;

//functions
 


//Constructor
 FuzzyColor::FuzzyColor(){
 
 	ColorClassifier *(blueCl) = ColorClassifierFactory::createClassifier("blue");
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



}
//function declarations.

void FuzzyColor::processFuzzyColor(Mat& img){

	 

	resize(img, rescaledImg, Size(), 0.25, 0.25, INTER_LINEAR);
	destImg = Mat::zeros(rescaledImg.size(), CV_8UC3);
	cout << "fuzzy Color"<<endl;
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

	laneDetection();
}



void FuzzyColor::multi_colourClassify_rg_3(float min_angle, float max_angle, float min_radius, float max_radius,
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
        Point midPoint = Point(100,170);
        for(;;){
           intersection(midPoint, 0, drawing, LeftP,RightP);
           trials++;
           if(trials > 80){ break;}
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

