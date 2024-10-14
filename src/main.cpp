#include <Arduino.h>
#include <Servo.h>
#include <XBOXUSB.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXUSB Xbox(&Usb);

Servo myservo;  // create servo object to control a servo

int angle =90;    // initial angle  for servo
int angleStep =5;


void setup() {
  Serial.begin(9600);          //  setup serial
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(LEFT,INPUT_PULLUP); // assign pin 12 ass input for Left button
  pinMode(RIGHT,INPUT_PULLUP);// assing pin 2 as input for right button
  myservo.write(angle);// send servo to the middle at 90 degrees

   #if !defined(_MIPSEL_)
    while (!Serial); // Wait for serial port to connect
    #endif
    
    if (Usb.Init() == -1) {
        Serial.print(F("\r\nOSC did not start"));
        while (1); // Halt
    }
    Serial.print(F("\r\nXBOX USB Library Started"));
}

void loop() {
  Usb.Task();
   if (Xbox.Xbox360Connected) {  
  while(Xbox.getButtonClick(A) == LOW){

    if (angle > 0 && angle <= 180) {
      angle = angle - angleStep;
       if(angle < 0){
        angle = 0;
       }else{
      myservo.write(angle); // move the servo to desired angle
      Serial.print("Moved to: ");
      Serial.print(angle);   // print the angle
      Serial.println(" degree");
       }
    }
  delay(100); // waits for the servo to get there
  }// while

  while(Xbox.getButtonClick(Y) == LOW){
    if (angle >= 0 && angle <= 180) {
      angle = angle + angleStep;
      if(angle >180){
        angle =180;
       }else{
      myservo.write(angle); // move the servo to desired angle
      Serial.print("Moved to: ");
      Serial.print(angle);   // print the angle
      Serial.println(" degree");
       }
    }
    
  delay(1); // waits for the servo to get there
  }// 

  
 }

}