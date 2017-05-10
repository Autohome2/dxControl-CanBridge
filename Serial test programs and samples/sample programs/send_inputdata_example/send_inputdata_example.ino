/*
Example program to read specific value from Speeduino Secondary serial port.
On Mega2560 version this is Serial3.
This example program was written for a arduino promini but any arduino could be used just change the pins for the software serial ports
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory 

Speeduino is Copyright (C) Josh Stewart
A full copy of the license may be found in the speeduino projects root directory

*/
#include "globals.h"
#include "comms.h"
#include "sensors.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, Tx   connect to Serial3 on the Speeduino Mega

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);  


}

void loop() {
  // put your main code here, to run repeatedly:
        if (mySerial.available() > 0)      // if mySerial has data then do the command subroutine
        {
          command();
        }
}
