#include <Servo.h>

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MOTOR_PIN 9
#define led 13

// ----- Ananlog Pins ----- //
const int sector1 = A1, sector2 = A2, sector3 = A3, sector4 = A4, sector5 = A5;
#define Tstop A9

//#define Tforward A10//was going the wrong way
//#define Treverse A11//
int Tforward= A10;
int Treverse =A11;

int Tsoftleft= A12;
int Tsoftright =A13;
int Thardleft =A14;
int Thardright= A15;
String string1, string2;
char command;
int speed1 = 1500, angle = 75, flag1 = 0, throttle = 1500, turnduration = 150, pole = 5; // turn duration (delay in milliseconds)
const int sonarLtrig = 35, sonarLecho = 37;
const int sonarRtrig = 45, sonarRecho = 47;
//int ForwardFlag = 1;
int ManualMode = 1;
int STOPFLAG = 0;
int BURST = 0;

Servo esc;
Servo steer;

void setup()
{
  esc.attach(9);                  // motor pwm control
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
  string1 = "75";                  // empty the bluetooth command
  Serial.begin(9600);             // the usb serial connection
  Serial.print("Program Start\n");
}

void loop()
{
  //processSerialInput(Serial1);
  //processSerialInput(Serial);
  //Bluetooth();

  UsbSerial();
  //string1 = "75";
  
  if (STOPFLAG==1){
    ebrake();
  }
  else{
    if (ManualMode==0){
      //checkcommand();  // no longer using gpio for communication.
    }
    if (digitalRead(led) == 1) {
      //checkcommand();
      throttle = speed1;
      //angle=100;
      //Serial.println(angle);
      steer.write(angle);
      esc.writeMicroseconds(throttle);
      delay(50);
      steer.write(angle);
      if (flag1 == 1) {             // if turned, centre the steering
        angle = 75;
        flag1 = 0;
        delay(turnduration);        // duration of turn
      }
      if (BURST == 1){
        delay(250); //stop and go mode
        ebrake();   //
        delay(50);  //
      }
    }
    if (digitalRead(led) == 0) {
      ebrake();
      speed1 = 1500;
      //checkcommand();
  
      //digitalWrite(led, HIGH);         // LED off
  }
  //  while (digitalRead(pole) == 0) {
  //    ebrake();
  //  }   
    
  //Serial.begin(9600);
  //Serial.println(speed1);
  }
}
void UsbSerial() {
  if (Serial.available() > 0)  {
    string1 = "";
  }
  while (Serial.available() > 0)  {
    command = ((byte)Serial.read());
    if (command == ':')  {
      break;
    }
    else {
      string1 += command;
    }
    delay(10);
  }
  if(string1.length() > 0){
    int commaIndex = string1.indexOf(",");      
    String firstValue = string1.substring(0, commaIndex);      
    int r = firstValue.toInt();
    String secondValue = "0";
    if(commaIndex > 0){
      secondValue = string1.substring(commaIndex+1); // To the end of the string      
    }
    int g = secondValue.toInt();
    String outStr;
    outStr += string1 +" : "+ r + ","+g;
    Serial.println(outStr);
    //Serial.print(string1);
    //Serial.print(" : ");
    //Serial.print(r);
    //Serial.print(',');    
    //Serial.println(100);//    
    angle = r;
    speed1 = g;    
  }  
}

void ebrake() {
  if (speed1 > 1500) {
    speed1 = 1500;
    esc.writeMicroseconds(1500);
    delay(10);
    esc.writeMicroseconds(1300);
    delay(50);
    esc.writeMicroseconds(1500);
    delay(10);
    esc.writeMicroseconds(1300);
    delay(50);
  }
  if (speed1 < 1500) {
    speed1 = 1500;
    esc.writeMicroseconds(1500);
    delay(50);
    esc.writeMicroseconds(1600);
    delay(50);
    esc.writeMicroseconds(1500);
    delay(50);
  }
}

