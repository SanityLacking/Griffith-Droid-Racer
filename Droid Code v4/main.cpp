//=========================================
// Created by Matthew Lee on 8/04/18.
//=========================================
#include <iostream>
#include <stdio.h>
#include "EngineController.h"

using namespace std;

int main(){
    EngineController ctrl;
    ctrl.setSteeringAngle(0.5);
    ctrl.setThrottle(1);
    cout << ctrl.getSteeringAngle() << endl;
    cout << ctrl.getThrottle() << endl;
    return 0;
}
