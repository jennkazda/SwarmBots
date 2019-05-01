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
*           INITIAL REVISION
*   1.2     2019-04-25  JK
*           FINAL REVISION
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

  #define NUM_SLAVES 3
  #define ARRAY_SIZE 12

  // State machine for master bot //
  enum commandState{
  C_IDLING = 0,
  C_GET,
  C_SEND
  };
  commandState cState;

   // Characteristics of each bot //
  typedef struct bot 
  {
    int botNum;
    int x;
    int y;
    byte lastMove;
    byte dir;
    uint8_t formation[ARRAY_SIZE];    
  }bot;

   bot slaveBot[NUM_SLAVES];
   
  ///////////////////////////
  // FUNCTION DECLARATIONS //
  ///////////////////////////
  
  // RF //
  void readPipe(uint8_t pipeNumber, uint8_t pipeAddress);
  void writePipe(uint8_t pipeAddress);
  void updateCoordinate(struct bot* slave);
  void initSlaveBot();

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

  ////////////////
  // FORMATIONS //
  ////////////////
  uint8_t slaveFormation[3][ARRAY_SIZE] = {
    {FORWARD, FORWARD, LEFT,  FORWARD, FORWARD, LEFT,  FORWARD, FORWARD, LEFT,    FORWARD, FORWARD, LEFT},
    {FORWARD, FORWARD, RIGHT, FORWARD, FORWARD, RIGHT, FORWARD, FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT},
    {FORWARD, FORWARD, RIGHT, RIGHT,   FORWARD, FORWARD, LEFT,  LEFT,    FORWARD, FORWARD, REVERSE, REVERSE}
    };
  uint8_t slave0formation[] = {FORWARD, FORWARD, LEFT,  FORWARD, FORWARD, LEFT,  FORWARD, FORWARD, LEFT,    FORWARD, FORWARD, LEFT};
  uint8_t slave1formation[] = {FORWARD, FORWARD, RIGHT, FORWARD, FORWARD, RIGHT, FORWARD, FORWARD, RIGHT,   FORWARD, FORWARD, RIGHT};
  uint8_t slave2formation[] = {FORWARD, FORWARD, RIGHT, RIGHT,   FORWARD, FORWARD, LEFT,  LEFT,    FORWARD, FORWARD, REVERSE, REVERSE};
  bool isFormation = false;

  /////////////////////
  // INITIALIZATIION //
  /////////////////////
  
  RF24 radio(CE, CSN);
  
void setup() {

  /* Initialize slave structs */
   pinMode(LED_BUILTIN, OUTPUT);                       //initialize digital pin LED_BUILTIN as an output.
  for (int i; i < 7; i++)  {                          //initialize all buttons on the joystick to be input buttons
    pinMode(buttons[i], INPUT_PULLUP);
  }
    Serial.begin(115200);
    printf_begin();

    #ifdef DEBUG
      //Serial.println("Initializing UART...")
      //Serial.println("Baud Rate: 115200")
    #endif
    /* Begin radio object */
    radio.begin();

    #ifdef DEBUG
      //Serial.println("Initializing nRF Transceivers...");
    #endif

    /* Setup read pipe to remote node */
    //radio.openReadingPipe(1,commandPipe);
    //radio.startListening();

    radio.openWritingPipe(slaveAddresses[0]);
    radio.stopListening();

    /* Set power level of the radio */
    radio.setPALevel(RF24_PA_LOW);

    /* Set radio channel to use - ensure all slaves match this */
    radio.setChannel(0x69);

    /* Set time between retries and max no. of retries */
    radio.setRetries(4, 5);
    
    /* Print out radio configuration data on serial monitor */
    radio.printDetails();

    #ifdef DEBUG
      //Serial.println("Initializing slave robots...");
    #endif
    
    /* Initialize slave bot structs */
    initSlaveBot();
}


  ////////////////////
  // MAIN FUNCITON //
  ///////////////////
