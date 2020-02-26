#include "Keywords.hpp"

//#define __DEBUG

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

byte *NumberParser::parse( byte *str){
  result = _REAL_;
  _dValue = 3.1415926e123;           
  return str;
}
