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
#define SCREEN_LED_PERIOD 240000
#define SCREEN_OFF_PERIOD 480000
#define POWER_OFF_PERIOD  720000

#include "./src/Keywords.hpp"
#include "./src/IOManager.hpp"
#include "./src/LCDManager.hpp"
#include "./src/MessageBox.hpp"
#include "./src/CommandLine.hpp"
#include "./src/RPNStackBox.hpp"
#include "TerminalBox.hpp"
#include "SDManager.hpp"
#include "RPNCalculator.hpp"
#include "BasicConsole.hpp"
#include "FileManager.hpp"

#define IO_MSG_POWER_BUTTON 0
#define IO_MSG_INACTIVE     1
#define IO_MSG_SHUTDOWN     2
#define IO_BUFFER_LENGTH    256

class ESP32Host{
  public:
    byte currentUI = UI_UNDEFINED;
    unsigned long init();
    unsigned long tick();
    void selectUI(byte ui); 
    void selectUI(byte *ui); 
    void show();
    void redraw();
    void deepSleep( byte msg = IO_MSG_SHUTDOWN);   
  private:
    byte _io_buffer[IO_BUFFER_LENGTH];
    void *_host_Components[UI_COMPONENTS_COUNT];
    void _checkSleepPin();
    inline void _waitUntilSleepPinHigh(){
      while( !digitalRead(POWER_DETECT_PIN))
        delay(5);
    };
};

#endif // ESP32HOST_HPP
