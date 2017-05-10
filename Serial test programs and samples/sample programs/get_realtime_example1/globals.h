/*
 The "r" command allows you to specify where in the reltime data list you want the Speeduino to start sending you data from , and how many bytes you want to be sent.
 The format is "r\$tsCanId\x30%2o%2c" where
 r == the main command
 tsCanId == the canid of the device being called
 x30 == dec48 the optimised output command
 2o == 2 bytes of offset
 2c == 2 bytes of length
 
 */
#define maincommand = "r";    // the command to send to the Speeduino
#define tsCanId = 0;          // this is the tunerstudio canID , this is 0 for the main ecu in the system which is usually the speeduino ecu . 
                              // this value is set in Tunerstudio when configuring your Speeduino
uint16_t realtime_offset = 4;  // the offset of the realtime data to start reading from
uint16_t realtime_bytes = 1;   // the number of bytes requested

/*
  
change the realtime_offset to what offset position you wish to read the realtime data from
eg : realtime_offset = 4 , this relates to the currentStatus.MAP
This is 1 byte long so if this is the only byte you want to read set realtime_bytes to 1

eg : realtime_offset = 8 , this relates to the  currentStatus.battery10 (battery voltage)
this is 1 byte long.
if you set realtime_bytes = 2
then you will also be sent offset 9 too which is currentStatus.O2

 */


                              
