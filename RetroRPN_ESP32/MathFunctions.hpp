/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _MATHFUNCTIONS_HPP
#define _MATHFUNCTIONS_HPP

#include <Arduino.h>
#include <math.h>
#include "Utilities.hpp"

#define _MODE_DEGREES_  0
#define _MODE_RADIAN_   1 
#define _MODE_GRADIAN_  2

#define NMATH_FUNCTIONS 7
#define NMATH_RETURNS   3
#define _MATH_PI_       3.14159265359

#define _RPN_COMMON_     0
#define _RPN_CHECK_TRIG_ 1
#define _RPN_SWAP_XY_    2

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
    void init( byte amod = _MODE_DEGREES_);
    void setAngleMode(byte m);
    MathFunction *getFunction(byte *str);
    double *Compute( MathFunction *mf, double *args);
    double *Compute( MathFunction *mf, double arg);
    double *Compute( MathFunction *mf);
    double getConvertedAngle( double a);
  private:
    size_t _id;
    double _rets[NMATH_RETURNS];
    MathFunction _mf[NMATH_FUNCTIONS];
    void _addFunction( const char *name0, const char *name1, byte nArgs, byte nRets, byte RPNtag=_RPN_COMMON_);
};

#endif // _MATHFUNCTIONS_HPP
