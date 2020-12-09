//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "PWMController.hpp"
#include "Utilities.hpp"

//
// PWM Controller writes into port
// while converting 0-100% to 0-255
// 
void PWMController::begin( uint8_t Port){
    _port = Port;
    pinMode(_port, OUTPUT);
    digitalWrite(_port, LOW);
}

//
// Performs conversion from command string and sets duty cycle
//
void PWMController::set( const char* command){
    set( _convert3Digits( command));
}
void PWMController::set( uint8_t dc){
    dutyCycle = dc;
    if( dutyCycle>100) dutyCycle = 100;
    uint16_t d = (uint16_t)dutyCycle * 255 / 100;
    if( d < 10){
        digitalWrite(_port, LOW);
        return;
    }
    if( d >= 255){
        digitalWrite(_port, HIGH);
        return;
    }
    analogWrite( _port, d);
}
