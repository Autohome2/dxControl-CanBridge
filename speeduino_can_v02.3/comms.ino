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
#include "storage.h"


void command()
{

    theCommand = Serial.read(); //read the command in first
sendA = 1;
    switch (theCommand)
          {
          case 'A':
              //The next byte sent represents the target TScanID
                while (Serial.available() == 0) {} 
                dest_canID = Serial.read();
              
             // next the number of bytes expected value
             while (Serial.available() == 0) {}  
                Alength = Serial.read();
            sendValues(Alength,dest_canID);
          break;  

          case 'B':
              //The next byte sent represents the target TScanID
               // while (Serial.available() == 0) {} 
               // dest_canID = Serial.read();               
              writeConfig();
          break;

          case 'C':
          if (caninitok == 1){
  Serial.print("yes");}
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
                while (Serial.available() == 0) {} 
                dest_canID = Serial.read();
              // next is the page number to be selected
                while (Serial.available() == 0) {} 
                Pval = Serial.read();
                currentPage = Pval;

               if (currentPage >= '0') {//This converts the ascii number char into binary
                  currentPage -= '0';
                  }
                if ( currentPage == ignMapPage || currentPage == afrMapPage) {// Detecting if the current page is a table/map
                  isMap = true;
                  }
                else {
                    isMap = false;
                     }
          break;
  
          case 'Q':

                //The next byte sent represents the target TScanID
                  while (Serial.available() == 0) {} 
                  dest_canID = Serial.read(); 
               // next the number of bytes expected value
                  while (Serial.available() == 0) {}  
                  Qlength = Serial.read();
                  for (unsigned int sg = 0; sg < sizeof(canio_RevNum) - 1; sg++){
                  Serial.write(canio_RevNum[sg]); 
                  }  
          break;
  
          case 'S':
                //The next byte sent represents the target TScanID
                while (Serial.available() == 0) {} 
                dest_canID = Serial.read();
               // next the number of bytes expected value
                while (Serial.available() == 0) {} 
                Slength = Serial.read();
                for (unsigned int sg = 0; sg < sizeof(canio_signature) - 1; sg++){
                  Serial.write(canio_signature[sg]); 
                  }
                currentStatus.secl = 0;
          break;

          case 'V':
                  //The next byte sent represents the target TScanID
                  while (Serial.available() == 0) {} 
                  dest_canID = Serial.read();
                  // next is the page number to be selected
                   while (Serial.available() == 0) {} 
                    Vval = Serial.read();  
                    currentPage = Vval;
                  uint8_t Vlength1, Vlength2 ;
                  uint16_t Vlength;
                  while (Serial.available() == 0) {}
                  Vlength1 = (Serial.read());
                  while (Serial.available() == 0 ) {} 
                  Vlength2 = (Serial.read());
                  Vlength = (Vlength1 << 8) | Vlength2;   
                  sendPage(currentPage,Vlength,false, dest_canID);   //
          break;

          case 'W':
                  //The next byte sent represents the target TScanID
                  while (Serial.available() == 0) {} 
                  dest_canID = Serial.read(); 
                     // Serial1.print(" wcid ");
                     // Serial1.print(dest_canID);
                     // Serial1.print(" "); 
                   // next is the page number to be selected
                   while (Serial.available() == 0) {} 
                    Pval = Serial.read();  
                    currentPage = Pval;
                 uint8_t W_offset1, W_offset2;
                 uint16_t W_offset;
                 uint8_t W_data;
    
                  while (Serial.available() == 0) { }
                  W_offset1 = Serial.read();
                  while (Serial.available() == 0) { }
                  W_offset2 = Serial.read();
                  W_offset = (W_offset2 << 8) | W_offset1;
                    //Serial1.print(" w offsetA ");
                    //Serial1.print(W_offset);
                    //Serial1.print(" ");
                  while (Serial.available() == 0) { }
                  W_data = Serial.read();
                    //Serial1.print(" w data ");
                    //Serial1.print(W_data);
                    //Serial1.print(" "); 
                receiveValue(currentPage,W_offset,W_data);  //Serial1.read());
          break;

    }
sendA = 0;
return;
 
}

