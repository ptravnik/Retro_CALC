//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Functions.hpp"

//#define __DEBUG

// common function messages
const char FUN_Error_Overflow[] PROGMEM = "Err: Too large";
const char FUN_Error_Underflow[] PROGMEM = "Err: Too small";
const char FUN_Error_Argument[] PROGMEM = "Err: Argument";
const char FUN_Error_Domain[] PROGMEM = "Err: Domain";
const char FUN_Error_InverseTrig[] PROGMEM = "Err: |X|>1";
const char FUN_Error_NaN[] PROGMEM = "Err: NaN";
const char FUN_Error_DivZero[] PROGMEM = "Err: Division by 0";
const char FUN_Error_Gain_Offset[] PROGMEM = "Err: Same X1 and X2";
const char FUN_Warning_Inaccurate[] PROGMEM = "Warn: Inaccurate";
const char FUN_Warning_ZeroPowerZero[] PROGMEM = "Warn: 0^0";
const char FUN_Warning_Complex[] PROGMEM = "Complex: \xb1iX";
const char FUN_Message_Trivial[] PROGMEM = "Trivial";
const char FUN_Message_NoRoots[] PROGMEM = "No roots";
const char FUN_Message_OneRoot[] PROGMEM = "Single root";
const char FUN_Message_TwoRoots[] PROGMEM = "Two roots";
const char FUN_Message_ComplexRoots[] PROGMEM = "Complex: X\xb1iY";
const char FUN_Message_Discriminant[] PROGMEM = "Discriminant";
const char FUN_Message_Root1[] PROGMEM = "Root 1";
const char FUN_Message_Root2[] PROGMEM = "Root 2";
const char FUN_Message_ComplexPart[] PROGMEM = "Complex part";
const char FUN_Message_RealPart[] PROGMEM = "Real part";
const char FUN_Message_Gain[] PROGMEM = "Gain:";
const char FUN_Message_Offset[] PROGMEM = "Offset:";
const char FUN_Message_Goff_Solution[] PROGMEM = "Y=Gain*X+Offset";

// add function includes here
#include "functions_RPN.hpp"
#include "functions_Math.hpp"
#include "functions_Solvers.hpp"

static Function VariableFunction;

