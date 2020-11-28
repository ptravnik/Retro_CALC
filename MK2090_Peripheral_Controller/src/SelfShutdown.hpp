//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef SELF_SHUTDOWN_HPP
#define SELF_SHUTDOWN_HPP

#include <Arduino.h>

class SelfShutdown{
  public:
    void init( uint8_t PortKey, uint8_t PortHold);
    inline bool isPowerPressed(){ return digitalRead(_portKey) == HIGH;};
    void shutdown();
  private:
    uint8_t _portKey = A2;
    uint8_t _portHold = A1;
};

#endif
