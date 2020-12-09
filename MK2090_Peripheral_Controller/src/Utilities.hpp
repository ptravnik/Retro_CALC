//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Arduino.h>

//
// More efficient version of atoi, for unsigned only
//
uint8_t _convert1Digit( const char *ptr);
uint8_t _convert2Digits( const char *ptr);
uint8_t _convert3Digits( const char *ptr);

//
// Utilities for converting into the DS3231 internal formats
//
uint8_t _BCD_To_UInt8(uint8_t val);
uint8_t _UInt8_To_BCD(uint8_t val);
bool _checkPrintInvalid( bool valid, char *buff, byte n);

#endif
