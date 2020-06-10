//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

// This include is for convenience of coding; comment out
//#include "Variables.hpp"


//
// Logarithms and anti-logarithms
// Note they presume RPN pointer assignment
//
static byte _function_Math_LG_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( tmp <= 0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG;
  }
  tmp = log10( tmp);
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

static byte _function_Math_LN_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( tmp <= 0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG;
  }
  tmp = log( tmp);
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

static byte _function_Math_LOG_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double a = isRPN? args[1]: args[0];
  double b = isRPN? args[0]: args[1];
  if( a <= 0.0 || b <= 0.0 || abs(b-1.0) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG;
  }
  a = log(a) / log(b);
  _vars->swapRPNXY(); // we are saving the previous value
  return _vars->_Universal_Mantra_( isRPN, a, rets, 1);
}

static byte _function_Math_EXP_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( abs(tmp) > 690){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  return _vars->_Universal_Mantra_( isRPN, exp( tmp), rets);
}

//
// Powers
//
static byte _function_Math_SQRT_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( tmp < 0.0 && isRPN){
    _vars->setScrMessage( FUN_Warning_Complex);
    _vars->_RPN_Mantra_( sqrt(-tmp)); 
    return _REQUEST_REDRAW_X;
  }
  if( tmp < 0.0) return _REQUEST_REDRAW_MSG;
  return _vars->_Universal_Mantra_( isRPN, sqrt( tmp), rets);
}
static byte _function_Math_SQ_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  return _vars->_Universal_Mantra_( isRPN, tmp * tmp, rets);
}

//
// Trigonometry functions
// Note they presume RPN pointer assignment
//
static byte _function_Math_ACOS_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( abs(tmp) > 1){
    if( isRPN) _vars->setScrMessage( FUN_Error_InverseTrig);
    return _REQUEST_REDRAW_MSG;
  }
  tmp = _vars->getUnconvertedAngle( acos( tmp));
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

static byte _function_Math_COS_( Variables *_vars, double *args, double *rets, bool isRPN){
  double tmp = _vars->getConvertedAngle( args[0]);
  byte rB = 1;
  if( abs( tmp) > 1e16){
    if( isRPN) _vars->setScrMessage( FUN_Warning_Inaccurate);
    rB = _REQUEST_REDRAW_X; 
    _vars->mathError = _WARNING_;
  }
  _vars->_Universal_Mantra_( isRPN, cos(tmp), rets);
  return rB;
}

static byte _function_Math_ASIN_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( abs(tmp) > 1){
    if( isRPN) _vars->setScrMessage( FUN_Error_InverseTrig);
    return _REQUEST_REDRAW_MSG;
  }
  tmp = _vars->getUnconvertedAngle( asin( tmp));
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

static byte _function_Math_SIN_( Variables *_vars, double *args, double *rets, bool isRPN){
  double tmp = _vars->getConvertedAngle( args[0]);
  byte rB = 1;
  if( abs( tmp) > 1e16){
    if( isRPN) _vars->setScrMessage( FUN_Warning_Inaccurate);
    _vars->mathError = _WARNING_;
    rB = _REQUEST_REDRAW_X; 
  }
  _vars->_Universal_Mantra_( isRPN, sin(tmp), rets);
  return rB;
}

