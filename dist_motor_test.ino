#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();
const byte stdbye = 1;
const byte Ain1 = 4;
const byte Ain2 = 5;
const byte PWMA = 3;
const byte Bin1 = 7;
const byte Bin2 = 8;
const byte PWMB = 11;

void setup() {
  Serial.begin(9600);
  pinMode(stdbye, OUTPUT);
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  digitalWrite(stdbye, LOW);

  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
}

void forward(int speed) {
  digitalWrite(stdbye, HIGH);
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, HIGH);
  digitalWrite(Bin1, LOW);
  digitalWrite(Bin2, HIGH);
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

void rightTurn() {
  digitalWrite(stdbye, HIGH);
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, HIGH);
  digitalWrite(Bin1, LOW);
  digitalWrite(Bin2, HIGH);
  analogWrite(PWMA, 50);
  analogWrite(PWMB, 100);
  delay(500);
}

void leftTurn() {
  digitalWrite(stdbye, HIGH);
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, HIGH);
  digitalWrite(Bin1, LOW);
  digitalWrite(Bin2, HIGH);
  analogWrite(PWMA, 100);
  analogWrite(PWMB, 50);
  delay(500);
}

void rightSpin() {
  digitalWrite(stdbye, HIGH);
  digitalWrite(Ain1, HIGH);
  digitalWrite(Ain2, LOW);
  digitalWrite(Bin1, LOW);
  digitalWrite(Bin2, HIGH);
  analogWrite(PWMA, 100);
  analogWrite(PWMB, 100);
  delay(250);
}

void leftSpin() {
  digitalWrite(stdbye, HIGH);
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, HIGH);
  digitalWrite(Bin1, HIGH);
  digitalWrite(Bin2, LOW);
  analogWrite(PWMA, 100);
  analogWrite(PWMB, 100);
  delay(250);
}

void backward(int speed) {
  digitalWrite(stdbye, HIGH);
  digitalWrite(Ain1, HIGH);
  digitalWrite(Ain2, LOW);
  digitalWrite(Bin1, HIGH);
  digitalWrite(Bin2, LOW);
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

void stopMotor() {
  digitalWrite(stdbye, LOW);
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, LOW);
  digitalWrite(Bin1, LOW);
  digitalWrite(Bin2, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void loop() {

  
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

 // if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); Serial.println(range);
    if( range > 50){
      forward(100);
      Serial.println("Going forwards!");
    }
    else if((range <= 50) && (range > 25)) {
      rightTurn();
      Serial.println("Turning right!");
    }
    else if(range < 20) {
      stopMotor();
      Serial.println("Stop!");
      
    }
    
//  }

 
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  delay(50);
}
