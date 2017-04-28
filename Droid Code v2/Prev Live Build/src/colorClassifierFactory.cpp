#include "colorClassifierFactory.h"
#include "colorClassifier.h"


ColorClassifier *ColorClassifierFactory::createClassifier(String input){
	if(input == "red"){
		return new red();
	}else if(input == "blue"){
		return new blue();
	}else if(input == "yellow"){
		return new yellow();
	}else{//default case
		return new red();
	}


	
}
