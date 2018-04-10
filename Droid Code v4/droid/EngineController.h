//=========================================
// Griffith Innovate 2018
// Droid Racer Competition
// Created by Matthew Lee on 8/04/18.
//=========================================

#ifndef DROID_RACER_ENGINECONTROLLER_H
#define DROID_RACER_ENGINECONTROLLER_H


class EngineController {
private:
    float steeringAngle = 0.0;
    float throttle = 0.0;

public:
    float setSteeringAngle(float steeringAngle);
    float setThrottle(float throttle);
    float getSteeringAngle();
    float getThrottle();
    virtual bool step(float steeringAngle, float throttle);
};


#endif //DROID_RACER_ENGINECONTROLLER_H
