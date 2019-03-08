

#include"Arduino.h"
#include <RF24.h>
#include <print.h>
#include <nRF24L01.h>
#include "RF_Sense.h"

/*  FUNCTION:
 *		readPipe(RF24* rfClass, uint8_t number, uint64_t pipe)
 *
 *  DESIGN DESCRIPTION:
 *		This function will allow the device to open a pipe for reading.
 *
 *  PARAMETER LIST:
 *		RF24* rfClass: RF24 class instance from RF24.h
 *		uint8_t number: Which pipe number to open, 0-5
 *		uint64_t pipe: The address of the pipe to open
 *
 *  RETURNED:
 *		None.
 *
 *  DESIGN OUTLINE:
 *		1.) Open reading pipe.
 *      2.) Device will start listening.
 *
 *  DESIGN CONSTRAINTS:
 *		None.
 *  ========================================================================================*/

void RF_Sense::readPipe(RF24* rfClass, uint8_t number, uint64_t pipe)
{
	rfClass.openReadingPipe(number, pipe);
	rfClass.startListening();
}

/*  FUNCTION:
 *		writingPipe(RF24* rfClass, uint64_t pipe)
 *
 *  DESIGN DESCRIPTION:
 *		This function will allow the device to open a pipe for writing.
 *
 *  PARAMETER LIST:
 *		RF24* rfClass: RF24 class instance from RF24.h
 *		uint64_t pipe: The address of the pipe to open
 *
 *  RETURNED:
 *		None.
 *
 *  DESIGN OUTLINE:
 *		1.) Open writing pipe.
 *      2.) Device will stop listening and be ready to write data.
 *
 *  DESIGN CONSTRAINTS:
 *		None.
 *  ========================================================================================*/

void RF_Sense::writingPipe(RF24* rfClass, uint64_t pipe)
{
	rfClass.openWritingPipe(PIPE);
	rfClass.stopListening();
}
