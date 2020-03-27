//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "MathFunctions.hpp"

//#define __DEBUG

const double _angleConversions[] PROGMEM = {1.74532925199e-2, 1.0, 1.5707963268e-2};

const char _MF_pi[] PROGMEM = "pi";
const char _MF_PI[] PROGMEM = "PI";
const char _MF_sin[] PROGMEM = "sin";
const char _MF_SIN[] PROGMEM = "SIN";
const char _MF_amode[] PROGMEM = "amode";
const char _MF_AMODE[] PROGMEM = "AMODE";
const char _MF_deg[] PROGMEM = "deg";
const char _MF_DEG[] PROGMEM = "DEG";
const char _MF_rad[] PROGMEM = "rad";
const char _MF_RAD[] PROGMEM = "RAD";
const char _MF_grad[] PROGMEM = "grad";
const char _MF_GRAD[] PROGMEM = "GRAD";
const char _MF_pow[] PROGMEM = "pow";
const char _MF_POW[] PROGMEM = "POW";

void MathFunctions::init( byte amod){
  angleMode = amod;
  _addFunction( _MF_pi, _MF_PI, 0, 1);
  _addFunction( _MF_sin, _MF_SIN, 1, 1, _RPN_CHECK_TRIG_);
  _addFunction( _MF_amode, _MF_AMODE, 1, 0);
  _addFunction( _MF_deg, _MF_DEG, 0, 1);
  _addFunction( _MF_rad, _MF_RAD, 0, 1);
  _addFunction( _MF_grad, _MF_GRAD, 0, 1);
  _addFunction( _MF_pow, _MF_POW, 2, 1, _RPN_SWAP_XY_);
}

void MathFunctions::setAngleMode(byte m){
  if(m != _MODE_RADIAN_ && m != _MODE_GRADIAN_) m = _MODE_DEGREES_;
  angleMode = m;
}

MathFunction *MathFunctions::getFunction(byte *str){
  MathFunction *ptr;
  for(size_t i=0; i<NMATH_FUNCTIONS; i++){
    ptr = _mf + i;
    if( IsKeyword(str, ptr->name0)) return ptr;
    if( IsKeyword(str, ptr->name1)) return ptr;
  }
  return NULL; // not found
}

double *MathFunctions::Compute( MathFunction *mf, double *args){
  switch(mf->id){
    case 0: // pi
      _rets[0] = _MATH_PI_;
      break;
    case 1: // sin
      #ifdef __DEBUG
      Serial.print("Computing sin(");
      Serial.print(args[0]);
      Serial.print("), which is sin(");
      Serial.print(getConvertedAngle(args[0]));
      Serial.print(" radian) = ");
      Serial.println(sin( getConvertedAngle(args[0])));
      #endif
      _rets[0] = sin( getConvertedAngle(args[0]));
      break;
    case 2: // amode (temporary)
      setAngleMode((byte)args[0]);
      break;
    case 3: // grd
      _rets[0] = 0.0;
      break;
    case 4: // rad
      _rets[0] = 1.0;
      break;
    case 5: // grad
      _rets[0] = 2.0;
      break;
    case 6: // pow
      _rets[0] = pow(args[0], args[1]);
      break;
    default:
      break;
  }
  return _rets;
}
double *MathFunctions::Compute( MathFunction *mf, double arg){
  return Compute( mf, &arg);  
}
double *MathFunctions::Compute( MathFunction *mf){
  double arg = 0.0;
  return Compute( mf, &arg);  
}

void MathFunctions::_addFunction( const char *name0, const char *name1, byte nArgs, byte nRets, byte RPNtag){
  if( _id >= NMATH_FUNCTIONS) return;
  MathFunction *tmp = _mf+_id;
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

double MathFunctions::getConvertedAngle( double a){
  return a * _angleConversions[angleMode];
}
