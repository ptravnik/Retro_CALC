//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "RTCData.hpp"

//
// Starts I2C two-wire comm
// 
void RTCData::init(){
}

//
// Reads voltage, performs conversion
//
int32_t RTCData::read(){
    lastRead = analogRead( _port);
    delay(5);
    lastRead += analogRead( _port);
    delay(5);
    lastRead += analogRead( _port);
    lastRead *= _gain;
    lastRead /= 3069;
    lastRead += _offset;
    return lastRead;
}

