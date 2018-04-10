//=========================================
// Griffith Innovate 2018
// Droid Racer Competition
// Created by Matthew Lee on 8/04/18.
//=========================================

#include <string>
#include <sstream>
#include <cstdio>
#include "EngineController.h"

bool EngineController::update(int steeringAngle, int throttle) {
    this->setSteeringAngle(steeringAngle);
    this->setThrottle(throttle);
    this->updateInstructions();
    return true;
}

/*
 *  Getters and Setters
 */
int EngineController::setSteeringAngle(int steeringAngle) {
    this->steeringAngle = steeringAngle;
    return this->steeringAngle;
}

int EngineController::setThrottle(int throttle) {
    this->throttle = throttle;
    return this->throttle;
}

int EngineController::getSteeringAngle() {
    return this->steeringAngle;
}

int EngineController::getThrottle() {
    return this->throttle;
}

void EngineController::updateInstructions() {
    snprintf(this->instructions, sizeof this->instructions, "%d%c%d", this->steeringAngle, ',', this->throttle);
}
