#ifndef SWARMALGORITHM_H
#define SWARMALGORITHM_H

// arduino included libraries
#include "Arduino.h"
#include <SPI.h>    
#include <Wire.h> 
//#include <printf.h>
#include "Adafruit_VL6180X.h"

// libraries that were made for SwarmBots
#include "motorControl.h"
#include "RF_Sense.h"
// #include "PID.h"

// defines
#define INIT_VALUE 0

// state machine for master bot
enum masterState {
	M_IDLING = 0,
	M_SEND,
	M_MOVE,
	M_RECEIVE,
	M_UPDATE
};

// state machine for slave bot
enum slaveState {
	S_IDLING = 0,
	S_RECEIVE,
	S_MOVE,
	S_SEND
};

// slave specifiers 
enum slaveName {
	SLAVE0 = 0,
	SLAVE1,
	SLAVE2
};

// characteristics of each bot
struct bot {
	float x;
	float y;
	unsigned int pipe;
	byte lastMove;
	byte direction;
};

class swarm{
public:  
  void initializeBot(bot* botName, unsigned int pipeAddr);
};
#endif
