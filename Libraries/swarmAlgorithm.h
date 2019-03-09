
// arduino included libraries
#include "Arduino.h"
#include <SPI.h>    
#include <Wire.h> 
#include <printf.h>
#include "Adafruit_VL6180X.h"

// libraries that were made for SwarmBots
#include "motorControl.h"
#include "RF_Sense.h"
#include "PID.h"

// defines
#define DEFAULT 0

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

/*  FUNCTION:
 *		initializeBot
 *
 *  DESIGN DESCRIPTION:
 *		Initializes each bot struct to a default value defined in the beginning of the swarmAlgorithm.h
 *
 *  PARAMETER LIST:
 *		bot* botName: bot struct
 *		unsigned int pipeAddr: the pipe address in which the bot will be assigned
 *	
 *  RETURNED:
 *		None.
 *
 *  DESIGN OUTLINE:
 *		1) Initialse struct values to default 
 *
 *  DESIGN CONSTRAINTS:
 *		None.
 *  ========================================================================================*/

void initializeBot(bot* botName, unsigned int pipeAddr) {
	botName->x = DEFAULT;
	botName->y = DEFAULT;
	botName->pipe = pipeAddr;
	botName->lastMove = DEFAULT;
	botName->direction = DEFAULT;
}






