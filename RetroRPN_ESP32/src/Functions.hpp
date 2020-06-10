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

#define NMATH_RETURNS       3
#define _MATH_PI_           3.14159265359
#define _MATH_PIo2_         1.5707963268
#define _MATH_PIx2_         6.28318530718
#define _MATH_SQ_PI_        1.77245385091
#define _MATH_NDIS_NORM_    3.98942280402e-1

struct Function{
  int16_t id = 0;
  int16_t kwid = -1;
  const char *name = NULL;
  byte nArgs = 1;
  size_t RPNtag = _RPN_COMMON_;
  size_t VarTag = 2;
};

class Functions{
  public:
    double _rets[NMATH_RETURNS];

    void init( void *components[]);
    Function *getFunction(byte *str);
    byte Compute( Function *mf, double *args);
    byte ComputeRPN( Keyword *kw);
    double *goff2( double *args);
  private:
    Keywords *_kwds;
    Variables *_vars;
    size_t _id;
    Function _functions[_FUNCTION_COUNT];
    void _addFunction( int16_t kwid, byte nArgs, byte RPNtag=_RPN_COMMON_);
    Function *_setVariable( Function *f, VariableToken vt);
    inline void _clearRets(){
      for(byte i=0; i<NMATH_RETURNS; i++)_rets[i] = 0.0; 
    };
    inline void _setFunction( int16_t id, void *func, int8_t argsN){
      Keyword *k = _kwds->getKeywordById( id);
      k->operator_ptr = func;
      k->argumentType = argsN;
    };
};

#endif // _FUNCTIONS_HPP
