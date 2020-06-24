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

  Keyboard commands, additional to Command Line:
  1 - arrow down - SWAP X and Y
  2 - LF - "silent" command line execution (stack operation follows)
  3 - CR = enter - command line execution
  4 - (+/-) (177) - sign change
*/

#ifndef RPNCALCULATOR_HPP
#define RPNCALCULATOR_HPP

#include "./src/Lexer.hpp"

#define HSCROLL_LIMIT  18

// Move must be one less than RPN_STACK times sizeof( double)
#define RPN_MOVE   (RPN_STACK-1)*sizeof( double)

class RPNCalculator{
  public:
    bool expectCommand = false;
    unsigned long init( void *components[]);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
    void processInput( bool silent=false);
    void updateIOM( bool refresh=true);
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Variables *_vars;
    Functions *_funs;
    ExpressionParser *_epar;
    Lexer *_lex;
    LCDManager *_lcd;
    SDManager *_sdm;
    RPNStackBox *_rsb;
    MessageBox *_mbox;
    CommandLine *_clb;
    void _processCommand(byte c);
    void _evaluateString();
};

#endif //RPNCALCULATOR_HPP
