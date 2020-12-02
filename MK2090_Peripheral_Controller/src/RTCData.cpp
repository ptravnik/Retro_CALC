//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "CP1251_mod.h" 
#include "RTCData.hpp"

//#define __DEBUG

const uint8_t _daysPerMonth[] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31};

//
// Utilities for converting into the DS3231 internal formats
//
uint8_t _BCD_To_UInt8(uint8_t val){
    return val - 6 * (val >> 4);
    }
uint8_t _UInt8_To_BCD(uint8_t val){
    return val + 6 * (val / 10);
    }
uint8_t _convertPair( const char *ptr){
    if( *ptr < 0x30) return _NUL_;
    uint8_t tmp1 = (uint8_t)(*ptr++) - 0x30;
    if( tmp1 > 9) return _NUL_;
    if( *ptr < 0x30) return _NUL_;
    uint8_t tmp2 = (uint8_t)(*ptr) - 0x30;
    if( tmp2 > 9) return _NUL_;
    return tmp1*10 + tmp2;
}
bool _checkPrintInvalid( bool valid, char *buff, byte n){
    if( valid) return false;
    snprintf_P(buff, n, PSTR("INVALID"));
    return true;
}

//
// Converts the temperature representation
// The temperature is read as a 10-bit integer:
// 8 bits in the high byte is the sighed units of C
// Two upper 2 bits in the lower byte is a fraction in 0.25C steps
//
DS3231_Temperature::DS3231_Temperature( int8_t *data){
    _wholeC = data[0];
    _fracC = ((data[1]>>6) & 0x03) * 25;
}

//
// Computes temperature in degrees C x 100
//
int16_t DS3231_Temperature::tempC_hundreds(){
    int16_t tmp = _wholeC * 100;
    tmp += (_wholeC<0)? -_fracC: _fracC;
    return tmp;  
}

//
// Prints temterature into a string
//
void DS3231_Temperature::print(char *buff, byte n){
    snprintf_P(buff, n,
        PSTR("%+02d%02d"), _wholeC, _fracC);
}
void DS3231_Temperature::printHI(char *buff, byte n){
    snprintf_P(buff, n,
        PSTR("%+02d.%02dC"), _wholeC, _fracC);
}

//
// Converts the date time representation
// Note that DS3231 clock does not support dates prior to 01-01-2000
//
DS3231_DateTime::DS3231_DateTime(){
}
DS3231_DateTime::DS3231_DateTime( uint8_t *data){
    second = _BCD_To_UInt8(data[0]);
    minute = _BCD_To_UInt8(data[1]);
    hour = _BCD_To_UInt8(data[2]);
    day = _BCD_To_UInt8(data[4]); // skip weekday
    month = _BCD_To_UInt8(data[5] & 0x7F); // drop wrap indicator
    year = _BCD_To_UInt8(data[6]);
    if( data[5] & 0x80) year += 100; // century wrap
    status = data[7];
    _checkValidity();
}
DS3231_DateTime::DS3231_DateTime( const char *command_line){
    valid = false;
    if( strlen( command_line) < 12) return;
    year = _convertPair( command_line);
    month = _convertPair( command_line + 2);
    day = _convertPair( command_line + 4);
    hour = _convertPair( command_line + 6);
    minute = _convertPair( command_line + 8);
    second = _convertPair( command_line + 10);
    _checkValidity();
}

//
// Prints the content into buffer
//
void DS3231_DateTime::print( char *buff, byte n){
    if( _checkPrintInvalid( valid, buff, n)) return;
    snprintf_P(buff, n,
        PSTR("%1X%02u%02u%02u%02u%02u%02u"),
        status, year, month, day, hour, minute, second);
}
void DS3231_DateTime::printHI( char *buff, byte n){
    if( _checkPrintInvalid( valid, buff, n)) return;
    snprintf_P(buff, n,
        PSTR("[0x%02x] %02u-%02u-20%02u %02u:%02u:%02u"),
            status, day, month, year, hour, minute, second);
}