void Functions::init( void *components[]){
  _kwds = (Keywords *)components[UI_COMP_Keywords];
  _vars = (Variables *)components[UI_COMP_Variables];
  VariableFunction.id = _FUN_Variable_;
  _setFunction( _OPR_ABS_KW, (void *)_function_Math_ABS_, 1);
  _setFunction( _OPR_ACOS_KW, (void *)_function_Math_ACOS_, 1);
  _setFunction( _OPR_ACOSH_KW, (void *)_function_Math_ACOSH_, 1);
  _setFunction( _OPR_ADD_KW, (void *)_function_RPN_ADD_, 0);
  _setFunction( _OPR_ASIN_KW, (void *)_function_Math_ASIN_, 1);
  _setFunction( _OPR_ASINH_KW, (void *)_function_Math_ASINH_, 1);
  _setFunction( _OPR_ATAN_KW, (void *)_function_Math_ATAN_, 1);
  _setFunction( _OPR_ATANH_KW, (void *)_function_Math_ATANH_, 1);
  _setFunction( _OPR_CATH_KW, (void *)_function_Solver_CATH_, 2);
  _setFunction( _OPR_COS_KW, (void *)_function_Math_COS_, 1);
  _setFunction( _OPR_COSH_KW, (void *)_function_Math_COSH_, 1);
  _setFunction( _OPR_COT_KW, (void *)_function_Math_COT_, 1);
  _setFunction( _OPR_DIV_KW, (void *)_function_RPN_DIV_, 0);
  _setFunction( _OPR_EXP_KW, (void *)_function_Math_EXP_, 1);
  _setFunction( _OPR_GOFF2_KW, (void *)_function_Solver_GOFF2_, 0);
  _setFunction( _OPR_LCIRC_KW, (void *)_function_Solver_LCIRC_, 1);
  _setFunction( _OPR_LIN2_KW, (void *)_function_Solver_LIN2_, 0);
  _setFunction( _OPR_LG_KW, (void *)_function_Math_LG_, 1);
  _setFunction( _OPR_LN_KW, (void *)_function_Math_LN_, 1);
  _setFunction( _OPR_LOG_KW, (void *)_function_Math_LOG_, 2);
  _setFunction( _OPR_NDIS_KW, (void *)_function_Math_NDIS_, 1);
  _setFunction( _OPR_NEG_KW, (void *)_function_RPN_NEG_, 0);
  _setFunction( _OPR_MUL_KW, (void *)_function_RPN_MUL_, 0);
  _setFunction( _OPR_OOX_KW, (void *)_function_RPN_OOX_, 0);
  _setFunction( _OPR_POP_KW, (void *)_function_RPN_POP_, 0);
  _setFunction( _OPR_POW_KW, (void *)_function_RPN_POW_, 2);
  _setFunction( _OPR_QUAD_KW, (void *)_function_Solver_QUAD_, 0);
  _setFunction( _OPR_RADIUS_KW, (void *)_function_Solver_RADIUS_, 2);
  _setFunction( _OPR_ROLL_KW, (void *)_function_RPN_ROLL_, 0);
  _setFunction( _OPR_ROOT_KW, (void *)_function_RPN_ROOT_, 2);
  _setFunction( _OPR_SCIRC_KW, (void *)_function_Solver_SCIRC_, 1);
  _setFunction( _OPR_SIN_KW, (void *)_function_Math_SIN_, 1);
  _setFunction( _OPR_SIGN_KW, (void *)_function_Math_SIGN_, 1);
  _setFunction( _OPR_SINH_KW, (void *)_function_Math_SINH_, 1);
  _setFunction( _OPR_SQ_KW, (void *)_function_Math_SQ_, 1);
  _setFunction( _OPR_SQRT_KW, (void *)_function_Math_SQRT_, 1);
  _setFunction( _OPR_SSPHERE_KW, (void *)_function_Solver_SSPHERE_, 1);
  _setFunction( _OPR_STACK_KW, (void *)_function_RPN_STACK_, 1);
  _setFunction( _OPR_SUB_KW, (void *)_function_RPN_SUB_, 0);
  _setFunction( _OPR_SWAP_KW, (void *)_function_RPN_SWAP_, 0);
  _setFunction( _OPR_TAN_KW, (void *)_function_Math_TAN_, 1);
  _setFunction( _OPR_TANH_KW, (void *)_function_Math_TANH_, 1);
  _setFunction( _OPR_VSPHERE_KW, (void *)_function_Solver_VSPHERE_, 1);

  _addFunction( _OPR_ABS_KW, 1);
  _addFunction( _OPR_ACOS_KW, 1);
  _addFunction( _OPR_ACOSH_KW, 1);
  _addFunction( _OPR_ASIN_KW, 1);
  _addFunction( _OPR_ASINH_KW, 1);
  _addFunction( _OPR_ATAN_KW, 1);
  _addFunction( _OPR_ATANH_KW, 1);
  _addFunction( _OPR_CATH_KW, 2);
  _addFunction( _OPR_COS_KW, 1);
  _addFunction( _OPR_COSH_KW, 1);

  _addFunction( _OPR_COT_KW, 1);
  _addFunction( _OPR_EXP_KW, 1);
  _addFunction( _OPR_LCIRC_KW, 1);
  _addFunction( _OPR_LIN2_KW, 1);
  _addFunction( _OPR_LG_KW, 1);
  _addFunction( _OPR_LN_KW, 1);
  _addFunction( _OPR_LOG_KW, 1);
  _addFunction( _OPR_NDIS_KW, 1);
  _addFunction( _OPR_POW_KW,2);
  _addFunction( _OPR_RADIUS_KW,2);

  _addFunction( _OPR_ROOT_KW,2);
  _addFunction( _OPR_SCIRC_KW, 1);
  _addFunction( _OPR_SIN_KW, 1);
  _addFunction( _OPR_SIGN_KW, 1);
  _addFunction( _OPR_SINH_KW, 1);
  _addFunction( _OPR_SQ_KW, 1);
  _addFunction( _OPR_SQRT_KW, 1);
  _addFunction( _OPR_SSPHERE_KW, 1);
  _addFunction( _OPR_STACK_KW, 1);
  _addFunction( _OPR_TAN_KW, 1);
  
  _addFunction( _OPR_TANH_KW, 1);
  _addFunction( _OPR_VSPHERE_KW, 1);
}

