//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef RTCDATA_HPP
#define RTCDATA_HPP

#include <Arduino.h>

class RTCData{
  public:
    int32_t lastRead = 0; // mV
    void init();
    int32_t read();
  private:
    uint8_t _port = A3;
    uint16_t _gain = 1;
    uint16_t _offset = 1;
};

#endif
