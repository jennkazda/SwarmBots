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

void setup(){
    Serial.begin(9600);
    printf_begin();
    Radio.begin();
    Radio.openReadingPipe(1, slave1);
    Radio.startListening();
    Radio.setPALevel(RF24_PA_MIN);
    Radio.printDetails();
    stopMotor();

    // wait for serial port to open on native usb devices
    while(!Serial){
        delay(1);
    }

    

}

void loop(){

}   
