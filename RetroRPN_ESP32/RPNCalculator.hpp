//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  RPN_Calculator.hpp 
  Implements an RPN terminal for immediate BASIC execution

  Keyboard commands:
  1 - arrow left / arrow right - moves input cursor in the command line
  2 - DEL / Backspace - command line editor
  3 - arrow down - SWAP X and Y
  4 - arrow up - previous command line recall
  5 - LF - "silent" command line execution (stack operation follows)
  6 - CR = enter - command line execution
  7 - (+/-) (177) - sign change
*/

#ifndef RPNCALCULATOR_HPP
#define RPNCALCULATOR_HPP

#include <Arduino.h>
#include "LCDManager.hpp"
#include "IOManager.hpp"
#include "Keywords.hpp"

#define INPUT_COLS    256
#define INPUT_LIMIT   255
#define HSCROLL_LIMIT  18
#define RPN_STACK      20
// Move must be one less than RPN_STACK times sizeof( double)
#define RPN_MOVE   (RPN_STACK-1)*sizeof( double)

class RPNCalculator{
  public:
    bool expectCommand = false;
    double rpnStack[RPN_STACK];
    double previous_X = 0.0;
    void init( IOManager iom, LCDManager lcd);
    void show();
    void redraw();
    void sendChar( byte c);
    void processInput();
    void push( bool refresh=true);
    void pop( bool refresh=true);
    void swap( bool refresh=true);
    void roll( bool refresh=true);
    void prev( bool refresh=true);
    void add( bool refresh=true);
    void subtract( bool refresh=true);
    void multiply( bool refresh=true);
    void divide( bool refresh=true);
  private:
    IOManager _iom;
    LCDManager _lcd;
    byte _input[INPUT_COLS];
    byte _inputPrevious[INPUT_COLS];
    uint16_t cursor_column = 0;
    uint16_t display_starts = 0;
    byte _messageBuffer[SCR_COLS * 4];
    bool _messageRedrawRequired[ 4];
    bool _stackRedrawRequired[ 3];
    byte *_messages[4];
    bool _force_scientific = false;
    byte _precision = 8;
    void processCommand(byte c);
    void processEntry(byte c);
    void processDEL();
    void processBS();
    void processESC();
    void processLEFT();
    void processRIGHT();
    void processHOME();
    void processEND();
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
    inline void copyToPrevious(){
      strcpy( (char *)_inputPrevious, (char *)_input);
      };
    inline bool isInputEmpty(){
      return *_input == _NUL_;
    }
    void copyFromPrevious();
    void updateIOM(bool refresh=true);
    void _popPartial();
};

#endif //RPNCALCULATOR_HPP