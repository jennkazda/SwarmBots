#include "swarmAlgorithm.h"

/*  FUNCTION:
 *    initializeBot
 *
 *  DESIGN DESCRIPTION:
 *    Initializes each bot struct to a default value defined in the beginning of the swarmAlgorithm.h
 *
 *  PARAMETER LIST:
 *    bot* botName: bot struct
 *    unsigned int pipeAddr: the pipe address in which the bot will be assigned
 *    int numberOfSlaves: the number of slaves in the
 *  
 *  RETURNED:
 *    None.
 *
 *  DESIGN OUTLINE:
 *    1) Initialse struct values to default 
 *
 *  DESIGN CONSTRAINTS:
 *    None.
 *  ========================================================================================*/


 void swarm::initializeBot(bot botName[], int numOfSlaves) {
  
  for (int i = 0; i < numOfSlaves; i++) {
    botName[i].x = INIT_VALUE;
    botName[i].y = INIT_VALUE;
    botName[i].pipe = ++i;
    botName[i].lastMove = INIT_VALUE;
    botName[i].direction = INIT_VALUE;
  }
}
