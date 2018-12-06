#include <Wire.h>

const byte stdbye = 1;
const byte Ain1 = 4;
const byte Ain2 = 5;
const byte PWMA = 3;
const byte Bin1 = 7;
const byte Bin2 = 8;
const byte PWMB = 11;

void setup() { // initialization
  Serial.begin(9600);
  pinMode(stdbye, OUTPUT);
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  digitalWrite(stdbye, LOW);
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
  forward(100);
  delay(1000);
  rightTurn();
  forward(100);
  delay(1000);
  leftTurn();
  forward(100);
  delay(1000);
  rightSpin();
  forward(100);
  delay(1000);
  leftSpin();
  stopMotor();
  delay(1000000);
}
