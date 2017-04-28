//  ----------------------  //
//  Author: Harinder Singh  //
//    Dated: 03/07/2016     //
//  ----------------------  //

// When interfacing a brushless motor through an ESC with an Arduino for the first time, this code needs to run. 
// The way this calibration process runs is the before turning the motor on, code should be running on the Arduino.
// Initially, the speed is set to one extreme. Use the keys "+" and "-" to shift towards the other extreme. 
// Do this a couple of times to finish the calibration. At this point, the motor should start spinning.
  
#include <Servo.h>

#define MAX_SIGNAL 2000           // Max forward speed
#define MIN_SIGNAL 1000           // Max reverse speed
#define MOTOR_PIN 9               // ESC attached to arduino as a servo
#define led 13                    // on board LED

Servo motor;
int speed1 = 1050;

void setup() {
  Serial.begin(9600);
  Serial.println("Power the ESC now and send any key when ready");

  motor.attach(MOTOR_PIN);
  motor.writeMicroseconds(MAX_SIGNAL);

  // Wait for input
  while (!Serial.available());
  Serial.read();
  motor.writeMicroseconds(MIN_SIGNAL);
  delay(4000);

  motor.writeMicroseconds(speed1); //start changing motor speed

  Serial.println("Start loop...");
}


void loop() { 
  if(Serial.available() > 0) {
    int incomingByte = Serial.read();
    if(incomingByte == 43)      // sends the character '+'
      speed1=0;                  
    else if(incomingByte == 45) // sends the character '-'
      speed1=180;                  
    Serial.print("Set speed to: ");
    Serial.println(speed1);

    motor.write(speed1);
  }
}
