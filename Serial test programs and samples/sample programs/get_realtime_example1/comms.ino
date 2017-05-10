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
    switch (mySerial2.read())
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
                  while (mySerial2.available() == 0) {}
                  cmdreply = (mySerial2.read());              //read in command letter first          
                  while (mySerial2.available() == 0) {}
                  replylength = (mySerial2.read());              //read in reply length
                  for (byte rdata = 0; rdata < replylength; rdata++) //read x bytes of data according to replylength
                      {
                       mySerial2.read(realtimebuffer[rdata]);
                      }             
          break;
          
          case 'R':
                  uint8_t Rdata1, Rdata2;//, Rdata3, Rdata4 ;
                  uint16_t Rdata;
                  while (mySerial2.available() == 0) {}
                  Rdata1 = (mySerial2.read());              //read in lsb first
                  while (mySerial2.available() == 0 ) {} 
                  Rdata2 = (mySerial2.read());
               //   while (mySerial2.available() == 0) {}
               //   Rdata3 = (mySerial2.read());
               //   while (mySerial2.available() == 0 ) {}    //read msb last
               //   Rdata4 = (mySerial2.read());
                     
   //   Rdata = Rdata4;
   //   Rdata = Rdata << 8;
   //   Rdata = Rdata | Rdata3;
   //   Rdata = Rdata << 8;
      Rdata = Rdata | Rdata2;
      Rdata = Rdata << 8;
      Rdata = Rdata | Rdata1;
                  //Rdata = (Rdata1 << 24) | (Rdata2 << 16) | (Rdata3 << 8) | (Rdata4);
                  mySerial1.print("rdata1 ");
                  mySerial1.print(Rdata1);
                  mySerial1.print(" rdata2 ");
                  mySerial1.print(Rdata2);
                 //                   mySerial1.print(" rdata3 ");
                 //  mySerial1.print(Rdata3);
                 //                    mySerial1.print(" rdata4 ");
                 //   mySerial1.print(Rdata4);
                 mySerial1.print(" rdata ");               
                 mySerial1.print(Rdata);     

                  getsensor_reading(Rdata);
                  
                  mySerial2.write("G"); // reply "G" cmd
                  mySerial2.write(1); //send 1 to confirm cmd received and valid
                  for (byte Gx = 0; Gx < 8; Gx++) //read all 8 bytes of data
                      {
                       mySerial2.write(Gdata[Gx]);
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

sendTheCommand(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength)
{
  Serial2.print(commandletter);
  Serial2.print(canid);
  Serial2.print(lowByte(cmdoffset));
  Serial2.print(highByte(cmdoffset));
  Serial2.print(lowByte(cmdlength));
  Serial2.print(highByte(cmdlength));  
}

