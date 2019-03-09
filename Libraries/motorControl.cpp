#include "motorControl.h"

/*  FUNCTION:
 *    motorControl.h
 *
 *  DESIGN DESCRIPTION:
 *    This header file holds all driving functions for the bots
 *
 *  PARAMETER LIST:
 *    int speed
 *
 *  RETURNED:
 *    None.
 *
 *  DESIGN OUTLINE:
 *    1) Write digitial signal to GPIO motor pins.
 *    2) Write PWM duty cycles to motor pins.
 *
 *  DESIGN CONSTRAINTS:
 *    None.
 *  ========================================================================================*/
void drive::forward(int speed) {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, speed);
	analogWrite(PWMB, speed);
}
void drive::rightTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 100);
	analogWrite(PWMB, 20);
	delay(500);
}
void drive::leftTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 20);
	analogWrite(PWMB, 100);
	delay(500);
}

void drive::stopMotor() {
	digitalWrite(STDBYE, LOW);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, LOW);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, LOW);
	analogWrite(PWMA, 0);
	analogWrite(PWMB, 0);
}

void drive::right90DegreeTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 100);
	analogWrite(PWMB, 20);
	delay(500);
}

void drive::left90DegreeTurn() {
	digitalWrite(STDBYE, HIGH);
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	analogWrite(PWMA, 20);
	analogWrite(PWMB, 100);
	delay(500);
}
void drive::backward(int speed) {
  digitalWrite(STDBYE, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}
