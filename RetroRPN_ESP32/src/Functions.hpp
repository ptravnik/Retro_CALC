/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _FUNCTIONS_HPP
#define _FUNCTIONS_HPP

#include "Variables.hpp"

#define NMATH_RETURNS   3
#define _MATH_PI_       3.14159265359

struct Function{
  int16_t id = 0;
  const char *name0;
  const char *name1;
  byte nArgs = 1;
  byte nRets = 1;
  size_t RPNtag = _RPN_COMMON_;
  size_t VarTag = 2;
};

class Functions{
  public:
    void init( void *components[]);
    Function *getFunction(byte *str);
    double *Compute( Function *mf, double *args);
    double *Compute( Function *mf, double arg);
    double *Compute( Function *mf);
    double *quad( double *args);
    double *goff2( double *args);
  private:
    Variables *_vars;
    size_t _id;
    double _rets[NMATH_RETURNS];
    Function _functions[NMATH_FUNCTIONS];
    void _addFunction( const char *name0, const char *name1, byte nArgs, byte nRets, byte RPNtag=_RPN_COMMON_);
    Function *_setVariable( Function *f, VariableToken vt);
    inline void _clearRets(){
      for(byte i=0; i<NMATH_RETURNS; i++)_rets[i] = 0.0; 
    };
};

#endif // _FUNCTIONS_HPP
