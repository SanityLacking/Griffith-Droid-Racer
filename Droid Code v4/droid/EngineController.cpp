//=========================================
// Griffith Innovate 2018
// Droid Racer Competition
// Created by Matthew Lee on 8/04/18.
//=========================================

#include "EngineController.h"

bool EngineController::step(float steeringAngle, float throttle) {
    /**
     * @param float steeringAngle - A value between -1 (left turn) and 1 (right turn). 0 specifies no turn
     * @param float throttle - A value between -1 (reverse) and 1 (forward). 0 specifies no throttle
     * Set the parameters for the steering and throttle
     * Set as a virtual function (likely to be overwritten by class inheritance)
     * @return true if successful
     */
    this->setSteeringAngle(steeringAngle);
    this->setThrottle(throttle);
    return true;
}


/*
 *  Getters and Setters
 */
float EngineController::setSteeringAngle(float steeringAngle) {
    this->steeringAngle = steeringAngle;
    return this->steeringAngle;
}

float EngineController::setThrottle(float throttle) {
    this->throttle = throttle;
    return this->throttle;
}

float EngineController::getSteeringAngle() {
    return this->steeringAngle;
}

float EngineController::getThrottle() {
    return this->throttle;
}