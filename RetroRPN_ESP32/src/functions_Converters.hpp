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
// Technical converters
// Note they presume RPN pointer assignment
//
static byte _function_Convert_API2GCC_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert2_( args, rets, isRPN, 141.5, 131.5);
}
static byte _function_Convert_GCC2API_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert2_( args, rets, isRPN, 141.5, 0.0, -131.5);
}

static byte _function_Convert_BAR2PSI_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 14.5038);
}

static byte _function_Convert_PSI2BAR_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.0/14.5038);
}

static byte _function_Convert_BBL2CM_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 158.987294928e-3);
}

static byte _function_Convert_CM2BBL_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 6.2898107704321);
}

static byte _function_Convert_C2F_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.8, 32.0);
}

static byte _function_Convert_F2C_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.55555556, -17.77777778);
}

static byte _function_Convert_C2K_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.0, 273.15);
}

static byte _function_Convert_K2C_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.0, -273.15);
}

static byte _function_Convert_CF2CM_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 2.83168466e-2);
}

static byte _function_Convert_CM2CF_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 35.3146667215);
}

static byte _function_Convert_D2DMS_( Variables *_vars, double *args, double *rets, bool isRPN){
  bool negative = args[0] < 0.0;
  double tmp = negative? -args[0]: args[0];
  double d = floor( tmp);
  tmp = (tmp-d) * 60.0;
  double m = floor( tmp); 
  double s = (tmp-m) * 60.0;
  tmp = d*100 + m + s*0.01;
  if( negative) tmp = -tmp;
  if( isRPN) _vars->setScrMessage( FUN_Message_DMMSS);
  _vars->_Universal_Mantra_( isRPN, tmp, rets);
  return _REQUEST_REDRAW_X;
}
static byte _function_Convert_DMS2D_( Variables *_vars, double *args, double *rets, bool isRPN){
  _vars->mathError = _ERROR_;
  bool negative = args[0] < 0.0;
  double tmp = negative? -args[0]: args[0];
  tmp /= 100.0;
  double d = floor( tmp);
  tmp = (tmp-d) * 100.0;
  if( tmp>=60.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG; 
  }
  double m = floor( tmp); 
  double s = (tmp-m) * 100.0;
  if( s>=60.0){
    if( isRPN) _vars->setScrMessage( FUN_Error_Argument);
    return _REQUEST_REDRAW_MSG; 
  }
  tmp = d + (m + s/60.0)/60.0;
  if( negative) tmp = -tmp;
  return _vars->_Universal_Mantra_( isRPN, tmp, rets);
}
static byte _function_Convert_FT2M_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.3048);
}

static byte _function_Convert_M2FT_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 3.28083990);
}

static byte _function_Convert_GAL2L_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 3.785411784);
}

static byte _function_Convert_L2GAL_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.26417205235814845);
}

static byte _function_Convert_KG2LBS_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 2.2046226218487757);
}

static byte _function_Convert_LBS2KG_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.45359237);
}

static byte _function_Convert_KM2MI_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.621371192237334);
}

static byte _function_Convert_MI2KM_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.609344);
}

static byte _function_Convert_KM2NM_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.5399568034557235);
}

static byte _function_Convert_NM2KM_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.852);
}

static byte _function_Convert_KMH2MS_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.2777777777777778);
}

static byte _function_Convert_MS2KMH_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 3.6);
}

static byte _function_Convert_IN2MM_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 25.4);
}

static byte _function_Convert_MM2IN_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.03937007874016);
}

static byte _function_Convert_LT2MT_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.016047);
}

static byte _function_Convert_MT2LT_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.98420643926905);
}

static byte _function_Convert_ST2MT_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.90718474);
}

static byte _function_Convert_MT2ST_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.10231131092439);
}

static byte _function_Convert_MPA2PSI_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 145.03774389728312);
}

static byte _function_Convert_PSI2MPA_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 6.894757e-3);
}

static byte _function_Convert_PA2PSI_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 1.4503774389728e-4);
}

static byte _function_Convert_PSI2PA_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 6.894757e+3);
}

static byte _function_Convert_PSIA2PSIG_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.0, -14.7);
}

static byte _function_Convert_PSIG2PSIA_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.0, +14.7);
}

static byte _function_Convert_SM2SFT_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 0.09290304);
}

static byte _function_Convert_SFT2SM_( Variables *_vars, double *args, double *rets, bool isRPN){
  return _vars->_convert1_( args, rets, isRPN, 10.763910416709722);
}
