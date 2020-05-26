//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Functions.hpp"

//#define __DEBUG

//#define _MF_AMODE_KW_ 0
const char _MF_amode[] PROGMEM = "amode";
const char _MF_AMODE[] PROGMEM = "AMODE";
//#define _MF_SIN_KW_ 1
const char _MF_sin[] PROGMEM = "sin";
const char _MF_SIN[] PROGMEM = "SIN";
//#define _MF_COS_KW_ 2
const char _MF_cos[] PROGMEM = "cos";
const char _MF_COS[] PROGMEM = "COS";
//#define _MF_TAN_KW_ 3
const char _MF_tan[] PROGMEM = "tan";
const char _MF_TAN[] PROGMEM = "TAN";
//#define _MF_POW_KW_ 4
const char _MF_pow[] PROGMEM = "pow";
const char _MF_POW[] PROGMEM = "POW";
//#define _MF_NEG_KW_ 5
const char _MF_neg[] PROGMEM = "neg";
const char _MF_NEG[] PROGMEM = "NEG";
//#define _MF_LN_KW_ 6
const char _MF_ln[] PROGMEM = "ln";
const char _MF_LN[] PROGMEM = "LN";
//#define _MF_LG_KW_ 7
const char _MF_lg[] PROGMEM = "lg";
const char _MF_LG[] PROGMEM = "LG";
//#define _MF_LOG_KW_ 8
const char _MF_log[] PROGMEM = "log";
const char _MF_LOG[] PROGMEM = "LOG";
//#define _MF_EXP_KW_ 9
const char _MF_exp[] PROGMEM = "exp";
const char _MF_EXP[] PROGMEM = "EXP";
//#define _MF_SQRT_KW_ 10
const char _MF_sqrt[] PROGMEM = "sqrt";
const char _MF_SQRT[] PROGMEM = "SQRT";
//#define _MF_SQ_KW_ 11
const char _MF_sq[] PROGMEM = "sq";
const char _MF_SQ[] PROGMEM = "SQ";
//#define _MF_ROOT_KW_ 12
const char _MF_root[] PROGMEM = "root";
const char _MF_ROOT[] PROGMEM = "ROOT";
//#define _MF_SIGN_KW_ 13
const char _MF_sign[] PROGMEM = "sign";
const char _MF_SIGN[] PROGMEM = "SIGN";
//#define _MF_ABS_KW_ 14
const char _MF_abs[] PROGMEM = "abs";
const char _MF_ABS[] PROGMEM = "ABS";
//#define _MF_INV_KW_ 15
const char _MF_inv[] PROGMEM = "inv";
const char _MF_INV[] PROGMEM = "INV";
//#define _MF_RADIUS_KW_ 16
const char _MF_radius[] PROGMEM = "radius";
const char _MF_RADIUS[] PROGMEM = "RADIUS";
//#define _MF_QUAD_KW_ 17
const char _MF_quad[] PROGMEM = "quad";
const char _MF_QUAD[] PROGMEM = "QUAD";
//#define _MF_SWAP_KW_ 18
const char _MF_swap[] PROGMEM = "swap";
const char _MF_SWAP[] PROGMEM = "SWAP";
//#define _MF_LCIRC_KW_ 19
const char _MF_lcirc[] PROGMEM = "lcirc";
const char _MF_LCIRC[] PROGMEM = "LCIRC";
//#define _MF_SCIRC_KW_ 20
const char _MF_scirc[] PROGMEM = "scirc";
const char _MF_SCIRC[] PROGMEM = "SCIRC";
//#define _MF_VSPHERE_KW_ 21
const char _MF_vsphere[] PROGMEM = "vsphere";
const char _MF_VSPHERE[] PROGMEM = "VSPHERE";
//#define _MF_SSPHERE_KW_ 22
const char _MF_ssphere[] PROGMEM = "ssphere";
const char _MF_SSPHERE[] PROGMEM = "SSPHERE";
//#define _MF_ASIN_KW_ 23
const char _MF_asin[] PROGMEM = "asin";
const char _MF_ASIN[] PROGMEM = "ASIN";
//#define _MF_ACOS_KW_ 24
const char _MF_acos[] PROGMEM = "acos";
const char _MF_ACOS[] PROGMEM = "ACOS";
//#define _MF_ATAN_KW_ 25
const char _MF_atan[] PROGMEM = "atan";
const char _MF_ATAN[] PROGMEM = "ATAN";
//#define _MF_GOFF2_KW_ 26
const char _MF_goff2[] PROGMEM = "goff2";
const char _MF_GOFF2[] PROGMEM = "GOFF2";
//#define _MF_CATH_KW_ 27
const char _MF_cath[] PROGMEM = "cath";
const char _MF_CATH[] PROGMEM = "CATH";
//#define _MF_STACK_KW_ 28
const char _MF_stack[] PROGMEM = "stack";
const char _MF_STACK[] PROGMEM = "STACK";
//#define _MF_STACK_KW_ 29
const char _MF_lin2[] PROGMEM = "lin2";
const char _MF_LIN2[] PROGMEM = "LIN2";

