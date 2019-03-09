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

RF24 Radio(CE, CSN);
Adafruit_VL6180X distSensor = Adafruit_VL6180X(); 
drive Drive;
swarm Swarm;
RF_Sense RF;
masterState mState;

bool result = false;
int state       = mState;
uint8_t Stop    = 0x00;
uint8_t Forward = 0x01;
uint8_t Reverse = 0x02;
uint8_t Left    = 0x03;
uint8_t Right   = 0x04; 
uint8_t command = 0x00;
unsigned int commandPipe  =  0xFF;
unsigned int slavePipe [] = {0x01, 0x02, 0x03, 0x04};

void setup(){
    Serial.begin(9600);
    //printf_begin();
    Radio.begin();
    Radio.openReadingPipe(1,commandPipe);
    Radio.startListening();
    Radio.setPALevel(RF24_PA_MIN);
    //Radio.printDetails();
    Drive.stopMotor();

    // wait for serial port to open on native usb devices
    while(!Serial){
        delay(1);
    }
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
        
        RF.writingPipe(&Radio, slavePipe[0]);
        result = Radio.write(&command, sizeof(command));
        
        state++;
      break;
/**********************************************************************************************/
      case M_MOVE:

        #ifdef DEBUG_MOVE
          Serial.println("MOVE");
        #endif
      
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
 
