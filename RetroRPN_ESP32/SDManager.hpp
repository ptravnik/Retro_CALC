//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

//
// Pin assignment:
//
// 04 - SD insert sensor (DET_A)
// 05 - VSPI CSO - SD Select (DAT3)
// 18 - VSPI CLK - SD Clock (CLK)
// 19 - VSPI MISO - SD out (DAT0)
// 23 - VSPI MOSI - SD command (CMD)
//

#ifndef SDMANAGER_HPP
#define SDMANAGER_HPP

#include <Arduino.h>
#include "./src/IOManager.hpp"
//#include "LCDManager.hpp"

// or use LED_BUILTIN=2 constant for built-in LED
#define SD_DETECT_PIN      4
#define SD_CSO             13
#define SD_CLOCK           18
#define SD_MISO            19
#define SD_MOSI            23
#define BOARD_LED_PIN      2

#define SD_CARD_OUT          255
#define SD_CARD_NOT_MOUNTED  254

class SDManager{
  public:
    volatile bool SDInserted = false;
    volatile bool SDMounted = false;
    volatile uint64_t cardSize = 0;

    unsigned long init(IOManager *iom);
    unsigned long tick();
    uint8_t cardType();
    inline unsigned long keepAwake(){
      return _iom->keepAwake();
    };
    void sleepOn();
    void sleepOff();
  private:
    byte *_io_buffer;
    IOManager *_iom;
    //LCDManager *_lcd;
    void _checkSDPin();
    bool _detectSDCard();
};

#endif // SDMANAGER_HPP
