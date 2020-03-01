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

class NumberParser{
  public:
    byte result = _NOT_A_NUMBER_;
    double realValue();
    int64_t integerValue();
    byte *parse( byte *str);
  private:
    double _dValue = 0.0;           
    int64_t _iValue = 0L;
    byte *_processMultiplier(byte *ptr, double v, double mult);
    byte *_processMultiplier(byte *ptr, int64_t v, double mult);
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
