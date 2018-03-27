#include <Servo.h>

#define BAUD_RATE 9600

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000

#define ESC_PIN   9    // ESC = Electronic Steering Control
#define STEER_PIN 10
#define LED_PIN   13

#define MIN_ANGLE 30
#define MID_ANGLE 75
#define MAX_ANGLE 130

#define POWER_OFF 0
#define MIN_POWER 100
#define MAX_POWER 130

char command;

Servo esc, steer;

int lastCommandAngle, lastCommandPower;
int angle = 75, power = 100;
int count = 0;

void prepareESC()
{
  // Prepare the connection to the ESC.

  esc.attach(MOTOR_PIN);
  delay(10);
}

void prepareSteering()
{
  // Prepare the connection to the steering mechanism.

  steer.attach(STEER_PIN);
  delay(10);
}

void prepareSerial()
{
  // Prepare the serial connection between the host (RasPi) and Arduino.

  Serial.begin(BAUD_RATE);
  delay(10);
}

void setSteeringAngle(int angle)
{
  // Set the angle of steering.
  // As of 27/03/2018, the allow values for steering are in the range:
  //   angle: 30 <= angle <= 130

  int _angle = MID_ANGLE
  if(angle >= MIN_ANGLE and angle <= MAX_ANGLE)
  {
    _angle = angle;
  }
  else
  {
    if(angle < MIN_ANGLE)
    {
      _angle = MIN_ANGLE;
    }
    else
    {
      _angle = MAX_ANGLE;
    }
  }

  steer.write(_angle);
  delay(10);
}


void setup()
{
  prepareElectronicSpeedController();
  prepareSteering();
  prepareSerial();

  setSteeringAngle(MID_ANGLE);  // Set the steering angle to be the centre on startup.

  Serial.print("Program Start\n");
}

// Everything following was used prior to 2018.

void loop()
{
  //processSerialInput(Serial1);
  //processSerialInput(Serial);
  //Bluetooth();

  usbSerial();
  processInput();
}
void usbSerial() {
  String inputStr;
  if (Serial.available() > 0)  {
    inputStr = "";
  }
  while (Serial.available() > 0)  {
    command = ((byte)Serial.read());
    if (command == ':')  {
      break;
    }
    else {
      inputStr += command;
    }
    delay(10);
  }
  if(inputStr.length() > 0){
    int commaIndex = inputStr.indexOf(",");
    String firstValue = inputStr.substring(0, commaIndex);
    angle = firstValue.toInt();
    String secondValue = String(power);
    if(commaIndex > 0){
      secondValue = inputStr.substring(commaIndex+1); // To the end of the string
      power = secondValue.toInt();
    }

    String outStr;
    outStr += firstValue +","+ secondValue;
    Serial.println(outStr);

  }
}

void processInput(){
  //check for a last command
  if (angle < MIN_ANGLE)
    angle = MIN_ANGLE;
  else if (angle > MAX_ANGLE)
    angle = MAX_ANGLE;



  //checkcommand();
  steer.write(angle);
  if (count <= 3){
    esc.write(power);
    count++;

  }else{
    esc.write(0);
    count++;
    count = count % 10;
  }

  delay(50);
  steer.write(angle);
}
