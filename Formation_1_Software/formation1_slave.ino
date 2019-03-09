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
*/
#include "swarmAlgorithm.h"
//#include "motorControl.h"
#include "RF_Sense.h" 



RF24 radioMS(CE, CSN); // (CE,CNS) radio master to slave
RF24 radioComm (CE, CSN); //(CE, CNS) radio commuincator line
masterState mState;
slaveState sState;
bot mBot;
bot s1Bot;
bot s2Bot;
bot s3Bot;

const unsigned int PIPE = 0x01;


void setup(){
    drive Drive;
    Serial.begin(9600);
    radioMS.begin();
    radioMS.openReadingPipe(1, PIPE);
    radioMS.startListening();
    radioMS.setPALevel(RF24_PA_MIN);
    //printf_begin(); 
    radioMS.printDetails();
    Drive.stopMotor();

    // wait for serial port to open on native usb devices
    while(!Serial){
        delay(1);
    }
}


uint8_t Stop = 0x00  ;
uint8_t Forward = 0x01;
uint8_t Reverse = 0x02;
uint8_t Left = 0x03;
uint8_t Right = 0x04; 
uint8_t command = 0x00;


void loop(){
    drive Drive;
    while(radioMS.available()){


      switch(sState) {

        case S_IDLING:
          bool isReady;
          isReady = false;
          radioMS.read(&isReady, sizeof(isReady));
          if(isReady == false){
            sState = S_IDLING;
          }
          else{
            sState = S_RECEIVE;
          }
        break;

        case S_RECEIVE:
          radioMS.read(&command, sizeof(command));
          sState = S_MOVE; 
        break;

        case S_MOVE:
          switch(command){
            case 0x01:
              Drive.forward(50);
              break;

            case 0x02:
              Drive.backward(50);
              break;

            case 0x03:
              Drive.left90DegreeTurn();
              break;

            case 0x04:
              Drive.right90DegreeTurn();
              break;

            case 0x00:
              Drive.stopMotor();
              break;

            default: 
              Drive.stopMotor();
              break;
          }
          sState = S_SEND;
        break;

        case S_SEND:
          bool isComplete;
          isComplete = true;
          radioMS.write(&isComplete, sizeof(isComplete));
          sState = S_IDLING;
        break;

        default:
          sState = S_IDLING;
      }
    }
  

}   
