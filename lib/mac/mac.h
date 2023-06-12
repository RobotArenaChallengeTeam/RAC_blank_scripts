#ifndef MAC_H
#define MAC_H
#include <Arduino.h>


// note, the first number cannot be odd due to wssp mac limitation
//possible first number can only end in  0,2,4,6,8,A,C,E, 
//ex: 0x1A is valid, 0xF3 is invalid
uint8_t robotAddress[] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};


#endif