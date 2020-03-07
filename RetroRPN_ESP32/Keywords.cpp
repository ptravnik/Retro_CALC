//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Keywords.hpp"
#include "CP1251_mod.h"

#define __DEBUG

const byte _multiplier_Prefixes[] PROGMEM = 
   { 'f', 'p', 'n', 'u', 'm', 'c', 'd', 'k', 'M', 'G', 'T', 'P'};
const int8_t _multiplier_Values[] PROGMEM = 
   { -15, -12,  -9,  -6,  -3,  -2,  -1,   3,   6,   9,  12,  15};

//
// Number parser components
//
double NumberParser::realValue(){
  switch(result){
    case _REAL_:
      return _dValue;
    case _INTEGER_:
      return (double)_iValue;
    default:
      break;
  }
  return 0.0;
}
int64_t NumberParser::integerValue(){
  switch(result){
    case _REAL_:
      if( _dValue >= 8e18) return _HUGE_POS_INTEGER_;
      if( _dValue <= -8e18) return _HUGE_NEG_INTEGER_;
      return (int64_t)round( _dValue);
    case _INTEGER_:
      return _iValue;
    default:
      break;
  }
  return 0;
}

//
// Attempts to parse a number
// The pointer returned is past the number if valid
// or at the origin if not parsed correctly
//
byte *NumberParser::parse( byte *str){
  #ifdef __DEBUG
  Serial.print("Parsing: [");
  Serial.print((char*)str);
  Serial.println("]");
  #endif  
  byte logical_position = 0; // 0-whole, 1-decmal, 2-exponent, 3-exp number
  result = _NOT_A_NUMBER_;
  byte *ptr = str;
  _dValue = 0.0;           
  _iValue = 0L;
  int8_t nExp = 0;
  
  // skip empty space
  while( IsSpacer(*ptr)) ptr++;
  if( !IsDigitOrDecimal(*ptr))
    return ptr;
  str = ptr;

  // check if hexadecimal
  if( *str == _ZERO_ && str[1] == 'x' && convertHex(str[2])<16)
    return _parseHex( str);

  // parse the integer part  
  while( isDigit(*ptr)){
    _iValue *= 10;
    _iValue += (*ptr++)-_ZERO_;
  }

  // a number may end on a decimal point, E,
  // an engineering mulitiplier, or number end
  switch(*ptr){
    case _NUL_:
      result = _INTEGER_;
      return ptr;
    case '.':
      _processMultiplier( ++ptr, _iValue, 0);
      logical_position = 1;
      break;
    case 'e':
    case 'E':
      _processMultiplier( ++ptr, _iValue, 0);
      logical_position = 2;
      break;
    default:
      nExp = _locateMultiplier(*ptr);
      if( nExp != 0)
        return _processMultiplier( ++ptr, _iValue, nExp);
      if( !IsNumberTerm(*ptr)) return str; // parsing failed!
      result = _INTEGER_;
      return ptr;   
  }

  // process decimals;
  // the allowed are digits, E,
  // an engineering mulitiplier, or number end
  // the second decimal point causes an error
  double mult = 0.1;
  while( logical_position == 1){
    switch(*ptr){
      case _NUL_:
        result = _REAL_;
        return ptr;
      case '.': // second decimal point is not allowed
        result = _NOT_A_NUMBER_;
        _dValue = 0.0;           
        return str;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        _dValue += mult * ((*ptr++)-_ZERO_);           
        mult /= 10.0;
        break;
      case 'e':
      case 'E':
        logical_position = 2;
        ptr++;
        break;
      default:
        nExp = _locateMultiplier(*ptr);
        if( nExp != 0)
          return _processMultiplier( ++ptr, _dValue, nExp);
        if( !IsNumberTerm(*ptr)){
          result = _NOT_A_NUMBER_;
          _dValue = 0.0;
          return str; // parsing failed!
        }           
        return ptr;   
    }      
  }
  
  // after E, must be only +, -, or digit
  bool exponent_negative = false;
  switch(*ptr){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;    
    case '-':
      exponent_negative = true;
    case '+':
      ptr++;
      if( !IsDigit(*ptr)){
        result = _NOT_A_NUMBER_;
        _dValue = 0.0;
        return str; // parsing failed!      
      }
      break;
    default:
      result = _NOT_A_NUMBER_;
      _dValue = 0.0;
      return str; // parsing failed!
  }

  // finally, the exponent digits
  #ifdef __DEBUG
  Serial.println("Exponents:");
  #endif
  uint16_t exponent_value = 0;
  while( isDigit(*ptr)){
    if( exponent_value <= 300){
      exponent_value *= 10;
      exponent_value += (*ptr)-_ZERO_;
      #ifdef __DEBUG
      Serial.println(exponent_value);
      #endif
    }
    ptr++;
  }
  if( exponent_value > 300)
    exponent_value = 300;
  if( exponent_negative)
    _dValue /= pow( 10.0, exponent_value);
  else
    _dValue *= pow( 10.0, exponent_value);
  return ptr; // parsing success
}

byte *NumberParser::_processMultiplier(byte *ptr, double v, int8_t mult){
  result = _REAL_;
  _dValue = pow( 10, mult) * v;
  return ptr;  
}
byte *NumberParser::_processMultiplier(byte *ptr, int64_t v, int8_t mult){
  result = _REAL_;
  _dValue = pow( 10, mult) * v;
  _iValue = 0;
  return ptr;  
}
int8_t NumberParser::_locateMultiplier(byte b){
  for( byte i=0; i<12; i++){
    if( _multiplier_Prefixes[i] == b)
        return _multiplier_Values[i];
  }
  return 0; 
}
byte *NumberParser::_parseHex( byte *str){
  // skip first two digits
  byte *ptr = str+2;
  byte tmp = convertHex(*ptr); 
  while( tmp < 16){
    _iValue <<= 4;
    _iValue += tmp;
    ptr++;
    tmp = convertHex(*ptr);
  }
  if( !IsNumberTerm(*ptr)){
    _iValue = 0;
    return str;
  }
  result = _INTEGER_;
  return ptr;
}
