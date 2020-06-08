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
// Quadratic equation solver (RPN only)
// For RPN-only functions, error setting is unnecessary, but performed for completeness
//
static byte _function_Solver_QUAD_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _NO_ERROR_;

  // Trivial solution
  if( _vars->isRPNRegisterZero(2) && _vars->isRPNRegisterZero(1) && _vars->isRPNRegisterZero()){
    _vars->setScrMessage( FUN_Message_Trivial);
    return _REQUEST_REDRAW_MSG;    
  }
  
  // No roots
  if( _vars->isRPNRegisterZero(2) && _vars->isRPNRegisterZero(1)){
    _vars->setScrMessage( FUN_Message_NoRoots);
    return _REQUEST_REDRAW_MSG;    
  }

  // Linear single root
  if( _vars->isRPNRegisterZero(2)){
    _vars->saveRPNPrev();
    rets[0] /= -rets[1];
    _vars->setScrMessage( FUN_Message_OneRoot);
    _vars->setRPNLabelX( FUN_Message_Root1);
    return _REQUEST_REDRAW_X + _REQUEST_REDRAW_LABELS;    
  }

  // Solve as quadratic
  double a = args[2];
  double b = args[1];
  double c = args[0];
  rets[2] = b*b - 4.0 * a*c;
  rets[1] = 0.5 / a;
  rets[0] = -b * rets[1];
  _vars->setRPNLabelZ( FUN_Message_Discriminant);

  // Complex roots
  if( rets[2]<0.0){
    rets[1] *= sqrt(-rets[2]);
    _vars->setScrMessage( FUN_Message_ComplexRoots);
    _vars->setRPNLabelY( FUN_Message_ComplexPart);
    _vars->setRPNLabelX( FUN_Message_RealPart);
    return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
  }
  _vars->setRPNLabelY( FUN_Message_Root2);
  _vars->setRPNLabelX( FUN_Message_Root1);

  // Two real roots
  if( rets[2]>0.0){
    b = rets[0];
    rets[1] *= sqrt(rets[2]);
    rets[0] -= rets[1];
    rets[1] += b;
    _vars->setScrMessage( FUN_Message_TwoRoots);
    return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
  }

  // One real root
  rets[1] = rets[0];
  _vars->setScrMessage( FUN_Message_OneRoot);
  return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
}

//
// Gain and offset solver (RPN only)
//
static byte _function_Solver_GOFF2_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _NO_ERROR_;

  // Trivial solution: no gain
  if( args[3] == args[1]){
    _vars->setScrMessage( FUN_Error_Gain_Offset);
    return _REQUEST_REDRAW_MSG;    
  }
  
  // use lin regression to solve
  _vars->clearRPNSumXY();
  _vars->addSample2RPNSumXY( args[1], args[0]);
  _vars->popRPNStack();
  _vars->popRPNStack();
  _vars->addSample2RPNSumXY( args[1], args[0]);
  rets[0] = _vars->offset[0];
  rets[1] = _vars->gain[0];
  _vars->setScrMessage( FUN_Message_Goff_Solution);
  _vars->setRPNLabelX( FUN_Message_Offset);
  _vars->setRPNLabelY( FUN_Message_Gain);
  return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
}

//
// Gain and offset solver
//
static byte _function_Solver_LIN2_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _NO_ERROR_;
  if( isRPN) _vars->saveRPNPrev();
  rets[0] = args[0] * _vars->gain[0] + _vars->offset[0];
  return 1;
}

//
// Circle solvers
//
static byte _function_Solver_RADIUS_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0]*args[0] + args[1]*args[1];
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( isRPN){
    _vars->popRPNStack();
    _vars->saveRPNPrev();
  }
  _vars->mathError = _NO_ERROR_;
  rets[0] = sqrt( tmp);
  return 1;
}

static byte _function_Solver_CATH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0]*args[0] - args[1]*args[1];
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( tmp<0.0) tmp = -tmp;
  if( isRPN){
    _vars->popRPNStack();
    _vars->saveRPNPrev();
  }
  _vars->mathError = _NO_ERROR_;
  rets[0] = sqrt( tmp);
  return 1;
}

static byte _function_Solver_LCIRC_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = 2.0 * _MATH_PI_ * args[0];
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( isRPN) _vars->saveRPNPrev();
  _vars->mathError = _NO_ERROR_;
  rets[0] = tmp;
  return 1;
}

static byte _function_Solver_SCIRC_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = _MATH_PI_ * args[0] * args[0];
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( isRPN) _vars->saveRPNPrev();
  _vars->mathError = _NO_ERROR_;
  rets[0] = tmp;
  return 1;
}

//
// Sphere solvers
//
static byte _function_Solver_VSPHERE_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0]*args[0]*args[0] / 3.0 * 4.0 *_MATH_PI_;
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( tmp<0.0) tmp = -tmp;
  if( isRPN){
    _vars->popRPNStack();
    _vars->saveRPNPrev();
  }
  _vars->mathError = _NO_ERROR_;
  rets[0] = tmp;
  return 1;
}

static byte _function_Solver_SSPHERE_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0]*args[0] * 4.0 *_MATH_PI_;
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( isRPN){
    _vars->popRPNStack();
    _vars->saveRPNPrev();
  }
  _vars->mathError = _NO_ERROR_;
  rets[0] = tmp;
  return 1;
}
