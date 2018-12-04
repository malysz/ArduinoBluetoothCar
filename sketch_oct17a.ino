#include <SPI.h>
#include <PS3BT.h>      //Include the necessary libraries.

#define enA 9 //lewy
#define in1 4
#define in2 5
#define enB 3 //prawy
#define in3 6
#define in4 7

int motorSpeedA = 0;
int motorSpeedB = 0;

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);


void setup() {
  //bluetooth setup
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  
  //engines setup
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void loop() {
    Usb.Task();

    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {

      
      int xAxis = PS3.getAnalogHat(LeftHatX);
      int yAxis = PS3.getAnalogHat(LeftHatY);

      if (yAxis < 127) {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          motorSpeedA = map(yAxis, 127, 0, 0, 255);
          motorSpeedB = map(yAxis, 127, 0, 0, 255);     
      } else if (yAxis > 127) {
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          motorSpeedA = map(yAxis, 127, 255, 0, 255);
          motorSpeedB = map(yAxis, 127, 255, 0, 255);
      } else {
          motorSpeedA = 0;
          motorSpeedB = 0;
      }

      if (xAxis < 127) {
          int mappedX = map(xAxis, 127, 0, 0, 255);
          motorSpeedA = motorSpeedA - mappedX;
          motorSpeedB = motorSpeedB + mappedX;
          if (motorSpeedA < 0) motorSpeedA = 0;
          if (motorSpeedB > 255) motorSpeedB = 255;

      } else if (xAxis > 127) {
          int mappedX = map(xAxis, 127, 255, 0, 255);
          motorSpeedA = motorSpeedA + mappedX;
          motorSpeedB = motorSpeedB - mappedX;
          if (motorSpeedB < 0) motorSpeedB = 0;
          if (motorSpeedA > 255) motorSpeedA = 255;
      }

    analogWrite(enA, motorSpeedA);
    analogWrite(enB, motorSpeedB);
      
    }

    if (PS3.getButtonClick(UP)) {
      Serial.print(F("\r\nUp"));
      if (PS3.PS3Connected) {
        PS3.setLedOff();
        PS3.setLedOn(LED1);
    }
  }

}
