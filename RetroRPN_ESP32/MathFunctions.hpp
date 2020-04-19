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

#define NMATH_RETURNS   3
#define _MATH_PI_       3.14159265359

#define _RPN_COMMON_     0
#define _RPN_CHECK_TRIG_ 1
#define _RPN_SWAP_XY_    2
#define _RPN_POWER_      3
#define _RPN_AMODE_      4
#define _RPN_ROOTYX_     5
#define _RPN_QUAD_SOLVER 6
#define _RPN_SQRT_CHECK_ 7
#define _RPN_DIV0_CHECK_ 8
#define _RPN_QUICK_PUSH_ 9
#define _RPN_SWAP_ONLY_  10
#define _RPN_INVTRIG_    11
#define _RPN_GOFF2_SOLVER 12

#define _MF_AMODE_KW_   0
#define _MF_DEG_KW_     1
#define _MF_RAD_KW_     2
#define _MF_GRAD_KW_    3
#define _MF_PI_KW_      4
#define _MF_SIN_KW_     5
#define _MF_COS_KW_     6
#define _MF_TAN_KW_     7
#define _MF_POW_KW_     8
#define _MF_NEG_KW_     9
#define _MF_LN_KW_      10
#define _MF_LG_KW_      11
#define _MF_LOG_KW_     12
#define _MF_EXP_KW_     13
#define _MF_SQRT_KW_    14
#define _MF_SQ_KW_      15
#define _MF_ROOT_KW_    16
#define _MF_SIGN_KW_    17
#define _MF_ABS_KW_     18
#define _MF_INV_KW_     19
#define _MF_RADIUS_KW_  20
#define _MF_QUAD_KW_    21
#define _MF_PREV_KW_    22
#define _MF_SWAP_KW_    23
#define _MF_LCIRC_KW_   24
#define _MF_SCIRC_KW_   25
#define _MF_VSPHERE_KW_ 26
#define _MF_SSPHERE_KW_ 27
#define _MF_ASIN_KW_    28
#define _MF_ACOS_KW_    29
#define _MF_ATAN_KW_    30
#define _MF_TRUE_KW_    31
#define _MF_FALSE_KW_   32
#define _MF_GOFF2_KW_   33

#define NMATH_FUNCTIONS 34

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
    double getUnconvertedAngle( double a);
    double *quad( double *args);
    double *goff2( double *args);
    double previous_X = 0.0;
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
