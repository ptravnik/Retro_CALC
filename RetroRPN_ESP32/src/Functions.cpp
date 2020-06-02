//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Functions.hpp"

//#define __DEBUG

//#define _FUN_AMODE_KW_ 0
const char _FUN_amode[] PROGMEM = "amode";
const char _FUN_AMODE[] PROGMEM = "AMODE";
//#define _FUN_SIN_KW_ 1
const char _FUN_sin[] PROGMEM = "sin";
const char _FUN_SIN[] PROGMEM = "SIN";
//#define _FUN_COS_KW_ 2
const char _FUN_cos[] PROGMEM = "cos";
const char _FUN_COS[] PROGMEM = "COS";
//#define _FUN_TAN_KW_ 3
const char _FUN_tan[] PROGMEM = "tan";
const char _FUN_TAN[] PROGMEM = "TAN";
//#define _FUN_POW_KW_ 4
const char _FUN_pow[] PROGMEM = "pow";
const char _FUN_POW[] PROGMEM = "POW";
//#define _FUN_NEG_KW_ 5
const char _FUN_neg[] PROGMEM = "neg";
const char _FUN_NEG[] PROGMEM = "NEG";
//#define _FUN_LN_KW_ 6
const char _FUN_ln[] PROGMEM = "ln";
const char _FUN_LN[] PROGMEM = "LN";
//#define _FUN_LG_KW_ 7
const char _FUN_lg[] PROGMEM = "lg";
const char _FUN_LG[] PROGMEM = "LG";
//#define _FUN_LOG_KW_ 8
const char _FUN_log[] PROGMEM = "log";
const char _FUN_LOG[] PROGMEM = "LOG";
//#define _FUN_EXP_KW_ 9
const char _FUN_exp[] PROGMEM = "exp";
const char _FUN_EXP[] PROGMEM = "EXP";
//#define _FUN_SQRT_KW_ 10
const char _FUN_sqrt[] PROGMEM = "sqrt";
const char _FUN_SQRT[] PROGMEM = "SQRT";
//#define _FUN_SQ_KW_ 11
const char _FUN_sq[] PROGMEM = "sq";
const char _FUN_SQ[] PROGMEM = "SQ";
//#define _FUN_ROOT_KW_ 12
const char _FUN_root[] PROGMEM = "root";
const char _FUN_ROOT[] PROGMEM = "ROOT";
//#define _FUN_SIGN_KW_ 13
const char _FUN_sign[] PROGMEM = "sign";
const char _FUN_SIGN[] PROGMEM = "SIGN";
//#define _FUN_ABS_KW_ 14
const char _FUN_abs[] PROGMEM = "abs";
const char _FUN_ABS[] PROGMEM = "ABS";
//#define _FUN_INV_KW_ 15
const char _FUN_inv[] PROGMEM = "inv";
const char _FUN_INV[] PROGMEM = "INV";
//#define _FUN_RADIUS_KW_ 16
const char _FUN_radius[] PROGMEM = "radius";
const char _FUN_RADIUS[] PROGMEM = "RADIUS";
//#define _FUN_QUAD_KW_ 17
const char _FUN_quad[] PROGMEM = "quad";
const char _FUN_QUAD[] PROGMEM = "QUAD";
//#define _FUN_SWAP_KW_ 18
const char _FUN_swap[] PROGMEM = "swap";
const char _FUN_SWAP[] PROGMEM = "SWAP";
//#define _FUN_LCIRC_KW_ 19
const char _FUN_lcirc[] PROGMEM = "lcirc";
const char _FUN_LCIRC[] PROGMEM = "LCIRC";
//#define _FUN_SCIRC_KW_ 20
const char _FUN_scirc[] PROGMEM = "scirc";
const char _FUN_SCIRC[] PROGMEM = "SCIRC";
//#define _FUN_VSPHERE_KW_ 21
const char _FUN_vsphere[] PROGMEM = "vsphere";
const char _FUN_VSPHERE[] PROGMEM = "VSPHERE";
//#define _FUN_SSPHERE_KW_ 22
const char _FUN_ssphere[] PROGMEM = "ssphere";
const char _FUN_SSPHERE[] PROGMEM = "SSPHERE";
//#define _FUN_ASIN_KW_ 23
const char _FUN_asin[] PROGMEM = "asin";
const char _FUN_ASIN[] PROGMEM = "ASIN";
//#define _FUN_ACOS_KW_ 24
const char _FUN_acos[] PROGMEM = "acos";
const char _FUN_ACOS[] PROGMEM = "ACOS";
//#define _FUN_ATAN_KW_ 25
const char _FUN_atan[] PROGMEM = "atan";
const char _FUN_ATAN[] PROGMEM = "ATAN";
//#define _FUN_GOFF2_KW_ 26
const char _FUN_goff2[] PROGMEM = "goff2";
const char _FUN_GOFF2[] PROGMEM = "GOFF2";
//#define _FUN_CATH_KW_ 27
const char _FUN_cath[] PROGMEM = "cath";
const char _FUN_CATH[] PROGMEM = "CATH";
//#define _FUN_STACK_KW_ 28
const char _FUN_stack[] PROGMEM = "stack";
const char _FUN_STACK[] PROGMEM = "STACK";
//#define _FUN_STACK_KW_ 29
const char _FUN_lin2[] PROGMEM = "lin2";
const char _FUN_LIN2[] PROGMEM = "LIN2";

