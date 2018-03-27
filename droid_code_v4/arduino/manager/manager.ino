// manager.ino
// Griffith Droid Racer 2018

#include "Groid.h"

Groid myGroid;
int angle, power;
char command;

void setup()
{
  Serial.print("Program Start\n");
}

void loop()
{
  String input = "";

  while(Serial.available() > 0)
  {
    command = (byte) Serial.read();
    if(command == ':')
    {
      break;
    }
    else
    {
      input += command;
    }
    delay(10);
  }

  if(input.length() > 0)
  {
    int commaIndex = input.indexOf(",");
    angle = input.substring(0, commaIndex).toInt();
    power = commaIndex > 0 ? (input.substring(commaIndex + 1).toInt()) : power;

    int actualAngle = myGroid.setSteeringAngle(angle);
    int actualPower = myGroid.setPower(power);   

    Serial.print("Angle: ");
    Serial.print(actualAngle);
    Serial.print(", power: ");
    Serial.print(actualPower);
    Serial.println(".");
  }
}
