//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <Arduino.h>
#include "./src/IOManager.hpp"
#include "./src/LCDManager.hpp"
#include "SDManager.hpp"
#include "Parser.hpp"
#include "Keywords.hpp"

class FileManager{
  public:
    byte nextUI = UI_UNDEFINED;
    unsigned long init(IOManager *iom, LCDManager *lcd, SDManager *sd, ExpressionParser *ep);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
  private:
    byte *_io_buffer;
    IOManager *_iom;
    LCDManager *_lcd;
    SDManager *_sd;
    ExpressionParser *_ep;
};

#endif // FILEMANAGER_HPP
