#include <Arduino.h>

/*
Speeduino Can interface
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory

Based on code by Josh Stewart for the Speeduino project , see www.Speeduino.com for more info
*/

/*
This is called when a command is received over serial from TunerStudio / Megatune
It parses the command and calls the relevant function
*/
#include "comms.h"
#include "globals.h"
//#include "storage.h"


void command()
{
    switch (mysoftSerial.read())
          {
          case 'A':
                  while (mysoftSerial.available() == 0) {}
                  replylength = (mysoftSerial.read());              //read in reply length
                  for (byte rdata = 0; rdata < replylength; rdata++) //read x bytes of data according to replylength
                      {
                       realtimebuffer[rdata] = (mysoftSerial.read());
                      }
          break;  

          case 'r':          
                  while (mysoftSerial.available() == 0) {}
                  replylength = (mysoftSerial.read());              //read in reply length
                  for (byte rdata = 0; rdata < replylength; rdata++) //read x bytes of data according to replylength
                      {
                       realtimebuffer[rdata] = (mysoftSerial.read());
                      }             
          break;
    }

return;
 
}

void sendTheCommand(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength)
{
  mysoftSerial.write(commandletter);          // send command letter to the Speeduino
   if (commandletter == 'r')                  //the next 5 bytes ned only be sent if it is an "r" command
    {
      mysoftSerial.write(canid);
      mysoftSerial.write(0x30);
      mysoftSerial.write(lowByte(cmdoffset));
      mysoftSerial.write(highByte(cmdoffset));
      mysoftSerial.write(lowByte(cmdlength));
      mysoftSerial.write(highByte(cmdlength));  
    }  
}

