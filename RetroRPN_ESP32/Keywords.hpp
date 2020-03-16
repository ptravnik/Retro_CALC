//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _KEYWORDS_HPP
#define _KEYWORDS_HPP

#include <Arduino.h>
#include <math.h>
#include "Utilities.hpp"

#define _NOT_AN_ID_ 0
#define _UNKNOWN_ID_ 1
#define _CONSTANT_ 2
#define _VARIABLE_ 3
#define _KEYWORD_ 4

class Keyword{
  public:
    bool is;
    void show();
    void redraw();
  private:
    bool _lcd;
};

#endif // _KEYWORDS_HPP
