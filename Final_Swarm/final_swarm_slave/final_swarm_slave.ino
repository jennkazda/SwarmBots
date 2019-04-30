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

  //////////////
  // INCLUDES //
  //////////////
  
  #include <SPI.h>    
  #include <Wire.h> 
  #include <RF24.h>
  #include <printf.h>
//  #include "Adafruit_VL6180X.h"

  /////////////
  // DEFINES //
  /////////////
  #define STOP    0x00                                      
  #define FORWARD 0x01                                   
  #define REVERSE 0x02                                   
  #define LEFT    0x03                                      
  #define RIGHT   0x04      

  #define SLAVE_ID 1

  #define DEBUG

  // State machine for SLAVE_ID bot //
  enum slaveState{
    S_IDLING = 0,
    S_MOVE
  };
  slaveState sState;

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

  // Interrupt //
  void trackDistanceL();
  void trackDistanceR();
  
  // Motor //
  void forward();
  void backward();  
  void rightTurn();
  void leftTurn();
  void stopMotor();
  void right90DegreeTurn();
  void left90DegreeTurn();

  // RF //
  void readPipe(uint8_t pipeNumber, uint8_t pipeAddress);
  void writePipe(uint8_t pipeAddress);

  ///////////////
  // VARIABLES //
  ///////////////
  volatile bool canMove, turning;
  int runningGoal = 0;
  int currentCountR = 0;
  int currentCountL = 0;
  int turningCountR = 0;
  int turningCountL = 0;
  int turningGoalR = 0;
  int turningGoalL = 0;
  int counter = 0;
  const byte interruptPinL = 2;
  const byte interruptPinR = 69;
  const byte STDBYE = 12;
  const byte AIN1 = 4;
  const byte AIN2 = 5;
  const byte PWMA = 3; //  was 3 before
  const byte BIN1 = 7;
  const byte BIN2 = 8;
  const byte PWMB = 10;
  const byte CE = 9;
  const byte CSN = 6; // was 6 before
  int state = sState;
  uint8_t command;
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
//  Adafruit_VL6180X distSensor = Adafruit_VL6180X();



  // Interrupt //
  void trackDistanceR() {
  if (turning) {
    turningCountR++;
    if (turningCountR > turningGoalR) {
      canMove = false;
      turning = false;
      turningCountR = 0;
    }
  }
  else {
    currentCountR++;
    if (currentCountR > runningGoal) {
      canMove = false;
      currentCountR = 0;
    }
  }
}

void trackDistanceL() {
  if (turning) {
    turningCountL++;
    if (turningCountL > turningGoalL) {
      canMove = false;
      turning = false;
      turningCountL = 0;
    }
  }
  else {
    currentCountL++;
    if (currentCountL > runningGoal) {
      canMove = false;
      currentCountL = 0;
    }
  }
  Serial.print("Ticks: "); Serial.println(turningCountL);
}
  

void forward() {
  turning = false;
  canMove = true;
  runningGoal = 10;
  while(currentCountL < runningGoal){
  digitalWrite(STDBYE, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, 75);
  analogWrite(PWMB, 75);
  }
}
void rightTurn() {
  turning = true;
  canMove = true;
  turningGoalL = 7;
  while (turningCountL < turningGoalL){
  
  digitalWrite(STDBYE, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 75);
  analogWrite(PWMB, 75);
  //delay(500);
  }

//    if(turningCountR < turningCountL){
//        digitalWrite(STDBYE, HIGH);
//        digitalWrite(AIN1, LOW);
//        digitalWrite(AIN2, HIGH);
//        digitalWrite(BIN1, HIGH);
//        digitalWrite(BIN2, LOW);
//        analogWrite(PWMA, 75);
//        analogWrite(PWMB, 75);
//    }
//    else if (turningCountR > turningCountL){  
//      digitalWrite(STDBYE, HIGH);
//      digitalWrite(AIN1, HIGH);
//      digitalWrite(AIN2, LOW);
//      digitalWrite(BIN1, LOW);
//      digitalWrite(BIN2, HIGH);
//      analogWrite(PWMA, 75);
//      analogWrite(PWMB, 75);}
}
void leftTurn() {
  turning = true;
  canMove = true;
  turningGoalL = 7;
  while (turningCountL < turningGoalL){
  digitalWrite(STDBYE, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, 75);
  analogWrite(PWMB, 75);
  }
}

