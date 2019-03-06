

#include "Arduino.h"
#include "motorControl.h"


void motorControl::forward(int speed) {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, speed);
	analogWrite(PWMB, speed);
}
void motorControl::rightTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 100);
	analogWrite(PWMB, 20);
	delay(500);
}
void motorControl::leftTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 20);
	analogWrite(PWMB, 100);
	delay(500);
}

void motorControl::stopMotor() {
	digitalWrite(STDBYE, LOW);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, LOW);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, LOW);
	analogWrite(PWMA, 0);
	analogWrite(PWMB, 0);
}

void motorControl::right90DegreeTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 100);
	analogWrite(PWMB, 20);
	delay(500);
}

void motorControl::left90DegreeTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 20);
	analogWrite(PWMB, 100);
	delay(500);
}
