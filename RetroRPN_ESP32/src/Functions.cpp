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
const char FUN_Message_Northing[] PROGMEM = "Northing:";
const char FUN_Message_Easting[] PROGMEM = "Easting:";
const char FUN_Message_Azimuth[] PROGMEM = "Azimuth:";
const char FUN_Message_Elevation[] PROGMEM = "Elevation:";
const char FUN_Message_AboveHorizon[] PROGMEM = "Above horizon:";
const char FUN_Message_Range[] PROGMEM = "Range:";
const char FUN_Message_DMMSS[] PROGMEM = "DDDMM.SS";

// add function includes here
#include "functions_RPN.hpp"
#include "functions_Math.hpp"
#include "functions_Solvers.hpp"
#include "functions_Converters.hpp"

static Function VariableFunction;

void Functions::init( void *components[]){
  _kwds = (Keywords *)components[UI_COMP_Keywords];
  _vars = (Variables *)components[UI_COMP_Variables];
  VariableFunction.id = _FUN_Variable_;
  _setFunction( _OPR_ABS_KW, (void *)_function_Math_ABS_, 1);
  _setFunction( _OPR_ACOS_KW, (void *)_function_Math_ACOS_, 1);
  _setFunction( _OPR_ACOSH_KW, (void *)_function_Math_ACOSH_, 1);
  _setFunction( _OPR_ADD_KW, (void *)_function_RPN_ADD_, 0);
  _setFunction( _OPR_AIN_KW, (void *)_function_RPN_AIN_, 2);
  _setFunction( _OPR_API2GCC_KW, (void *)_function_Convert_API2GCC_, 1);
  _setFunction( _OPR_ASIN_KW, (void *)_function_Math_ASIN_, 1);
  _setFunction( _OPR_ASINH_KW, (void *)_function_Math_ASINH_, 1);
  _setFunction( _OPR_ATAN_KW, (void *)_function_Math_ATAN_, 1);
  _setFunction( _OPR_ATANH_KW, (void *)_function_Math_ATANH_, 1);
  _setFunction( _OPR_AZIMUTH_KW, (void *)_function_Solver_AZIMUTH_, 2);
  _setFunction( _OPR_AZIMUTH3_KW, (void *)_function_Solver_AZIMUTH3_, 2);
  
  _setFunction( _OPR_BAR2PSI_KW, (void *)_function_Convert_BAR2PSI_, 1);
  _setFunction( _OPR_BBL2CM_KW, (void *)_function_Convert_BBL2CM_, 1);
  
  _setFunction( _OPR_C2F_KW, (void *)_function_Convert_C2F_, 1);
  _setFunction( _OPR_C2K_KW, (void *)_function_Convert_C2K_, 1);
  _setFunction( _OPR_CATH_KW, (void *)_function_Solver_CATH_, 2);
  _setFunction( _OPR_CF2CM_KW, (void *)_function_Convert_CF2CM_, 1);
  _setFunction( _OPR_CM2BBL_KW, (void *)_function_Convert_CM2BBL_, 1);
  _setFunction( _OPR_CM2CF_KW, (void *)_function_Convert_CM2CF_, 1);
  _setFunction( _OPR_CNK_KW, (void *)_function_Solver_CNK_, 2);
  _setFunction( _OPR_COS_KW, (void *)_function_Math_COS_, 1);
  _setFunction( _OPR_COORDS_KW, (void *)_function_Solver_COORDS_, 2);
  _setFunction( _OPR_COORDS3_KW, (void *)_function_Solver_COORDS3_, 3);
  _setFunction( _OPR_COSH_KW, (void *)_function_Math_COSH_, 1);
  _setFunction( _OPR_COT_KW, (void *)_function_Math_COT_, 1);
  
  _setFunction( _OPR_D2DMS_KW, (void *)_function_Convert_D2DMS_, 1);
  _setFunction( _OPR_DIV_KW, (void *)_function_RPN_DIV_, 0);
  _setFunction( _OPR_DMS2D_KW, (void *)_function_Convert_DMS2D_, 1);
  
  _setFunction( _OPR_EXP_KW, (void *)_function_Math_EXP_, 1);
  
  _setFunction( _OPR_F2C_KW, (void *)_function_Convert_F2C_, 1);
  _setFunction( _OPR_FACT_KW, (void *)_function_Solver_FACT_, 1);
  _setFunction( _OPR_FT2M_KW, (void *)_function_Convert_FT2M_, 1);
  
  _setFunction( _OPR_GAL2L_KW, (void *)_function_Convert_GAL2L_, 1);
  _setFunction( _OPR_GCC2API_KW, (void *)_function_Convert_GCC2API_, 1);
  _setFunction( _OPR_GOFF2_KW, (void *)_function_Solver_GOFF2_, 0);

  _setFunction( _OPR_IN2MM_KW, (void *)_function_Convert_IN2MM_, 1);
  
  _setFunction( _OPR_K2C_KW, (void *)_function_Convert_K2C_, 1);
  _setFunction( _OPR_KG2LBS_KW, (void *)_function_Convert_KG2LBS_, 1);
  _setFunction( _OPR_KM2MI_KW, (void *)_function_Convert_KM2MI_, 1);
  _setFunction( _OPR_KM2NM_KW, (void *)_function_Convert_KM2NM_, 1);
  _setFunction( _OPR_KMH2MS_KW, (void *)_function_Convert_KMH2MS_, 1);
  
  _setFunction( _OPR_L2GAL_KW, (void *)_function_Convert_L2GAL_, 1);
  _setFunction( _OPR_LBS2KG_KW, (void *)_function_Convert_LBS2KG_, 1);
  _setFunction( _OPR_LCIRC_KW, (void *)_function_Solver_LCIRC_, 1);
  _setFunction( _OPR_LG_KW, (void *)_function_Math_LG_, 1);
  _setFunction( _OPR_LIN2_KW, (void *)_function_Solver_LIN2_, 0);
  _setFunction( _OPR_LN_KW, (void *)_function_Math_LN_, 1);
  _setFunction( _OPR_LOG_KW, (void *)_function_Math_LOG_, 2);
  _setFunction( _OPR_LT2MT_KW, (void *)_function_Convert_LT2MT_, 1);

  _setFunction( _OPR_M2FT_KW, (void *)_function_Convert_M2FT_, 1);
  _setFunction( _OPR_MI2KM_KW, (void *)_function_Convert_MI2KM_, 1);
  _setFunction( _OPR_MM2IN_KW, (void *)_function_Convert_MM2IN_, 1);
  _setFunction( _OPR_MPA2PSI_KW, (void *)_function_Convert_MPA2PSI_, 1);
  _setFunction( _OPR_MS2KMH_KW, (void *)_function_Convert_MS2KMH_, 1);
  _setFunction( _OPR_MT2LT_KW, (void *)_function_Convert_MT2LT_, 1);
  _setFunction( _OPR_MT2ST_KW, (void *)_function_Convert_MT2ST_, 1);
  _setFunction( _OPR_MUL_KW, (void *)_function_RPN_MUL_, 0);
  
  _setFunction( _OPR_NDIS_KW, (void *)_function_Math_NDIS_, 1);
  _setFunction( _OPR_NM2KM_KW, (void *)_function_Convert_NM2KM_, 1);
  _setFunction( _OPR_NEG_KW, (void *)_function_RPN_NEG_, 0);

  _setFunction( _OPR_OOX_KW, (void *)_function_RPN_OOX_, 0);

  _setFunction( _OPR_PA2PSI_KW, (void *)_function_Convert_PA2PSI_, 1);
  _setFunction( _OPR_POP_KW, (void *)_function_RPN_POP_, 0);
  _setFunction( _OPR_POW_KW, (void *)_function_RPN_POW_, 2);
  _setFunction( _OPR_PROB_KW, (void *)_function_Solver_PROB_, 1);
  _setFunction( _OPR_PROBIT_KW, (void *)_function_Solver_PROBIT_, 1);
  _setFunction( _OPR_PSI2BAR_KW, (void *)_function_Convert_PSI2BAR_, 1);
  _setFunction( _OPR_PSI2MPA_KW, (void *)_function_Convert_PSI2MPA_, 1);
  _setFunction( _OPR_PSI2PA_KW, (void *)_function_Convert_PSI2PA_, 1);
  _setFunction( _OPR_PSIA2PSIG_KW, (void *)_function_Convert_PSIA2PSIG_, 1);
  _setFunction( _OPR_PSIG2PSIA_KW, (void *)_function_Convert_PSIG2PSIA_, 1);
  
  _setFunction( _OPR_QUAD_KW, (void *)_function_Solver_QUAD_, 0);
  
  _setFunction( _OPR_RADIUS_KW, (void *)_function_Solver_RADIUS_, 2);
  _setFunction( _OPR_ROLL_KW, (void *)_function_RPN_ROLL_, 0);
  _setFunction( _OPR_ROOT_KW, (void *)_function_RPN_ROOT_, 2);
  
  _setFunction( _OPR_SCIRC_KW, (void *)_function_Solver_SCIRC_, 1);
  _setFunction( _OPR_SIGN_KW, (void *)_function_Math_SIGN_, 1);
  _setFunction( _OPR_SIN_KW, (void *)_function_Math_SIN_, 1);
  _setFunction( _OPR_SINH_KW, (void *)_function_Math_SINH_, 1);
  _setFunction( _OPR_SFT2SM_KW, (void *)_function_Convert_SFT2SM_, 1);
  _setFunction( _OPR_SM2SFT_KW, (void *)_function_Convert_SM2SFT_, 1);
  _setFunction( _OPR_SQ_KW, (void *)_function_Math_SQ_, 1);
  _setFunction( _OPR_SQRT_KW, (void *)_function_Math_SQRT_, 1);
  _setFunction( _OPR_SSPHERE_KW, (void *)_function_Solver_SSPHERE_, 1);
  _setFunction( _OPR_ST2MT_KW, (void *)_function_Convert_ST2MT_, 1);
  _setFunction( _OPR_STACK_KW, (void *)_function_RPN_STACK_, 1);
  _setFunction( _OPR_SUB_KW, (void *)_function_RPN_SUB_, 0);
  _setFunction( _OPR_SWAP_KW, (void *)_function_RPN_SWAP_, 0);
  
  _setFunction( _OPR_TAN_KW, (void *)_function_Math_TAN_, 1);
  _setFunction( _OPR_TANH_KW, (void *)_function_Math_TANH_, 1);
  _setFunction( _OPR_VSPHERE_KW, (void *)_function_Solver_VSPHERE_, 1);

  _addFunction( _OPR_ABS_KW, 1);
  _addFunction( _OPR_ACOS_KW, 1);
  _addFunction( _OPR_ACOSH_KW, 1);
  _addFunction( _OPR_AIN_KW, 2);
  _addFunction( _OPR_API2GCC_KW, 1); // 5

  _addFunction( _OPR_ASIN_KW, 1);
  _addFunction( _OPR_ASINH_KW, 1);
  _addFunction( _OPR_ATAN_KW, 1);
  _addFunction( _OPR_ATANH_KW, 1);
  _addFunction( _OPR_AZIMUTH_KW, 2); // 10

  _addFunction( _OPR_AZIMUTH3_KW, 3);
  _addFunction( _OPR_BAR2PSI_KW, 1);
  _addFunction( _OPR_BBL2CM_KW, 1);
  _addFunction( _OPR_C2F_KW, 1);
  _addFunction( _OPR_C2K_KW, 1); // 15

  _addFunction( _OPR_CATH_KW, 2);
  _addFunction( _OPR_CF2CM_KW, 1);
  _addFunction( _OPR_CM2BBL_KW, 1);
  _addFunction( _OPR_CM2CF_KW, 1);
  _addFunction( _OPR_CNK_KW, 2); // 20

  _addFunction( _OPR_COS_KW, 1);
  _addFunction( _OPR_COSH_KW, 1);
  _addFunction( _OPR_COT_KW, 1);
  _addFunction( _OPR_D2DMS_KW, 1);
  _addFunction( _OPR_DMS2D_KW, 1); // 25

  _addFunction( _OPR_EXP_KW, 1);
  _addFunction( _OPR_F2C_KW, 1);
  _addFunction( _OPR_FACT_KW, 1);
  _addFunction( _OPR_FT2M_KW, 1);
  _addFunction( _OPR_GAL2L_KW, 1); // 30

  _addFunction( _OPR_GCC2API_KW, 1);
  _addFunction( _OPR_IN2MM_KW, 1);
  _addFunction( _OPR_K2C_KW, 1);
  _addFunction( _OPR_KG2LBS_KW, 1);
  _addFunction( _OPR_KM2MI_KW, 1); // 35

  _addFunction( _OPR_KM2NM_KW, 1);
  _addFunction( _OPR_KMH2MS_KW, 1);
  _addFunction( _OPR_L2GAL_KW, 1);
  _addFunction( _OPR_LCIRC_KW, 1);
  _addFunction( _OPR_LIN2_KW, 1); // 40

  _addFunction( _OPR_LG_KW, 1);
  _addFunction( _OPR_LN_KW, 1);
  _addFunction( _OPR_LOG_KW, 2);
  _addFunction( _OPR_LT2MT_KW, 1);
  _addFunction( _OPR_M2FT_KW, 1); // 45

  _addFunction( _OPR_MI2KM_KW, 1);
  _addFunction( _OPR_MM2IN_KW, 1);
  _addFunction( _OPR_MPA2PSI_KW, 1);
  _addFunction( _OPR_MT2LT_KW, 1);
  _addFunction( _OPR_MT2ST_KW, 1); // 50

  _addFunction( _OPR_NDIS_KW, 1);
  _addFunction( _OPR_NM2KM_KW, 1);
  _addFunction( _OPR_POW_KW,2);
  _addFunction( _OPR_PSI2BAR_KW, 1);
  _addFunction( _OPR_PA2PSI_KW, 1); // 55

  _addFunction( _OPR_PROB_KW, 1);
  _addFunction( _OPR_PROBIT_KW, 1);
  _addFunction( _OPR_PSI2MPA_KW, 1);
  _addFunction( _OPR_PSI2PA_KW, 1);
  _addFunction( _OPR_PSIA2PSIG_KW, 1); // 60

  _addFunction( _OPR_PSIG2PSIA_KW, 1);
  _addFunction( _OPR_RADIUS_KW,2);
  _addFunction( _OPR_ROOT_KW,2);
  _addFunction( _OPR_SCIRC_KW, 1);
  _addFunction( _OPR_SFT2SM_KW, 1); // 65

  _addFunction( _OPR_SIN_KW, 1);
  _addFunction( _OPR_SIGN_KW, 1);
  _addFunction( _OPR_SINH_KW, 1);
  _addFunction( _OPR_SM2SFT_KW, 1);
  _addFunction( _OPR_SINH_KW, 1); // 70

  _addFunction( _OPR_SQ_KW, 1);
  _addFunction( _OPR_SQRT_KW, 1);
  _addFunction( _OPR_SSPHERE_KW, 1);
  _addFunction( _OPR_ST2MT_KW, 1);  
  _addFunction( _OPR_STACK_KW, 1); // 75

  _addFunction( _OPR_TAN_KW, 1);
  _addFunction( _OPR_TANH_KW, 1);
  _addFunction( _OPR_VSPHERE_KW, 1); // 78
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

void Functions::_addFunction( int16_t kwid, byte nArgs){
  if( _id >= _FUNCTION_COUNT) return;
  Function *tmp = _functions+_id;
  tmp->id = _id++;
  tmp->kwid = kwid;
  tmp->nArgs = nArgs;
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

