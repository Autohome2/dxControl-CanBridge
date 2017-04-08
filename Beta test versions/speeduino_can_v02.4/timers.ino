/*
Speeduino Can interface
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory

Based on code by Josh Stewart for the Speeduino project , see www.Speeduino.com for more info
*/

/*
Timers are used for having actions performed repeatedly at a fixed interval (Eg every 100ms)
They should not be confused with Schedulers, which are for performing an action once at a given point of time in the future

Timers are typically low resolution (Compared to Schedulers), with maximum frequency currently being approximately every 10ms
*/
#include "timers.h"
#include "globals.h"

void initialiseTimers() 
{
#if defined(PROCESSOR_MEGA_ALL)    
   //Configure Timer2 for our low-freq interrupt code. 
   TCCR2B = 0x00;          //Disbale Timer2 while we set it up
   TCNT2  = 131;           //Preload timer2 with 131 cycles, leaving 125 till overflow. As the timer runs at 125Khz, this causes overflow to occur at 1Khz = 1ms
   TIFR2  = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
   TIMSK2 = 0x01;          //Timer2 Set Overflow Interrupt enabled.
   TCCR2A = 0x00;          //Timer2 Control Reg A: Wave Gen Mode normal
   /* Now configure the prescaler to CPU clock divided by 128 = 125Khz */
   TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
   TCCR2B &= ~(1<<CS21);             // Clear bit
   
#elif defined(PROCESSOR_TEENSY_3_2)
  lowResTimer.begin(oneMSInterval, 1000);  // oneMSInterval to run every 0.001 seconds ie 1ms
  lowResTimer2.begin(fiftyMSInterval, 500000);  //50000);  //fiftyMSInterval to run every 0.05 seconds ie 50ms or 20hz
#endif   
}


//Timer2 Overflow Interrupt Vector, called when the timer overflows.
//Executes every ~1ms.
#if defined(PROCESSOR_MEGA_ALL)
ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
#endif
#if defined(PROCESSOR_TEENSY_3_2)
void oneMSInterval()
#endif 
{  

  //Increment Loop Counters
  loop50ms++;
  loop200ms++;
  loopSec++;

#if defined(PROCESSOR_MEGA_ALL)    
  //Loop executed every 1 second (1ms x 50 = 50ms or 20hz)
  if (loop50ms == 1000) //50) 
  {
    loop50ms = 0; //Reset counter.
     if (configPage1.canio_11bit_can_bcast > 1){Serial1.write("A");}  //request current data from speeduino    
  }  
#endif
    
  //Loop executed every 1 second (1ms x 1000 = 1000ms)
  if (loopSec == 1000) 
  {
    loopSec = 0; //Reset counter.

    //**************************************************************************************************************************************************
    //This records the number of main loops the system has completed in the last second
    currentStatus.canio_loopsPerSecond = mainLoopCount;
    mainLoopCount = 0;
    //**************************************************************************************************************************************************
    //increament secl (secl is simply a counter that increments every second and is used to track whether the system has unexpectedly reset
    currentStatus.secl++;

  }
  
#if defined(PROCESSOR_MEGA_ALL)
      //Reset Timer2 to trigger in another ~1ms 
    TCNT2 = 131;            //Preload timer2 with 100 cycles, leaving 156 till overflow.
    TIFR2  = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
void fiftyMSInterval(void)
{
 if (sendA == 0)
  {
  //this interupt runs every 50ms or 20hz.
  if (configPage1.canio_11bit_can_bcast > 1){Serial1.write("A");}  //request current data from speeduino
  }
}
//#endif
