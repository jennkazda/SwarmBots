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
#include "commandCenter.h"
RF24 Radio(9,10);
unsigned int commandPipe  =  0xff;

RF_Sense RF;                                            //initalizes RF class

uint8_t command = 0x00;                                 //default value for command to be sent from master
bool isReady;

unsigned int  STOP = 0x00;                                      //value to stop motors
unsigned int  FORWARD = 0x01;                                   //value to move forward
unsigned int  REVERSE = 0x02;                                   //value to move backwards
unsigned int  LEFT = 0x03 ;                                     //value to pivot left
unsigned int  RIGHT = 0x04;                                     //value to pivot right

enum commandState{
  C_IDLING = 0,
  C_GET,
  C_SEND
};

commandState cState;
void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);                       //initialize digital pin LED_BUILTIN as an output.
  for (int i; i < 7; i++)  {                          //initialize all buttons on the joystick to be input buttons
    pinMode(buttons[i], INPUT_PULLUP);
  }
  Serial.begin(9600);               
  Radio.begin();
  Radio.openReadingPipe(1, commandPipe);
  Radio.startListening();
  Radio.setPALevel(RF24_PA_MIN);
  printf_begin(); 
  Radio.printDetails();
 
  while(!Serial){                                      // wait for serial port to open on native usb devices
      delay(1);
  }
  
}


void loop() {
  digitalWrite(LED_BUILTIN, LOW);                     //turn the LED off by making the voltage LOW
  
  isReady = 0;
  //Serial.println(commandPipe);
  RF.readPipe(&Radio, 1, commandPipe);
  while(Radio.available()){

    switch(cState){
      case C_IDLING:
                            //opens rf pipe      
            Serial.print("isntReady: ");
            Serial.println(isReady);
            Radio.read(&isReady, sizeof(isReady));
            if (isReady == 1){
              digitalWrite(LED_BUILTIN, HIGH);
              Serial.print("isReady: ");
              Serial.println(isReady);
              cState = C_GET;
              isReady = 0;
            }
            else{
              cState = C_IDLING;
            }
            break;

      case C_GET:
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
            }
            Serial.print("Command = ");
            Serial.println(command);
            cState = C_SEND;
            break;
            
    case C_SEND:
            Serial.print("Sending command: ");
            Serial.println("command");
            RF.writingPipe(&Radio, commandPipe);
            Radio.write(&command, sizeof(command));
            Serial.println("command sent ");
            Serial.println(" ");
            cState = C_IDLING;
            break;
   }
  }
}
