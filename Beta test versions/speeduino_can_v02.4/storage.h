#ifndef STORAGE_H
#define STORAGE_H
#include <EEPROM.h>

void writeConfig();
void loadConfig();

/*
Current layout of EEPROM data (Version 3) is as follows (All sizes are in bytes):
mega 2560 has 4k but teensy only has 2048bytes!
|---------------------------------------------------|
|Byte # |Size | Description                         |
|---------------------------------------------------|
| 0     |1    | Data structure version              |
| 1     |2    |                                     |
| 3     |4    |                                     |
| 10    |64   | Page 1 settings                     |
|       |     |                                     |
|       |
|       |     |                                     |
|         |
|       |     |                                     |
|       |     |                                     |
|      |
|       |     |                                     |


-----------------------------------------------------
*/

#define EEPROM_CONFIG1_START 10
#define EEPROM_CONFIG1_END 73   

byte EEPROM_loc;

#endif // STORAGE_H
