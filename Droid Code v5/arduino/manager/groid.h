// groid.h
// Griffith Droid Racer 2019

#ifndef __GROID_H_GUARD__
#define __GROID_H_GUARD__

#include <Servo.h>

class Groid {
    Servo esc, steer;
    int angle = 0, power = 0;
public:
    void prepareESC();
    void prepareSteering();
    void prepareSerial();

    int setSteeringAngle(int);
    int setPower(int);

    const static int BAUD_RATE = 9600;
    const static int ESC_PIN = 9;
    const static int STEER_PIN = 10;
    const static int MIN_ANGLE = 30;
    const static int MID_ANGLE = 75;
    const static int MAX_ANGLE = 130;
    const static int NO_POWER = 0;
    const static int MIN_POWER = 100;
    const static int MAX_POWER = 130;
};

#endif

