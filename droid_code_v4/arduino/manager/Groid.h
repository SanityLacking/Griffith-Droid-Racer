// Groid.h
// Griffith Droid Racer 2018

#ifndef __GROID_H_GUARD__
#define __GROID_H_GUARD__

#include <Servo.h>

class Groid {
    Servo esc, steer;
    int angle = 0, power = 0;
public:
    Groid();
    ~Groid();
    void prepareESC();
    void prepareSteering();
    void prepareSerial();

    bool setSteeringAngle(int);
    bool setPower(int);

    const static int BAUD_RATE = 9600;
    const static int ESC_PIN = 9;
    const static int STEER_PIN = 10;
    const static int MIN_ANGLE = 30;
    const static int MID_ANGLE = 75;
    const static int MAX_ANGLE = 130;
    const static int MIN_POWER = 100;
    const static int MAX_POWER = 130;
};

#endif
