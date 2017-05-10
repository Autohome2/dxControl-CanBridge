/*
Example program to read specific value from Speeduino Secondary serial port.
On Mega2560 version this is Serial3.
This example program was written for a arduino uno but any arduino could be used just change the pins for the software serial ports
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory 

Speeduino is Copyright (C) Josh Stewart
A full copy of the license may be found in the speeduino projects root directory

*/
#include "globals.h"
#include "comms.h"
#include "sensors.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial1(3, 2); // RX, Tx   connect to Serial3 on the Speeduino Mega
SoftwareSerial mySerial2(6, 5); // RX, TX   open this with your serial console to see the data from the Speeduino

void setup() {
  // put your setup code here, to run once:
  mySerial1.begin(115200);
  mySerial2.begin(115200);  


}

void loop() {
  // put your main code here, to run repeatedly:
        // send command to the Speeduino
        sendTheCommand(maincommand,tsCanId,realtime_offset,realtime_length);
        if (mySerial2.available() > 0)      // if serial2 has data then do the command subroutine
        {
          command();
          for (byte senddata = 0; senddata < replylength; senddata++) //read all 8 bytes of data
                      {
                       mySerial1.write(realtimebuffer[senddata]);   // print the data received to the serial monitor
                      }
        }
      delay (1000);     // wait 1 second
}
