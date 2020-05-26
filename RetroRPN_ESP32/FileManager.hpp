//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "./src/LCDManager.hpp"
#include "./src/SDManager.hpp"

class FileManager{
  public:
    byte nextUI = UI_UNDEFINED;
    unsigned long init(void *components[]);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
  private:
    byte *_io_buffer;
    IOManager *_iom;
    LCDManager *_lcd;
    SDManager *_sdm;
    ExpressionParser *_epar;
};

#endif // FILEMANAGER_HPP
