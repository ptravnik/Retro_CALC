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
