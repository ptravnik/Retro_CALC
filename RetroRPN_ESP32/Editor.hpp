//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  Editor.hpp 
  Implements a BASIC text editor

  Keyboard commands, additional to Command Line:
  1 - arrow down/up - move between the code lines
  2 - page down/up - move between the code lines
  3 - CR = enter - command line execution
*/

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "RPNCalculator.hpp"
#include "./src/TerminalBox.hpp"

class Editor{
  public:
    unsigned long init(void *components[]);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
    void processInput(bool silent=false);
    void updateIOM(bool refresh=true);
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Variables *_vars;
    Functions *_funs;
    LCDManager *_lcd;
    SDManager *_sdm;
    ExpressionParser *_epar;
    Lexer *_lex;
    CommandLine *_clb;
    RPNStackBox *_rsb;
    MessageBox *_mbox;
    RPNCalculator *_rpn;
    TerminalBox *_trm;
    void _evaluateString();
};

#endif //EDITOR_HPP
