//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef PIEZO_CONTROLLER_HPP
#define PIEZO_CONTROLLER_HPP

#include <Arduino.h>

class PiezoController{
  public:
    void begin( uint8_t Port);
    void play( uint16_t Frequency, uint16_t Duration);
    void play( char *buff, byte n);
    inline void print( char *buff, byte n){
      snprintf_P(buff, n, PSTR("TONE"));};
  private:
    uint8_t _port = 10;
};

#endif