Function *Functions::getFunction(byte *str){
  Function *ptr;
  Keyword *kwd = _kwds->getKeyword( str);
  if( kwd == NULL){
    VariableToken vt = _vars->findByName( str);
    if( vt) return _setVariable( &VariableFunction, vt);
    return NULL; // not found
  }
  #ifdef __DEBUG
  Serial.print("Keyword found: ");
  Serial.println( kwd->name0);
  #endif
  for(size_t i=0; i<_FUNCTION_COUNT; i++){
    ptr = _functions + i;
    if( ptr->kwid == kwd->id) return ptr;
  }
  #ifdef __DEBUG
  Serial.print("Function not found: ");
  Serial.println( kwd->id);
  #endif
  return NULL; // not found
}

byte Functions::Compute( Function *mf, double *args){
  #ifdef __DEBUG
  Serial.println( "Compute called");
  #endif
  Keyword *kwd;
  byte (*myFunction)(Variables *, double *, double *, bool);
  byte fReturn = 0;
  int iStack = 0;
  if(mf->id < 0){
    #ifdef __DEBUG
    Serial.print( "Variable found: ");
    Serial.println( (char *)mf->name);
    #endif
    _rets[0] = _vars->realValue( mf->VarTag);
    return 0;
  }
  #ifdef __DEBUG
  Serial.print( "Case called: ");
  Serial.println( mf->kwid);
  #endif
  kwd = _kwds->getKeywordById( mf->kwid);
  if( !kwd) return 1;
  #ifdef __DEBUG
  Serial.println( "Keyword found (2)");
  #endif
  if( !kwd->operator_ptr) return 1;
  myFunction = (byte (*)(Variables *, double *, double *, bool))(kwd->operator_ptr);
  fReturn = myFunction( _vars, args, _rets, false);
  #ifdef __DEBUG
  Serial.print("Ref function");
  Serial.print( (char*)kwd->name0);
  Serial.print( "(");
  Serial.print(args[0]);
  Serial.print(") = ");
  Serial.println(_rets[0]);
  #endif
  if( _vars->mathError < _ERROR_) return 0;
  _rets[0] = -999.25;
  return 1;
}

byte Functions::ComputeRPN( Keyword *kw){
  if( kw->operator_ptr == NULL){
    Serial.print("Not implemented: ");
    Serial.println( (char *)kw->name0);
    return 0;
  }
  byte (*myFunction)(Variables *, double *, double *, bool) =
      (byte (*)(Variables *, double *, double *, bool))(kw->operator_ptr);
  double *args = _vars->getRPNStackPtr();
  return myFunction( _vars, args, args, true);
}

void Functions::_addFunction( int16_t kwid, byte nArgs, byte RPNtag){
  if( _id >= _FUNCTION_COUNT) return;
  Function *tmp = _functions+_id;
  tmp->id = _id++;
  tmp->kwid = kwid;
  tmp->nArgs = nArgs;
  tmp->RPNtag = RPNtag;
  #ifdef __DEBUG
  Keyword *kwd = _kwds->getKeywordById( kwid);
  Serial.print( tmp->id+1);
  Serial.print(" added function ");
  Serial.print( kwd->name0);
  Serial.print(", AKA ");
  Serial.print( kwd->name1);
  Serial.print( ", with ");
  Serial.print( tmp->nArgs);
  Serial.println( " argument(s)");
  #endif
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
  f->name = (const char *)_vars->getVarName( vt);
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
