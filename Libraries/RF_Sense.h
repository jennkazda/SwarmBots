
#ifndef RF_SENSE_H
#define RF_SENSE_H

#include"Arduino.h"
#include <RF24.h>
#include <print.h>
#include <nRF24L01.h>

const byte CE = 9;
const byte CSN = 6;

//put in main code
/*
RF24 radioMS(CE, CSN) // (CE,CNS) radio master to slave
RF24 radioComm (CE, CSN) //(CE, CNS) adio commincator line
*/

class RF_Sense {
public:

	void readPipe(RF24* rfClass, uint8_t number, uint8_t *pipe);
	void writingPipe(RF24* rfClass, uint8_t *pipe);
};

#endif
