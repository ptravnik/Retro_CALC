//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "CP1251_mod.h" 
#include "RTCData.hpp"

//#define _DEBUG_

const uint8_t _daysPerMonth[] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31};

//
// Converts the temperature representation
// The temperature is read as a 10-bit integer:
// 8 bits in the high byte is the sighed units of C
// Two upper 2 bits in the lower byte is a fraction in 0.25C steps
//
DS3231_Temperature::DS3231_Temperature( uint8_t hB, uint8_t lB){
    _wholeC = hB;
    _fracC = ((lB>>6) & 0x03) * 25;
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
        PSTR("%+02d.%02d"), _wholeC, _fracC);
}

///////////////////////////////////////////////////

//
// Converts the date time representation
//
DS3231_DateTime::DS3231_DateTime(){
}
DS3231_DateTime::DS3231_DateTime( uint8_t *data){
    valid = true;
    second = _BCD_To_UInt8(data[0]);
    minute = _BCD_To_UInt8(data[1]);
    hour = _BCD_To_24hr(data[2]);
    day = _BCD_To_UInt8(data[4]); // skip weekday
    month = _BCD_To_UInt8(data[5] & 0x7F); // drop wrap indicator
    year = _BCD_To_UInt8(data[6]) + 2000;
    if( data[5] & 0x80) year += 100; // century wrap
}
DS3231_DateTime::DS3231_DateTime( const char *command_line){
    valid = true;
    year = (uint16_t)_ConvertPair( command_line);
    year *= 100; 
    year += (uint16_t)_ConvertPair( command_line + 2);
    month = _ConvertPair( command_line + 5);
    day = _ConvertPair( command_line + 8);
    hour = _ConvertPair( command_line + 11);
    minute = _ConvertPair( command_line + 14);
    second = _ConvertPair( command_line + 17);
}

//
// Prints the content into buffer
//
void DS3231_DateTime::print( char *buff, byte n){
    if( !valid){
      snprintf_P(buff, n, PSTR("INVALID"));
      return;
    }
    snprintf_P(buff, n,
        PSTR("%04u-%02u-%02u %02u:%02u:%02u"),
        year, month, day, hour, minute, second);
}

//
// Converts to the set of bytes for injecting into the chip
// Note 1-Jan-2000 was Saturday (weekday 6), Sunday is 7
//
void DS3231_DateTime::getSetting( uint8_t *buff){
    buff[0] = _UInt8_To_BCD( second);
    buff[1] = _UInt8_To_BCD( minute);
    buff[2] = _UInt8_To_BCD( hour); // 24-hour mode
    buff[3] = (uint8_t)(daysSince2000_01_01() % 7) + 6;
    if( buff[3] >= 7) buff[3] -= 7;
    if( buff[3] == 0) buff[3] = 7;    
    buff[4] = _UInt8_To_BCD( day);
    buff[5] = _UInt8_To_BCD( month);
    buff[6] = _UInt8_To_BCD( (uint8_t)(year-2000));
    if( buff[6] < 100) return;
    buff[6] -= 100;
    buff[5] |= 0x80; // century flag
}

//
// Counts dates since 1 Jan 2000
//
uint16_t DS3231_DateTime::daysSince2000_01_01(){
    uint16_t count = day;
    for( uint16_t y=2000; y<year; y++){
        count += (y%4!=0)? 365: 366;
        #ifdef _DEBUG_
        Serial.print( "year: ");
        Serial.println( count);
        #endif
    }
    for( uint16_t m=1; m<month; m++){
        count += pgm_read_byte(_daysPerMonth + m - 1);
        if( m==2 && (year%4)==0) count += 1;
        #ifdef _DEBUG_
        Serial.print( "month: ");
        Serial.println( count);
        #endif
    }
    return count-1;
}

//
// Converts a pair of chars into a decimal 
//
uint8_t DS3231_DateTime::_ConvertPair( const char *ptr){
    if( *ptr < 0x30) return _NUL_;
    uint8_t tmp1 = (uint8_t)(*ptr++) - 0x30;
    if( tmp1 > 9) return _NUL_;
    if( *ptr < 0x30) return _NUL_;
    uint8_t tmp2 = (uint8_t)(*ptr) - 0x30;
    if( tmp2 > 9) return _NUL_;
    return tmp1*10 + tmp2;
};

//
// Binary code to 24-hour clock
// 
uint8_t DS3231_DateTime::_BCD_To_24hr(uint8_t val){
    // Small values - simple conversion
    if (!(val & 0x40)) return _BCD_To_UInt8(val);
    
    // 12 hour mode, convert to 24
    bool isPm = ((val & 0x20) != 0);
    uint8_t hour = _BCD_To_UInt8(val & 0x1f);
    if (isPm) hour += 12;
    return hour;
}

///////////////////////////////////////////////////

