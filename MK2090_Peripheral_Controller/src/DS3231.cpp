//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "CP1251_mod.h" 
#include "DS3231.hpp"

//#define __DEBUG

//
// if set, the delays are at Micro Pro
// if not set, the host ESP32 takes care not to call too frequently
// 
//#define __DELAYS_ON_SLAVE

//
// Mandatory start
//
void DS3231_Controller::begin(){
    Wire.begin();
    delay(10); // allow the comm to start

    // store the rason for microprocessor start-up (0-power button, 1,2 - alarms, 3 - both alarms)
    latchAlarms();
    _setRegister( DS3231_REG_CONTROL, DS3231_CONTROL_DEF);

    // All these manipulations are not needed - basically set by _setRegister, above

    // // try to get time and check validity
    // if( !isDateTimeValid()){
    //     if(lastError != 5) return; // communication failure
    //     setDateTime(DS3231_DateTime()); // set the clock to the BigBang
    // }

    // make sure the clock is running 
    // startClock();

    // As a security precaution, never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    // stop32kHz();
    // setSQW( DS3231_SWC_ALRB);
}

//
// Checks if clock running 
//
bool DS3231_Controller::isRunning(){
    uint8_t creg = _getRegister(DS3231_REG_CONTROL) & DS3231_EOSC_MASK;
    return (!creg) && (!lastError);
}

//
// Checks time validity 
//
bool DS3231_Controller::isDateTimeValid(){
    uint8_t rv = _getRegister(DS3231_REG_STATUS);
    if( lastError) return false;
    Serial.print(rv, HEX);
    //if( rv & DS3231_OSF_MASK != 0) lastError = 5;
    return lastError == 0;
}

//
// Sets 7 bytes for date and time
//
void DS3231_Controller::setDateTime( DS3231_DateTime datetime){
    if(!datetime.valid) return;
    
    // Clear invalid datetime flag (nominal delay is 100)
    _clearFlag( DS3231_REG_STATUS, DS3231_OSF_MASK, 100);

    // Now setting data
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write( DS3231_REG_DATETIME);

    uint8_t data[7];
    datetime.getSetting( data);

    #ifdef __DEBUG
    Serial.println( "Dataset: ");
    for( uint8_t i=0; i<7; i++){
        Serial.println( data[i]);
    }
    #endif

    for( uint8_t i=0; i<7; i++) Wire.write( data[i]);    
    lastError = Wire.endTransmission();

    #ifdef __DEBUG
    Serial.print( "Last Error: ");
    Serial.println( lastError);
    #endif

    // after setting time, at least 250 ms must elapse
    #ifdef __DELAYS_ON_SLAVE
    delay(250);
    #endif
}

//
// Reads date and time as 7 bytes
//
DS3231_DateTime DS3231_Controller::getDateTime(){
    uint8_t data[7];
    if( !_getRegister(DS3231_REG_DATETIME, 7, data))
        return DS3231_DateTime();
    return DS3231_DateTime(data);
}

//
// Reads temperature as 2 bytes
//
DS3231_Temperature DS3231_Controller::getTemperature(){
    uint8_t data[2] = {-50, 0};
    _getRegister(DS3231_REG_TEMP, 2, data);
    return DS3231_Temperature( data);
}

