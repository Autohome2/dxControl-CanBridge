#include "globals.h"
#include "comms.h"
#include "sensors.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial1(3, 2); // RX, 
SoftwareSerial mySerial2(6, 5); // RX, TX

void setup() {
  // put your setup code here, to run once:
  mySerial1.begin(115200);
   mySerial2.begin(115200);  


}

void loop() {
  // put your main code here, to run repeatedly:
      if ( (mySerial2.available() > 16) )
      {
        if (mySerial2.available() > 0)
        {
          command();
        }
      }
}
