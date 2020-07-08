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
    //return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS + _REQUEST_DO_IOM;
    return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
  }

  // One real root
  rets[1] = rets[0];
  _vars->setScrMessage( FUN_Message_OneRoot);
  //return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS + _REQUEST_DO_IOM;
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
  return _vars->_convert1_( args, rets, isRPN, _vars->gain[0], _vars->offset[0]);
}

//
// Circle solvers
//
static byte _function_Solver_RADIUS_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0]*args[0] + args[1]*args[1];
  return _vars->_Universal_Mantra_( isRPN, sqrt( tmp), rets, 1);
}

static byte _function_Solver_CATH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double tmp = args[0]*args[0] - args[1]*args[1];
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( tmp<0.0) tmp = -tmp;
  return _vars->_Universal_Mantra_( isRPN, sqrt( tmp), rets, 1);
}

static byte _function_Solver_LCIRC_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 2.0 * _MATH_PI_);
}

static byte _function_Solver_SCIRC_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, _MATH_PI_* args[0]);
}

//
// Sphere solvers
//
static byte _function_Solver_VSPHERE_( Variables *_vars, double *args, double *rets, bool isRPN){
  double tmp = args[0] * args[0] * _MATH_PI_ / 3.0 * 4.0;
  return _vars->_convert1_( args, rets, isRPN, tmp);
}

static byte _function_Solver_SSPHERE_( Variables *_vars, double *args, double *rets, bool isRPN){
  double tmp = args[0] * _MATH_PI_ * 4.0;
  return _vars->_convert1_( args, rets, isRPN, tmp);
}

static byte _function_Solver_AZIMUTH_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double E = args[0];
  double N = args[1];
  double tmp = E*E + N*N;
  if( isnan(tmp)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  tmp = sqrt( tmp);
  _vars->mathError = _NO_ERROR_;
  if( isRPN) _vars->pushRPNStack( 0.0);
  else rets[0] = 0.0;
  if( tmp > 1e-300){
    N = acos( N/tmp);
    if( E<0) N = _MATH_PIx2_ - N;
    rets[0] = _vars->getUnconvertedAngle( N);
  }
  if( isRPN){
    _vars->setScrMessage( _vars->getAMODEString());
    _vars->setRPNLabelZ( FUN_Message_Northing);
    _vars->setRPNLabelY( FUN_Message_Easting);
    _vars->setRPNLabelX( FUN_Message_Azimuth);
    _vars->setRPNPrev(tmp);
  }
  return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
}

static byte _function_Solver_COORDS_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double Az = _vars->getConvertedAngle( args[0]);
  double Az1 = args[0];
  double R = args[1];
  if( R<0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG;
  }
  _vars->mathError = _NO_ERROR_;
  if( isRPN){
    _vars->pushRPNStack( 0.0);
    _vars->setRPNRegister( R, 2);
    _vars->setRPNPrev( Az1);
    _vars->setScrMessage( _vars->getAMODEString());
    _vars->setRPNLabelY( FUN_Message_Northing);
    _vars->setRPNLabelX( FUN_Message_Easting);
    _vars->setRPNLabelZ( FUN_Message_Range);
  }
  else{
    rets[0] = 0.0;
    rets[1] = 0.0;
    rets[2] = R;
  }
  if( R > 1e-300){
    rets[0] = R * sin( Az);
    rets[1] = R * cos( Az);
  }
  return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
}

static byte _function_Solver_AZIMUTH3_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double E = args[1];
  double N = args[0];
  double Z = args[2];
  double R = E*E + N*N;
  if( isnan(R)){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  R = sqrt( R);
  _vars->mathError = _NO_ERROR_;
  rets[1] = _vars->getUnconvertedAngle( _MATH_PIo2_);
  rets[0] = 0.0;
  if( R > 1e-300){
    rets[1] = _vars->getUnconvertedAngle( atan(Z/R));
    N = acos( N/R);
    if( E<0) N = _MATH_PIx2_ - N;
    rets[0] = _vars->getUnconvertedAngle( N);
  }
  if( isRPN){
    _vars->setScrMessage( _vars->getAMODEString());
    _vars->setRPNLabelZ( FUN_Message_Elevation);
    _vars->setRPNLabelY( FUN_Message_AboveHorizon);
    _vars->setRPNLabelX( FUN_Message_Azimuth);
    _vars->setRPNPrev(R);
  }
  return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
}

