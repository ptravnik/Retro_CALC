/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _LEXER_HPP
#define _LEXER_HPP

#include "Parsers.hpp"
#include "IOManager.hpp"
//#include "LCDManager.hpp"
//#include "RPNStackBox.hpp"
//#include "MessageBox.hpp"
//#include "CommandLine.hpp"
//#include "SDManager.hpp"

class Lexer{
  public:
    byte result = _NOT_A_NUMBER_;
    void init( void *components[]);
    byte *parse( byte *str);
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
    //LCDManager *_lcd;
    //SDManager *_sdm;
    //RPNStackBox *_rsb;
    //MessageBox *_mbox;
    //CommandLine *_clb;

    bool _expression_error = false;
    byte *_lexer_position;
    byte *_next_operator;
    inline void _ignore_Blanks(){
      while(IsSpacer(*_lexer_position))
        _lexer_position++;
    };
    inline bool _check_NextToken( byte c){
      if( *_lexer_position != c ) return false;
      _lexer_position++;
      return true;
    };
    //bool _validate_NextCharacter( byte c);
    //bool _validate_NextOperation( const char *op1);
    //bool _validate_NextOperation( const char *op1, const char *op2);
    //bool _parse_ListMember( byte terminator);
    //bool _parse_FunctionArguments(Function *f, double *_args);
    //byte *_parse_Expression_Logic();
    //byte *_parse_Expression_NOT();
    //byte *_parse_Expression_Comparison();
    //byte *_parse_Expression_Add_Sub();
    //byte *_parse_Expression_Mult_Div();
    //byte *_parse_Expression_Power();
    //byte *_parse_Expression_Value();
    //byte *_bracket_Check();
};

#endif // _LEXER_HPP