static Function VariableFunction;
static Function ConstantFunction;

void Functions::init( void *components[]){
  _vars = (Variables *)components[UI_COMP_Variables];
  VariableFunction.id = -1;
  ConstantFunction.id = -2;
  _addFunction( _MF_amode, _MF_AMODE, 1, 0, _RPN_AMODE_); // 0
  _addFunction( _MF_sin, _MF_SIN, 1, 1, _RPN_CHECK_TRIG_); // 1
  _addFunction( _MF_cos, _MF_COS, 1, 1, _RPN_CHECK_TRIG_); // 2
  _addFunction( _MF_tan, _MF_TAN, 1, 1, _RPN_CHECK_TRIG_); // 3
  _addFunction( _MF_pow, _MF_POW, 2, 1, _RPN_POWER_); // 4
  _addFunction( _MF_neg, _MF_NEG, 1, 1); // 5
  _addFunction( _MF_ln, _MF_LN, 1, 1); // 6
  _addFunction( _MF_lg, _MF_LG, 1, 1); // 7
  _addFunction( _MF_log, _MF_LOG, 2, 1, _RPN_SWAP_XY_); // 8
  _addFunction( _MF_exp, _MF_EXP, 1, 1); // 9
  _addFunction( _MF_sqrt, _MF_SQRT, 1, 1, _RPN_SQRT_CHECK_); // 10
  _addFunction( _MF_sq, _MF_SQ, 1, 1); // 11
  _addFunction( _MF_root, _MF_ROOT, 2, 1, _RPN_ROOTYX_); // 12
  _addFunction( _MF_sign, _MF_SIGN, 1, 1); // 13
  _addFunction( _MF_abs, _MF_ABS, 1, 1); // 14
  _addFunction( _MF_inv, _MF_INV, 1, 1, _RPN_DIV0_CHECK_); // 15  
  _addFunction( _MF_radius, _MF_RADIUS, 2, 1); // 16
  _addFunction( _MF_quad, _MF_QUAD, 3, 3, _RPN_QUAD_SOLVER); // 17
  _addFunction( _MF_swap, _MF_SWAP, 2, 2, _RPN_SWAP_ONLY_); // 18
  _addFunction( _MF_lcirc, _MF_LCIRC, 1, 1); // 19
  _addFunction( _MF_scirc, _MF_SCIRC, 1, 1); // 20
  _addFunction( _MF_vsphere, _MF_VSPHERE, 1, 1); // 21
  _addFunction( _MF_ssphere, _MF_SSPHERE, 1, 1); // 22
  _addFunction( _MF_asin, _MF_ASIN, 1, 1, _RPN_INVTRIG_); // 23
  _addFunction( _MF_acos, _MF_ACOS, 1, 1, _RPN_INVTRIG_); // 24
  _addFunction( _MF_atan, _MF_ATAN, 1, 1); // 25
  _addFunction( _MF_goff2, _MF_GOFF2, 4, 2, _RPN_GOFF2_SOLVER); // 26
  _addFunction( _MF_cath, _MF_CATH, 2, 1); // 27
  _addFunction( _MF_stack, _MF_STACK, 1, 1); // 28
  _addFunction( _MF_lin2, _MF_LIN2, 1, 1); // 29
}