//
// Converts to the set of bytes for injecting into the chip
// Note 1-Jan-2000 was Saturday (weekday 6), Sunday is 7
//
void DS3231_DateTime::getSetting( uint8_t *buff){
    buff[0] = _UInt8_To_BCD( second);
    buff[1] = _UInt8_To_BCD( minute);
    buff[2] = _UInt8_To_BCD( hour); // 24-hour mode forced
    buff[3] = 5; // always Friday - we don't care    
    buff[4] = _UInt8_To_BCD( day);
    buff[5] = _UInt8_To_BCD( month);
    uint8_t y = year;
    if( y >= 100){
        y -= 100;
        buff[5] |= 0x80; // century flag
    }
    buff[6] = _UInt8_To_BCD(y);
}

//
// Makes next alarm, up to 24 hours forward
// 24*60*60 = 86400
//
DS3231_Alarm DS3231_DateTime::getNextAlarm( uint32_t dt){
    DS3231_Alarm alr = DS3231_Alarm();
    long tmp = dt + second;
    alr.second = (uint8_t)(tmp % 60);
    tmp = tmp / 60 + minute;
    alr.minute = (uint8_t)(tmp % 60);
    tmp = tmp / 60 + hour;
    alr.hour = (uint8_t)(tmp % 24);
    alr.day = (uint8_t)(tmp / 24 + day);
    uint8_t daymax = pgm_read_byte(_daysPerMonth + month - 1);
    if( alr.day > daymax) alr.day -= daymax;
    alr.valid = true; // force "valid"
    return alr;
}

//
// Performs date validity check
//
void DS3231_DateTime::_checkValidity(){
    valid = false;
    if( year > 199) return;
    if( month == 0) return;
    if( month > 12) return;
    if( day == 0) return;
    if( hour > 23) return;
    if( minute > 59) return;
    if( second > 59) return;
    uint8_t daymax = pgm_read_byte(_daysPerMonth + month - 1);
    if( month == 2 && year != 100 && year%4 == 0) daymax++;
    if( day>daymax) return;
    valid = true;
}

//
// Converts the alarm time representation
//
DS3231_Alarm::DS3231_Alarm(){
}
DS3231_Alarm::DS3231_Alarm( uint8_t *data){
    second = _BCD_To_UInt8(data[0] & 0x7F);
    minute = _BCD_To_UInt8(data[1] & 0x7F);
    hour = _BCD_To_UInt8(data[2] & 0x7F);
    day = _BCD_To_UInt8(data[3] & 0x3F);
    _checkValidity();
}
DS3231_Alarm::DS3231_Alarm( const char *command_line){
    valid = false;
    if( strlen(command_line) < 8) return;
    day = _convertPair( command_line + 0);
    hour = _convertPair( command_line + 2);
    minute = _convertPair( command_line + 4);
    second = _convertPair( command_line + 6);
    _checkValidity();
}

//
// Prints the content into buffer
//
void DS3231_Alarm::print( char *buff, byte n){
    if( _checkPrintInvalid( valid, buff, n)) return;
    snprintf_P(buff, n,
        PSTR("%02u%02u%02u%02u"),
            day, hour, minute, second);
}
void DS3231_Alarm::printHI( char *buff, byte n){
    if( _checkPrintInvalid( valid, buff, n)) return;
    snprintf_P(buff, n,
        PSTR("ALR: %02u %02u:%02u:%02u"),
            day, hour, minute, second);
}

//
// Converts to the set of bytes for injecting into the chip
//
void DS3231_Alarm::getSetting( uint8_t *buff){
    buff[0] = _UInt8_To_BCD( second);
    buff[1] = _UInt8_To_BCD( minute);
    buff[2] = _UInt8_To_BCD( hour);
    buff[3] = _UInt8_To_BCD( day);
}

//
// Performs date validity check
//
void DS3231_Alarm::_checkValidity(){
    valid = false;
    if( day > 31) return;
    if( hour > 23) return;
    if( minute > 59) return;
    if( second > 59) return;
    valid = true;
}
