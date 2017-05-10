#ifndef SENSORS_H
#define SENSORS_H

uint16_t EGT1 ;
uint16_t EGT2 ;
uint16_t EGT3 ;
uint16_t EGT4 ;
uint16_t Eot;
uint16_t Tot;

void getsensor_reading(uint16_t param);

#endif // SENSORS_H
