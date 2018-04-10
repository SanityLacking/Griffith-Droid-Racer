//=========================================
// Created by Matthew Lee on 8/04/18.
//=========================================

#ifndef DROID_RACER_ARDUINOCONTROLLER_H
#define DROID_RACER_ARDUINOCONTROLLER_H

#include <cstring>
#include "EngineController.h"


class ArduinoController : public EngineController{
    /**
     *  This class controls the connection to the Arduino device and extends
     *  the engine controller class
     */
private:
    const char * port;
    FILE * arduino;
public:
    ArduinoController(const char * port);
    bool step(int steeringAngle, int throttle);
};


#endif //DROID_RACER_ARDUINOCONTROLLER_H
