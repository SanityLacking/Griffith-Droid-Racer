// Groid.c
// Griffith Droid Racer 2018

#include <Arduino.h>
#include "Groid.h"

Groid::Groid()
{
  this->prepareESC();
  this->prepareSteering();
  this->prepareSerial();
}

void Groid::prepareESC()
{
  // Prepare the connection to the ESC.

  this->esc.attach(this->ESC_PIN);
  delay(10);
}

void Groid::prepareSteering()
{
  // Prepare the connection to the steering mechanism.

  this->steer.attach(this->STEER_PIN);
  delay(10);
}

void Groid::prepareSerial()
{
  // Prepare the serial communication to RasPi.

  Serial.begin(this->BAUD_RATE);
  delay(10);
}

int Groid::setSteeringAngle(int angle)
{
  // Set the angle of steering.

  int _angle = angle < this->MIN_ANGLE ? this->MIN_ANGLE : (angle > this->MAX_ANGLE ? this-> MAX_ANGLE : angle);

  this->steer.write(_angle);
  delay(10);

  return angle;
}

int Groid::setPower(int power)
{
  // Set the power of steering.

  int _power = 0;
  if(power != 0)
  {
    _power = power < this->MIN_POWER ? this->MIN_POWER : (power > this->MAX_POWER ? this->MAX_POWER : power);
  }
  
  this->esc.write(_power);
  delay(10);

  return _power;
}
