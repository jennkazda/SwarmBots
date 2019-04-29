/*  AUTHOR: Josue Mendez & Jenn Kazda
/*  ===================================================================
*   FILE: formation2_command.c
*   MODULE: formation1_command
*
*   TITLE: Command Center Algorithm for Formation #2
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
*   1.1     2019-04-18  JK
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

  #define NUM_SLAVES 2

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
 // uint8_t command = 0x00;
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
  uint8_t slave0formation[] = {FORWARD, FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT};
  uint8_t slave1formation[] = {FORWARD, LEFT,    FORWARD, FORWARD, LEFT,    FORWARD, FORWARD, LEFT,    FORWARD, FORWARD, LEFT,    FORWARD};
  uint8_t slave2formation[] = {FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT,   FORWARD};
  uint8_t command[2] = {STOP, STOP};

  Serial.println("Start");



  if (!digitalRead(E_BTN)){
      
     for(int i = 0; i < sizeof(slave0formation); i++){
      
            int moveSequence = i+1;
            Serial.print("Move Sequence: "); Serial.println(moveSequence);
            
            radio.openWritingPipe(slaveAddresses[0]);
            radio.stopListening();  
            tx_sent = radio.write(&slave0formation[i], sizeof(slave0formation[i]));
            Serial.print("slave 0 command: "); Serial.println(slave0formation[i]);

            tx_sent = radio.write(&slave0formation[i], sizeof(slave0formation[i]));
                         
            radio.openWritingPipe(slaveAddresses[1]);
            radio.stopListening(); 
            tx_sent = radio.write(&slave1formation[i], sizeof(slave1formation[i]));
            Serial.print("slave 1 command: "); Serial.println(slave1formation[i]);

            tx_sent = radio.write(&slave1formation[i], sizeof(slave1formation[i]));

            radio.openWritingPipe(slaveAddresses[2]);
            radio.stopListening(); 
            tx_sent = radio.write(&slave2formation[i], sizeof(slave2formation[i]));
            Serial.print("slave 2 command: "); Serial.println(slave2formation[i]);

            tx_sent = radio.write(&slave2formation[i], sizeof(slave2formation[i]));
         
            delay(3000);
          }
          
            
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
