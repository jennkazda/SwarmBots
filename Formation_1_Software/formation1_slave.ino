/*  AUTHOR: Josue Mendez & Jenn Kazda
/*  ===================================================================
*   FILE: formation1_slave.c
*   MODULE: formation1_slave
*
*   TITLE: Slave Bot Algorithm for Formation #1
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
*   1.1     2019-03-08  JK
*           Initial Revision.
*   1.2     2019-03-09  JK
*           ADDED COMMENTS, MADE CHANGES TO ACCOMADATE MULTIPLE SLAVE BOTS.
*/
#include "swarmAlgorithm.h"



#define DEBUG_IDLE                                      //debug define for IDLING state
#define DEBUG_SEND                                      //debug define for SEND state
#define DEBUG_MOVE                                      //debug define for MOVE state
#define DEBUG_RECEIVE                                   //debug define for RECEIVE state


RF24 Radio(CE, CSN);                                    //initalizes Radio with correct pins

drive Drive;                                            //initalizes Drive class
RF_Sense RF;                                            //initalizes RF class
slaveState sState;                                      //initalizes slave state enum
slaveName sName;                                        //initalizes slave name enum
bot sBot;                                               //initalizes slave bot (only one is needed for slave code, needs to be hardcoded in main for each bot)

//swarm Swarm;                                          //initalizes swarm class

unsigned int commandPipe  =  0xFF;                      //pipe for command bot pipline
unsigned int slavePipe [] = {0x01, 0x02, 0x03, 0x04};   //different addresses for each smaster slave bot pipeline
int slaveNum = sName;                                   //sets slave name to an integer

uint8_t Stop = 0x00;                                    //value to stop motors
uint8_t Forward = 0x01;                                 //value to move forward
uint8_t Reverse = 0x02;                                 //value to move backwards
uint8_t Left = 0x03;                                    //value to pivot left
uint8_t Right = 0x04;                                   //value to pivot right
uint8_t command = 0x00;                                 //default value for command to be sent from master

void setup(){
    Serial.begin(9600);               
    Radio.begin();
    Radio.openReadingPipe(1, sBot.pipe);
    Radio.startListening();
    Radio.setPALevel(RF24_PA_MIN);
    //printf_begin(); 
    //Radio.printDetails();
    Drive.stopMotor();                                  //begin with motors stopped

    
    while(!Serial){                                     // wait for serial port to open on native usb devices
        delay(1);
    }
}

void loop(){
   sBot.pipe = slavePipe[0];                            //NEED TO HARDCODE WHICH SLAVE THIS IS
   sName = SLAVE0;                                      //HARDCODES SLAVE NAME AND NUMBER 
   RF.readPipe(&Radio, slaveNum, sBot.pipe);            //opens radio for specific slave using slave number and slave pipe address
   RF.writingPipe(&Radio, sBot.pipe);                   //opens radio for specific slave using the slave pipe address

    while(Radio.available()){

      switch(sState) {                                  //state machinefor slave
/**********************************************************************************************/
        case S_IDLING:                                  //IDLING state: waits until master is ready to send command
          #ifdef DEBUG_IDLE                             //debug statement
          Serial.println("IDLE");
         #endif

          bool isReady;                                 //bool for determing next state
          isReady = false;
          Radio.read(&isReady, sizeof(isReady));        //reads rf sensor for isReady bool
          if(isReady == false){                         //if master is not ready, then slave stays in idle
            sState = S_IDLING;
          }
          else{                                         //if master is ready, slave goes to RECEIVE to get command
            sState = S_RECEIVE;
          }
        break;
/**********************************************************************************************/
        case S_RECEIVE:                                 //RECEIVE state: slave gets command from the master of how to move
          #ifdef DEBUG_RECEIVE                          //debug statement
          Serial.println("RECEIVE");
          #endif

          Radio.read(&command, sizeof(command));        //reads rf sensor for command
          sState = S_MOVE;                              //slave goes to MOVE state
        break;
/**********************************************************************************************/
        case S_MOVE:                                    //MOVE state: lave executes whatever command it is given
          #ifdef DEBUG_MOVE                             //debug statement
          Serial.println("MOVE");           
          #endif
          
          switch(command){                              //switch case statement of possible movements for the slave to receive
            case 0x01:                                  //case 0x01: drive forward at a standard speed
              Drive.forward(50);                        //calls drive class that holds forward(int speed) function
              break;

            case 0x02:                                  //case 0x02: drive backwards at a standard speed
              Drive.backward(50);                       //calls drive class that holds backward(int speed) function
              break;

            case 0x03:                                  //case 0x03: pivot left (rotate left)
              Drive.left90DegreeTurn();                 //calls drive class that holds left90DegreeTurn() function
              break;

            case 0x04:
              Drive.right90DegreeTurn();                //case 0x04: pivot right (rotate right)
              break;                                    //calls drive class that holds right90DegreeTurn() function

            case 0x00:                                  //case 0x00: stop driving
              Drive.stopMotor();                        //calls drive class that holds stopMotor() function
              break;

            default:                                    //default case: stop driving
              Drive.stopMotor();                        //calls drive class that holds stopMotor() function
              break;
          }
          sState = S_SEND;                              //slave goes to SEND state after executing the command
        break;
/**********************************************************************************************/
        case S_SEND:                                    //SEND case: sends a bool back to the master to tell the master that the command has been completed
          #ifdef DEBUG_SEND                             //debug statement
          Serial.println("SEND");
          #endif

          bool isComplete;                              //bool used to send messge to master
          isComplete = true;
          Radio.write(&isComplete, sizeof(isComplete)); //wirtes the message to the master via the rf sensor
          sState = S_IDLING;                            //slave goes back to IDLING to wait for next command
        break;
/**********************************************************************************************/
        default:                                        //default state: sends slave to IDLING
          sState = S_IDLING;                            
      }
    }
  

}   
