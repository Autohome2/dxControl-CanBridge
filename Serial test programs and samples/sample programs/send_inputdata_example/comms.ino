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
    switch (mySerial.read())
          {
          case 'A':

          break;  

          case 'B':

          break;

          case 'C':
//          if (caninitok == 1){
//  Serial.print("yes");}
          break;
  
          case 'F':
              Serial.print("001");
          break;

         //case 'G':
           //   Serial.print("G cmd");
              //"G" doesnt send canid so use the last read id

              //The next byte sent represents the target TScanID
              //while (Serial.available() == 0) {} 
              //dest_canID = Serial.read();
          //break;
          
          case 'P':
              //The next byte sent represents the target TScanID
          break;
  
          case 'Q':  
          
          break;

          case 'r':
                  uint8_t cmdreply;
                  while (mySerial.available() == 0) {}
                  cmdreply = (mySerial.read());              //read in command letter first          
                  while (mySerial.available() == 0) {}
                  replylength = (mySerial.read());              //read in reply length
                  for (byte rdata = 0; rdata < replylength; rdata++) //read x bytes of data according to replylength
                      {
                       realtimebuffer[rdata] = mySerial.read();
                      }             
          break;
          
          case 'R':
                  uint8_t Rdata1, Rdata2;//, Rdata3, Rdata4 ;
                  uint16_t Rdata;
                  while (mySerial.available() == 0) {}
                  Rdata1 = (mySerial.read());               //read in lsb of paramgroup
                  while (mySerial.available() == 0 ) {} 
                  Rdata2 = (mySerial.read());               //read msb of paramgroup

                  Rdata = Rdata | Rdata2;
                  Rdata = Rdata << 8;
                  Rdata = Rdata | Rdata1;
               
               //send rdata values to console
                  Serial.print("rdata1 ");
                  Serial.print(Rdata1);
                  Serial.print(" rdata2 ");
                  Serial.print(Rdata2);
                  Serial.print(" rdata ");               
                  Serial.print(Rdata);     

                  getsensor_reading(Rdata);
                  
                  mySerial.write("G");                      // reply "G" cmd
                  mySerial.write(1);                        //send 1 to confirm cmd received and valid
                  for (byte Gx = 0; Gx < 8; Gx++)           //write all 8 bytes of data
                      {
                       mySerial.write(Gdata[Gx]);
                      }
          break;
  
          case 'S':

          break;

          case 'V':

          break;

          case 'W':

          break;

    }
//sendA = 0;
return;
 
}

void sendTheCommand(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength)
{
  mySerial.print(commandletter);
  mySerial.print(canid);
  mySerial.print(lowByte(cmdoffset));
  mySerial.print(highByte(cmdoffset));
  mySerial.print(lowByte(cmdlength));
  mySerial.print(highByte(cmdlength));  
}

