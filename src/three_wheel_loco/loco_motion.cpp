#include <Arduino.h>
#include <XBOXUSB.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXUSB Xbox(&Usb);

#define SIZE 3  // Size of the 3x3 matrix and 3x1 vector

// Define a 3x3 matrix matrixA
float matrixA[SIZE][SIZE] = {
    {0.58, -0.33, 0.33},
    {-0.58, -0.33, 0.33},
    {0, 0.67, 0.33}
};
#define f1_PWM 3
#define f2_PWM 5
#define f3_PWM 6



#define f1_DIR  2
#define f2_DIR  4
#define f3_DIR  7
// Resultant 3x1 vector resultC
float resultC[SIZE] = {0};

// Vector to hold joystick values
float vectorB[SIZE] = {0, 0, 0}; // Left Hat Y, Left Hat X, Right Hat X

void setup() {
    Serial.begin(9600);  // Use a standard baud rate
    pinMode(f1_PWM ,OUTPUT);
    pinMode(f2_PWM ,OUTPUT);
    pinMode(f3_PWM ,OUTPUT);

    pinMode(f1_DIR ,OUTPUT);
    pinMode(f2_DIR,OUTPUT);
    pinMode(f3_DIR ,OUTPUT);
    #if !defined(MIPSEL)
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
    // Check if Xbox controller is connected
if (Xbox.Xbox360Connected) {
    // Read joystick values and map them to a suitable range (0 to 255)
    if (Xbox.getAnalogHat(LeftHatY)>7500){
        vectorB[0] = map(Xbox.getAnalogHat(LeftHatY),  7500, 32767, 0, 255);
    }else if(Xbox.getAnalogHat(LeftHatY)<-7500){
        vectorB[0] = map(Xbox.getAnalogHat(LeftHatY), -7500, -32767, 0, -255);
    }else{
        vectorB[0]=0;
    }
    if (Xbox.getAnalogHat(LeftHatX)>7500){
        vectorB[1] = map(Xbox.getAnalogHat(LeftHatX),  7500, 32767, 0, 255);
    }else if(Xbox.getAnalogHat(LeftHatX)<-7500) {
        vectorB[1] = map(Xbox.getAnalogHat(LeftHatX), -7500, -32767, 0, -255);
    }else{
        vectorB[1]=0;
    }
     if (Xbox.getAnalogHat(RightHatX)>7500){
        vectorB[2] = map(Xbox.getAnalogHat((RightHatX)),  7500, 32767, 0, 255);
    }else if(Xbox.getAnalogHat((RightHatX))<-7500){
        vectorB[2] = map(Xbox.getAnalogHat((RightHatX)), -7500, -32767, 0, -255);
    }else{
        vectorB[2]=0;
    }
    // vectorB[1] = map(Xbox.getAnalogHat(LeftHatX), -32768, 32767, 0, 255);
    // vectorB[2] = map(Xbox.getAnalogHat(RightHatX), -32768, 32767, 0, 255);

    // // Print joystick values for debugging
    // Serial.print("Joystick Values: ");
    // Serial.print(vectorB[0]);
    // Serial.print(", ");
    // Serial.print(vectorB[1]);
    // Serial.print(", ");
    // Serial.println(vectorB[2]);

    // Perform matrix-vector multiplication
    for (int i = 0; i < SIZE; i++) {
        resultC[i] = 0; // Reset the resultant vector
        for (int j = 0; j < SIZE; j++) {
            resultC[i] += matrixA[i][j] * vectorB[j]; // Multiply matrix A with vector B
        }
    }

    // Print the resultant vector resultC
    Serial.print("Resultant Vector C: ");
    for (int i = 0; i < SIZE; i++) {
        Serial.print(resultC[i]);
        Serial.print(" ");
    }
    Serial.print("F1: ");
    Serial.println(resultC[0]);
     Serial.print("F2: ");
    Serial.println(resultC[1]);
     Serial.print("F3: ");
    Serial.println(resultC[2]);
    // Constrain and write PWM values
    analogWrite(f1_PWM, abs(resultC[0]));
    analogWrite(f2_PWM, abs(resultC[1]));
    analogWrite(f3_PWM, abs(resultC[2]));

    // Control direction based on the sign of resultC
    digitalWrite(f1_DIR, resultC[0] > 0 ? HIGH : LOW);
    digitalWrite(f2_DIR, resultC[1] > 0 ? HIGH : LOW);
    digitalWrite(f3_DIR, resultC[2] > 0 ? HIGH : LOW);

    // Optional: Add a small delay for stability
    delay(100);
}

}