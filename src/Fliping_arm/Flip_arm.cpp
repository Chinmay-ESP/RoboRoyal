#include <Arduino.h>
//#include <Servo.h>
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

#define  BallGripMotorPwm 6
#define BallGripMotorDir 2
#define GripPWM 90

#define forw HIGH
#define back LOW

#define servopin 9
void setup() {
  Serial.begin(9600);          //  setup serial
   myservo.attach(servopin);  // attaches the servo on pin 9 to the servo object
   myservo.write(angle);// send servo to the middle at 90 degrees
  pinMode(BallGripMotorDir,OUTPUT);
  pinMode(BallGripMotorPwm,OUTPUT);
 
  
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

    if(Xbox.getButtonClick(UP)){
      angle +=angleStep;
      if(angle >=180){
         angle =180;
      }
        myservo.write(angle); // move the servo to desired angle
       Serial.print("Moved to: ");
       Serial.print(angle);   // print the angle
    }

    if(Xbox.getButtonClick(DOWN)){
       angle -=angleStep;
        if(angle <= 0){
         angle = 0;
        }
       myservo.write(angle); // move the servo to desired angle
       Serial.print("Moved to: ");
       Serial.print(angle);   // print the angle
      
    }
    if (Xbox.getButtonClick(X)){
    
        digitalWrite(BallGripMotorDir,forw);
        analogWrite(BallGripMotorPwm,GripPWM);
        
    }
    if(Xbox.getButtonClick(BACK)){
      
        digitalWrite(BallGripMotorDir,back);
        analogWrite(BallGripMotorPwm,0);
    }
    if (Xbox.getButtonClick(B)){
       
        digitalWrite(BallGripMotorDir,back);
        analogWrite(BallGripMotorPwm,GripPWM);
       
    }
    delay(1);
   }

}