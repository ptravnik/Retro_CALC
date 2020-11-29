//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

//
// It is presumed that DS3231 is connected via hardware interface
// On Arduino Micro Pro:
// SDA is pin 2
// SCL is pin 3
// The calls are to the standard Wire object from Wire.h - no need to complicate things
//

#ifndef RTCDATA_HPP
#define RTCDATA_HPP

#include <Arduino.h>
#include <Wire.h>

//I2C Slave Address  
#define DS3231_SLAVE_ADDRESS  0x68

//DS3231 Data Registers  
#define DS3231_REG_DATETIME   0x00
#define DS3231_REG_ALARMONE   0x07
#define DS3231_REG_ALARMTWO   0x0B
#define DS3231_REG_CONTROL    0x0E
#define DS3231_REG_STATUS     0x0F
#define DS3231_REG_AGING      0x10
#define DS3231_REG_TEMP       0x11

// DS3231 Control Register Bits
#define DS3231_A1IE_MASK      0x01
#define DS3231_A2IE_MASK      0x02
#define DS3231_AIE_MASK       0x03
#define DS3231_INTCN_MASK     0x04
#define DS3231_RS1_MASK       0x08
#define DS3231_RS2_MASK       0x10
#define DS3231_RS_MASK        0x18
#define DS3231_CONV_MASK      0x20
#define DS3231_BBSQW_MASK     0x40
#define DS3231_EOSC_MASK      0x80

// DS3231 Status Register Bits
#define DS3231_A1F_MASK       0x01
#define DS3231_A2F_MASK       0x02
#define DS3231_AIF_MASK       0x03
#define DS3231_BSY_MASK       0x04
#define DS3231_EN32KHZ_MASK   0x08
#define DS3231_OSF_MASK       0x80

// DS3231 Square Wave Clock Rate
#define DS3231_SWC_1HZ        0x00 
#define DS3231_SWC_1KHZ       0x08
#define DS3231_SWC_4KHZ       0x10
#define DS3231_SWC_8KHZ       0x18

// DS3231 Square Wave Clock Mode
#define DS3231_SWC_NONE       0x00 
#define DS3231_SWC_ALR1       0x01
#define DS3231_SWC_ALR2       0x02
#define DS3231_SWC_ALRB       0x03
#define DS3231_SWC_CLOCK      0x04

class DS3231_Temperature{
  public:
    DS3231_Temperature( uint8_t hB, uint8_t lB);
    int16_t tempC_hundreds();
    inline float tempC(){
      return (float)tempC_hundreds() * 0.01f;};
    inline float tempF(){
      return (float)tempC_hundreds() * 0.018f + 32.0f;};
    void print( char *buff, byte n);
  private:
    int8_t _wholeC = 0;
    int8_t _fracC = 0;
};

class DS3231_DateTime{
  public:
    bool valid = false;
    uint16_t year = 2000;
    uint8_t month = 1;
    uint8_t day = 1;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
    DS3231_DateTime();
    DS3231_DateTime( uint8_t *data);
    DS3231_DateTime( const char *command_line);
    void print( char *buff, byte n);
    void getSetting( uint8_t *buff);
    uint16_t daysSince2000_01_01();
  private:
    inline uint8_t _BCD_To_UInt8(uint8_t val){
        return val - 6 * (val >> 4);};
    inline uint8_t _UInt8_To_BCD(uint8_t val){
        return val + 6 * (val / 10);};
    uint8_t _BCD_To_24hr(uint8_t val);
    uint8_t _ConvertPair( const char *ptr);
};

class DS3231_Data{
  public:
    uint8_t lastError = 0;

    inline init(){ Wire.begin();};
    bool isRunning();
    bool isDateTimeValid();
    void setDateTime( DS3231_DateTime datetime);
    DS3231_DateTime getDateTime();
    DS3231_Temperature getTemperature();
    
    inline void startClock(){
        _setFlag( DS3231_REG_CONTROL, DS3231_EOSC_MASK);};
    inline void stopClock(){
        _clearFlag( DS3231_REG_CONTROL, DS3231_EOSC_MASK);};
    inline void start32kHz(){
        _setFlag( DS3231_REG_STATUS, DS3231_EN32KHZ_MASK);};
    inline void stop32kHz(){
        _clearFlag( DS3231_REG_STATUS, DS3231_EN32KHZ_MASK);};
    void setSQW( uint8_t mode, bool enableOnBattery = true);
    void setSQWFrequency( uint8_t freq);

  private:
    bool _getRegister(uint8_t address);
    uint8_t _setRegister(uint8_t address, uint8_t value);
    void _setFlag( uint8_t Register, uint8_t Mask);
    void _clearFlag( uint8_t Register, uint8_t Mask);
};

#endif
