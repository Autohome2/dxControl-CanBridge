#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>

#if defined(__arm__)   
    #if defined(__MK20DX256__) && defined(CORE_TEENSY)      
        #define PROCESSOR_TEENSY_3_2  1 //compile for teensy 3.1/2 only
    #elif defined(__MK64FX512__) && defined(CORE_TEENSY)      
        #define PROCESSOR_TEENSY_3_5  1 //compile for teensy 3.5 only
    #endif    
    #if defined(__MK20DX256__) && defined(CORE_TEENSY) || defined(__MK64FX512__) && defined(CORE_TEENSY)
        #define PROCESSOR_TEENSY_3_x  1 //compile for both teensy 3.1/2 and 3.5 
    #elif defined (CORE_TEENSY)
        #error "Unknown Teensy"
    #elif defined (__arm__)
        #error "Unknown ARM chip"    
    #else
        #error "Unknown board"
    #endif
    
#elif defined(__AVR__)
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        #define PROCESSOR_MEGA_NO61 1
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
        #define PROCESSOR_MEGA_ALL  1
    #endif          
    #endif
#endif

const unsigned char canio_RevNum[]    = "speeduino 201610-canif"; 
const unsigned char canio_signature[] = "speeduino 201610-canif  DS Can Interface"; 

const byte data_structure_version = 2; //This identifies the data structure when reading / writing. 

uint8_t canio_canid = 0; //fix the can interface tscanid


// Page sizes 
const byte page_1_size = 64;;

uint8_t theCommand;
bool sendA;
uint8_t Slength;

uint8_t Abuffer[50];
//uint8_t B1store[40];
uint8_t ECUstreambuffer[50];  //array holding data from speeduino broadcast output stream


//Handy bitsetting macros
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_CHECK(var,pos) ((var) & (1<<(pos)))

//Table sizes

#define SERIAL_BUFFER_THRESHOLD 32 // When the serial buffer is filled to greater than this threshold value, the serial processing operations will be performed more urgently in order to avoid it overflowing. Serial buffer is 64 bytes long, so the threshold is set at half this as a reasonable figure

//The status struct contains the current values for all 'live' variables

struct statuses{  
  uint8_t canio_system_status; //bitfield for system status to TS
  volatile uint8_t secl; //Continous 
  volatile uint16_t canio_loopsPerSecond;
  volatile uint16_t bcast_address ;
  uint16_t canio_freeRAM;
  int16_t currentInputvalue;
  uint16_t RPM;
  long MAP; //Has to be a long for PID calcs (Boost control)
 // int16_t baro; //Barometric pressure is simply the inital MAP reading, taken before the engine is running
  uint8_t TPS; //The current TPS reading (0% - 100%)
 // uint8_t TPSlast; //The previous TPS reading
 // unsigned long TPS_time; //The time the TPS sample was taken
  //unsigned long TPSlast_time; //The time the previous TPS sample was taken
 // uint8_t tpsADC; //0-255 byte representation of the TPS
  uint8_t tpsDOT;
  int16_t rpmDOT;
  uint8_t VE;
  uint8_t O2;
  uint8_t O2_2;
  int16_t coolant;
  int16_t cltADC;
  int16_t IAT;
 // int16_t iatADC;
 // int16_t batADC;
 // int16_t O2ADC;
//  int16_t O2_2ADC;
  int16_t dwell;
  uint8_t dwellCorrection; //The amount of correction being applied to the dwell time.
  uint8_t battery10; //The current BRV in volts (multiplied by 10. Eg 12.5V = 125)
  uint8_t advance;
 // uint8_t corrections;
 // uint8_t TAEamount; //The amount of accleration enrichment currently being applied
  //uint8_t egoCorrection; //The amount of closed loop AFR enrichment currently being applied
 // uint8_t wueCorrection; //The amount of warmup enrichment currently being applied
 // uint8_t batCorrection; //The amount of battery voltage enrichment currently being applied
 // uint8_t iatCorrection; //The amount of inlet air temperature adjustment currently being applied
 // uint8_t launchCorrection; //The amount of correction being applied if launch control is active
  uint8_t afrTarget;
  uint8_t idleDuty;
  uint8_t flex; //Ethanol reading (if enabled). 0 = No ethanol, 100 = pure ethanol. Eg E85 = 85. 
  uint16_t PW; //In uS
     
  //Helpful bitwise operations:
  //Useful reference: http://playground.arduino.cc/Code/BitMath
  // y = (x >> n) & 1;    // n=0..15.  stores nth bit of x in y.  y becomes 0 or 1.
  // x &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
  // x |= (1 << n);       // forces nth bit of x to be 1.  all other bits left alone.
  
};

//Page 1 of the config - See the ini file for further reference
//this is laid out as first the byte size data then the words
struct __attribute__ ( ( packed ) ) config1 {
 uint8_t canio_canid ;  // The TS can id of this interface unit byte 
 uint16_t canio_true_canid ;//  the true canid starting at 0x200(512) but is offset by 512 eg 0x200 is 0
 uint8_t canio_obd_canid;//  add 1774 for actual address displayed. if <241 or <241+1774=2015 then is off
 uint16_t canio_11bit_can_bcast;// add 511 for actual address displayed .if <1 or <1+511=512 then is off
 uint8_t spare1_6 ; //true canid in decimal starting at 0x200(512) but is offset by 512 eg 0x200 is 0
 uint8_t ECU_canid ;
 uint16_t ECU_true_canid  ;//
 uint8_t ECU_obd_canid  ;
 uint8_t ECU_poll_period;
 uint8_t qty_analogin ;
 uint8_t qty_digitalin  ;
 uint8_t  p1unused14;
 uint8_t  p1unused15;
 uint8_t  p1unused16;
 uint8_t  p1unused17;
 uint8_t  p1unused18;
 uint8_t  p1unused19;
 uint8_t  p1unused20;
 uint8_t  p1unused21;
 uint8_t  p1unused22;
 uint8_t  p1unused23;
 uint8_t  p1unused24;
 uint8_t  p1unused25;
 uint8_t  p1unused26; 
 uint8_t  p1unused27; 
 uint8_t  p1unused28;
 uint8_t  p1unused29;
 uint8_t  pinMapping:4 ;
 byte  p1unused31;
 byte  p1unused32;
 byte  p1unused33;
 byte  p1unused34;
 byte  p1unused35;
 byte  p1unused36;
 byte p1unused37;
 byte p1unused38;  
 byte p1unused39;
 uint8_t p1unused40;
 uint8_t p1unused41;
 uint8_t p1unused42;
 uint8_t p1unused43;
 uint8_t p1unused44;
 byte p1unused45;
 byte p1unused46;
 byte p1unused47;
 byte p1unused48;
 byte p1unused49;
 byte p1unused50;
 byte p1unused51;
 byte p1unused52;
 byte p1unused53;
 byte p1unused54;
 byte p1unused55;
 byte p1unused56;
 byte p1unused57;
 byte p1unused58;
 byte p1unused59;
 byte p1unused60;
 byte p1unused61;
 byte p1unused62;
 byte obd_active;
};

// global variables // from speeduino.ino
extern struct statuses currentStatus; // from speeduino.ino
extern struct config1 configPage1;

extern unsigned long currentLoopTime; //The time the current loop started (uS)
extern unsigned long previousLoopTime; //The time the previous loop started (uS) 

#endif
