//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  MessageBox.hpp 
  Implements a simple message interface (inverted line above the command line)

  Keyboard commands:
  None
*/

#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include "Keywords.hpp"
#include "Variables.hpp"
#include "IOManager.hpp"
#include "LCDManager.hpp"

#define MB_MESSAGE_LCD_On  0
#define MB_MESSAGE_LCD_Off 1

class MessageBox{
  public:
    unsigned long init(void *components[]);
    void show();
    void redraw();
    void setLabel( byte *message, bool refresh=true);
    void appendLabel( byte *message, bool refresh=true);
    void updateIOM( bool refresh=true);
    void report( size_t message, bool refresh=true);
    void report_LCDBrightness( byte val, bool refresh=true);
    inline void setLabel( char *message, bool refresh=true){
      setLabel( (byte *)message, refresh);
    };
    inline void setLabel( const char *message, bool refresh=true){
      setLabel( (byte *)message, refresh);
    };
    inline void appendLabel( char *message, bool refresh=true){
      appendLabel( (byte *)message, refresh);
    };
    inline void appendLabel( const char *message, bool refresh=true){
      appendLabel( (byte *)message, refresh);
    };
    inline char *getMessageBuffer(){
      return (char *)_messageBuffer;
    };
  private:
    IOManager *_iom;
    Variables *_vars;
    LCDManager *_lcd;
    byte *_messageBuffer;
    bool _messageRedrawRequired = false;
};

#endif //MESSAGEBOX_HPP
