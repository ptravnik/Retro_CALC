/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _MATHFUNCTIONS_HPP
#define _MATHFUNCTIONS_HPP

#include <Arduino.h>
#include <math.h>
#include "./src/Utilities.hpp"
#include "Keywords.hpp"

#define _MODE_DEGREES_  0
#define _MODE_RADIAN_   1 
#define _MODE_GRADIAN_  2

#define NMATH_RETURNS   3
#define _MATH_PI_       3.14159265359

struct MathFunction{
    int16_t id = 0;
    const char *name0;
    const char *name1;
    byte nArgs = 1;
    byte nRets = 1;
    byte RPNtag = _RPN_COMMON_;
};

class MathFunctions{
  public:
    byte angleMode = _MODE_DEGREES_;
    double rpnStack[RPN_STACK];
    double previous_X = 0.0;

    void init( byte amod = _MODE_DEGREES_);
    void setAngleMode(byte m);
    MathFunction *getFunction(byte *str);
    double *Compute( MathFunction *mf, double *args);
    double *Compute( MathFunction *mf, double arg);
    double *Compute( MathFunction *mf);
    double getConvertedAngle( double a);
    double getUnconvertedAngle( double a);
    double *quad( double *args);
    double *goff2( double *args);
  private:
    size_t _id;
    double _rets[NMATH_RETURNS];
    MathFunction _mf[NMATH_FUNCTIONS];
    void _addFunction( const char *name0, const char *name1, byte nArgs, byte nRets, byte RPNtag=_RPN_COMMON_);
    inline void _clearRets(){
      for(byte i=0; i<NMATH_RETURNS; i++)_rets[i] = 0.0; 
    };
};

#endif // _MATHFUNCTIONS_HPP
