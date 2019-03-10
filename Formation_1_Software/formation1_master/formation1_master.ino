/*  AUTHOR: Josue Mendez & Jenn Kazda
/*  ===================================================================
*   FILE: Swarm_Algorithm_Master.c
*   MODULE: Swarm_Algorithm_Master
*
*   TITLE: Master Bot Algorithm for Formation #1
*
*   MODULE DESCRIPTION:
*   
*   MAIN FUNCTIONS:
*
*   HELPER FUNCTIONS:
*
*   ADDITIONAL INFORMATION:
*
*   --------------------------------------------------------------------
*   REVISION HISTORY:
*   Rev     Date        Init
*           Changes
*   -----   --------    -----
*   1.1     2019-03-05  JM
*           Initial Revision.
*/

#include "swarmAlgorithm.h"

#define DEBUG_IDLE
#define DEBUG_SEND
#define DEBUG_MOVE
#define DEBUG_UPDATE
#define NUM_SLAVES 2

RF24 Radio(CE, CSN);
Adafruit_VL6180X distSensor = Adafruit_VL6180X(); 
drive Drive;
swarm Swarm;
RF_Sense RF;
masterState mState;
slaveState sState;
bot mBot;
bot sBot[NUM_SLAVES];


/*
#ifdef ONE_SLAVE 
  bot s1Bot;
  unsigned int slavePipe [] = {0x01};
  Swarm.initializeBot(s1Bot, slavePipe[0]);
#endif

#ifdef TWO_SLAVES 
  bot s1Bot;
  bot s2Bot;
  unsigned int slavePipe [] = {0x01, 0x02};
  Swarm.initializeBot(s1Bot, slavePipe[0]);
  Swarm.initializeBot(s2Bot, slavePipe[1]);
#endif

#ifdef THREE_SLAVES 
  bot s1Bot;
  bot s2Bot;
  bot s3Bot;
  unsigned int slavePipe [] = {0x01, 0x02, 0x03};
  Swarm.initializeBot(s1Bot, slavePipe[0]);
  Swarm.initializeBot(s2Bot, slavePipe[1]);
  Swarm.initializeBot(s3Bot, slavePipe[2]);
#endif

#ifdef FOUR_SLAVES 
  bot s1Bot;
  bot s2Bot;
  bot s3Bot;
  bot s4Bot;
  unsigned int slavePipe [] = {0x01, 0x02, 0x03, 0x04};
  Swarm.initializeBot(s1Bot, slavePipe[0]);
  Swarm.initializeBot(s2Bot, slavePipe[1]);
  Swarm.initializeBot(s3Bot, slavePipe[2]);
  Swarm.initializeBot(s4Bot, slavePipe[3]);
#endif

#define NUM_OF_SLAVES (sizeof(slavePipe)/sizeof(slavePipe[0]))
*/
#define STOP    0x00
#define FORWARD 0x01
#define REVERSE 0x02
#define LEFT    0x03
#define RIGHT   0x04

bool result     = false;
int state       = mState;
uint8_t command = 0x00;
unsigned int commandPipe  =  0xFF;

void setup(){
    Serial.begin(9600);
    // printf_begin();
    Radio.begin();
    Radio.openWritingPipe(commandPipe);
    Radio.stopListening();
    Radio.setPALevel(RF24_PA_MIN);
    //Radio.printDetails();
    Drive.stopMotor();
    Swarm.initializeBot(sBot, NUM_SLAVES);
}

void loop(){

  while(Radio.available())
  {
    switch(state){
/**********************************************************************************************/      
      case M_IDLING:
      
        #ifdef DEBUG_IDLE
          Serial.println("IDLE");
        #endif

        /* Reads in command data from command center */
        Radio.read(&command, sizeof(command));  
        
//        if(command != Stop){}
//        else if(command != Forward){}
//        else if(command != Reverse){}
//        else if(command != Left){}
//        else if(command != Right){}
//        else{state++;}
          state++;
      break;
/**********************************************************************************************/
      case M_SEND:

        #ifdef DEBUG_SEND
          Serial.println("SEND");
        #endif

        /* Master bot transmits command data to slave bot receivers (Slave bots 0-3) */
        for(int i = 0; i < NUM_SLAVES; i++)
        {
          #ifdef DEBUG_SEND
            Serial.print("Slave Pipe: "); Serial.println(i);
          #endif

          /* Switch radio pipes to transmit to different bots */
          RF.writingPipe(&Radio, sBot[i].pipe);

          /* Acknowledges transmitted message was received by distant end */
          while(Radio.write(&command, sizeof(command)) == false);
        }

        state++;
      break;
/**********************************************************************************************/
      case M_MOVE:

        #ifdef DEBUG_MOVE
          Serial.println("MOVE");
        #endif

         /* Master moves according to command data */
         switch(command){
            case FORWARD :
              Drive.forward(50);
              break;

            case REVERSE:
              Drive.backward(50);
              break;

            case LEFT:
              Drive.left90DegreeTurn();
              break;

            case RIGHT:
              Drive.right90DegreeTurn();
              break;

            case STOP:
              Drive.stopMotor();
              break;

            default: 
              Drive.stopMotor();
              break;
          }
      state++;
      break;
/**********************************************************************************************/
      case M_UPDATE:

        #ifdef DEBUG_UPDATE
          Serial.println("UPDATE");
        #endif
        
      state++;
      break;
/**********************************************************************************************/      
    }
  }
}  
 
