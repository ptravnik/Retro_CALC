//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

// This include is for convenience of coding; comment out
//#include "Variables.hpp"
// Note all functions presume RPN pointer assignment


//
// Stack manipulation functions
//
static byte _function_RPN_POP_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->saveRPNPrev();
  _vars->popRPNStack();
  _vars->mathError = _NO_ERROR_;
  return 3;
}
static byte _function_RPN_SWAP_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->saveRPNPrev();
  _vars->swapRPNXY();
  _vars->mathError = _NO_ERROR_;
  return 2;
}
static byte _function_RPN_ROLL_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->saveRPNPrev();
  _vars->rollRPNStack();
  _vars->mathError = _NO_ERROR_;
  return 3;
}

//
// Basic RPN functions
//
static byte _function_RPN_ADD_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  return _vars->_Universal_Mantra_( isRPN, args[1] + args[0], rets, 1);
}

static byte _function_RPN_SUB_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  return _vars->_Universal_Mantra_( isRPN, args[1] - args[0], rets, 1);
}

static byte _function_RPN_MUL_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  return _vars->_Universal_Mantra_( isRPN, args[1] * args[0], rets, 1);
}

static byte _function_RPN_DIV_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( abs(tmp) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_DivZero);
    return _REQUEST_REDRAW_MSG;
  }
  return _vars->_Universal_Mantra_( isRPN, args[1] / tmp, rets, 1);
}

static byte __fun_Power( Variables *_vars, double a, double b, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  //_messages
  // power zero: using "1 convention"  
  if( a==0.0 && b==0.0){
    _vars->mathError = _NO_ERROR_;
    if( isRPN){
      _vars->setScrMessage( FUN_Warning_ZeroPowerZero);
      _vars->saveRPNPrev();
      _vars->popRPNStack();
    }
    rets[0] = 1.0;
    return _REQUEST_REDRAW_ALL;
  }
  // positive power of zero: zero
  if( a==0.0 && b>0.0)
    return _vars->_Universal_Mantra_( isRPN, 0.0, rets, 1);

  // negative power of zero: div by zero
  if( a==0.0 && b<0.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_DivZero);
    return _REQUEST_REDRAW_MSG;
  }
  a = pow( a, b);
  return _vars->_Universal_Mantra_( isRPN, a, rets, 1);
}

static byte _function_RPN_POW_( Variables *_vars, double *args, double *rets, bool isRPN){
  return isRPN? __fun_Power( _vars, args[1], args[0], rets, isRPN):
    __fun_Power( _vars, args[0], args[1], rets, isRPN);
}

static byte _function_RPN_ROOT_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double a = isRPN? args[1]: args[0];
  double b = isRPN? args[0]: args[1];
  if( abs(b) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_DivZero);
    return _REQUEST_REDRAW_MSG;
  }
  return __fun_Power( _vars, a, 1.0/b, rets, isRPN);
}

static byte _function_RPN_NEG_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _NO_ERROR_;
  return _vars->_Universal_Mantra_( isRPN, -args[0], rets, 0);
}

static byte _function_RPN_OOX_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0];
  if( abs(tmp) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_DivZero);
    return _REQUEST_REDRAW_MSG;
  }
  return _vars->_Universal_Mantra_( isRPN, 1.0/tmp, rets, 0);
}

static byte _function_RPN_AIN_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double a = args[1];
  double b = args[0];
  if( abs(a) < 1e-300 || abs(b) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_DivZero);
    return _REQUEST_REDRAW_MSG;
  }
  a = 1.0 / a + 1.0 / b;
  if( abs(a) < 1e-300){
    if( isRPN) _vars->setScrMessage( FUN_Error_DivZero);
    return _REQUEST_REDRAW_MSG;
  }
  return _vars->_Universal_Mantra_( isRPN, 1.0/a, rets, 1);
}

static byte _function_RPN_STACK_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _NO_ERROR_;
  int index = 0;
  if( !isRPN){
    double tmp = args[0];
    if( tmp < 0.0) tmp=0;
    if( tmp > 19.0) tmp=19.0;
    index = (int)floor( tmp);
  }
  rets[0] = _vars->getRPNRegister( index);
  return 1;
}
