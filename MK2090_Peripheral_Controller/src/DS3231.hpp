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

//
// This implements only the subset of functionality in order to save memory and ticks:
// * The weekday logic is handled at the ESP host (it is not necessary to keep it in the controller)
// * Only Alarm1 in DDHHMMSS mode is used - and it is only used for the system hardware wake-up
// * Square Wave generator is disabled, frequency is forced into 1Hz
// * 24-hour clock is used explicitly
//

#ifndef DS3231_HPP
#define DS3231_HPP

#include <Arduino.h>
#include <Wire.h>
#include "RTCData.hpp"

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

// Default control - battery oscillator ON and Alarm 1 ENABLED
#define DS3231_CONTROL_DEF    0x04
#define DS3231_CONTROL_ALR    0x05

// DS3231 Status Register Bits
#define DS3231_A1F_MASK       0x01
#define DS3231_A2F_MASK       0x02
#define DS3231_AIF_MASK       0x03
#define DS3231_BSY_MASK       0x04
#define DS3231_EN32KHZ_MASK   0x08
#define DS3231_OSF_MASK       0x80

// Default status - all clear
#define DS3231_STATUS_DEF    0x00

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

class DS3231_Controller{
  public:
    bool firstRun = true;
    uint8_t lastError = 0;
    uint8_t wakeupStatus = 0;

    void begin();
    bool isRunning();
    void setDateTime( DS3231_DateTime datetime);
    DS3231_DateTime getDateTime();
    DS3231_Temperature getTemperature();
    void setWakeUp(DS3231_Alarm alr);
    DS3231_Alarm getWakeUp();

  private:
    uint8_t _getRegister(uint8_t address, uint8_t n=1, uint8_t *data=NULL);
    uint8_t _setRegister(uint8_t address, uint8_t n, uint8_t *data=NULL);
};

#endif
