/*
//put in main code
masterState mState;
slaveState sState;
bot mBot;
bot s1Bot;
bot s2Bot;
bot s3Bot;
*/

//arduino included libraries
#include "Arduino.h"
#include <SPI.h>    
#include <Wire.h> 
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include "Adafruit_VL6180X.h"

//libraries that were made for SwarmBots
#include "motorControl.h"
#include "RF_Sense.h"
#include "PID.h"


enum masterState {
	IDLING = 0,
	SEND,
	MOVE,
	RECEIVE,
	UPDATE
};

enum slaveState {
	IDLING = 0,
	RECEIVE,
	MOVE,
	SEND
};

struct bot {
	float x;
	float y;
	byte lastMove;
	byte direction;
};




