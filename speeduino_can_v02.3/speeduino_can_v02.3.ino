//#include <mcp_can_dfs.h>

/*
Speeduino Can interface
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory

Based on code by Josh Stewart for the Speeduino project , see www.Speeduino.com for more info
*/
#include "globals.h"
#include <mcp_can.h>    //use the cory fowler lib
#include <SPI.h>
#include <Wire.h>
#include "comms.h"
#include "math.h"
#include "timers.h"
#include "utils.h"
#include "can.h"          //my can.h file
#include "obd.h"          //my obd.h file

#if defined(PROCESSOR_TEENSY_3_2)

    #include "usb_serial.h"
    #include <FlexCAN.h>
#endif

// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
//#define _sclk 13
//#define _miso 12
//#define _mosi 11
//#define _cs 10
//#define _dc 9
//#define _rst 8

#include "storage.h"

  
#if defined(PROCESSOR_MEGA_NO61)
       const int SPI_CS1_PIN = 53;
       const int SPI_CS2_PIN = 10;
    MCP_CAN CAN1(SPI_CS1_PIN);    //create instance of mcp can as can1
    MCP_CAN CAN2(SPI_CS2_PIN);           // create instance of mcp_can as can2 
    
#elif defined(PROCESSOR_TEENSY_3_2)
        const int SPI_CS1_PIN = 10;
        const int slaveSelectPin = 10;
    MCP_CAN CAN1(SPI_CS1_PIN);    //create instance of mcp can as can1
    MCP_CAN CAN2(SPI_CS1_PIN);   // create instance of mcp_can as can2
    
        FlexCAN CANbus1(250000);   //setup can interface to 500k
        static CAN_message_t txmsg,rxmsg;
#endif

struct config1 configPage1;

unsigned long currentLoopTime; //The time the current loop started (uS)
unsigned long previousLoopTime; //The time the previous loop started (uS)

struct statuses currentStatus;
volatile int mainLoopCount;

void setup() {

  // put your setup code here, to run once:
 // set the slaveSelectPin as an output:
//  pinMode (slaveSelectPin, OUTPUT);
    // initialize SPI:
//  SPI.begin(); 
 // loadConfig();   //load saved parameters from eeprom into ram
 
  //Initial values for loop times
  previousLoopTime = 0;
  currentLoopTime = micros();

     Serial.begin(115200);  //this starts serial0 on the megas or the usb on the teensy
#if defined(PROCESSOR_MEGA_NO61)
     Serial1.begin(115200);   // the serial connection to the speeduino ecu
//     Serial2.begin(115200);
     
#elif defined(PROCESSOR_TEENSY_3_2)
      Serial1.begin(115200);  // the serial connection to the speeduino ecu
     // Serial2.begin(115200);
     // Serial3.begin(115200);
#endif     
Serial.print (SPI_CS1_PIN);
loadConfig();   //load saved parameters from eeprom into ram

  //Set the pin mappings
  setPinMapping(configPage1.pinMapping);  //setup the pin to function mapping according to TS selection
  
  initialiseTimers();   //init hardware timers
  initialiseCAN1();   //init can interface 1
//  if (configPage1.obd_active){initialiseCAN2();} //init can interface 2 
       
    mainLoopCount = 0;
    unsigned int testRPM = 1430;
    ECUstreambuffer[13] = lowByte(testRPM);
    ECUstreambuffer[14] = highByte(testRPM);
}

void loop() {
    
      mainLoopCount++;  
 
      //Check for any requests from serial. Serial operations are checked under 2 scenarios:
      // 1) Every 64 loops (64 Is more than fast enough for TunerStudio). This function is equivalent to ((loopCount % 64) == 1) but is considerably faster due to not using the mod or division operations
      // 2) If the amount of data in the serial buffer is greater than a set threhold (See globals.h). This is to avoid serial buffer overflow when large amounts of data is being sent
      if ( ((mainLoopCount & 63) == 1) or (Serial.available() > SERIAL_BUFFER_THRESHOLD) )
      //if ( ((mainLoopCount & 63) == 1) or (Serial.available() > 10)) // > SERIAL_BUFFER_THRESHOLD)) 
            {
            if (Serial.available() > 0) 
                {
                command();
                }
            }
      
     if ( ((mainLoopCount & 63) == 1) or (Serial1.available() > SERIAL_BUFFER_THRESHOLD)) 
        {
        if (Serial1.available() > 0) 
            {
            command1();
            }
        }

     if ((mainLoopCount & 63) == 1)
        {
        //receive_CAN_message(1);          //check can1 for new messages
        //if (configPage1.obd_active ==1)
        //      {
        //      receive_CAN_message(2);     //check can2 for new messages
        //      }
        }
        
}
