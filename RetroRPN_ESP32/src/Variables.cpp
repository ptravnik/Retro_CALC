//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Variables.hpp"

//#define __DEBUG

const double _angleConversions[] PROGMEM = {1.74532925199e-2, 1.0, 1.5707963268e-2};

const char _VAR_stack[] PROGMEM = "stack";
const char _VAR_prev[] PROGMEM = "prev";
const char _VAR_amode[] PROGMEM = "amode%";
const char _VAR_current_dir[] PROGMEM = "current_dir$";
const char _VAR_scrMessage[] PROGMEM = "scrMessage$";
const char _VAR_rpnLabelX[] PROGMEM = "rpnLabelX$";
const char _VAR_rpnLabelY[] PROGMEM = "rpnLabelY$";
const char _VAR_rpnLabelZ[] PROGMEM = "rpnLabelZ$";
const char _CON_deg[] PROGMEM = "deg2";
const char _CON_DEG[] PROGMEM = "DEG";
const char _CON_rad[] PROGMEM = "rad2";
const char _CON_RAD[] PROGMEM = "RAD";
const char _CON_grad[] PROGMEM = "grad2";
const char _CON_GRAD[] PROGMEM = "GRAD";
const char _CON_pi[] PROGMEM = "pi2";
const char _CON_PI[] PROGMEM = "PI";
const char _CON_True[] PROGMEM = "True2";
const char _CON_TRUE[] PROGMEM = "TRUE";
const char _CON_False[] PROGMEM = "False2";
const char _CON_FALSE[] PROGMEM = "FALSE";

const char SD_root2[] PROGMEM = "/";

void Variables::init(){
  VariableToken vt = placeNewVariable( _VAR_stack, VARTYPE_VECTOR, RPN_STACK);
  setVector( vt, 0.0);
  _rpnStack = (double *)_getDataPtr( vt);
  vt = placeNewVariable( _VAR_prev, VARTYPE_NUMBER);
  setValue( vt, 0.0);   
  _prev = (double *)_getDataPtr( vt);   
  vt = placeNewVariable( _VAR_amode, VARTYPE_NUMBER);
  setValue( vt, 0.0);  
  _amode = (int64_t *)_getDataPtr( vt);
  vt = placeNewVariable( _VAR_current_dir, VARTYPE_STRING, CURRENT_DIR_LEN+1);
  setValue( vt, SD_root2);  
  currentDir = (char *)_getDataPtr( vt);
  vt = placeNewVariable( _VAR_scrMessage, VARTYPE_STRING, SCR_COLS);
  scrMessage = _getDataPtr( vt);
  *scrMessage = _NUL_;
  vt = placeNewVariable( _VAR_rpnLabelX, VARTYPE_STRING, SCR_COLS);
  rpnLabelX = _getDataPtr( vt);
  *rpnLabelX = _NUL_;
  vt = placeNewVariable( _VAR_rpnLabelY, VARTYPE_STRING, SCR_COLS);
  rpnLabelY = _getDataPtr( vt);
  *rpnLabelY = _NUL_;
  vt = placeNewVariable( _VAR_rpnLabelZ, VARTYPE_STRING, SCR_COLS);
  rpnLabelZ = _getDataPtr( vt);
  *rpnLabelZ = _NUL_;
  _standard_bottom = _var_bottom;
  #ifdef __DEBUG
  Serial.print( "Placed all standard variables, standard bottom: ");
  Serial.println( _standard_bottom);
  for( int i=0; i<_standard_bottom; i++){
    Serial.print(i);   
    Serial.print(" ");   
    Serial.println(_buffer[i], HEX);   
  }
  #endif
  placeNewConstantValue( _CON_True, 1.0);
  placeNewConstantValue( _CON_TRUE, 1.0);
  placeNewConstantValue( _CON_False, 0.0);
  placeNewConstantValue( _CON_FALSE, 0.0);
  placeNewConstantValue( _CON_pi, RPN_PI);
  placeNewConstantValue( _CON_PI, RPN_PI);
  placeNewConstantValue( _CON_deg, 0.0);
  placeNewConstantValue( _CON_DEG, 0.0);
  placeNewConstantValue( _CON_rad, 1.0);
  placeNewConstantValue( _CON_RAD, 1.0);
  placeNewConstantValue( _CON_grad, 2.0);
  placeNewConstantValue( _CON_GRAD, 2.0);
  _standard_top = _const_top;
}

