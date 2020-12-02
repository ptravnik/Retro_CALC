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
// Mandatory start
//
void DS3231_Controller::begin(){
    Wire.begin();
    wakeupStatus = _getRegister( DS3231_REG_STATUS);
    _setRegister( DS3231_REG_CONTROL, DS3231_CONTROL_DEF); // Alarms off
    _setRegister( DS3231_REG_STATUS, DS3231_STATUS_DEF);
}

//
// Checks if clock running 
//
bool DS3231_Controller::isRunning(){
    uint8_t creg = _getRegister(DS3231_REG_CONTROL) & DS3231_EOSC_MASK;
    return (!creg) && (!lastError);
}

//
// Sets 7 bytes for date and time
//
void DS3231_Controller::setDateTime( DS3231_DateTime datetime){
    if(!datetime.valid) return;
    uint8_t data[7];
    datetime.getSetting( data);
    _setRegister( DS3231_REG_STATUS, DS3231_STATUS_DEF);
    _setRegister( DS3231_REG_DATETIME, 7, data);
}

//
// Reads date and time as 7 bytes
//
DS3231_DateTime DS3231_Controller::getDateTime(){
    uint8_t data[8];
    if( firstRun){
        firstRun = false;
        data[7] = wakeupStatus;
    }
    else{
        data[7] = _getRegister(DS3231_REG_STATUS);
    }
    if( !_getRegister(DS3231_REG_DATETIME, 7, data))
        return DS3231_DateTime();
    return DS3231_DateTime(data);
}

//
// Reads temperature as 2 bytes
//
DS3231_Temperature DS3231_Controller::getTemperature(){
    int8_t data[2] = {-50, 0};
    _getRegister(DS3231_REG_TEMP, 2, (uint8_t *)data);
    return DS3231_Temperature( data);
}

//
// Set alarm data
//
void DS3231_Controller::setWakeUp(DS3231_Alarm alr){
    if(!alr.valid) return;
    uint8_t data[4];
    alr.getSetting( data);
    _setRegister( DS3231_REG_CONTROL, DS3231_CONTROL_ALR);
    _setRegister( DS3231_REG_STATUS, DS3231_STATUS_DEF);
    _setRegister( DS3231_REG_ALARMONE, 4, data);
}

//
// Extract alarm data
//
DS3231_Alarm DS3231_Controller::getWakeUp(){
    uint8_t data[4];
    if( !_getRegister(DS3231_REG_ALARMONE, 4, data))
        return DS3231_Alarm();
    return DS3231_Alarm( data);
}

//
// Low-level get and set via 2-wire interface
//
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
uint8_t DS3231_Controller::_setRegister(uint8_t address, uint8_t n, uint8_t *data){
    Wire.beginTransmission(DS3231_SLAVE_ADDRESS);
    Wire.write(address);
    if( data == NULL){
        Wire.write( n);
    }
    else{
        for( uint8_t i=0; i<n; i++) Wire.write( data[i]);
    }
    lastError = Wire.endTransmission();
    return lastError;
}
