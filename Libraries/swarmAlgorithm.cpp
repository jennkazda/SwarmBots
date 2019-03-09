#include "swarmAlgorithm.h"

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

void swarm::initializeBot(bot* botName, unsigned int pipeAddr) {
	botName->x = INIT_VALUE;
	botName->y = INIT_VALUE;
	botName->pipe = pipeAddr;
	botName->lastMove = INIT_VALUE;
	botName->direction = INIT_VALUE;
}
