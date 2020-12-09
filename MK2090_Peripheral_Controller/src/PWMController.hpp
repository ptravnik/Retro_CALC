//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef PWM_CONTROLLER_HPP
#define PWM_CONTROLLER_HPP

#include <Arduino.h>

class PWMController{
  public:
    uint8_t dutyCycle = 50; // percent
    void begin( uint8_t Port);
    void set( const char* command);
    void set( uint8_t dc);
    inline void print( char *buff, byte n){
      snprintf_P(buff, n, PSTR("%03u"), dutyCycle);};
    inline void printHI( char *buff, byte n){
      snprintf_P(buff, n, PSTR("PWM = %03u%"), dutyCycle);};
  private:
    uint8_t _port = 6;
};

#endif
