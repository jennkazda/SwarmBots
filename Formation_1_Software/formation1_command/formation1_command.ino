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

#include "swarmAlgorithm.h"

RF24 Radio(CE, CSN);
unsigned int commandPipe  =  0xFF;

RF_Sense RF;                                            //initalizes RF class

uint8_t Stop = 0x00;                                    //value to stop motors
uint8_t Forward = 0x01;                                 //value to move forward
uint8_t Reverse = 0x02;                                 //value to move backwards
uint8_t Left = 0x03;                                    //value to pivot left
uint8_t Right = 0x04;                                   //value to pivot right
uint8_t command = 0x00;                                 //default value for command to be sent from master

int const UP_BTN = 2;
int const DOWN_BTN = 4;
int const LEFT_BTN = 5;
int const RIGHT_BTN = 3;
int const E_BTN = 6;
int const F_BTN = 7;
int const JOYSTICK_BTN = 8;
int const JOYSTICK_AXIS_X = A0;
int const JOYSTICK_AXIS_Y = A1;
int buttons[] = {UP_BTN, DOWN_BTN, LEFT_BTN, RIGHT_BTN, E_BTN, F_BTN, JOYSTICK_BTN};

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i; i < 7; i++)  {
    pinMode(buttons[i], INPUT_PULLUP);
  }
  Serial.begin(9600);               
  Radio.begin();
  Radio.openReadingPipe(1, commandPipe);
  Radio.startListening();
  Radio.setPALevel(RF24_PA_MIN);
  //printf_begin(); 
  //Radio.printDetails();
 
  while(!Serial){                                     // wait for serial port to open on native usb devices
      delay(1);
  }
  
}


void loop() {
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  
  RF.readPipe(&Radio, 1, commandPipe);
  RF.writingPipe(&Radio, commandPipe);
  
  bool isReady;
  isReady = false;
  Radio.read(&isReady, sizeof(isReady));
  if (isReady == true){
    digitalWrite(LED_BUILTIN, HIGH);
    if (!digitalRead(UP_BTN)){
      command = Stop;
    }
    else if (!digitalRead(DOWN_BTN)){
      command = Stop;
    }
    else if (!digitalRead(LEFT_BTN)){
      command = Left;
    }
    else if (!digitalRead(RIGHT_BTN)){
      command = Right;
    }
    else if (!digitalRead(E_BTN)){
      command = Stop;
    }
    else if (!digitalRead(F_BTN)){
      command = Stop;
    }
    else if (!digitalRead(JOYSTICK_BTN)){
      command = Stop;
    }
    else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == 0 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == 1){
      command = Forward;
    }
    else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == -1 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == 0){
      command = Left;
    }
    else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == 1 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == 0){
      command = Right;
    }
    else if (map(analogRead(JOYSTICK_AXIS_X), 0, 668, -1, 1) == 0 && map(analogRead(JOYSTICK_AXIS_Y), 0, 668, -1, 1) == -1){
      command = Reverse;
    }
    else {
      command = Stop;
    }

    Radio.write(&command, sizeof(command));

  }

  
}
