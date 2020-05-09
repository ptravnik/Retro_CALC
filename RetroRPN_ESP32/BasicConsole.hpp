//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
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

#ifndef BASICCONSOLE_HPP
#define BASICCONSOLE_HPP

#include <Arduino.h>
#include "./src/IOManager.hpp"
#include "./src/LCDManager.hpp"
#include "./src/CommandLine.hpp"
#include "./src/Utilities.hpp"
#include "RPNCalculator.hpp"
//#include "SDManager.hpp"
//#include "Parser.hpp"
//#include "Keywords.hpp"
//#include "MathFunctions.hpp"

#define INPUT_COLS    256
#define INPUT_LIMIT   255
#define HSCROLL_LIMIT  18
#define VIRTUAL_SCREEN_COLS  80
#define VIRTUAL_SCREEN_ROWS  40

// Move must be one less than RPN_STACK times sizeof( double)
#define RPN_MOVE   (RPN_STACK-1)*sizeof( double)
#define RPN_PI        3.14159265359

class BasicConsole{
  public:
    byte nextUI = UI_UNDEFINED;
    bool expectCommand = false;
    unsigned long init(IOManager *iom, LCDManager *lcd, SDManager *sd, ExpressionParser *ep, CommandLine *cl, RPNCalculator *rpn);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
    void processInput(bool silent=false);
    void push( bool refresh=true);
    void pop( bool refresh=true);
    void swap( bool refresh=true);
    void roll( bool refresh=true);
    void prev( bool refresh=true);
    void add( bool refresh=true);
    void subtract( bool refresh=true);
    void multiply( bool refresh=true);
    void divide( bool refresh=true);
    void power( bool refresh=true);
    void signchange( bool refresh=true);
    void quad( bool refresh=true);
    void goff2( bool refresh=true);
    void loadState();
    void saveState();
    inline void setStackRedraw(){
      memset( _stackRedrawRequired, true, 3);
    };
  private:
    byte *_io_buffer;
    IOManager *_iom;
    LCDManager *_lcd;
    SDManager *_sd;
    ExpressionParser *_ep;
    CommandLine *_cl;
    RPNCalculator *_rpn;
    byte _virtualScreen[VIRTUAL_SCREEN_COLS * VIRTUAL_SCREEN_ROWS];
    size_t _vs_bottom_row = 0;
    bool _stackRedrawRequired[ 3];
    bool _sdPrevMounted = false;
    void _checkSDStatus();
    void processCommand(byte c);
    void _updateIOM(bool refresh=true);
    void _evaluateCommand();
    void _evaluateString();
    void _checkTrigAccuracy();
    void _swapQuick();
    void _popPartial();
    void _popPartial( double v);
    void _pushQuick();
    void _pushQuick(double v);
    void _popQuick(byte start=1);
    inline void _savePrev( byte i=0){
      _ep->mathFunctions.previous_X = _ep->mathFunctions.rpnStack[i];
    };
    inline void _restorePrev(){
      _ep->mathFunctions.rpnStack[0] = _ep->mathFunctions.previous_X;
    };
    inline double _getSt( byte i){
      return _ep->mathFunctions.rpnStack[i];
    };
    inline void _setSt( byte i, double v){
      _ep->mathFunctions.rpnStack[i] = v;
    };
    inline bool _isStZero( byte i){
      return _ep->mathFunctions.rpnStack[i] == 0.0;
    };
    inline void _setRedrawAndUpdateIOM( bool refresh){    
      setStackRedraw();
      _updateIOM(refresh);
    };
    inline void _savePopAndUpdate( double v, bool refresh) {
      _savePrev();
      _popPartial(v);
      _updateIOM(refresh);
    };
};

#endif //BASICCONSOLE_HPP
