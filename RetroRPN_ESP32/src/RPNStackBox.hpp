//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  RPNStackBox.hpp 
  Implements the display portion of the RPN calculator
*/

#ifndef RPNSTACKBOX_HPP
#define RPNSTACKBOX_HPP

#include <Arduino.h>
#include "Keywords.hpp"
#include "IOManager.hpp"
#include "LCDManager.hpp"
#include "Parser.hpp"

#define INPUT_COLS    256
#define INPUT_LIMIT   255
#define HSCROLL_LIMIT  18

class RPNStackBox{
  public:
    unsigned long init(void *components[]);
    void show();
    void redraw();
    void updateIOM( bool refresh=true);
    void resetRPNLabels();
    void setRPNLabel( byte label, byte *message);
    inline void setRPNLabel( byte label, char *message){
      setRPNLabel( label, (byte *)message);
    };
    inline void setRPNLabel( byte label, const char *message){
      setRPNLabel( label, (byte *)message);
    };
    inline void setStackRedraw(){
      memset( _stackRedrawRequired, true, 3);
    };
    inline void setMessageRedraw(){
      memset( _messageRedrawRequired, true, 3);
    };
    inline void setStackRedrawRequired( byte n=0){
      _stackRedrawRequired[ n] = true;
    };
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Variables *_vars;
    LCDManager *_lcd;
    ExpressionParser *_ep;
    bool _messageRedrawRequired[ 3];
    bool _stackRedrawRequired[ 3];
    byte *_messages[ 3];
};

#endif //RPNSTACKBOX_HPP
