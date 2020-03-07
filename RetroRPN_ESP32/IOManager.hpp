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
// 35 - (input only) Pro Micro active
//

#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#define IO_RXD2 16
#define IO_TXD2 17
#define IO_PM_ACTIVE 35
#define HWKBD_CLK 25
#define HWKBD_RST 26
#define HWKBD_CLK 25
#define HWKBD_DATA 33
#define HWKBDLED_RST 32
#define HWKBDLED_CLK 27

#define SERIAL_HARD_BAUD_RATE 115200
#define SERIAL2_BAUD_RATE 19200

class IOManager{
  public:
    bool isAsleep = false;
    volatile bool HWKeyboardConnected = false;
    volatile unsigned long lastInput = 0;
    unsigned long init();
    unsigned long tick();
    void setKBDLEDs( bool L0=false, bool L1=false, bool L2=false, bool L3=false);
    void setKBDLEDs( byte val);
    void flashKBDLEDs();
    char input();
    void sendToSerials( char *unicodeBuff, byte *message, bool cr=true);
    inline void sendToSerials( char *unicodeBuff){
      sendToSerials( unicodeBuff, NULL, false);
      };
    inline void keepAwake(){
      lastInput = millis();
    };
    void sleepOn();
    void sleepOff();
};

#endif // IOMANAGER_HPP
