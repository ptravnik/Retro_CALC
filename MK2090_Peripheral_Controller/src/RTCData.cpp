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
const uint8_t _DS3231alarms[] PROGMEM = { ALARM1_DHMS, ALARM1_WHMS, ALARM1_HMS, ALARM1_MS, 
                                          ALARM2_DHM, ALARM2_WHM, ALARM2_HM};

//
// Utilities for converting into the DS3231 internal formats
//
uint8_t _BCD_To_UInt8(uint8_t val){
    return val - 6 * (val >> 4);
    }
uint8_t _UInt8_To_BCD(uint8_t val){
    return val + 6 * (val / 10);
    }
uint8_t _BCD_To_24hr(uint8_t val){
    // Small values - simple conversion
    if (!(val & 0x40)) return _BCD_To_UInt8(val);
    
    // 12 hour mode, convert to 24
    bool isPm = ((val & 0x20) != 0);
    uint8_t hour = _BCD_To_UInt8(val & 0x1f);
    if (isPm) hour += 12;
    return hour;
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
DS3231_Temperature::DS3231_Temperature( uint8_t *data){
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
    hour = _BCD_To_24hr(data[2]);
    day = _BCD_To_UInt8(data[4]); // skip weekday
    month = _BCD_To_UInt8(data[5] & 0x7F); // drop wrap indicator
    year = _BCD_To_UInt8(data[6]);
    if( data[5] & 0x80) year += 100; // century wrap
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
        PSTR("%02u%02u%02u%02u%02u%02u"),
        year, month, day, hour, minute, second);
}
void DS3231_DateTime::printHI( char *buff, byte n){
    if( _checkPrintInvalid( valid, buff, n)) return;
    snprintf_P(buff, n,
        PSTR("%02u-%02u-20%02u %02u:%02u:%02u"),
            day, month, year, hour, minute, second);
}

//
// Converts to the set of bytes for injecting into the chip
// Note 1-Jan-2000 was Saturday (weekday 6), Sunday is 7
//
void DS3231_DateTime::getSetting( uint8_t *buff){
    buff[0] = _UInt8_To_BCD( second);
    buff[1] = _UInt8_To_BCD( minute);
    buff[2] = _UInt8_To_BCD( hour); // 24-hour mode forced
    buff[3] = weekday();    
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
// Counts dates since 1 Jan 2000: (1-Jan-2000 is assumed 0)
//
uint16_t DS3231_DateTime::daysSince2000_01_01(){
    uint16_t count = day;
    for( uint8_t y=0; y<year; y++){
        count += (y%4!=0 || y==100)? 365: 366;
        #ifdef __DEBUG
        Serial.print( "year: ");
        Serial.println( count);
        #endif
    }
    for( uint16_t m=1; m<month; m++){
        count += pgm_read_byte(_daysPerMonth + m - 1);
        if( m==2 && (year%4)==0 && year!=100) count += 1;
        #ifdef __DEBUG
        Serial.print( "month: ");
        Serial.println( count);
        #endif
    }
    return count-1;
}

//
// Finds the day of the week: 1-Jan-2000 was Saturday(6)
// Sunday is 7; the forums say the week in DS3231 starts at 1 and ends at 7
//
uint8_t DS3231_DateTime::weekday(){
    uint8_t wd = (uint8_t)(daysSince2000_01_01() % 7) + 6;
    if( wd >= 7) wd -= 7;
    return (wd == 0)? 7: wd;    
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
DS3231_Alarm::DS3231_Alarm( uint8_t c){
    code = c;
}
DS3231_Alarm::DS3231_Alarm( uint8_t *data, uint8_t c){
    code = c & 0x80; // alarm 1 or 2
    uint8_t *ptr = data;
    uint8_t shift = 7;
    uint8_t v;
    if( !code){ // alarm 1 has seconds
        v = *ptr++;
        code |= (v & 0x80) >> (shift--);
        second = _BCD_To_UInt8(v & 0x7F);
    }
    v = *ptr++;
    code |= (v & 0x80) >> (shift--);
    minute = _BCD_To_UInt8(v & 0x7F);
    v = *ptr++;
    code |= (v & 0x80) >> (shift--);
    hour = _BCD_To_24hr(v & 0x7F);
    v = *ptr++;
    code |= (v & 0xc0) >> (--shift);
    day = _BCD_To_UInt8(v & 0x3F);
    // if( code == ALARM2_WHM) day %= 7; // not sure why. This may produce zero
    //_checkValidity();
    valid = true;
}
DS3231_Alarm::DS3231_Alarm( const char *command_line){
    valid = false;
    if( strlen(command_line) < 9) return;
    if( *command_line < 0x30) return;
    uint8_t c = *command_line - 0x30;
    switch(c){
        case 0:
        case 1:
            day = _convertPair( command_line + 1);
        case 2:
            hour = _convertPair( command_line + 3);
        case 3:
            minute = _convertPair( command_line + 5);
            second = _convertPair( command_line + 7);
            break;
        case 4:
        case 5:
            day = _convertPair( command_line + 1);
        case 6:
            hour = _convertPair( command_line + 3);
            minute = _convertPair( command_line + 5);
            break;
        default:
            return;
    }
    code = pgm_read_byte(_DS3231alarms + c);
    Serial.print("Code select: 0x");
    Serial.println(code, HEX);
    _checkValidity();
}

//
// Prints the content into buffer
//
void DS3231_Alarm::print( char *buff, byte n){
    if( _checkPrintInvalid( valid, buff, n)) return;
    snprintf_P(buff, n,
        PSTR("%02u%02u%02u%02u%02u"),
            code, day, hour, minute, second);
}
void DS3231_Alarm::printHI( char *buff, byte n){
    if( _checkPrintInvalid( valid, buff, n)) return;
    uint8_t c = isAlarm1()? 1: 2; 
    snprintf_P(buff, n,
        PSTR("ALR%1u: %02u %02u:%02u:%02u"),
            c, day, hour, minute, second);
}

//
// Converts to the set of bytes for injecting into the chip
//
void DS3231_Alarm::getSetting( uint8_t *buff){    
    uint8_t *ptr = buff;
    if( isAlarm1()) *ptr++ = _UInt8_To_BCD( second);
    *ptr++ = _UInt8_To_BCD( minute);
    *ptr++ = _UInt8_To_BCD( hour);
    *ptr = _UInt8_To_BCD( day);
    buff[0] |= (code & 0x01) << 7;
    buff[1] |= (code & 0x02) << 6;
    if( isAlarm1()){
        buff[2] |= (code & 0x04) << 5;
        buff[3] |= (code & 0x18) << 3;
        return;
    }
    buff[2] |= (code & 0x0C) << 4;
}

//
// Performs date validity check
//
void DS3231_Alarm::_checkValidity(){
    valid = false;
    if( day == 0) return;
    if( day > 31) return;
    if( hour > 23) return;
    if( minute > 59) return;
    if( second > 59) return;
    valid = true;
}
