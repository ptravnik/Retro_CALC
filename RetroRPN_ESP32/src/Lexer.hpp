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
    byte currentUI = UI_UNDEFINED;
    byte nextUI = UI_UNDEFINED;
    Keyword *lastKeyword = NULL;
    VariableToken lastVariable = 0;
    byte result = _RESULT_UNDEFINED_;
    void init( void *components[]);
    byte *parse( byte *str);

    // Operator declarations here
    bool operator_AMODE();

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

    bool operator_MEM();

    bool operator_REM();

    bool operator_PUSH();

    bool operator_RUN();

    bool operator_STORE();
    bool operator_STORE_Vars( bool constants);

    bool operator_SUM();

    bool operator_SUMXY();

    inline byte *_getCurrentPosition(){
      return _lexer_position;
    };

    byte *_skipToEOL( byte *str=NULL);
    byte *_skipToNextOperator( byte *str=NULL);
    bool processRPNKeywordByID( int16_t id, bool refresh=true);

  private:
    byte *_io_buffer;
    IOManager *_iom;
    Keywords *_kwds;
    Variables *_vars;
    ProgramCode *_code;
    Functions *_funs;
    ExpressionParser *_epar;
    LCDManager *_lcd;
    SDManager *_sdm;
    RPNStackBox *_rsb;
    MessageBox *_mbox;
    CommandLine *_clb;

    bool _expression_error = false;
    byte *_lexer_position;
    double _listValues[10];

    void _parseOperator();
    bool _processKeyword();
    bool _processRPNKeyword( Keyword *kwd);
    bool _processVariable( bool asConstant=false);
    bool _findAssignment();
    bool _validate_NextCharacter( byte c);

    void _operatorListProgramCode(uint16_t lFrom, uint16_t lTo=MAX_LINE_NUMBER);
    void _operatorClearProgramCode(uint16_t lFrom, uint16_t lTo=MAX_LINE_NUMBER);

    inline void _ignore_Blanks(){
      while(IsSpacer(*_lexer_position))
        _lexer_position++;
    };
    bool _check_NextToken( byte c);
    bool _peek_NextToken( byte c);
    byte _parseList( byte maxVal=10);
};

#endif // _LEXER_HPP
