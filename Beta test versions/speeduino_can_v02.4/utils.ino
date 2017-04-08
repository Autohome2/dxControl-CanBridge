/*
Speeduino Can interface
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory

Based on code by Josh Stewart for the Speeduino project , see www.Speeduino.com for more info
*/

/*
Returns how much free dynamic memory exists (between heap and stack)
*/
#include "utils.h"

int canio_freeRam ()
{
  #if defined(PROCESSOR_MEGA_NO61)
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  #endif
}


uint16_t canio_freeram32()
{                             // for Teensy 3.0
  #if defined(PROCESSOR_TEENSY_3_2)
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is the free, available ram.
    return (uint16_t)stackTop - heapTop;

  #endif
}

void setPinMapping(byte boardID)
{

  #if defined(PROCESSOR_MEGA_NO61)
    switch (boardID)
    {
    case 0:
      //Pin mappings as per the v0.1 shield

    break;
    default:

    break;
    }
    
  #elif defined(PROCESSOR_TEENSY_3_2)
    switch (boardID)
    {

    case 0:
      //Pin mappings as per the v0.1 shield
      
    break;
    default:

    break;
    
    }
  #endif
}

