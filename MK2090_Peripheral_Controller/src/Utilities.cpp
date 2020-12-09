//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Utilities.hpp"

//
// More efficient version of atoi, for unsigned only
//
uint8_t _convertDigit( const char *ptr, uint8_t add){
    if( *ptr < 0x30) return 0;
    uint8_t tmp = (uint8_t)(*ptr) - 0x30;
    if( tmp > 9) return 0;
    return tmp + add;
}

uint8_t _convert1Digit( const char *ptr){
    return _convertDigit( ptr, 0);
}

uint8_t _convert2Digits( const char *ptr){
    if( strlen(ptr) < 2) return 0;
    uint8_t tmp = _convertDigit( ptr, 0);
    return _convertDigit( ptr+1, tmp*10);
}

uint8_t _convert3Digits( const char *ptr){
    if( strlen(ptr) < 3) return 0;
    uint8_t tmp = _convert2Digits( ptr);
    if( tmp > 24) return 0;
    return _convertDigit( ptr+2, tmp*10);
}

//
// Utilities for converting into the DS3231 internal formats
//
uint8_t _BCD_To_UInt8(uint8_t val){
    return val - 6 * (val >> 4);
}
uint8_t _UInt8_To_BCD(uint8_t val){
    return val + 6 * (val / 10);
}
bool _checkPrintInvalid( bool valid, char *buff, byte n){
    if( valid) return false;
    snprintf_P(buff, n, PSTR("INVALID"));
    return true;
}
