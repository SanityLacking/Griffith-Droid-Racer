// groid.c
// Griffith Droid Racer 2019

#include <Arduino.h>
#include "groid.h"

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

  if(angle < this->MIN_ANGLE)
  {
    angle = this->MIN_ANGLE;
  }
  else if(angle > this->MAX_ANGLE)
  {
    angle = this->MAX_ANGLE;
  }

  this->steer.write(angle);
  delay(10);

  return angle;
}

int Groid::setPower(int power)
{
  // Set the motor power.

  if(power != this->NO_POWER)
  {
    if(power <= this->MIN_POWER)
    {
      power = this->MIN_POWER;
    }
    else if(power > this->MAX_POWER)
    {
      power = this->MAX_POWER;
    }
  }

  this->esc.write(power);
  delay(10);

  return power;
}