//
// Stops and starts square wave generator
//
void DS3231_Controller::setSQW( uint8_t mode, bool enableOnBattery){
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
void DS3231_Controller::setSQWFrequency( uint8_t freq){
    uint8_t creg = _getRegister(DS3231_REG_CONTROL);
    creg &= ~DS3231_RS_MASK;
    creg |= (freq & DS3231_RS_MASK);
    _setRegister(DS3231_REG_CONTROL, creg);
}

//
// Set alarm data
//
void DS3231_Controller::setAlarm(DS3231_Alarm alr){
    if(!alr.valid) return;
    uint8_t n = 3;
    uint8_t r = DS3231_REG_ALARMTWO;
    if (alr.isAlarm1()){
        n = 4;
        r = DS3231_REG_ALARMONE;
    }
    uint8_t data[4];
    alr.getSetting( data);

    // Now setting data
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write( r);

    //#ifdef __DEBUG
    Serial.println( "Dataset: ");
    for( uint8_t i=0; i<4; i++){
        Serial.println( data[i]);
    }
    //#endif

    for( uint8_t i=0; i<n; i++) Wire.write( data[i]);    
    lastError = Wire.endTransmission();

    //#ifdef __DEBUG
    Serial.print( "Last Error: ");
    Serial.println( lastError);
    //#endif
}

//
// Extract alarm data
//
DS3231_Alarm DS3231_Controller::getAlarm1(){
    uint8_t data[4];
    if( !_getRegister(DS3231_REG_ALARMONE, 4, data))
        return DS3231_Alarm(ALARM1_DHMS);
    return DS3231_Alarm( data, ALARM1_DHMS);
}
DS3231_Alarm DS3231_Controller::getAlarm2(){
    uint8_t data[3];
    if( !_getRegister(DS3231_REG_ALARMTWO, 3, data))
        return DS3231_Alarm(ALARM2_DHM);
    return DS3231_Alarm( data, ALARM2_DHM);
}
void DS3231_Controller::latchAlarms(){    
    lastAlarms = _getRegister( DS3231_REG_STATUS) & DS3231_AIF_MASK;   
    _clearFlag( DS3231_REG_STATUS, DS3231_AIF_MASK);
}

//
// Low-level get and set via 2-wire interface
//
void DS3231_Controller::_setFlag( uint8_t Register, uint8_t Mask, uint16_t d)
{
    #ifdef __DEBUG 
    Serial.print( "Set flag: 0x");
    Serial.print( Register, HEX);
    Serial.print( " / 0x");
    Serial.println( Mask, HEX);
    #endif

    uint8_t rv = _getRegister(Register);
    if( lastError) return; // cannot communicate

    #ifdef __DEBUG 
    Serial.print( "rv : 0x");
    Serial.println( rv, HEX);
    #endif

    if( rv & Mask != 0) return; // already set

    #ifdef __DEBUG 
    Serial.print( "Force: 0x");
    Serial.println( rv | Mask, HEX);
    #endif

    _setRegister(Register, rv | Mask);
    if( d) delay( d);
}
void DS3231_Controller::_clearFlag(uint8_t Register, uint8_t Mask, uint16_t d)
{
    #ifdef __DEBUG 
    Serial.print( "Clear flag: 0x");
    Serial.print( Register, HEX);
    Serial.print( " / 0x");
    Serial.println( Mask, HEX);
    #endif

    uint8_t rv = _getRegister(Register);
    if( lastError) return; // cannot communicate

    #ifdef __DEBUG 
    Serial.print( "rv : 0x");
    Serial.println( rv, HEX);
    #endif

    if( rv & Mask == 0) return; // already cleared

    #ifdef __DEBUG 
    Serial.print( "Force: 0x");
    Serial.println( (rv & (~Mask)), HEX);
    #endif

    _setRegister(Register, (rv & (~Mask)));
    if( d) delay( d);
}
uint8_t DS3231_Controller::_getRegister(uint8_t address, uint8_t n, uint8_t *data){
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write(address);
    lastError = Wire.endTransmission();
    if ( lastError) return 0;
    uint8_t k = (uint8_t)Wire.requestFrom(DS3231_SLAVE_ADDRESS, (int)n);
    if( k != n){
        lastError = 4;
        return 0;
    }
    if( n<=1) return Wire.read();
    for( uint8_t i=0; i<n; i++) data[i] = Wire.read();
    return n;
}
uint8_t DS3231_Controller::_setRegister(uint8_t address, uint8_t value){
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write(address);
    Wire.write(value);
    lastError = Wire.endTransmission();
    return lastError;
}