Function *Functions::getFunction(byte *str){
  Function *ptr;
  for(size_t i=0; i<NMATH_FUNCTIONS; i++){
    ptr = _functions + i;
    if( IsKeyword(str, ptr->name0)) return ptr;
    if( IsKeyword(str, ptr->name1)) return ptr;
  }
  VariableToken vt = _vars->getConstant( str);
  if( vt) return _setVariable( &ConstantFunction, vt);
  vt = _vars->getVariable( str);
  if( vt) return _setVariable( &VariableFunction, vt);
  return NULL; // not found
}

double *Functions::Compute( Function *mf, double *args){
  byte a = _MODE_DEGREES_;
  int iStack = 0;
  switch(mf->id){
    case -1:
      Serial.print( "Variable found: ");
      Serial.println( (char *)mf->name0);
      _rets[0] = _vars->realValue( mf->VarTag);
      break;
    case -2:
      #ifdef __DEBUG
      Serial.print( "Constant found: ");
      Serial.println( (char *)mf->name0);
      #endif
      _rets[0] = _vars->realValue( mf->VarTag);
      break;
    case _MF_AMODE_KW_: // amode (TODO: temporary)
      if( 0.5 <= args[0] && args[0] < 1.5) a = _MODE_RADIAN_; 
      if( 1.5 <= args[0] && args[0] < 2.5) a = _MODE_GRADIAN_; 
      _vars->setAngleMode( a);
      break;
    case _MF_SIN_KW_:
      #ifdef __DEBUG
      Serial.print("Computing sin(");
      Serial.print(args[0]);
      Serial.print("), which is sin(");
      Serial.print(_vars->getConvertedAngle(args[0]));
      Serial.print(" radian) = ");
      Serial.println(sin( _vars->getConvertedAngle(args[0])));
      #endif
      _rets[0] = sin( _vars->getConvertedAngle(args[0]));
      break;
    case _MF_ASIN_KW_:
      _rets[0] = _vars->getUnconvertedAngle(asin(args[0]));
      break;
    case _MF_COS_KW_:
      _rets[0] = cos( _vars->getConvertedAngle(args[0]));
      break;
    case _MF_ACOS_KW_:
      _rets[0] = _vars->getUnconvertedAngle(acos(args[0]));
      break;
    case _MF_TAN_KW_:
      _rets[0] = tan( _vars->getConvertedAngle(args[0]));
      break;
    case _MF_ATAN_KW_:
      _rets[0] = _vars->getUnconvertedAngle(atan(args[0]));
      break;
    case _MF_POW_KW_: 
      _rets[0] = pow(args[0], args[1]);
      break;
    case _MF_NEG_KW_:
      _rets[0] = -args[0];
      break;
    case _MF_LN_KW_:
      _rets[0] = log( args[0]);
      break;
    case _MF_LG_KW_:
      _rets[0] = log10( args[0]);    
      break;
    case _MF_LOG_KW_:
      _rets[0] = log10( args[0])/log10( args[1]);    
      break;
    case _MF_EXP_KW_:
      _rets[0] = exp( args[0]);    
      break;
    case _MF_SQRT_KW_:
      _rets[0] = sqrt( args[0]);    
      break;
    case _MF_SQ_KW_:
      _rets[0] = args[0] * args[0];    
      break;
    case _MF_ROOT_KW_:
      _rets[0] = pow(args[1], 1./args[0]);
      break;
    case _MF_SIGN_KW_:
      _rets[0] = 0.0;
      if(args[0] == 0.0) break;
      if(args[0] > 0.0){
        _rets[0] = 1.0;
        break;
      }
      _rets[0] = -1.0;
      break;
    case _MF_ABS_KW_:
      _rets[0] = abs(args[0]);
      break;
    case _MF_INV_KW_:
      _rets[0] = 1/args[0];
      break;
    case _MF_RADIUS_KW_:
      _rets[0] = sqrt( args[0]*args[0]+args[1]*args[1]);
      break;
    case _MF_CATH_KW_:
      _rets[0] = sqrt( abs(args[0]*args[0]-args[1]*args[1]));
      break;
    case _MF_QUAD_KW_:
      quad( args);
      break;
    case _MF_SWAP_KW_:
      _rets[0] = args[1];
      _rets[1] = args[0];
      break;
    case _MF_STACK_KW_:
      iStack = (int)floor(args[0]);
      if( iStack<0) iStack = 0;
      if( iStack>=RPN_STACK) iStack = RPN_STACK-1;
      _rets[0] = _vars->rpnGetStack(iStack);
      break;
    case _MF_LCIRC_KW_:
      _rets[0] = _MATH_PI_ * 2.0 * args[0];
      break;
    case _MF_SCIRC_KW_:
      _rets[0] = _MATH_PI_ * args[0] * args[0];
      break;
    case _MF_VSPHERE_KW_:
      _rets[0] = _MATH_PI_ * 4.0 * args[0] * args[0] * args[0] / 3.0;
      break;
    case _MF_SSPHERE_KW_:
      _rets[0] = _MATH_PI_ * 4.0 * args[0] * args[0];
      break;
    case _MF_GOFF2_KW_:
      goff2( args);
      break;
    case _MF_LIN2_KW_:
      _rets[0] = _vars->gain[0] * args[0] + _vars->offset[0];
      break;
    default:
      break;
  }
  return _rets;
}
double *Functions::Compute( Function *mf, double arg){
  return Compute( mf, &arg);  
}
double *Functions::Compute( Function *mf){
  double arg = 0.0;
  return Compute( mf, &arg);  
}

