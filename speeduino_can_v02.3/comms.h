#ifndef COMMS_H
#define COMMS_H

//These are the page numbers that the Tuner Studio serial protocol uses to transverse the different map and config pages.
#define canio_setupPage        1   //Config Page 1

#define ignMapPage   11
#define afrMapPage   12

//#define do_int_pid_search 1
//#define PID_REQUEST    0x7DF
//#define PID_REPLY      0x7E8

unsigned long requestCount = 0; //The number of times the A command has been issued
uint8_t dest_canID;
uint16_t cmdlength = 0;
int intdev_gauge = 0;
byte bytedev_gauge = 0;
byte bufpos = 0;
byte Pval = 0;
byte Vval = 0;
uint8_t Alength;
byte Qlength;
byte Qpos = 0;
int count = 0 ;
int bytecount = 0;           

byte currentPage = 1;//Not the same as the speeduino config page numbers
boolean isMap = true;
  
void command(); //serial commands sent to serial0
void sendValues(uint8_t packet_Length,uint8_t can_id);
void receiveValue(uint16_t now_page, uint16_t page_offset, uint8_t newValue);
void saveConfig();
void sendPage(uint8_t pagenum, uint16_t send_page_Length,bool useChar, uint8_t can_id);
void setPage(byte Pdata, byte can_id);
void testComm();
void command1();  //serial commands sent to serial 1

#endif // COMMS_H