//
// Checks if clock running 
//
bool DS3231_Data::isRunning(){
    uint8_t creg = _getRegister(DS3231_REG_CONTROL) & DS3231_EOSC_MASK;
    return (!creg) && (!lastError);
}

//
// Checks time validity 
//
bool DS3231_Data::isDateTimeValid(){
    uint8_t rv = _getRegister(DS3231_REG_STATUS);
    if( lastError) return false;
    if( rv & DS3231_OSF_MASK) lastError = 5;
    return lastError == 0; 
}

//
// Sets 7 bytes for date and time
//
void DS3231_Data::setDateTime( DS3231_DateTime datetime){
    if(!datetime.valid) return;
    
    // Clear invalid datetime flag
    _clearFlag( DS3231_REG_STATUS, DS3231_OSF_MASK);

    // Now setting data
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write( DS3231_REG_DATETIME);

    uint8_t data[7];
    datetime.getSetting( data);
    Serial.println( data[3]);
    for( uint8_t i=0; i<7; i++) Wire.write( data[i]);
    
    lastError = Wire.endTransmission();
}

//
// Reads date and time as 7 bytes
//
DS3231_DateTime DS3231_Data::getDateTime(){
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write(DS3231_REG_DATETIME);
    lastError = Wire.endTransmission();
    if ( lastError )
        return DS3231_DateTime();
    if ( Wire.requestFrom(DS3231_SLAVE_ADDRESS, 7) != 7){
        lastError = 4;
        return DS3231_DateTime();
    }
    uint8_t data[7];
    for( uint8_t i=0; i<7; i++) data[i] = Wire.read();
    return DS3231_DateTime( data);
}

//
// Reads temperature as 2 bytes
//
DS3231_Temperature DS3231_Data::getTemperature(){
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write(DS3231_REG_TEMP);
    lastError = Wire.endTransmission();
    if ( lastError )
        return DS3231_Temperature(-50,0);
    if ( Wire.requestFrom(DS3231_SLAVE_ADDRESS, 2) != 2){
        lastError = 4;
        return DS3231_Temperature(-50,0);
    }
    int8_t hB = Wire.read();
    int8_t lB = Wire.read();
    return DS3231_Temperature(hB,lB);
}

//
// Stops and starts square wave generator
//
void DS3231_Data::setSQW( uint8_t mode, bool enableOnBattery){
    uint8_t creg = _getRegister(DS3231_REG_CONTROL);

    // clear all bits and enable INTCN (to disable clock SQW)
    creg &= ~(DS3231_AIE_MASK | DS3231_BBSQW_MASK);
    creg |= DS3231_INTCN_MASK;
    
    switch(mode){
        case DS3231_SWC_ALR1:
            creg &= ~DS3231_A1IE_MASK;
            break;
        case DS3231_SWC_ALR2:
            creg &= ~DS3231_A2IE_MASK;
            break;
        case DS3231_SWC_ALRB:
            creg &= ~DS3231_AIE_MASK;
            break;
        case DS3231_SWC_CLOCK:
            creg &= ~DS3231_INTCN_MASK;
            break;
        default:
            _setRegister(DS3231_REG_CONTROL, creg);
            return;
    }
    if (enableOnBattery) creg |= DS3231_BBSQW_MASK;
    _setRegister(DS3231_REG_CONTROL, creg);
}

//
// Sets square-wave frequency
//
void DS3231_Data::setSQWFrequency( uint8_t freq){
    uint8_t creg = _getRegister(DS3231_REG_CONTROL);
    creg &= ~DS3231_RS_MASK;
    creg |= (freq & DS3231_RS_MASK);
    _setRegister(DS3231_REG_CONTROL, creg);
}

//
// Low-level get and set via 2-wire interface
//
void DS3231_Data::_setFlag( uint8_t Register, uint8_t Mask)
{
    uint8_t rv = _getRegister(Register);
    if( lastError) return; // cannot communicate
    if( rv & Mask) return; // already running
    _setRegister(Register, rv & (~Mask));
}
void DS3231_Data::_clearFlag(uint8_t Register, uint8_t Mask)
{
    uint8_t rv = _getRegister(Register);
    if( lastError) return; // cannot communicate
    if( !(rv & Mask)) return; // already stopped
    _setRegister(Register, rv | Mask);
}
bool DS3231_Data::_getRegister(uint8_t address){
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write(address);
    lastError = Wire.endTransmission();
    if ( lastError) return 0;
    if( Wire.requestFrom(DS3231_SLAVE_ADDRESS, 1) != 1){
        lastError = 4;
        return 0;
    }
    return Wire.read();
}
uint8_t DS3231_Data::_setRegister(uint8_t address, uint8_t value){
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write(address);
    Wire.write(value);
    lastError = Wire.endTransmission();
    return lastError;
}