void Functions::_addFunction( const char *name0, const char *name1, byte nArgs, byte nRets, byte RPNtag){
  if( _id >= NMATH_FUNCTIONS) return;
  Function *tmp = _functions+_id;
  tmp->id = _id++;
  tmp->name0 = name0;
  tmp->name1 = name1;
  tmp->nArgs = nArgs;
  tmp->nRets = nRets;
  tmp->RPNtag = RPNtag;
  #ifdef __DEBUG
  Serial.print( tmp->id+1);
  Serial.print(" added function ");
  Serial.print( tmp->name0);
  Serial.print(", AKA ");
  Serial.print( tmp->name1);
  Serial.print( ", with ");
  Serial.print( tmp->nArgs);
  Serial.print( " args and ");
  Serial.print( tmp->nRets);
  Serial.println( " rets.");
  #endif
}

//
// Solves a quadratic equation
//
double *Functions::quad( double *stack) {
  // Trivial solution or no roots
  _clearRets();
  if( stack[2] == 0.0 && stack[1] == 0.0)
    return _rets;
  double a = stack[2] * 2.0;
  // Linear single root
  if( a == 0.0){
    _rets[0] = -stack[0] / stack[1];
    _rets[1] = _rets[0];
    return _rets;
  }
  // Discriminant
  _rets[2] = stack[1] * stack[1] - 4.0 * stack[0] * stack[2];
  // Quad single root
  if( -1e-300 < _rets[2] && _rets[2] < 1e-300){
    _rets[2] = 0.0;
    _rets[0] = -stack[1] / a;
    _rets[1] = _rets[0];
    return _rets;
  }
  // Quad two roots
  if( _rets[2] > 0.0){
    _rets[0] = sqrt(_rets[2]);
    _rets[1] = -(stack[1] +_rets[0]) / a;
    _rets[0] -= stack[1];
    _rets[0] /= a;
    return _rets;
  }
  // Complex roots
  _rets[1] = sqrt(-_rets[2]) / a;
  _rets[0] -= stack[1] / a;
  return _rets;
}

//
// Solves gain-offset
//
double *Functions::goff2( double *stack) {
  // Trivial solution with the same X1, X2
  _clearRets();
  double dx = stack[3] - stack[1];
  if( dx == 0.0) return _rets;
  _rets[1] = (stack[2] - stack[0])/dx;
  _rets[0] = stack[0] - _rets[1]*stack[1];
  _vars->gain[0] = _rets[1];
  _vars->offset[0] = _rets[0];
  return _rets;
}

//
// Sets constant o variable
//
Function *Functions::_setVariable( Function *f, VariableToken vt) {
    f->name0 = _vars->getVarName( vt);
    f->name1 = f->name0;
    f->nArgs = 0;
    f->VarTag = vt;
    //if( _vars->getVarType() == VARTYPE_MATRIX) f->nArgs = 2;
    //if( _vars->getVarType() == VARTYPE_NUMBER) f->nArgs = 0;
  return f;
}
