//=========================================
// Created by Matthew Lee on 8/04/18.
//=========================================

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "ArduinoController.h"

ArduinoController::ArduinoController(const char *port) {
    /**
     * @param const char *port: The serial port to connect to the arduino (e.g /dev/ttyACM1)
     */
    this->port = port;
}

bool ArduinoController::step(int steeringAngle, int throttle) {
    this->update(steeringAngle, throttle);
    this->arduino = fopen(this->port,"w");
    fprintf(this->arduino, this->instructions);
    fclose(this->arduino);
}
