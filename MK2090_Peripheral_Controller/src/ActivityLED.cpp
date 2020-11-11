//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "ActivityLED.hpp"

//
// Blinker LED is attached to Port and set to LOW
// 
void ActivityLED::init( byte Port){
    _port = Port;
    pinMode( _port, OUTPUT);
    digitalWrite( _port, LOW);
    lastActive = millis();
}

//
// Blinks external LED if last active is recent
//
void ActivityLED::flop(){
    long t = millis();
    if( t < lastActive) // wraparound
        lastActive = 0L;
    if( t - lastActive > INACTIVITY_INTERVAL){
        if(!_lit) return;
        _lit = false;
        digitalWrite( _port, _lit);
        lastBlinked = t;    
        return;
    }
    if( t - lastBlinked < BLINK_INTERVAL) return;
    _lit = !_lit;
    digitalWrite( _port, _lit);  
    lastBlinked = t;
}
