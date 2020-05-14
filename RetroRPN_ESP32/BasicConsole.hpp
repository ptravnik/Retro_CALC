//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  BasicConsole.hpp 
  Implements a console terminal for immediate BASIC execution

  Keyboard commands, additional to Command Line:
  1 - arrow down/up - move between the screen stack lines
  2 - CR = enter - command line execution
*/

#ifndef BASICCONSOLE_HPP
#define BASICCONSOLE_HPP

#include "RPNCalculator.hpp"
#include "TerminalBox.hpp"

class BasicConsole{
  public:
    byte nextUI = UI_UNDEFINED;
    bool expectCommand = false;
    unsigned long init(void *components[]);
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
    void updateIOM(bool refresh=true);
  private:
    byte *_io_buffer;
    IOManager *_iom;
    LCDManager *_lcd;
    SDManager *_sd;
    ExpressionParser *_ep;
    CommandLine *_cl;
    MessageBox *_mb;
    RPNCalculator *_rpn;
    TerminalBox *_trm;
    void processCommand(byte c);
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
      _rpn->setStackRedraw();
      updateIOM(refresh);
    };
    inline void _savePopAndUpdate( double v, bool refresh) {
      _savePrev();
      _popPartial(v);
      updateIOM(refresh);
    };
};

#endif //BASICCONSOLE_HPP