static Function VariableFunction;

void Functions::init( void *components[]){
  _vars = (Variables *)components[UI_COMP_Variables];
  VariableFunction.id = _FUN_Variable_;
  _addFunction( _FUN_amode, _FUN_AMODE, 1, 0, _RPN_AMODE_); // 0
  _addFunction( _FUN_sin, _FUN_SIN, 1, 1, _RPN_CHECK_TRIG_); // 1
  _addFunction( _FUN_cos, _FUN_COS, 1, 1, _RPN_CHECK_TRIG_); // 2
  _addFunction( _FUN_tan, _FUN_TAN, 1, 1, _RPN_CHECK_TRIG_); // 3
  _addFunction( _FUN_pow, _FUN_POW, 2, 1, _RPN_POWER_); // 4
  _addFunction( _FUN_neg, _FUN_NEG, 1, 1); // 5
  _addFunction( _FUN_ln, _FUN_LN, 1, 1); // 6
  _addFunction( _FUN_lg, _FUN_LG, 1, 1); // 7
  _addFunction( _FUN_log, _FUN_LOG, 2, 1, _RPN_SWAP_XY_); // 8
  _addFunction( _FUN_exp, _FUN_EXP, 1, 1); // 9
  _addFunction( _FUN_sqrt, _FUN_SQRT, 1, 1, _RPN_SQRT_CHECK_); // 10
  _addFunction( _FUN_sq, _FUN_SQ, 1, 1); // 11
  _addFunction( _FUN_root, _FUN_ROOT, 2, 1, _RPN_ROOTYX_); // 12
  _addFunction( _FUN_sign, _FUN_SIGN, 1, 1); // 13
  _addFunction( _FUN_abs, _FUN_ABS, 1, 1); // 14
  _addFunction( _FUN_inv, _FUN_INV, 1, 1, _RPN_DIV0_CHECK_); // 15  
  _addFunction( _FUN_radius, _FUN_RADIUS, 2, 1); // 16
  _addFunction( _FUN_quad, _FUN_QUAD, 3, 3, _RPN_QUAD_SOLVER); // 17
  _addFunction( _FUN_swap, _FUN_SWAP, 2, 2, _RPN_SWAP_ONLY_); // 18
  _addFunction( _FUN_lcirc, _FUN_LCIRC, 1, 1); // 19
  _addFunction( _FUN_scirc, _FUN_SCIRC, 1, 1); // 20
  _addFunction( _FUN_vsphere, _FUN_VSPHERE, 1, 1); // 21
  _addFunction( _FUN_ssphere, _FUN_SSPHERE, 1, 1); // 22
  _addFunction( _FUN_asin, _FUN_ASIN, 1, 1, _RPN_INVTRIG_); // 23
  _addFunction( _FUN_acos, _FUN_ACOS, 1, 1, _RPN_INVTRIG_); // 24
  _addFunction( _FUN_atan, _FUN_ATAN, 1, 1); // 25
  _addFunction( _FUN_goff2, _FUN_GOFF2, 4, 2, _RPN_GOFF2_SOLVER); // 26
  _addFunction( _FUN_cath, _FUN_CATH, 2, 1); // 27
  _addFunction( _FUN_stack, _FUN_STACK, 1, 1); // 28
  _addFunction( _FUN_lin2, _FUN_LIN2, 1, 1); // 29
}

Function *Functions::getFunction(byte *str){
  Function *ptr;
  for(size_t i=0; i<_FUNCTION_COUNT; i++){
    ptr = _functions + i;
    if( IsKeyword(str, ptr->name0)) return ptr;
    if( IsKeyword(str, ptr->name1)) return ptr;
  }
  VariableToken vt = _vars->findByName( str);
  if( vt) return _setVariable( &VariableFunction, vt);
  return NULL; // not found
}

