/*
Speeduino Can interface
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory

Based on code by Josh Stewart for the Speeduino project , see www.Speeduino.com for more info
*/

#include <EEPROM.h>
#include "storage.h"
#include "globals.h"

/*
Takes the current configuration (config pages and maps)
and writes them to EEPROM as per the layout defined in storage.h
*/
void writeConfig()
{
  /*
  We only ever write to the EEPROM where the new value is different from the currently stored byte
  This is due to the limited write life of the EEPROM (Approximately 100,000 writes)
  */
  
  //Create a pointer to the config page
  uint8_t* pnt_configPage;  
  if(EEPROM.read(0) != data_structure_version) { EEPROM.write(0,data_structure_version); }   //Write the data structure version
  
  /*---------------------------------------------------
  | Config page 1 (See storage.h for data layout)
  | 64 byte long config table
  -----------------------------------------------------*/
  pnt_configPage = (uint8_t *)&configPage1; //Create a pointer to Page 1 in memory
  for(uint16_t x=EEPROM_CONFIG1_START; x<EEPROM_CONFIG1_END; x++) 
  { 
    if(EEPROM.read(x) != *(pnt_configPage + uint8_t(x - EEPROM_CONFIG1_START))) { EEPROM.write(x, *(pnt_configPage + uint8_t(x - EEPROM_CONFIG1_START))); }
  }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------
void loadConfig()
{
  //int offset;
  //Create a pointer to the config page
  uint8_t* pnt_configPage;
    
  pnt_configPage = (uint8_t *)&configPage1; //Create a pointer to Page 1 in memory
  for(uint16_t x=EEPROM_CONFIG1_START; x<EEPROM_CONFIG1_END; x++)        // x=100;x<164
  { 
    *(pnt_configPage + uint8_t(x - EEPROM_CONFIG1_START)) = EEPROM.read(x);
  }
  //That concludes the reading of config1

}

