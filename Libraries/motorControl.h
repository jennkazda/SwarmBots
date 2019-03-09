#include "Arduino.h"

const byte STDBYE = 12;
const byte AIN1 = 4;
const byte AIN2 = 5;
const byte PWMA = 11;
const byte BIN1 = 7;
const byte BIN2 = 8;
const byte PWMB = 10;

class drive
{
public:
	void forward(int speed);
	void rightTurn();
	void leftTurn();
	void stopMotor();
	void right90DegreeTurn();
	void left90DegreeTurn();
	void backward(int speed);
};
