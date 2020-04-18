/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <Arduino.h>
#include <math.h>
#include "Utilities.hpp"
#include "MathFunctions.hpp"

#define _NOT_A_NUMBER_  0 
#define _INTEGER_       1
#define _REAL_          2
#define _STRING_        3
#define _HUGE_POS_INTEGER_  8000000000000000000
#define _HUGE_NEG_INTEGER_  -8000000000000000000

#define _MAX_IDENTIFIER_  12
#define _NUMBER_LENGTH_  22
//#define _NOT_AN_ID_ 0
//#define _UNKNOWN_ID_ 1
//#define _CONSTANT_ 2
//#define _VARIABLE_ 3
//#define _KEYWORD_ 4

//
// Encapsulates number parsing
// An integer starts with a digit and contains only digits
// A hexadecimal starts with 0x and contains only hexadecimal digits 0-E
// A real starts with a digit or . and contains digits, e+/-
//
class NumberParser{
  public:
    byte result = _NOT_A_NUMBER_;
    byte precision = 8;
    bool force_sci = false;
    double realValue();
    int64_t integerValue();
    byte *stringValue( double n, byte *ptr=NULL, byte max_len=_NUMBER_LENGTH_);
    byte *stringValue( byte *ptr=NULL, byte max_len=_NUMBER_LENGTH_);
    byte *stringHex( double n, byte *ptr=NULL, byte max_len=_NUMBER_LENGTH_);
    byte *stringHex( byte *ptr=NULL, byte max_len=_NUMBER_LENGTH_);
    byte *parse( byte *str);
    void negate();
    inline void setValue( double v){
      result = _REAL_;
      _dValue = v;
    }
    inline void setValue( int64_t v){
      result = _INTEGER_;
      _iValue = v;
    }
private:
    byte _numString[_NUMBER_LENGTH_];
    double _dValue = 0.0;           
    int64_t _iValue = 0L;
    byte *_processMultiplier(byte *ptr, double v, int8_t mult);
    byte *_processMultiplier(byte *ptr, int64_t v, int8_t mult);
    int8_t _locateMultiplier(byte b);
    byte *_parseHex( byte *ptr);
    byte * _convertInt(byte *ptr, byte max_len);
    byte * _convertDouble(double n, byte *ptr, byte max_len);
};

class NameParser{
  public:
    bool result = false;
    byte *parse( byte *str);
    inline bool isKeyword( const char *kw){
      return result && (strcmp(kw, (char *)_name)==0);
    }
    inline byte * Name(){
      return _name;
    }
    inline void _reset_name(){
      result = false;
      *_name = _NUL_;
      _name_position = 0;
    };
  private:
    byte _name[_MAX_IDENTIFIER_ + 2];
    byte _name_position;
    inline void _add_char_to_name( byte *ptr){
      _name[_name_position++] = *ptr;
      _name[_name_position] = _NUL_;
    };
};

class ExpressionParser{
  public:
    byte result = _NOT_A_NUMBER_;
    inline void init(){
      mathFunctions.init();
    };
    byte *parse( byte *str);
    NumberParser numberParser;
    NameParser nameParser;
    MathFunctions mathFunctions;
    MathFunction *lastMathFunction;
    inline byte *_getCurrentPosition(){
      return _parser_position;
    }
  private:
    bool _expression_error = false;
    byte *_parser_position;
    //double _args[3];
    inline void _ignore_Blanks(){
      while(IsSpacer(*_parser_position))
        _parser_position++;
    };
    inline bool _check_NextToken( byte c){
      if( *_parser_position != c ) return false;
      _parser_position++;
      return true;
    };
    bool _validate_NextCharacter( byte c);
    bool _validate_NextOperation( const char *op1);
    bool _validate_NextOperation( const char *op1, const char *op2);
    bool _parse_ListMember( byte terminator);
    bool _parse_FunctionArguments(MathFunction *mf, double *_args);
    byte *_parse_Expression_Comparison();
    byte *_parse_Expression_Add_Sub();
    byte *_parse_Expression_Mult_Div();
    byte *_parse_Expression_Power();
    byte *_parse_Expression_Value();
    byte *_bracket_Check();
};

#endif // _PARSER_HPP
