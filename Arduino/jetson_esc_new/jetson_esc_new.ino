#include <Servo.h>
#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MOTOR_PIN 9
#define led 13
char command;

#define MIN_ANGLE 20
#define MAX_ANGLE 130

#define MIN_POWER 100
#define POWER_OFF 0
#define MAX_POWER 130

Servo esc;
Servo steer;
int lastCommandAngle, lastCommandPower;
int angle = 75, power = 100;
int count = 0;
void setup()
{
  esc.attach(MOTOR_PIN);                  // motor pwm control
  
  steer.attach(10);               // steering
  delay(1);
  steer.write(angle);             // centre at start
  Serial1.begin(9600);            // bluetooth initialise
  
  pinMode( 19, INPUT_PULLUP );    // fix Serial1
  pinMode(led, OUTPUT);           // onboard LED
  //  pinMode(pole, INPUT);           // onboard LED

  digitalWrite(led, HIGH);         // LED off

  //  digitalWrite(pole, HIGH);

  //  while (digitalRead(led) == 0) { // wait until LED turns on via Bluetooth
  //    Bluetooth();
  //    delay(50);
  //  }
  //string1 = "75";                  // empty the bluetooth command
  Serial.begin(9600);             // the usb serial connection
  Serial.print("Program Start\n");
}

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
  if (count <=3){
    esc.write(100);
    count++;
    
  }else{
    esc.write(0);
    count++;
    count = count % 7;
  }
  
  delay(50);
  steer.write(angle);     
}

