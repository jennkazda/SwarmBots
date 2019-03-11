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
#include <printf.h>

#define DEBUG_IDLE
#define DEBUG_SEND
#define DEBUG_MOVE
#define DEBUG_UPDATE
#define NUM_SLAVES 3

RF24 Radio(CE, CSN);
Adafruit_VL6180X distSensor = Adafruit_VL6180X(); 
drive Drive;
swarm Swarm;
RF_Sense RF;
masterState mState;
slaveState sState;
bot mBot;
bot sBot[NUM_SLAVES];

#define STOP    0
#define FORWARD 1
#define REVERSE 2
#define LEFT    3
#define RIGHT   4

bool result     = false;
bool isReady    = true;
int state       = mState;
uint8_t command = 0x00;
unsigned int commandPipe  =  0xFF;

void setup(){
    Serial.begin(9600);
    printf_begin();
    Radio.begin();
    Radio.openWritingPipe(commandPipe);
    Radio.stopListening();
    Radio.setPALevel(RF24_PA_MIN);
    Radio.printDetails();
    Drive.stopMotor();
    Swarm.initializeBot(sBot, NUM_SLAVES);
}

void loop(){


    switch(state){
/**********************************************************************************************/      
      case M_IDLING:
      
        #ifdef DEBUG_IDLE
          Serial.println("IDLE");
        #endif
        
        while(command == STOP){
          /* Sends a isReady boolen value to the command center */
          RF.writingPipe(&Radio, commandPipe);
          while(Radio.write(&isReady, sizeof(isReady)) == false);
          Serial.println("IDLE WRITE");

          /* Reads in command data from command center */
          RF.readPipe(&Radio, 1, commandPipe);
          Radio.read(&command, sizeof(command));
          Serial.println(command);  
        }
        
        
        
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

          Serial.println("SEND WRITE");
          
          /* Acknowledges transmitted message was received by distant end */
          while(Radio.write(&command, sizeof(command)) == false);

          Serial.println("SEND READ`");
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
              delay(100);
              break;

            case REVERSE:
              Drive.backward(50);
              delay(100);
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
         
      Drive.stopMotor();
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
 
