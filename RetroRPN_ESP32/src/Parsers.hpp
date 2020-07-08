/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _PARSERS_HPP
#define _PARSERS_HPP

#include "Functions.hpp"

//
// Encapsulates number parsing
// An integer starts with a digit and contains only digits
// A hexadecimal starts with 0x and contains only hexadecimal digits 0-E
// A real starts with a digit or . and contains digits, e+/-
//
class NumberParser{
  public:
    byte result = _RESULT_UNDEFINED_;
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
      result = _RESULT_REAL_;
      _dValue = v;
    }
    inline void setValue( int64_t v){
      result = _RESULT_INTEGER_;
      _iValue = v;
    }
    inline void setValue( int32_t v){
      result = _RESULT_INTEGER_;
      _iValue = (int64_t)v;
    }
    inline void setValue( int16_t v){
      result = _RESULT_INTEGER_;
      _iValue = (int64_t)v;
    }
    inline void setValue( size_t v){
      result = _RESULT_INTEGER_;
      _iValue = (int64_t)v;
    }
    inline void setValue( byte v){
      result = _RESULT_INTEGER_;
      _iValue = (int64_t)v;
    }
    inline void resetValue(){
      result = _RESULT_UNDEFINED_;
      _dValue = 0.0;
      _iValue = 0L;
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

//
// Encapsulates name parsing
// A name starts with a letter or an undescore and contains only letters, digits and underscores
// A vaiable name may end with symbols $ for strings and % for integers
//
class NameParser{
  public:
    bool result = false;
    byte *parse( byte *str);
    inline byte * Name(){
      return _name;
    }
    inline void _reset_name(){
      result = false;
      *_name = _NUL_;
      _name_position = 0;
    };
    byte VarType();
  private:
    byte _name[_MAX_IDENTIFIER_ + 2]; // TODO: replace with dynamic
    byte _name_position;
    inline void _add_char_to_name( byte *ptr){
      _name[_name_position++] = *ptr;
      _name[_name_position] = _NUL_;
    };
};

//
// Encapsulates filename parsing
// A valid filename contains symbols "/", "-" space, ",", "."
//
class FilenameParser{
  public:
    bool result = false;
    inline void init( Variables *vars){
      _vars = vars;
    };
    byte *parse( byte *str);
    inline byte * Name(){
      return _name;
    }
    inline void _reset_name(){
      result = false;
      *_name = _NUL_;
      _name_position = 0;
    };
  private:
    Variables *_vars;
    byte _name[INPUT_COLS+1]; // TODO: replace with dynamic
    byte _name2[INPUT_COLS+1];
    byte *_parser_start = NULL;
    byte *_parser_end = NULL;
    void _locateTerminator( byte *start);
    void _backpedalDirectory();
    uint16_t _name_position;
};

//
// Encapsulates algebraic formula parsing
//
class ExpressionParser{
  public:
    byte result = _RESULT_UNDEFINED_;
    void init( void *components[]);

    byte *parseName( byte *str);
    byte *parseAlgebraic( byte *str);

    inline byte *Name(){ return _vars->getNewNamePtr();};

    NumberParser numberParser;
    NameParser nameParser;
    FilenameParser filenameParser;
    Function *lastMathFunction;
    inline byte *_getCurrentPosition(){
      return _parser_position;
    };
  private:
    Keywords *_kwds;
    Variables *_vars;
    Functions *_funs;
    bool _expression_error = false;
    byte *_parser_position;
    int _martixRow = 0;
    int _martixColumn = 0;
    inline void _ignore_Blanks(){
      while(IsSpacer(*_parser_position))
        _parser_position++;
    };
    inline bool _check_NextToken( byte c){
      if( *_parser_position != c ) return false;
      _parser_position++;
      return true;
    };
    inline bool _check_NextTokens( byte c1, byte c2){
      if( *_parser_position != c1 && *_parser_position != c2) return false;
      _parser_position++;
      return true;
    };
    bool _validate_NextCharacter( byte c);
    bool _validate_NextOperation( const char *op1);
    bool _validate_NextOperation( const char *op1, const char *op2);
    bool _parse_ListMember( byte terminator);
    bool _parse_FunctionArguments( Function *f, double *_args);
    bool _parse_VariableArguments(VariableToken vt);
    byte *_parse_Expression_Logic();
    byte *_parse_Expression_NOT();
    byte *_parse_Expression_Comparison();
    byte *_parse_Expression_Add_Sub();
    byte *_parse_Expression_Mult_Div();
    byte *_parse_Expression_Power();
    byte *_parse_Expression_Value();
    byte *_bracket_Check();
};

#endif // _PARSERS_HPP
