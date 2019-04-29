/*  AUTHOR: Josue Mendez & Jenn Kazda
/*  ===================================================================
*   FILE: formation1_command.c
*   MODULE: formation1_command
*
*   TITLE: Command Center Algorithm for Formation #1
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
*   1.1     2019-03-09  JK
*           Initial Revision.
*
*/

  //////////////
  // INCLUDES //
  //////////////
  
  #include <SPI.h>    
  #include <Wire.h> 
  #include <RF24.h>
  #include <printf.h>
  //#include "Adafruit_VL6180X.h"

  /////////////
  // DEFINES //
  /////////////
  #define STOP    0x00                                      
  #define FORWARD 0x01                                   
  #define REVERSE 0x02                                   
  #define LEFT    0x03                                      
  #define RIGHT   0x04      

  #define NUM_SLAVES 1

  // State machine for master bot //
  enum commandState{
  C_IDLING = 0,
  C_GET,
  C_SEND
  };
  commandState cState;

   // Characteristics of each bot //
  struct bot 
  {
    float x;
    float y;
    unsigned int pipe;
    byte lastMove;
    byte direction;
  };
  
  ///////////////////////////
  // FUNCTION DECLARATIONS //
  ///////////////////////////
  
  // RF //
  void readPipe(uint8_t pipeNumber, uint8_t pipeAddress);
  void writePipe(uint8_t pipeAddress);

  ///////////////
  // VARIABLES //
  ///////////////

  // Pinout //
  int const UP_BTN = 2;
  int const DOWN_BTN = 4;
  int const LEFT_BTN = 5;
  int const RIGHT_BTN = 3;
  int const E_BTN = 6;
  int const F_BTN = 7;
  int const JOYSTICK_BTN = 8;
  int const JOYSTICK_AXIS_X = A0;
  int const JOYSTICK_AXIS_Y = A1;
  const byte CE = 9;
  const byte CSN = 10;
  int buttons[] = { UP_BTN, DOWN_BTN, LEFT_BTN, RIGHT_BTN, E_BTN, F_BTN, JOYSTICK_BTN };

  // RF// 
  int state = cState;
  bool isReady = 1;
  uint8_t command = 0x00;
  uint8_t commandPipe[5]  = {'C','O','M','M','1'};
  uint8_t slaveAddresses[3][5] = {
    {'S','L','A','V','1'},
    {'S','L','A','V','2'},
    {'S','L','A','V','3'}
  };
  // int remoteNodeData[2] = {1,1};

  /////////////////////
  // INITIALIZATIION //
  /////////////////////
  
  RF24 radio(CE, CSN);
  
void setup() {

   pinMode(LED_BUILTIN, OUTPUT);                       //initialize digital pin LED_BUILTIN as an output.
  for (int i; i < 7; i++)  {                          //initialize all buttons on the joystick to be input buttons
    pinMode(buttons[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
    printf_begin();
    
    /* Begin radio object */
    radio.begin();

    /* Setup read pipe to remote node */
    //radio.openReadingPipe(1,commandPipe);
    //radio.startListening();

    radio.openWritingPipe(commandPipe);
    radio.stopListening();

    /* Set power level of the radio */
    radio.setPALevel(RF24_PA_LOW);

    /* Set radio channel to use - ensure all slaves match this */
    radio.setChannel(0x69);

    /* Set time between retries and max no. of retries */
    radio.setRetries(4, 10);

    /* Enable ackpayload - enables each slave to reply with data */
    //radio.enableAckPayload();

    /* Preload the payload with initial data */
    // radio.writeAckPayload(1, &remoteNodeData, sizeof(remoteNodeData));
    
    /* Print out radio configuration data on serial monitor */
    radio.printDetails();

      while(!Serial){                                      // wait for serial port to open on native usb devices
      delay(1);
  }
}

void loop() {
  bool tx_sent = false;
  uint8_t data = 0;
  isReady = 0;
  
  Serial.println("Start");
    
    switch(cState){
/***********************************************************************************************************/       
      case C_IDLING:

         /*   Serial.print("isntReady: ");
            Serial.println(isReady);
            RF.readPipe(&radio, 1, commandPipe);
            radio.writeAckPayload(1,&isReady, sizeof(isReady));
            if (isReady == 1){
              digitalWrite(LED_BUILTIN, HIGH);
              Serial.print("isReady: ");
              Serial.println(isReady);*/
              cState = C_GET;
            //  isReady = 0;
           // }
           // else{
           //   digitalWrite(LED_BUILTIN, LOW);                     //turn the LED off by making the voltage LOW
           //   cState = C_IDLING;
           // }
            break;
/***********************************************************************************************************/ 
      case C_GET:
      while( command == STOP){
            if (!digitalRead(UP_BTN)){
              command = STOP;
            }
            else if (!digitalRead(DOWN_BTN)){
              command = STOP;
            }
            else if (!digitalRead(LEFT_BTN)){
              command = LEFT;
            }
            else if (!digitalRead(RIGHT_BTN)){
              command = RIGHT;
            }
            else if (!digitalRead(E_BTN)){
              command = STOP;
            }
            else if (!digitalRead(F_BTN)){
              command = STOP;
            }
            else if (!digitalRead(JOYSTICK_BTN)){
              command = STOP;
            }
            else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == 0 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == 1){
              command = FORWARD;
            }
            else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == -1 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == 0){
              command = LEFT;
            }
            else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == 1 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == 0){
              command = RIGHT;
            }
            else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == 0 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == -1){
              command = REVERSE;
            }
            else {
              command = STOP;
              
            for(int i = 0; i < NUM_SLAVES; i++){
              writePipe(slaveAddresses[i]);
              tx_sent = radio.write(&command, sizeof(command));
          }
        }
      }
            Serial.print("Command = ");
            Serial.println(command);
            cState = C_SEND;
            break;
/***********************************************************************************************************/            
    case C_SEND:
            Serial.print("Sending command: ");
            Serial.println(command);

          for(int i = 0; i < NUM_SLAVES; i++){
            
            writePipe(slaveAddresses[i]);
            tx_sent = radio.write(&command, sizeof(command));
            Serial.print("Slave_ID: "); Serial.println(i);

              while(tx_sent == false){
                tx_sent = radio.write(&command, sizeof(command));
              }
          }
            command = STOP;
            cState = C_IDLING;
            break;
/***********************************************************************************************************/ 
   }
}

void readPipe(uint8_t pipeNumber, uint8_t pipeAddress)
{
  radio.openReadingPipe(pipeNumber, pipeAddress);
  radio.startListening();
}

void writePipe(uint8_t pipeAddress)
{
  radio.openWritingPipe(pipeAddress);
  radio.stopListening();  
}
