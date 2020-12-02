//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "CP1251_mod.h"
#include "Commander.hpp"

void Commander::begin( CircularBuffer *buffer){
    _buffer = buffer;
}

void Commander::addCommand( uint8_t code, void (*action)( char *, uint8_t)){
    if( _ncomm >= NCOMMANDS) return;
    Command *c = _commands + _ncomm;
    c->code = code;
    c->action = action;
    _ncomm++;
}

void Commander::read(){
    if( !Serial.available()) return;
    uint8_t b = Serial.read();
    if( b == _CR_) return;
    for( uint8_t i=0; i<_ncomm; i++){
        if( b != _commands[i].code) continue;
        _commands[i].action( _inputBuff, INPUT_BUFFER_LENGTH);
        Serial.print(_inputBuff);
        Serial.println();
        return;
    }
    Serial.print( "Unknown: ");
    Serial.println( b);
}
