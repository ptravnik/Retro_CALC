/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _LEXER_HPP
#define _LEXER_HPP

#include "ProgramCode.hpp"
#include "RPNStackBox.hpp"
#include "MessageBox.hpp"
#include "CommandLine.hpp"
#include "SDManager.hpp"

class Lexer{
  public:
    Keyword *lastKeyword = NULL;
    VariableToken lastVariable = 0;
    byte result = _RESULT_UNDEFINED_;
    void init( void *components[]);
    byte *parse( byte *str);

    // Operator declarations here
    bool operator_CLEAR();
    bool operator_CLEAR_Program();
    bool operator_CLEAR_Vars( bool constants);

    bool operator_CONST();

    bool operator_LET();

    bool operator_LIST();
    bool operator_LIST_Program();
    bool operator_LIST_Vars( bool constants);

    bool operator_LOAD();
    bool operator_LOAD_Program();
    bool operator_LOAD_Vars( bool constants);

    bool operator_REM();

    bool operator_STORE();
    bool operator_STORE_Vars( bool constants);

    inline byte *_getCurrentPosition(){
      return _lexer_position;
    };
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Keywords *_kwds;
    Variables *_vars;
    Functions *_funs;
    ExpressionParser *_epar;
    LCDManager *_lcd;
    SDManager *_sdm;
    RPNStackBox *_rsb;
    MessageBox *_mbox;
    CommandLine *_clb;

    bool _expression_error = false;
    byte *_lexer_position;
    void _parseOperator();
    bool _processKeyword();
    bool _processVariable( bool asConstant=false);
    bool _findAssignment();
    byte *_skipToEOL( byte *str);
    byte *_skipToNextOperator( byte *str);
    bool _validate_NextCharacter( byte c);
    inline void _ignore_Blanks(){
      while(IsSpacer(*_lexer_position))
        _lexer_position++;
    };
    inline bool _check_NextToken( byte c){
      if( *_lexer_position != c ) return false;
      _lexer_position++;
      return true;
    };
};

#endif // _LEXER_HPP
