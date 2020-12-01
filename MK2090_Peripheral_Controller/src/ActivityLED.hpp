//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef ACTIVITY_LED_HPP
#define ACTIVITY_LED_HPP

#include <Arduino.h>

#define INACTIVITY_INTERVAL 1000
#define BLINK_INTERVAL 100

class ActivityLED{
  public:
    long lastActive = 0L;
    long lastBlinked = 0L;
    void begin( uint8_t Port);
    inline void blink( uint16_t d1, uint16_t d2){
        delay(d1); digitalWrite( _port, HIGH);
        delay(d2); digitalWrite( _port, LOW);};
    void flop();
    inline void setActive(){
        lastActive = millis();
        flop();};
  private:
    uint8_t _port = 10;
    bool _lit = false;
};

#endif
