// ArduinoController.h
// Griffith Droid Racer 2019

#ifndef __ARDUINOCONTROLLER_H_GUARD__
#define __ARDUINOCONTROLLER_H_GUARD__

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>

class ArduinoController
{
  private:
    int angle = 0, power = 0;
    int serialPort = 0;
    struct termios tty;
    int configureTty();
    int update();

    // Normalising power:
    //  power = ((MAX_POWER - MIN_POWER) / (100-0)) * rawPower + MIN_POWER
    //  power ~ (3/10)rawPower + 100
    // Special case for rawPower <= 0: power = NO_POWER
    int getNormalisedPower(int);

    // Natural inverse of normalisePower.
    int getRawPower(int);

    // Normalising angle:
    //  angle = ((MAX_ANGLE - MIN_ANGLE) / (100-0)) * rawAngle + MIN_ANGLE
    //  angle ~ rawAngle + 30
    // Special case for rawAngle <= 0: angle = MIN_ANGLE
    int getNormalisedAngle(int);

    // Natural inverse of normaliseAngle.
    int getRawAngle(int);

  public:
    const static speed_t BAUD_RATE = B9600;
    const static int MIN_ANGLE = 30;
    const static int MID_ANGLE = 75;
    const static int MAX_ANGLE = 130;
    const static int NO_POWER = 0;
    const static int MIN_POWER = 100;
    const static int MAX_POWER = 130;

    ArduinoController(std::string);
    ~ArduinoController();

    std::string receive();
    void setPower(int);
    void setAngle(int);
    int getPower();
    int getAngle();
    void stop();
};

#endif