//
// Places new variable in either var or const stack
// Returns ID or 0 if not enough memory
//
VariableToken Variables::placeNewVariable( const char *name, byte type, size_t row_size, size_t column_size){
  size_t nameLen = _limitName( name);
  size_t total_size = row_size * column_size;
  size_t varLen = _getVarLength( nameLen, type, total_size);
  if( _var_bottom + varLen >= _const_top) return 0; // not enough space
  VariableToken vt = _var_bottom + 2; 
  _placeVar( _buffer + _var_bottom, name, type, nameLen, total_size, row_size);
  _var_bottom += varLen;  
  return vt; 
}
VariableToken Variables::placeNewConstant( const char *name, byte type, size_t row_size, size_t column_size){
  size_t nameLen = _limitName( name);
  size_t total_size = row_size * column_size;
  size_t varLen = _getVarLength( nameLen, type, total_size);
  if( _const_top - _var_bottom <= varLen) return 0; // not enough space
  _const_top -= varLen;
  VariableToken vt = _const_top + 2;
  _placeVar( _buffer + _const_top, name, type, nameLen, total_size, row_size);
  return vt; 
}
VariableToken Variables::placeNewConstantValue( const char *name, double v){
  VariableToken vt = placeNewConstant( name, VARTYPE_NUMBER);
  if( vt < 2) return 0;
  setValue( vt, v);
  #ifdef __DEBUG
  Serial.print("Constant set: ");
  Serial.print( name);
  Serial.print(" = ");
  Serial.println(v);
  #endif
}

//
// Removes variables from either var o const stack
//
VariableToken Variables::removeVariable( const char *name){
  VariableToken vt = getVariable(name);
  if(vt < _standard_bottom) return 0; // standard cannot be deleted
  VariableToken vt2 = getNextVar( vt);  
  byte *dest = _buffer + vt - 2;
  byte *src = _buffer + vt2 - 2;
  for( size_t i=vt2-2; i<_var_bottom; i++)
    *dest++ = *src++;
  _var_bottom -= (vt2 - vt);
  return vt;
}
VariableToken Variables::removeConstant( const char *name){
  VariableToken vt = getConstant(name);
  if(vt >= _standard_top) return 0; // standard cannot be deleted
  VariableToken vt2 = getNextVar( vt);  
  byte *dest = _buffer + vt2 - 2;
  byte *src = _buffer + vt - 2;
  for( size_t i=vt-2; i>=_const_top; i--)
    *dest-- = *src--;
  _const_top += (vt2 - vt);
  return vt;
}

//
// returns variable name's offset or 0 if not found;
// note that the actual variable starts two bytes earlier
//
VariableToken Variables::getVariable( const char *name){
  if( _var_bottom == 0) return 0;
  if( strlen(name) == 0) return 0;
  VariableToken vt = getFirstVar();
  const char *ptr = (const char*)_buffer + vt;
  #ifdef __DEBUG
  Serial.print( "Looking for: ");
  Serial.println(name);
  #endif
  while(true){
    #ifdef __DEBUG
    Serial.print( "Checking: ");
    Serial.println(ptr);
    #endif
    if( strcmp( name, ptr) == 0) return vt;
    vt = getNextVar( vt);
    if( vt == 0 || vt>=_var_bottom) break; // no more variables
    ptr = (const char*)_buffer + vt;
  }
  return 0;
}
VariableToken Variables::getConstant( const char *name){
  if( _const_top >= VARIABLE_SPACE) return 0;
  if( strlen(name) == 0) return 0;
  VariableToken vt = getFirstConst();
  const char *ptr = (const char*)_buffer + vt;
  while(true){
    if( strcmp( name, ptr) == 0) return vt;
    vt = getNextVar( vt);
    if( vt == 0 || vt>=VARIABLE_SPACE) break; // no more constants
    ptr = (const char*)_buffer + vt;
  }
  return 0;
}