//---------------------------------------------------------------------------------------
/*
sets the current page to be read
*/
void setPage(byte Pdata, byte can_id)
{
  //byte PdataB(Pdata); //convert Pdata from int to PdataB a byte , only a bytes worth will ever be sent anyway
  
   // if ((can_id == canio_canid) or (can_id == 99))
     //  {
            currentPage = Pdata;
             if (currentPage >= '0') {//This converts the ascii number char into binary
            currentPage -= '0';
              }
            if ( currentPage == ignMapPage || currentPage == afrMapPage) {// Detecting if the current page is a table or map
              isMap = true;
              }
            else {
              isMap = false;
              }
     //  }
                   
}
/*
Gathers the realtime data requested by Ts and sends it back
*/
void sendValues(uint8_t packet_Length,uint8_t can_id)
{
    byte response[packet_Length];
sendA = 1;
      if(requestCount == 0) { currentStatus.secl = 0; }
      requestCount++;
    
    //  if ((can_id == canio_canid)or (can_id == 99))
    //   {  
          response[0] = 0x01  ;//currentStatus.canio_system_status;
          response[1] = currentStatus.secl; //secl is simply a counter that increments each second. Used to track unexpected resets (Which will reset this count to 0)
          //Need to send the int loopsPerSecond value into 2 bytes
          response[2] = lowByte(currentStatus.canio_loopsPerSecond);
          response[3] = highByte(currentStatus.canio_loopsPerSecond);
          //The following can be used to show the amount of free memory
          #if defined(PROCESSOR_MEGA_NO61)
          currentStatus.canio_freeRAM = canio_freeRam();
          #elif defined(PROCESSOR_TEENSY_3_2)
          currentStatus.canio_freeRAM = canio_freeram32();
          #endif
          response[4] = lowByte(currentStatus.canio_freeRAM); //(byte)((currentStatus.loopsPerSecond >> 8) & 0xFF);
          response[5] = highByte(currentStatus.canio_freeRAM);
          response[6] = currentStatus.MAP;
          response[7] = currentStatus.IAT;
          response[8] = currentStatus.coolant;
          response[9] = currentStatus.battery10; //battery voltage
          response[10] = currentStatus.O2; //O2;
          response[11] = ECUstreambuffer[13];//lowByte(currentStatus.RPM); //
          response[12] = ECUstreambuffer[14];//highByte(currentStatus.RPM); /
          response[13] = currentStatus.VE;
          response[14] = currentStatus.afrTarget;
          response[15] = currentStatus.PW;
          response[16] = currentStatus.tpsDOT ; //TPS DOT
          response[17] = currentStatus.advance ;
          response[18] = currentStatus.TPS ; // TPS (0% to 100%)
          response[19] = currentStatus.O2_2 ; //O2 
            //rpmDOT must be sent as a signed integer
          response[20] = lowByte(currentStatus.rpmDOT);
          response[21] = highByte(currentStatus.rpmDOT);
          response[22] = currentStatus.flex ; 
           
          intdev_gauge = (configPage1.canio_obd_canid)+1774;
          response[23] = ECUstreambuffer[14];//lowByte(intdev_gauge);//       a 16bit unsigned development gauge
          response[24] = 0;//highByte(intdev_gauge);//
          bytedev_gauge = configPage1.canio_obd_canid;
          response[25] =ECUstreambuffer[13];// bytedev_gauge;//               a 8 bit unsigned development gauge
     //  }
            
  //now send that buffer back to TS
 Serial.write(response, (size_t)packet_Length);                  
 sendA= 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void receiveValue(uint8_t now_page, uint16_t page_offset, uint8_t newValue)
{      
  

        currentPage = now_page;
  void* pnt_configPage;//This only stores the address of the value that it's pointing to and not the max size

  switch (currentPage)
  {

    case canio_setupPage:
      pnt_configPage = &configPage1; //Setup a pointer to the relevant config page
     //For some reason, TunerStudio is sending offsets greater than the maximum page size. I'm not sure if it's their bug or mine, but the fix is to only update the config page if the offset is less than the maximum size
      if ( page_offset < page_1_size)
      {
        *((uint8_t *)pnt_configPage + (uint8_t)page_offset) = newValue; 
      }
      break;


      
  }
}   

 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
sendPage() packs the data within the current page (As set with the 'P' command)
into a buffer and sends it.
Note that some translation of the data is required to lay it out in the way Megasqurit / TunerStudio expect it
useChar - If true, all values are send as chars, this is for the serial command line interface. TunerStudio expects data as raw values, so this must be set false in that case
*/
void sendPage(uint8_t pagenum, uint16_t send_page_Length,bool useChar, uint8_t can_id)
{

 currentPage = pagenum;
 
  // if ((can_id == canio_canid) or (can_id == 99))
    //  {
        void* pnt_configPage;

     //   byte currentTitleIndex = 0;// This corresponds to the count up to the first char of a string in pageTitles

        switch (currentPage)
          {

            case canio_setupPage:  //
                {
                pnt_configPage = &configPage1; //Create a pointer to Page 1 in memory   
                }
            break;               
          }
    
          //All other bytes can simply be copied from the config table
          uint8_t response[send_page_Length];
          for ( uint16_t x = 0; x < send_page_Length; x++)
            {
              response[x] = *((uint8_t *)pnt_configPage + x); //Each byte is simply the location in memory of the configPage + the offset + the variable number (x)
            }
          Serial.write((uint8_t *)&response, sizeof(response));
      //} 
      
}

//----------------------------------------------------------------

void testComm()
{
  Serial.write(1);
  return;
}

//----------------------------------------------------------------

void command1()
{
    theCommand = Serial1.read(); //read the command in first

    switch (theCommand)
          {
           case 'A':
            uint8_t Alength;
              while (Serial1.available() == 0) { }
              Alength= Serial1.read();              // next the number of bytes expected value
              for (uint8_t Acount = 0; Acount <Alength ;Acount++)
                { 
                  while (Serial1.available() == 0){} 
                  // receive all x bytes into "ECUstreambuffer[]"
                  ECUstreambuffer[Acount] = Serial1.read();
                  transmit_ECUstream();      //broadcast the ECU stream onto the canbus for std 11bit can devices
                  speeduinostatuses();
                }
          break;    
          case 'G':
          int canaddress;
          int tableid;
          int Goffset;
              //The next byte sent represents the target TScanID
                while (Serial1.available() == 0) {} 
                canaddress = Serial1.read();
                  while (Serial1.available() == 0) {} 
                tableid = Serial1.read();
                                while (Serial1.available() == 0) {} 
                Goffset = Serial1.read();
         break;
          }       
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void transmit_ECUstream()
{
  currentStatus.bcast_address = ((configPage1.canio_11bit_can_bcast) + 511);  //set base address to broadcast from
  //base can broadcast transmit address is canio_Bcast1_true_canid
  
  Send_CAN_message(1, currentStatus.bcast_address,0);
  //ECUstreambuffer[0] = currentStatus.secl; //secl is simply a counter that increments each second. Used to track unexpected resets (Which will reset this count to 0)
  //ECUstreambuffer[1] = currentStatus.squirt; //Squirt Bitfield
  //ECUstreambuffer[2] = currentStatus.engine; //Engine Status Bitfield
  //ECUstreambuffer[3] = (byte)(divu100(currentStatus.dwell)); //Dwell in ms * 10
  //ECUstreambuffer[4] = (byte)(currentStatus.MAP >> 1); //map value is divided by 2
  //ECUstreambuffer[5] = (byte)(currentStatus.IAT + CALIBRATION_TEMPERATURE_OFFSET); //mat
  //ECUstreambuffer[6] = (byte)(currentStatus.coolant + CALIBRATION_TEMPERATURE_OFFSET); //Coolant ADC
  //ECUstreambuffer[7] = currentStatus.tpsADC; //TPS (Raw 0-255)
  
  //currentStatus.bcast_address++;   //inc to next address 
   Send_CAN_message(1, currentStatus.bcast_address+1,8);
  //ECUstreambuffer[8] = currentStatus.battery10; //battery voltage
  //ECUstreambuffer[9] = currentStatus.O2; //O2
  //ECUstreambuffer[10] = currentStatus.egoCorrection; //Exhaust gas correction (%)
  //ECUstreambuffer[11] = currentStatus.iatCorrection; //Air temperature Correction (%)
  //ECUstreambuffer[12] = currentStatus.wueCorrection; //Warmup enrichment (%)
  //ECUstreambuffer[13] = lowByte(currentStatus.RPM); //
  //ECUstreambuffer[14] = highByte(currentStatus.RPM); //
  //ECUstreambuffer[15] = currentStatus.TAEamount; //acceleration enrichment (%)
  
  //currentStatus.bcast_address++;   //inc to next address
 Send_CAN_message(1, currentStatus.bcast_address+2, 16);  
  //ECUstreambuffer[16] = 0x00; //Barometer correction (%)
  //ECUstreambuffer[17] = currentStatus.corrections; //Total GammaE (%)
  //ECUstreambuffer[18] = currentStatus.VE; //Current VE 1 (%)
  //ECUstreambuffer[19] = currentStatus.afrTarget;
  //ECUstreambuffer[20] = (byte)(currentStatus.PW / 100); //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
  //ECUstreambuffer[21] = currentStatus.tpsDOT; //TPS DOT
  //ECUstreambuffer[22] = currentStatus.advance;
  //ECUstreambuffer[23] = currentStatus.TPS; // TPS (0% to 100%)
  
  //currentStatus.bcast_address++; //inc to next addresss
  Send_CAN_message(1, currentStatus.bcast_address+3, 24);
   
  //Need to split the int loopsPerSecond value into 2 bytes
  //ECUstreambuffer[24] = lowByte(currentStatus.loopsPerSecond);
  //ECUstreambuffer[25] = highByte(currentStatus.loopsPerSecond);

  //The following can be used to show the amount of free memory
  //currentStatus.freeRAM = freeRam();
  //ECUstreambuffer[26] = lowByte(currentStatus.freeRAM); //(byte)((currentStatus.loopsPerSecond >> 8) & 0xFF);
  //ECUstreambuffer[27] = highByte(currentStatus.freeRAM);

  //ECUstreambuffer[28] = currentStatus.batCorrection; //Battery voltage correction (%)
  //ECUstreambuffer[29] = currentStatus.spark; //Spark related bitfield
  //ECUstreambuffer[30] = currentStatus.O2_2; //O2
  
  //currentStatus.bcast_address++;   //inc to next address
 Send_CAN_message(1, currentStatus.bcast_address+4, 31);
  //rpmDOT must be sent as a signed integer
  //ECUstreambuffer[31] = lowByte(currentStatus.rpmDOT);
  //ECUstreambuffer[32] = highByte(currentStatus.rpmDOT);

  //ECUstreambuffer[33] = currentStatus.flex; //Flex sensor value (or 0 if not used)
  //ECUstreambuffer[34] = getNextError();
}

void speeduinostatuses()
{
   currentStatus.MAP = ECUstreambuffer[4];
   currentStatus.IAT = ECUstreambuffer[5];
   currentStatus.coolant = ECUstreambuffer[6];
   currentStatus.battery10 = ECUstreambuffer[8] ; //battery voltage
   currentStatus.O2 = ECUstreambuffer[9] ; //O2
   currentStatus.RPM = (ECUstreambuffer[13] << 8) | ECUstreambuffer[14];
   currentStatus.VE = ECUstreambuffer[18] ; //Current VE 1 (%)
   currentStatus.afrTarget = ECUstreambuffer[19] ;
   currentStatus.PW = ECUstreambuffer[20] ; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
   currentStatus.tpsDOT = ECUstreambuffer[21] ; //TPS DOT
   currentStatus.advance = ECUstreambuffer[22] ;
   currentStatus.TPS = ECUstreambuffer[23] ; // TPS (0% to 100%)
   currentStatus.O2_2 = ECUstreambuffer[30] ; //O2 
   currentStatus.rpmDOT = (ECUstreambuffer[31] << 8) | ECUstreambuffer[32];
   currentStatus.flex = ECUstreambuffer[33] ; 
}

