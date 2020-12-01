//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "VoltageSensor.hpp"

//
// Voltage sensor reads from Port
// The formula is V = gain * reading + offset
// 
void VoltageSensor::begin( uint8_t Port, int16_t gain, int16_t offset){
    _port = Port;
    _gain = gain;
    _offset = offset;
}

//
// Reads voltage, performs conversion
//
int32_t VoltageSensor::read(){
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

