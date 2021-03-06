#ifndef COLORCLASSIFIER_H
#define COLORCLASSIFIER_H


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"



class ColorClassifier{
public:
	ColorClassifier(){
	
	
	};
	int t_min_contrast_angle, t_max_contrast_angle;
	int t_min_angle, t_max_angle, t_min_radius, t_max_radius;
	float min_contrast_angle, max_contrast_angle;
	float min_angle, max_angle, min_radius, max_radius;
	int redContrast, redContrastLevel;
	int greenContrast, greenContrastLevel;
	int blueContrast, blueContrastLevel;
	
private:

};


class red : public ColorClassifier{
public:
	red(){};
	

};

class blue : public ColorClassifier{
public:
	blue(){
	t_min_angle = 175;
    t_max_angle = 226;
    t_min_radius = 31;
    t_max_radius = 523;
    t_min_contrast_angle = 142; t_max_contrast_angle = 251;
    redContrast =2; redContrastLevel=1;
    greenContrast = 2; greenContrastLevel=1;
    blueContrast = 1; blueContrastLevel=1;
    
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;
    
	};
	

};

class yellow : public ColorClassifier{
public:
	yellow(){
		t_min_angle = 18;
		t_max_angle = 49;
		t_min_radius = 31;//351;
		t_max_radius = 10000;
		t_min_contrast_angle = 42; t_max_contrast_angle = 55;
		//0-degrade, 1-enhance, 2-retain
		redContrast =2; redContrastLevel=0;
		greenContrast = 2; greenContrastLevel=3;//2;
		blueContrast = 2; blueContrastLevel=3;//2;
		//---
		min_radius = (float) t_min_radius / 10000;
		max_radius = (float) t_max_radius / 10000;
	 
		min_angle = (float) t_min_angle;
		max_angle = (float) t_max_angle;
	   
		min_contrast_angle = (float) t_min_contrast_angle;
		max_contrast_angle = (float) t_max_contrast_angle;
		
	};
	

};

class green : public ColorClassifier{
public:
	green(){
	t_min_angle = 58;
    t_max_angle = 119;
    t_min_radius = 246;
    t_max_radius = 1000;
    t_min_contrast_angle = 42; t_max_contrast_angle = 103;
    //0-degrade, 1-enhance, 2-retain
    redContrast = 2; redContrastLevel=1;
    greenContrast = 2; greenContrastLevel=1;//2;
    blueContrast = 1; blueContrastLevel=3;//2;
    //---
    min_radius = (float) t_min_radius / 10000;
    max_radius = (float) t_max_radius / 10000;
 
    min_angle = (float) t_min_angle;
    max_angle = (float) t_max_angle;
   
    min_contrast_angle = (float) t_min_contrast_angle;
    max_contrast_angle = (float) t_max_contrast_angle;
    

	};
	

};




#endif

