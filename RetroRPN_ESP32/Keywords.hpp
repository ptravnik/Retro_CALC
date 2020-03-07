//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef KEYWORDS_HPP
#define KEYWORDS_HPP

#include <Arduino.h>
#include <math.h>
#include "Utilities.hpp"

#define _NOT_A_NUMBER_  0 
#define _INTEGER_       1
#define _REAL_          2
#define _HUGE_POS_INTEGER_  8000000000000000000
#define _HUGE_NEG_INTEGER_  -8000000000000000000

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
    double realValue();
    int64_t integerValue();
    byte *parse( byte *str);
  private:
    double _dValue = 0.0;           
    int64_t _iValue = 0L;
    byte *_processMultiplier(byte *ptr, double v, int8_t mult);
    byte *_processMultiplier(byte *ptr, int64_t v, int8_t mult);
    int8_t _locateMultiplier(byte b);
    byte *_parseHex( byte *ptr);
};

class NameParser{
  public:
    bool result = false;
    byte *parse( byte *str);
  private:
};

class Keyword{
  public:
    bool is;
    void show();
    void redraw();
  private:
    bool _lcd;
};

#endif // KEYWORDS_HPP
