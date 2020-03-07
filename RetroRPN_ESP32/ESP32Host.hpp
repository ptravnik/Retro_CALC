//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

//
// Pin assignment:
//
// 01 - TX0
// 03 - RX0
// 16 - RX2 (serial to Pro Micro 8) Note Pro Micro 14, 15, 16 are SPI/burn
// 17 - TX2 (serial to Pro Micro 9)
// 25 - KBD CLK
// 26 - KBD RST
// 27 - KBD_LED CLK
// 32 - KBD_LED RST
// 33 - KBD DATA
// 34 - POWER ON/OFF
// 35 - (input only) Pro Micro active

#ifndef ESP32HOST_HPP
#define ESP32HOST_HPP

#define POWER_DETECT_PIN 34
#define SCREEN_LED_PERIOD  60000
#define SCREEN_OFF_PERIOD 180000
#define POWER_OFF_PERIOD  720000

#include <Arduino.h>
#include "Utilities.hpp"
#include "IOManager.hpp"
#include "LCDManager.hpp"
#include "SDManager.hpp"
#include "RPNCalculator.hpp"

#define IO_MSG_POWER_BUTTON 4
#define IO_MSG_INACTIVE 5
#define IO_MSG_SHUTDOWN 6

class ESP32Host{
  public:
    unsigned long init();
    unsigned long tick();
    void deepSleep( byte msg = IO_MSG_SHUTDOWN);  
  private:
    void checkSleepPin();
    inline void waitUntilSleepPinHigh(){
      while( !digitalRead(POWER_DETECT_PIN))
        delay(5);
    };
};

#endif // ESP32HOST_HPP
