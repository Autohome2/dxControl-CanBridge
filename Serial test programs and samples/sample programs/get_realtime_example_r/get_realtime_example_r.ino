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
#include <SoftwareSerial.h>

SoftwareSerial mysoftSerial(10, 11);  // RX, Tx   connect to Serial3 on the Speeduino Mega
/*
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);       // your console serial port
  mysoftSerial.begin(115200);  


}

void loop() {
  // put your main code here, to run repeatedly:
        // send command to the Speeduino
        sendTheCommand(maincommand,tsCanId,realtime_offset,realtime_bytes);
        if (mysoftSerial.available() > 0)      // if mysoftSerial has data then do the command subroutine
        {
          command();
          for (byte senddata = 0; senddata < replylength; senddata++) //read all 8 bytes of data
                      {
                       Serial.print(realtimebuffer[senddata]);   // print the data received to the serial monitor
                      }
        }
      delay (1000);     // wait 1 second
}