static byte _function_Math_ATAN_( Variables *_vars, double *args, double *rets, bool isRPN){
  double tmp = _vars->getUnconvertedAngle( atan( args[0]));
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

static byte _function_Math_TAN_( Variables *_vars, double *args, double *rets, bool isRPN){
  double tmp = _vars->getConvertedAngle( args[0]);
  byte rB = 1;
  if( abs( tmp) > 1e16){
    if( isRPN) _vars->setScrMessage( FUN_Warning_Inaccurate);
    _vars->mathError = _WARNING_;
    rB = _REQUEST_REDRAW_X; 
  }
  double dv = cos( tmp);
  if( abs(dv) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    _vars->mathError = _ERROR_;
    return _REQUEST_REDRAW_MSG;
  }
  _vars->_Universal_Mantra_( isRPN, sin(tmp) / dv, rets);
  return rB;
}

static byte _function_Math_COT_( Variables *_vars, double *args, double *rets, bool isRPN){
  double tmp = _vars->getConvertedAngle( args[0]);
  byte rB = 1;
  if( abs( tmp) > 1e16){
    if( isRPN) _vars->setScrMessage( FUN_Warning_Inaccurate);
    _vars->mathError = _WARNING_;
    rB = _REQUEST_REDRAW_X; 
  }
  double dv = sin( tmp);
  if( abs(dv) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    _vars->mathError = _ERROR_;
    return _REQUEST_REDRAW_MSG;
  }
  _vars->_Universal_Mantra_( isRPN, cos(tmp) / dv, rets);
  return rB;
}

//
// Hyperbolic functions
// Note they presume RPN pointer assignment
//
static byte _function_Math_ACOSH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0] * args[0] - 1.0;
  if( tmp < 0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG; 
  }
  tmp = args[0] + sqrt( tmp);
  if( tmp <= 0.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Underflow);
    return _REQUEST_REDRAW_MSG; 
  }
  return _vars->_Universal_Mantra_( isRPN, log( tmp), rets);
}

static byte _function_Math_COSH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  if( args[0] < -690 || 690 < args[0]){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG; 
  }
  double tmp = exp( args[0]); 
  tmp = (tmp + 1/tmp) * 0.5;
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

static byte _function_Math_ASINH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0] * args[0] + 1.0;
  tmp = args[0] + sqrt( tmp);
  if( tmp <= 0.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Underflow);
    return _REQUEST_REDRAW_MSG; 
  }
  return _vars->_Universal_Mantra_( isRPN, log( tmp), rets);
}

static byte _function_Math_SINH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  if( args[0] < -690 || 690 < args[0]){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG; 
  }
  double tmp = exp( args[0]); 
  tmp = (tmp - 1/tmp) * 0.5;
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

static byte _function_Math_ATANH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = 1 - args[0];
  if( abs( tmp) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG; 
  }
  tmp = (1+args[0]) / tmp;
  if( tmp <= 0.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Underflow);
    return _REQUEST_REDRAW_MSG; 
  }
  return _vars->_Universal_Mantra_( isRPN, log( tmp) * 0.5, rets);
}

static byte _function_Math_TANH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  if( args[0] < -690 || 690 < args[0]){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG; 
  }
  double tmp1 = exp( args[0]);
  double tmp2 = exp( -args[0]);
  tmp1 = (tmp1 - tmp2) / (tmp1 + tmp2);
  return _vars->_Universal_Mantra_( isRPN, tmp1, rets);
}

//
// Statistical functions
// Note they presume RPN pointer assignment
//
static byte _function_Math_NDIS_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  double sigma = _vars->stdev[0];
  if( abs(sigma) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  tmp -= _vars->mean[0];
  tmp /= sigma;
  tmp *= tmp;
  if( isnan(tmp) || abs(tmp) < 1e-30) tmp = 1.0;
  else tmp = exp( -tmp*0.5);
  tmp /= sigma;
  tmp *= _MATH_NDIS_NORM_;
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}

//
// Other functions
//
static byte _function_Math_SIGN_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _NO_ERROR_;
  if( isRPN) _vars->saveRPNPrev();
  double tmp = args[0];
  if( abs(tmp)<1e-300){
    rets[0] = 0.0;
    return 1;
  }
  rets[0] = ( tmp>0.0)? 1.0: -1.0;
  return 1;
}

static byte _function_Math_ABS_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _NO_ERROR_;
  if( isRPN) _vars->saveRPNPrev();
  rets[0] = abs( args[0]);
  return 1;
}
