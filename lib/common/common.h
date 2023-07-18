#ifndef COMMON_H
#define COMMON_H
#include <Arduino.h>

//ESNOW VARS

#define ESPNOW_RAC_CHANNEL 1

// note, the first number cannot be odd due to wssp mac limitation
//possible first number can only end in  0,2,4,6,8,A,C,E, 
//ex: 0x1A is valid, 0xF3 is invalid
uint8_t robotAddress[] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};

//PACKETS
// remote -> robot
typedef struct {
  int16_t speedmotorLeft;
  int16_t speedmotorRight;
  int16_t packetArg1;
  int16_t packetArg2;
  int16_t packetArg3;
} packet_t;

#endif