VariableToken Variables::getNextVar( VariableToken vt){
  if( vt < 2) return 0;
  if( vt >= VARIABLE_SPACE) return 0;
  byte vtype = _buffer[vt-2];
  byte nameLen = _buffer[vt-1];
  size_t offset = vt + nameLen + 1;
  size_t *size_ptr = (size_t *)(_buffer + offset);
  switch(vtype){
    case VARTYPE_NUMBER:
      offset += sizeof(double);
      break;
    case VARTYPE_VECTOR:
      offset += sizeof(size_t);
      offset += *size_ptr * sizeof(double);
      break;
    case VARTYPE_MATRIX:
      offset += sizeof(size_t);
      offset += sizeof(size_t);
      offset += *size_ptr * sizeof(double);
      break;
    case VARTYPE_STRING:
      offset += sizeof(size_t);
      offset += *size_ptr + 1;
      break;
    default:
      return 0;  
  }
  return offset + 2;  
}

//
// By the name convention, BASIC names end with $ if a string or
// with a % if an integer; not sure if this convention should
// be supported in the future, but leave for now (TODO).
//
byte Variables::nameEndsWith(VariableToken vt){
  return _buffer[vt + getVarNameLength( vt) - 1];
}
bool Variables::isNameBASICReal( VariableToken vt){
  byte b = nameEndsWith(vt);
  return b != _DOLLAR_ && b != _PERCENT_;
}
bool Variables::isNameBASICString( VariableToken vt){
  return nameEndsWith(vt) == _DOLLAR_;
}
bool Variables::isNameBASICInteger( VariableToken vt){
  return _buffer[ vt + getVarNameLength( vt) - 1] == _PERCENT_;
}

//
// Return values in different forms
// Note that the variable naming follows the BASIC convention:
// % - integers
// $ - strings
//
double Variables::realValue( VariableToken vt, size_t i, size_t j){
  if( vt < 2) return 0.0;
  if( getVarType(vt) == VARTYPE_STRING) return 0.0;
  if( isNameBASICReal(vt)){
    double *ptrd = (double *)_getDataPtr( vt, i, j);
    return *ptrd;
  }
  int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
  return (double)*ptri;
}
int64_t Variables::integerValue( VariableToken vt, size_t i, size_t j){
  if( vt < 2) return 0L;
  if( getVarType(vt) == VARTYPE_STRING) return 0L;
  if( isNameBASICReal(vt)){
    double *ptrd = (double *)_getDataPtr( vt, i, j);
    return _limitHuge( *ptrd);
  }
  int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
  return *ptri;
}
byte * Variables::stringValue( VariableToken vt){
  if( vt < 2) return NULL;
  if( getVarType(vt) != VARTYPE_STRING) return NULL;
  return _getDataPtr( vt);  
}

//
// Setting values
//
void Variables::setValue( VariableToken vt, double v, size_t i, size_t j){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;
  if( isNameBASICReal(vt)){
    double *ptrd = (double *)_getDataPtr( vt, i, j);
    *ptrd = v;
    return;
  }
  int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
  *ptri = _limitHuge( v);
}
void Variables::setValue( VariableToken vt, int64_t v, size_t i, size_t j){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;
  if( isNameBASICReal(vt)){
    double *ptrd = (double *)_getDataPtr( vt, i, j);
    *ptrd = (double)v;
    return;
  }
  int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
  *ptri = v;
}
void Variables::setValue( VariableToken vt, const char *v){
  if( vt < 2) return;
  if( getVarType(vt) != VARTYPE_STRING) return;
  char *ptr = (char *)_getVarBlockPtr( vt);
  size_t *size_ptr = (size_t *)ptr;
  strncpy(ptr+sizeof(size_t), v, *size_ptr);  
}
void Variables::setVector( VariableToken vt, double v){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;  
  if( getVarType(vt) == VARTYPE_NUMBER){
    setValue( vt, v);
    return;  
  }
  size_t *total_length = (size_t *)_getVarBlockPtr( vt);
  if( isNameBASICReal(vt)){
    double *ptrd = (double *)_getDataPtr( vt, 0, 0);
    for( size_t i=0; i<*total_length; i++) *ptrd++ = v;
    return;
  }
  int64_t *ptri = (int64_t *)_getDataPtr( vt, 0, 0);
  int64_t tmp = _limitHuge( v);
  for( size_t i=0; i<*total_length; i++) *ptri++ = tmp;
}
void Variables::setVector( VariableToken vt, int64_t v){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;
  if( getVarType(vt) == VARTYPE_NUMBER){
    setValue( vt, v);
    return;  
  }
  size_t *total_length = (size_t *)_getVarBlockPtr( vt);
  if( isNameBASICReal(vt)){
    double *ptrd = (double *)_getDataPtr( vt, 0, 0);
    double tmp = (double)v;
    for( size_t i=0; i<*total_length; i++) *ptrd++ = tmp;
    return;
  }
  int64_t *ptri = (int64_t *)_getDataPtr( vt, 0, 0);
  for( size_t i=0; i<*total_length; i++) *ptri++ = v;
}

