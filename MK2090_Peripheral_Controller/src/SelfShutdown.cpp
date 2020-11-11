//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "SelfShutdown.hpp"

//
// Power button is at PortKey and goes high if pressed
// Solid state relay is attached to PortHold, if held high, battery power is connected
// 
void SelfShutdown::init( uint8_t PortKey, uint8_t PortHold){
    uint8_t _portKey = PortKey;
    uint8_t _portHold = PortHold;
    pinMode( _portHold, OUTPUT);
    digitalWrite( _portHold, HIGH); // Lock yourself in power-on
    pinMode( _portKey, INPUT);
    delay(30);
    while( digitalRead(_portKey) == HIGH)
        delay(30); // suppress power button jitter
}

void SelfShutdown::shutdown(){
    digitalWrite(_portHold, LOW);
    delay(500);
    digitalWrite(_portHold, HIGH); // if at this point we are still alive, must be USB power... 
}