double *Functions::Compute( Function *mf, double *args){
  byte a = _MODE_DEGREES_;
  int iStack = 0;
  switch(mf->id){
    case -1:
      #ifdef __DEBUG
      Serial.print( "Variable found: ");
      Serial.println( (char *)mf->name0);
      #endif
      _rets[0] = _vars->realValue( mf->VarTag);
      break;
    case _FUN_AMODE_KW_: // amode (TODO: temporary)
      if( 0.5 <= args[0] && args[0] < 1.5) a = _MODE_RADIAN_; 
      if( 1.5 <= args[0] && args[0] < 2.5) a = _MODE_GRADIAN_; 
      _vars->setAngleMode( a);
      break;
    case _FUN_SIN_KW_:
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
    case _FUN_ASIN_KW_:
      _rets[0] = _vars->getUnconvertedAngle(asin(args[0]));
      break;
    case _FUN_COS_KW_:
      _rets[0] = cos( _vars->getConvertedAngle(args[0]));
      break;
    case _FUN_ACOS_KW_:
      _rets[0] = _vars->getUnconvertedAngle(acos(args[0]));
      break;
    case _FUN_TAN_KW_:
      _rets[0] = tan( _vars->getConvertedAngle(args[0]));
      break;
    case _FUN_ATAN_KW_:
      _rets[0] = _vars->getUnconvertedAngle(atan(args[0]));
      break;
    case _FUN_POW_KW_: 
      _rets[0] = pow(args[0], args[1]);
      break;
    case _FUN_NEG_KW_:
      _rets[0] = -args[0];
      break;
    case _FUN_LN_KW_:
      _rets[0] = log( args[0]);
      break;
    case _FUN_LG_KW_:
      _rets[0] = log10( args[0]);    
      break;
    case _FUN_LOG_KW_:
      _rets[0] = log10( args[0])/log10( args[1]);    
      break;
    case _FUN_EXP_KW_:
      _rets[0] = exp( args[0]);    
      break;
    case _FUN_SQRT_KW_:
      _rets[0] = sqrt( args[0]);    
      break;
    case _FUN_SQ_KW_:
      _rets[0] = args[0] * args[0];    
      break;
    case _FUN_ROOT_KW_:
      _rets[0] = pow(args[0], 1./args[1]);
      break;
    case _FUN_SIGN_KW_:
      _rets[0] = 0.0;
      if(args[0] == 0.0) break;
      if(args[0] > 0.0){
        _rets[0] = 1.0;
        break;
      }
      _rets[0] = -1.0;
      break;
    case _FUN_ABS_KW_:
      _rets[0] = abs(args[0]);
      break;
    case _FUN_INV_KW_:
      _rets[0] = 1/args[0];
      break;
    case _FUN_RADIUS_KW_:
      _rets[0] = sqrt( args[0]*args[0]+args[1]*args[1]);
      break;
    case _FUN_CATH_KW_:
      _rets[0] = sqrt( abs(args[0]*args[0]-args[1]*args[1]));
      break;
    case _FUN_QUAD_KW_:
      quad( args);
      break;
    case _FUN_SWAP_KW_:
      _rets[0] = args[1];
      _rets[1] = args[0];
      break;
    case _FUN_STACK_KW_:
      iStack = (int)floor(args[0]);
      if( iStack<0) iStack = 0;
      if( iStack>=RPN_STACK) iStack = RPN_STACK-1;
      _rets[0] = _vars->getRPNRegister(iStack);
      break;
    case _FUN_LCIRC_KW_:
      _rets[0] = _MATH_PI_ * 2.0 * args[0];
      break;
    case _FUN_SCIRC_KW_:
      _rets[0] = _MATH_PI_ * args[0] * args[0];
      break;
    case _FUN_VSPHERE_KW_:
      _rets[0] = _MATH_PI_ * 4.0 * args[0] * args[0] * args[0] / 3.0;
      break;
    case _FUN_SSPHERE_KW_:
      _rets[0] = _MATH_PI_ * 4.0 * args[0] * args[0];
      break;
    case _FUN_GOFF2_KW_:
      goff2( args);
      break;
    case _FUN_LIN2_KW_:
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
  if( _id >= _FUNCTION_COUNT) return;
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
// Sets constant or variable
//
Function *Functions::_setVariable( Function *f, VariableToken vt) {
  f->name0 = (const char *)_vars->getVarName( vt);
  f->name1 = f->name0;
  f->nArgs = 1;
  f->VarTag = vt;
  switch(_vars->getVarType( vt)){
    case VARTYPE_MATRIX:
      f->nArgs = 2;
      return f;
    case VARTYPE_NUMBER:
      f->nArgs = 0;
      return f;
    default:
      break;
    }
  return f;
}
