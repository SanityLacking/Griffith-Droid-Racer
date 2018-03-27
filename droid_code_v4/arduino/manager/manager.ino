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
    
    Serial.printf("Angle: %3d, power: %3d.", angle, power);

    myGroid.setSteeringAngle(angle);
    myGroid.setPower(power);
  }
}
