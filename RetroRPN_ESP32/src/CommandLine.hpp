//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  CommandLine.hpp 
  Implements an command line portion of the BASIC terminal

  Keyboard commands:
  1 - arrow left / arrow right - moves input cursor in the command line
  2 - DEL / Backspace - command line editor
  3 - arrow up - previous command line recall
*/

#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include "Keywords.hpp"
#include "IOManager.hpp"
#include "LCDManager.hpp"

class CommandLine{
  public:
    uint16_t cursor_column = 0;
    uint16_t display_starts = 0;
    unsigned long init(void *components[]);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
    byte *getInput( size_t offset=0){
      return _input + offset;
    };
    byte *getInputDisplay(){
      return _input + display_starts;
    };
    byte getInputChar( size_t offset=0){
      return _input[offset];
    };
    size_t getInputLength(){
      return strlen(_input);
    };
    inline void copyToPrevious(){
      strncpy( (char *)_inputPrevious, (char *)_input, INPUT_COLS);
    };
    inline bool isInputEmpty(){
      return *_input == _NUL_;
    };
    void copyFromPrevious();
    void clearInput();
    void updateIOM(bool refresh=true);
    void showPrompt();
    void processEntry(byte c);
    void processDEL();
    void processBS();
    void processESC();
    void processLEFT();
    void processRIGHT();
    void processHOME();
    void processEND();
    bool isMagnitudeEntry();
  private:
    byte *_io_buffer;
    IOManager *_iom;
    LCDManager *_lcd;
    byte _input[INPUT_COLS];
    byte _inputPrevious[INPUT_COLS];
};

#endif //COMMANDLINE_HPP
