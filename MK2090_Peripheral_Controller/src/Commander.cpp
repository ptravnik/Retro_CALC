//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "CP1251_mod.h"
#include "Commander.hpp"

void Commander::init( CircularBuffer *buffer){
    _buffer = buffer;
}

void Commander::addCommand( uint8_t code, void (*action)( char *, uint8_t)){
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
    switch(b){
        case COMMAND_SET_DATETIME:
            Serial.println( "here!");
            break;
        default:
            Serial.print( "Unknown: ");
            Serial.println( b);
            break;
    }
}

void Commander::_printDateTime(const RtcDateTime& dt){
    char tmp[20];
    snprintf_P(tmp, 20,
        PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
        dt.Day(), dt.Month(), dt.Year(),
        dt.Hour(), dt.Minute(), dt.Second() );
    Serial.print(tmp);
}
