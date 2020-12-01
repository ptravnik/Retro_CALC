//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef RTCDATA_HPP
#define RTCDATA_HPP

#include <Arduino.h>

//
// Alarm 1 codes:
// A1M4 DY/DT A1M3 A1M2 A1M1
// ALARM1_OPS and ALARM1_S are not used in this hardware 
//
#define ALARM1_DHMS (uint8_t)0x00
#define ALARM1_OPS  (uint8_t)0x17
#define ALARM1_S    (uint8_t)0x16
#define ALARM1_MS   (uint8_t)0x14
#define ALARM1_HMS  (uint8_t)0x10
#define ALARM1_WHMS (uint8_t)0x08

//
// Alarm 2 codes:
// A2M4 DY/DT A2M3 A2M2
// Additonal top bit indicates that the code belongs to Alarm2
// ALARM2_OPM and ALARM2_M are not used in this hardware 
//
#define ALARM2_DHM  (uint8_t)0x80
#define ALARM2_OPM  (uint8_t)0x8b
#define ALARM2_M    (uint8_t)0x8a
#define ALARM2_HM   (uint8_t)0x88
#define ALARM2_WHM  (uint8_t)0x84

class DS3231_Temperature{
  public:
    DS3231_Temperature( uint8_t *data);
    int16_t tempC_hundreds();
    inline float tempC(){
      return (float)tempC_hundreds() * 0.01f;};
    inline float tempF(){
      return (float)tempC_hundreds() * 0.018f + 32.0f;};
    void print( char *buff, byte n);
    void printHI( char *buff, byte n);
  private:
    int8_t _wholeC = 0;
    int8_t _fracC = 0;
};

class DS3231_DateTime{
  public:
    bool valid = false;
    uint8_t year = 0; // presume +2000
    uint8_t month = 1;
    uint8_t day = 1;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
    DS3231_DateTime();
    DS3231_DateTime( uint8_t *data);
    DS3231_DateTime( const char *command_line);
    void print( char *buff, byte n);
    void printHI( char *buff, byte n);
    void getSetting( uint8_t *buff);
    uint16_t daysSince2000_01_01();
    uint8_t weekday();
  private:
    void _checkValidity();
};

class DS3231_Alarm{
  public:
    bool valid = false;
    uint8_t code = ALARM1_DHMS; // alarm code includes the alarm number
    uint8_t day = 1; // day of the week or day of the month
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0; // not used for Alarm 2
    DS3231_Alarm( uint8_t c);
    DS3231_Alarm( uint8_t *data, uint8_t c);
    DS3231_Alarm( const char *command_line);
    void print( char *buff, byte n);
    void printHI( char *buff, byte n);
    void getSetting( uint8_t *buff);
    inline bool isAlarm1(){ return (code & 0x80) == 0;};
  private:
    void _checkValidity();
};

#endif
