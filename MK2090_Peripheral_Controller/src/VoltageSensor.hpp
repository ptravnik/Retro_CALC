//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef VOLTAGE_SENSOR_HPP
#define VOLTAGE_SENSOR_HPP

#include <Arduino.h>

#define SCALE_VOLTAGE 3300
#define OFFSET_VOLTAGE 0

class VoltageSensor{
  public:
    int32_t lastRead = 0; // mV
    void begin( uint8_t Port, int16_t gain=SCALE_VOLTAGE, int16_t offset=OFFSET_VOLTAGE);
    int32_t read();
    inline void print( char *buff, byte n){
      snprintf_P(buff, n, PSTR("%04u"), lastRead);};
    inline void printHI( char *buff, byte n){
      snprintf_P(buff, n, PSTR("+%1u.%03uV"), lastRead / 1000, lastRead % 1000);};
  private:
    uint8_t _port = 10;
    uint16_t _gain = SCALE_VOLTAGE;
    uint16_t _offset = OFFSET_VOLTAGE;
};

#endif
