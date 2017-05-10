#ifndef COMMS_H
#define COMMS_H

uint8_t cmdreply;
uint8_t realtimebuffer[60];   //buffer array to store the realtime data read
volatile uint8_t replylength;          // the length of the reply variable

void command();
void sendTheCommand(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength);

#endif // COMMS_H
