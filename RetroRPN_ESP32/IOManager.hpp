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
// 25 - KBD CLK
// 26 - KBD RST
// 27 - LED CLK
// 32 - LED RST
// 33 - KBD DATA
//

#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#define HWKBD_RST 26
#define HWKBD_CLK 25
#define HWKBD_DATA 33
#define HWKBDLED_RST 32
#define HWKBDLED_CLK 27

#include <Arduino.h>
#include "HWKbd.h"
#include "Cyrillic_Phonetic_CP1251.h"

class IOManager{
  public:
    void init();
    void setKBDLEDs( bool L0=false, bool L1=false, bool L2=false, bool L3=false);
    void setKBDLEDs( byte val);
    char input();
    void sendToSerials( char *unicodeBuff, byte *message, bool cr=true);
    inline void sendToSerials( char *unicodeBuff){
      sendToSerials( unicodeBuff, NULL, false);
      };
};

#endif // IOMANAGER_HPP