static byte _function_Solver_COORDS3_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double El = _vars->getConvertedAngle( args[0]);
  double Az = _vars->getConvertedAngle( args[1]);
  double Az1 = args[1];
  double R = args[2] * cos(El);
  if( R<0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG;
  }
  _vars->mathError = _NO_ERROR_;
  if( isRPN){
    _vars->setRPNRegister( R, 2);
    _vars->setRPNPrev( Az1);
    _vars->setScrMessage( _vars->getAMODEString());
    _vars->setRPNLabelY( FUN_Message_Northing);
    _vars->setRPNLabelX( FUN_Message_Easting);
    _vars->setRPNLabelZ( FUN_Message_Range);
  }
  else{
    rets[0] = 0.0;
    rets[1] = 0.0;
    rets[2] = R;
  }
  if( R > 1e-300){
    rets[0] = R * sin( Az);
    rets[1] = R * cos( Az);
  }
  return _REQUEST_REDRAW_ALL + _REQUEST_REDRAW_LABELS;
}

//
// Zelen and Severo algorithm (1964) is 7.5e-8 accurate 
//
static double _PDF_ZelenSevero_( double x){
  bool negative = x<0;
  if( negative) x = -x;
  double t = 1.0/(1.0 + 0.2316419 * x);
  double tv = 0.5307027142649737 * t;
  tv = (tv - 0.7265760130580615) * t;
  tv = (tv + 0.7107068706716199) * t;
  tv = (tv - 0.14224836829963933) * t;
  tv = (tv + 0.1274147958962986) * t;
  tv *= exp( - 0.5 * x * x);
  if( negative) return tv;
  return 1.0-tv;
}

//
// Zelen and Severo inverse 1e-6 accurate 
//
static double _PROBIT_ZelenSevero_( double p){
  double x0 = -4.0;
  double x2 = 4.0;
  if( p < 0.5) x2 = 0.0;
  else x0 = 0.0;
  while( true){
    double x1 = (x0+x2) * 0.5;
    double pc = _PDF_ZelenSevero_(x1);
    if( abs( pc-p) < 0.0000001) return x1;
    if( p<pc) x2 = x1;
    else x0 = x1;
  }
}

static byte _function_Solver_PROB_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double x = args[0];
  if( _vars->stdev[0] <= 0.0)
      return _vars->_Universal_Mantra_( isRPN, 0.0, rets, 1);
  x = (x - _vars->mean[0]) / _vars->stdev[0];
  if( x <= -4.0)
      return _vars->_Universal_Mantra_( isRPN, 0.0, rets, 1);
  if( x >= 4.0)
      return _vars->_Universal_Mantra_( isRPN, 1.0, rets, 1);
  return _vars->_Universal_Mantra_( isRPN, _PDF_ZelenSevero_(x), rets, 1);
}

static byte _function_Solver_PROBIT_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double p = args[0];
  if( p == 0.5)
      return _vars->_Universal_Mantra_( isRPN, _vars->mean[0], rets, 1);
  if( p <0.0 || p>1.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Domain);
    return _REQUEST_REDRAW_MSG;
  }
  if( p <= 0.0)
      return _vars->_Universal_Mantra_( isRPN, _vars->mean[0] - 4.0*_vars->stdev[0], rets, 1);
  if( p >= 1.0)
      return _vars->_Universal_Mantra_( isRPN, _vars->mean[0] + 4.0*_vars->stdev[0], rets, 1);
  double x = _PROBIT_ZelenSevero_( p);
  return _vars->_Universal_Mantra_( isRPN, _vars->mean[0] + x*_vars->stdev[0], rets, 1);
}

static byte _function_Solver_FACT_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double n = args[0];
  if( n > 142){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( n < 0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Domain);
    return _REQUEST_REDRAW_MSG;
  }
  int cnt = (int)floor( n);
  n = 1.0;
  for( int i=1; i<=cnt; i++) n *= i;
  return _vars->_Universal_Mantra_( isRPN, n, rets, 0);
}

static byte _function_Solver_CNK_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  double n = args[1];
  double k = args[0];
  if( n > 142.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Overflow);
    return _REQUEST_REDRAW_MSG;
  }
  if( n < 0.0 || k < 0.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Domain);
    return _REQUEST_REDRAW_MSG;
  }
  if( n <= 0 || k >= n){
    return _vars->_Universal_Mantra_( isRPN, 1, rets, 1);
  }
  int c1 = (int)floor( k) + 2;
  int c2 = (int)floor( n);
  n = floor( k) + 1.0;
  for( int i=c1; i<=c2; i++){
    n *= i;
    n /= -k + i;
  }
  return _vars->_Universal_Mantra_( isRPN, n, rets, 1);
}
