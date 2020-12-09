//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "PiezoController.hpp"
#include "Utilities.hpp"

//
// Piezo Controller sends tone into port
// 
void PiezoController::begin( uint8_t Port){
    _port = Port;
}

//
// Performs conversion from command string and plays tone
//
void PiezoController::play( char *buff, byte n){
    uint16_t frequency = 0;
    uint16_t duration = 0;
    if(strlen( buff) >= 7){
        frequency = (uint16_t)_convert2Digits( buff) * 100;
        frequency += _convert2Digits( buff+2);
        duration = (uint16_t)_convert2Digits( buff+4) * 100;
        duration += _convert1Digit( buff+6);
    }
    play( frequency, duration);
}
void PiezoController::play( uint16_t Frequency, uint16_t Duration){
    if(Frequency < 31 || 4978 < Frequency){
        noTone(_port);
        return;
        }
    if( Duration == 0){
        tone( _port, (unsigned int)Frequency); // play forever!
        return;
        }
    tone( _port, Frequency, Duration);
    delay( Duration + (Duration>>2)); // pause between tones at 1.25 tone duration
}
