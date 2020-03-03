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
  Serial.print("Parsing: [");
  Serial.print((char*)str);
  Serial.println("]");  
  byte logical_position = 0; // 0-whole, 1-decmal, 2-exponent, 3-exp number
  result = _NOT_A_NUMBER_;
  byte *ptr = str;
  _dValue = 0.0;           
  _iValue = 0L;
  while( IsSpacer(*ptr)) ptr++;
  if( !isDigit(*ptr) && *ptr != '.')
    return ptr;
  while( isDigit(*ptr)){
    _iValue *= 10;
    _iValue += (*ptr++)-_ZERO_;
  }
  switch(*ptr){
    case _NUL_:
      result = _INTEGER_;
      return ptr;
    case '.':
      _processMultiplier( ++ptr, _iValue, 1.0);
      logical_position = 1;
      break;
    case 'e':
    case 'E':
      _processMultiplier( ++ptr, _iValue, 1.0);
      logical_position = 2;
      break;
    case 'f':
      return _processMultiplier( ++ptr, _iValue, 1e-15);
    case 'p':
      return _processMultiplier( ++ptr, _iValue, 1e-12);
    case 'n':
      return _processMultiplier( ++ptr, _iValue, 1e-9);
    case 'u':
      return _processMultiplier( ++ptr, _iValue, 1e-6);
    case 'm':
      return _processMultiplier( ++ptr, _iValue, 1e-3);
    case 'c':
      return _processMultiplier( ++ptr, _iValue, 1e-2);
    case 'd':
      return _processMultiplier( ++ptr, _iValue, 0.1);
    case 'k':
      return _processMultiplier( ++ptr, _iValue, 1e3);
    case 'M':
      return _processMultiplier( ++ptr, _iValue, 1e6);
    case 'G':
      return _processMultiplier( ++ptr, _iValue, 1e9);
    case 'T':
      return _processMultiplier( ++ptr, _iValue, 1e12);
    case 'P':
      return _processMultiplier( ++ptr, _iValue, 1e15);
    default:
      if( IsNumberTerm(*ptr)) result = _INTEGER_;
      return ptr;   
  }
  double mult = 0.1;
  while( logical_position == 1){
    switch(*ptr){
      case _NUL_:
        result = _REAL_;
        return ptr;
      case '.': // second decimal not allowed
        result = _NOT_A_NUMBER_;
        _dValue = 0.0;           
        return ptr;
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
      case 'f':
        return _processMultiplier( ++ptr, _dValue, 1e-15);
      case 'p':
        return _processMultiplier( ++ptr, _dValue, 1e-12);
      case 'n':
        return _processMultiplier( ++ptr, _dValue, 1e-9);
      case 'u':
        return _processMultiplier( ++ptr, _dValue, 1e-6);
      case 'm':
        return _processMultiplier( ++ptr, _dValue, 1e-3);
      case 'c':
        return _processMultiplier( ++ptr, _dValue, 1e-2);
      case 'd':
        return _processMultiplier( ++ptr, _dValue, 0.1);
      case 'k':
        return _processMultiplier( ++ptr, _dValue, 1e3);
      case 'M':
        return _processMultiplier( ++ptr, _dValue, 1e6);
      case 'G':
        return _processMultiplier( ++ptr, _dValue, 1e9);
      case 'T':
        return _processMultiplier( ++ptr, _dValue, 1e12);
      case 'P':
        return _processMultiplier( ++ptr, _dValue, 1e15);
      default:
        if( !IsNumberTerm(*ptr)){
          result = _NOT_A_NUMBER_;
          _dValue = 0.0;
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
        return ptr;        
      }
      break;
    default:
      result = _NOT_A_NUMBER_;
      _dValue = 0.0;
      return ptr;
  }
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
  return ptr;
}

byte *NumberParser::_processMultiplier(byte *ptr, double v, double mult){
  result = _REAL_;
  _dValue = mult * v;
  return ptr;  
}
byte *NumberParser::_processMultiplier(byte *ptr, int64_t v, double mult){
  result = _REAL_;
  _dValue = mult * v;
  _iValue = 0;
  return ptr;  
}
