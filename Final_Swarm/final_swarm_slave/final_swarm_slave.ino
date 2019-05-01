/*  AUTHOR: Josue Mendez & Jenn Kazda
/*  ===================================================================
*   FILE: Swarm_Algorithm_slave.c
*   MODULE: Swarm_Algorithm_slave
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
*   1.1     2019-03-05  JM
*           Initial Revision.
*   1.2     2019-04-26  JK
*           FINAL REVISION
*/

  //////////////
  // INCLUDES //
  //////////////
  
  #include <SPI.h>    
  #include <Wire.h> 
  #include <RF24.h>
  #include <printf.h>


  /////////////
  // DEFINES //
  /////////////
  #define STOP    0x00                                      
  #define FORWARD 0x01                                   
  #define REVERSE 0x02                                   
  #define LEFT    0x03                                      
  #define RIGHT   0x04      

//************************************//
  #define SLAVE_ID 1  //CHANGE THIS LINE OF CODE TO MATCH THE NUMBER ON THE SLAVE BOT LABEL

  #define DEBUG //enables the dEBUG serial print lines

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

  //PIN VARIABLES
  const byte interruptPinL = 2;
  const byte interruptPinR = 69;
  const byte STDBYE = 12;
  const byte AIN1 = 4;
  const byte AIN2 = 5;
  const byte PWMA = 3; 
  const byte BIN1 = 7;
  const byte BIN2 = 8;
  const byte PWMB = 10;
  const byte CE = 9;
  const byte CSN = 6; 
  
  
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
  
  RF24 radio(CE, CSN); //RF sensor variable


  /////////////////////////
  // INTERRUPT FUNCTIONS //
  ////////////////////////
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
  
  /////////////////////
  // MOTOR FUNCTIONS //
  /////////////////////
void forward() { //goes forward for 10 tick counts
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
void rightTurn() { //90 degree right turn
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
  }

}
void leftTurn() { //90 degree left turn
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

void stopMotor() { //stops the motors
  digitalWrite(STDBYE, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void backward() { //drives backward for 10 ticks
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


  /////////////////////
  // INITIALIZATIION //
  /////////////////////
void setup() {
    Serial.begin(115200); //sets baudrate of the serial port monitor 
    printf_begin();

   #ifdef DEBUG
    Serial.println("Initializing...");
    delay(500);
   #endif

    /* Enables Encoder Interrupts */
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

  ////////////////
  // MAIN CODE //
  ///////////////
void loop() {
  
  #ifdef DEBUG
      Serial.println("Starting main loop...");
      Serial.println(state);
  #endif
  
  
  
  delay(1000);
  ////////////////////
  // STATE MACHINE //
  ///////////////////
  switch(state) 
  {
  /***********************************************************************/
  case S_IDLING: //IDLE state
    #ifdef DEBUG
      Serial.println("IDLE");
    #endif

    /* reads command form rf sensor */
    while(command == STOP)
    {
      radio.openReadingPipe(1, slaveAddresses[SLAVE_ID]); //reads the radio pipe for the specific slave depending on the SLAVE_ID set above
      radio.startListening();
      radio.read(&command, sizeof(command));  //reads command form the radio       
    }  
      radio.stopListening(); //finsihed lisening for the command
      radio.flush_tx();
      
    #ifdef DEBUG
      Serial.println("IDLE complete!");
      Serial.println(command);
    #endif
    
    state++; //increments to the next state
  break;
  /***********************************************************************/
  case S_MOVE: //MOVE state

    #ifdef DEBUG
          Serial.println("MOVE");
        #endif

         /* moves according to command data */
         switch(command){ //switch statement of the commands that can be given from the master
            case FORWARD : //case to drive forward
              forward();
              delay(100);
              break;

            case REVERSE: //case to drive backwards
              backward();
              delay(100);
              break;

            case LEFT: //case to execute 90 degree left turn
              leftTurn();
              delay(100);
              break;

            case RIGHT: //case to execute 90 degree right turn
              rightTurn();
              delay(100);
              break;

            case STOP: //case to stop driving
              stopMotor();
              delay(100);
              break;

            default: //switch statements need a default case: stop driving
              stopMotor();
              break;
          }
          
      while(canMove); //sense loop
      stopMotor(); 
      
      command = STOP; //resets command to STOP to stop motors and receive next command
      state = S_IDLING; // send the state machine back to the IDLE case
    
    #ifdef DEBUG
      Serial.println("MOVE complete");
    #endif
  break;
  /***********************************************************************/
  }
  
}


  /////////////////////
  // RADIO FUNCTIONS //
  /////////////////////
void readPipe(uint8_t pipeNumber, uint8_t pipeAddress) //function that sets the reading of the RF sensor
{
  radio.openReadingPipe(pipeNumber, pipeAddress);
  radio.startListening();
}

void writePipe(uint8_t pipeAddress) //function that sets teh writing to pipe of the RF sensor
{
  radio.openWritingPipe(pipeAddress);
  radio.stopListening();  
}