//
// Estimates variable length
//
size_t Variables::_getVarLength( size_t nameLen, byte type,
                                 size_t total_size){
  nameLen += 3;
  switch( type){
    case VARTYPE_NUMBER:
      // +8 for double or int64 value
      return nameLen + sizeof(double);
    case VARTYPE_VECTOR:
      return nameLen + sizeof(size_t) + total_size * sizeof(double);
    case VARTYPE_MATRIX:
      return nameLen + 2 * sizeof(size_t) + total_size * sizeof(double);
    case VARTYPE_STRING:
      return nameLen + sizeof(size_t) + total_size + 1;
    default:
      break;
  }
  return nameLen;
}

//
// points to the data, including strings and arrays
//
byte * Variables::_getVarBlockPtr( VariableToken vt){
  if( vt < 2) return NULL;
  return _buffer + vt + getVarNameLength(vt) + 1;
}
byte * Variables::_getDataPtr( VariableToken vt, size_t i, size_t j){
  if( vt < 2) return NULL;
  byte *ptr = _getVarBlockPtr( vt);
  size_t *size_ptr = (size_t *)ptr;
  size_t row_size;
  switch(getVarType(vt)){
    case VARTYPE_NUMBER:
      #ifdef __DEBUG
      Serial.print("Number offset: ");
      Serial.println((size_t)(ptr-_buffer+2));
      #endif
      return ptr;
    case VARTYPE_STRING:
      #ifdef __DEBUG
      Serial.print("String offset: ");
      Serial.println((size_t)(ptr-_buffer+2));
      #endif
      size_ptr++;
      return (byte *)size_ptr;
    case VARTYPE_MATRIX:
      ptr += sizeof( size_t);
      row_size = size_ptr[1];
      if( i >= row_size) i = row_size - 1;
      i += j * row_size;
    case VARTYPE_VECTOR: // fall-through
      ptr += sizeof( size_t);
      if( i >= *size_ptr) i = *size_ptr - 1;
      ptr += i << 3;
      #ifdef __DEBUG
      Serial.print("Vector offset: ");
      Serial.println((size_t)(ptr-_buffer+2));
      #endif
      return ptr;
    default:
      break;
  }
  return NULL;
}

//
// Limits huge integers
//
int64_t Variables::_limitHuge( double v){
  if( v > _HUGE_POS_INTEGER_) return _HUGE_POS_INTEGER_;
  if( v < _HUGE_NEG_INTEGER_) return _HUGE_NEG_INTEGER_;
  return v;
}

void Variables::_placeVar( byte *ptr, const char *name, byte type, size_t nameLen, size_t total_size, size_t row_size){
  *ptr++ = type;
  *ptr++ = (byte)nameLen;
  size_t *ptr2 = (size_t *)(strncpy( (char *)ptr, name, nameLen) + nameLen + 1);
  if( type == VARTYPE_NUMBER) return;
  *ptr2 = total_size;
  if( type == VARTYPE_MATRIX) ptr2[1] = row_size;
}

size_t Variables::_limitName( const char *name){
  size_t nameLen = strlen( name);
  if( nameLen>254) return 254;
  return nameLen;
}

void Variables::rpnSWAP(){
  double tmp = _rpnStack[0];
  _rpnStack[0] = _rpnStack[1];
  _rpnStack[1] = tmp;
}

double Variables::getConvertedAngle( double a){
  return a * _angleConversions[ (size_t)(*_amode)];
}

double Variables::getUnconvertedAngle( double a){
  if( isnan(a)) return a;
  return a / _angleConversions[(size_t)(*_amode)];
}