void stopMotor() {
  digitalWrite(STDBYE, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void backward() {
  turning = false;
  canMove = true;
  runningGoal = 10;
  while(currentCountL < runningGoal){
  digitalWrite(STDBYE, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 75);
  analogWrite(PWMB, 75);
  }
}


  
void setup() {
    Serial.begin(115200);
    printf_begin();

   #ifdef DEBUG
    Serial.println("Initializing...");
    delay(500);
   #endif

    /* Encoder interrupts */
    attachInterrupt(digitalPinToInterrupt(interruptPinR), trackDistanceR, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPinL), trackDistanceL, RISING);
    
    /* Begin radio object */
    radio.begin();

    /* Setup read pipe to remote node */
    radio.openReadingPipe(1, slaveAddresses[SLAVE_ID]);
    radio.startListening();

    /* Set power level of the radio */
    radio.setPALevel(RF24_PA_LOW);

    /* Set radio channel to use - ensure all SLAVE_IDs match this */
    radio.setChannel(0x69);

    /* Set time between retries and max no. of retries */
    radio.setRetries(4, 10);

    /* Enable ackpayload - enables each SLAVE_ID to reply with data */
    //radio.enableAckPayload();

    /* Preload the payload with initial data */
    // radio.writeAckPayload(1, &remoteNodeData, sizeof(remoteNodeData));
    
    /* Print out radio configuration data on serial monitor */
    radio.printDetails();

    /* Initializes bot to no motor movement */
    stopMotor();
    
    #ifdef DEBUG
      Serial.println("Setup complete!");
      delay(500);
    #endif
}

void loop() {
  
  #ifdef DEBUG
      Serial.println("Starting main loop...");
      Serial.println(state);
  #endif
  
  //while(radio.available()){
  
  delay(1000);
  switch(state)
  {
  /***********************************************************************/
  case S_IDLING:
    #ifdef DEBUG
      Serial.println("IDLE");
    #endif
    //radio.flush_rx();
    while(command == STOP)
    {
      radio.openReadingPipe(1, slaveAddresses[SLAVE_ID]);
      radio.startListening();
      radio.read(&command, sizeof(command));        
    }  
      radio.stopListening();
      radio.flush_tx();
      
    #ifdef DEBUG
      Serial.println("IDLE complete!");
      Serial.println(command);
    #endif

    #ifdef DEBUG
      Serial.println("IDLE complete!");
      Serial.println(command);
    #endif
    
    state++;
  break;
  /***********************************************************************/
  case S_MOVE:

    #ifdef DEBUG
          Serial.println("MOVE");
        #endif

         /* Master moves according to command data */
         switch(command){
            case FORWARD :
              Serial.println('A');
              forward();
              delay(100);
              break;

            case REVERSE:
              Serial.println('B');
              backward();
              delay(100);
              break;

            case LEFT:
              Serial.println('C');
              leftTurn();
              delay(100);
              break;

            case RIGHT:
              Serial.println('D');
              rightTurn();
              delay(100);
              break;

            case STOP:
              Serial.println('E');
              stopMotor();
              delay(100);
              break;

            default:
              Serial.println('F');
              stopMotor();
              break;
          }
      Serial.println('G');
      while(canMove);
      stopMotor();
      Serial.println('H');
      command = STOP;
      state = S_IDLING;
    
    #ifdef DEBUG
      Serial.println("MOVE complete");
    #endif
  break;
  /***********************************************************************/
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
