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
	MIDLING = 0,
	MSEND,
	MMOVE,
	MRECEIVE,
	MUPDATE
};

enum slaveState {
	SIDLING = 0,
	SRECEIVE,
	SMOVE,
	SSEND
};

enum direction {
	STOP = 0,
	FORWARD = 1,
	BACKWARD = 2,
	LEFT = 3,
	RIGHT = 4
};

struct bot {
	float x;
	float y;
	byte lastMove;
	direction direction;
};


enum slaveName {
	SLAVE0 = 0,
	SLAVE1,
	SLAVE2
};