void loop() {
  bool tx_sent = false;
  uint8_t data = 0;
  isReady = 0;
    
    switch(cState){  //state machine switch statement
/***********************************************************************************************************/       
      case C_IDLING: //IDLE state
            /* Do nothing */
            delay(3000); //delay and wait for the swarm to do stuff
            cState = C_GET; //go to get command state
/****************************************************************** *****************************************/ 
      case C_GET: //GET COMMAND state
      while( (command == STOP) && (isFormation == false)){ //gets command from joystick module
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
              if(NUM_SLAVES == 2){
                isFormation = true;
              }
            }
            else if (!digitalRead(F_BTN)){
              if(NUM_SLAVES == 3){
                isFormation = true;
              }
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
              
            for(int i = 0; i < NUM_SLAVES; i++){ //sends command to all slaves, one at a time
              writePipe(slaveAddresses[i]);
              tx_sent = radio.write(&command, sizeof(command));
          }
        }
      }

            cState = C_SEND; //goes to SEND state
            break;
/***********************************************************************************************************/            
    case C_SEND: //SEND state

            Serial.print("Command = ");
            Serial.println(command);
            Serial.print("isFormation = ");
            Serial.println(isFormation);
            
      if(isFormation == false){ //if command is not a formation button, send command to all slaves
            Serial.print("Sending command: ");
            Serial.println(command);
            
          for(int i = 0; i < NUM_SLAVES; i++){
            radio.openWritingPipe(slaveAddresses[i]);
            radio.stopListening();
            tx_sent = radio.write(&command, sizeof(command));
            slaveBot[i].lastMove = command;
            updateCoordinate(&slaveBot[i]);
          }
          Serial.println(" ");          
      }
      else{ //if button is a formaiton button send the commands of the formation, one at a time and wait 4 seconds 
        for(int i = 0; i < ARRAY_SIZE; i++){
          for(int j = 0; j < NUM_SLAVES; j++){
            radio.openWritingPipe(slaveAddresses[j]);
            radio.stopListening();
            tx_sent = radio.write(&slaveBot[j].formation[i], sizeof(slaveBot[j].formation[i]));
            slaveBot[j].lastMove = slaveBot[j].formation[i]; 
            updateCoordinate(&slaveBot[j]);
          }
          Serial.println(" ");
          delay(4000);
        }
    }
            isFormation = false; //resets the isFormation bool
            command = STOP; //resets the command
            cState = C_IDLING; //send the state machine back to IDLE
            break;
  }
}

void updateCoordinate(struct bot *slave){ //update coordinates function
  switch(slave->lastMove){
/***********************************************************************************************************/     
    case FORWARD: //FORWARD command case
      switch(slave->dir){
        case FORWARD: //FORWARD DIRECTION command case
          slave->y++;
          slave->dir = FORWARD;
        break;

        case LEFT: //LEFT DIRECTION command case
          slave->x--;
          slave->dir = LEFT;
        break;

        case RIGHT: //RIGHT DIRECTION command case
          slave->x++;
          slave->dir = RIGHT;
        break;

        case REVERSE: //BACKWARD DIRECTION command case
          slave->y--;
          slave->dir = REVERSE;
        break;
        }
    break;
/***********************************************************************************************************/ 
    case LEFT: //LEFT TURN command case
        switch(slave->dir){
        case FORWARD:
          slave->dir = LEFT;
        break;

        case LEFT:
          slave->dir = REVERSE;
        break;

        case RIGHT:
          slave->dir = FORWARD;
        break;

        case REVERSE:
          slave->dir = RIGHT;
        break;
        }
    break;
/***********************************************************************************************************/     
    case RIGHT://RIGHT TURN command case
        switch(slave->dir){
          case FORWARD:
          slave->dir = RIGHT;
        break;

        case LEFT:
          slave->dir = FORWARD;
        break;

        case RIGHT:
          slave->dir = REVERSE;
        break;

        case REVERSE:
          slave->dir = LEFT;
        break;
          }
      break;
/***********************************************************************************************************/       
      case REVERSE: //BACKWARDS command case
        switch(slave->dir){
          case FORWARD:
            slave->dir = FORWARD;
            slave->y--;
          break;
    
          case LEFT:
            slave->dir = LEFT;
            slave->x++;
          break;
    
          case RIGHT:
            slave->dir = RIGHT;
            slave->x--;
          break;
    
          case REVERSE:
            slave->dir = REVERSE;
            slave->y++;
          break;
        }
    break;
    }
    
    Serial.print("Bot: "); 
    Serial.print(slave->botNum);
    Serial.print("\tx:");
    Serial.print(slave->x);
    Serial.print("\ty: ");
    Serial.println(slave->y);
  }
void initSlaveBot() //initalizes the slaveBot struct variables
{  
  for (int i = 0; i < NUM_SLAVES; i++){
    slaveBot[i].botNum = i;
    slaveBot[i].x = i;
    slaveBot[i].y = 0;
    slaveBot[i].dir = FORWARD;
    memcpy(slaveBot[i].formation, slaveFormation[i], sizeof(slaveFormation[i]));    
  }    
}

void readPipe(uint8_t pipeNumber, uint8_t pipeAddress) //function that sets the reading of the RF sensor

{
  radio.openReadingPipe(pipeNumber, pipeAddress);
  radio.startListening();
}

void writePipe(uint8_t pipeAddress) //function that sets the writing to pipe of the RF sensor

{
  radio.openWritingPipe(pipeAddress);
  radio.stopListening();  
}
