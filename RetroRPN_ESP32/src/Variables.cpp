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

const char _RPN_Error_NaN[] PROGMEM = "Err: NaN";
const char _RPN_Error_Argument[] PROGMEM = "Err: Argument";
const char _RPN_Mode_Degrees[] PROGMEM = "Mode: Degrees";
const char _RPN_Mode_Radians[] PROGMEM = "Mode: Radians";
const char _RPN_Mode_Gradians[] PROGMEM = "Mode: Gradians";
const char *const _RPN_AMODE_Table[] PROGMEM = {
  _RPN_Mode_Degrees,
  _RPN_Mode_Radians,
  _RPN_Mode_Gradians
  };

const char _VAR_prgCounter[] PROGMEM = "prgCounter%";
const char _VAR_stack[] PROGMEM = "stack";
const char _VAR_prev[] PROGMEM = "prev";
const char _VAR_amode[] PROGMEM = "amode%";

const char _VAR_nMean[] PROGMEM = "nMean";
const char _VAR_Mean[] PROGMEM = "Mean";
const char _VAR_StDev[] PROGMEM = "StDev";

const char _VAR_nMeanXY[] PROGMEM = "nMeanXY";
const char _VAR_MeanX[] PROGMEM = "MeanX";
const char _VAR_StDevX[] PROGMEM = "StDevX";
const char _VAR_MeanY[] PROGMEM = "MeanY";
const char _VAR_StDevY[] PROGMEM = "StDevY";
const char _VAR_Gain[] PROGMEM = "Gain";
const char _VAR_Offset[] PROGMEM = "Offset";

const char _VAR_lcdPWM[] PROGMEM = "lcdPWM%";
const char _VAR_current_dir[] PROGMEM = "current_dir$";
const char _VAR_current_file[] PROGMEM = "current_file$";
const char _VAR_scrMessage[] PROGMEM = "scrMessage$";
const char _VAR_rpnLabelX[] PROGMEM = "rpnLabelX$";
const char _VAR_rpnLabelY[] PROGMEM = "rpnLabelY$";
const char _VAR_rpnLabelZ[] PROGMEM = "rpnLabelZ$";

const char _CON_deg[] PROGMEM = "deg";
const char _CON_DEG[] PROGMEM = "DEG";
const char _CON_rad[] PROGMEM = "rad";
const char _CON_RAD[] PROGMEM = "RAD";
const char _CON_grad[] PROGMEM = "grad";
const char _CON_GRAD[] PROGMEM = "GRAD";
const char _CON_pi[] PROGMEM = "pi";
const char _CON_PI[] PROGMEM = "PI";
const char _CON_True[] PROGMEM = "True";
const char _CON_TRUE[] PROGMEM = "TRUE";
const char _CON_False[] PROGMEM = "False";
const char _CON_FALSE[] PROGMEM = "FALSE";
const char _CON_High[] PROGMEM = "High%";
const char _CON_Low[] PROGMEM = "Low%";
const char _CON_varMemory[] PROGMEM = "varMemory%";
const char _CON_prgMemory[] PROGMEM = "prgMemory%";

const char SD_root2[] PROGMEM = "/";
const char SD_default_file[] PROGMEM = "/autotest.bas";

void Variables::init( void *components[]){
  _buffer = (byte *)malloc( VARIABLE_SPACE);
  _kwds = (Keywords *)components[UI_COMP_Keywords];

  _prgCounter = (int64_t *)_getDataPtr( _placeNumber( false, _VAR_prgCounter, 0));
  _amode = (int64_t *)_getDataPtr( _placeNumber( false, _VAR_amode, _MODE_DEGREES_));
  nmean = (double *)_getDataPtr( _placeNumber( false, _VAR_nMean, 0.0));
  nmeanXY = (double *)_getDataPtr( _placeNumber( false, _VAR_nMeanXY, 0.0));
  _read_only_bottom = _var_bottom; // the variables above cannot be written from BASIC

  mean = (double *)_getDataPtr( _placeNumber( false, _VAR_Mean, 0.0));
  stdev = (double *)_getDataPtr( _placeNumber( false, _VAR_StDev, 1.0));
  meanX = (double *)_getDataPtr( _placeNumber( false, _VAR_MeanX, 0.0));
  stdevX = (double *)_getDataPtr( _placeNumber( false, _VAR_StDevX, 1.0));
  meanY = (double *)_getDataPtr( _placeNumber( false, _VAR_MeanY, 0.0));
  stdevY = (double *)_getDataPtr( _placeNumber( false, _VAR_StDevY, 1.0));
  _rpnStack = (double *)_getDataPtr( _placeVector( false, _VAR_stack, RPN_STACK));
  _prev = (double *)_getDataPtr( _placeNumber( false, _VAR_prev));
  gain = (double *)_getDataPtr( _placeNumber( false, _VAR_Gain, 1.0));
  offset = (double *)_getDataPtr( _placeNumber( false, _VAR_Offset, 0.0));
  currentDir = (char *)_getDataPtr( _placeString( false,
      _VAR_current_dir, CURRENT_DIR_LEN, SD_root2));
  currentFile = (char *)_getDataPtr( _placeString( false,
      _VAR_current_file, CURRENT_FILE_LEN, SD_default_file));
  scrMessage = _getDataPtr( _placeString( false, _VAR_scrMessage, SCR_COLS));
  rpnLabelX = _getDataPtr( _placeString( false, _VAR_rpnLabelX, SCR_COLS));
  rpnLabelY = _getDataPtr( _placeString( false, _VAR_rpnLabelY, SCR_COLS));
  rpnLabelZ = _getDataPtr( _placeString( false, _VAR_rpnLabelZ, SCR_COLS));
  _placeNumber( false, _VAR_lcdPWM, 200);
  _standard_bottom = _var_bottom; // the variables above cannot be removed from BASIC
  #ifdef __DEBUG
  Serial.print( "Placed all standard variables, standard bottom: ");
  Serial.println( _standard_bottom);
  #endif

  _placeNumber( true, _CON_True, 1.0);
  _placeNumber( true, _CON_TRUE, 1.0);
  _placeNumber( true, _CON_False, 0.0);
  _placeNumber( true, _CON_FALSE, 0.0);
  _placeNumber( true, _CON_pi, RPN_PI);
  _placeNumber( true, _CON_PI, RPN_PI);
  _placeNumber( true, _CON_deg, 0.0);
  _placeNumber( true, _CON_DEG, 0.0);
  _placeNumber( true, _CON_rad, 1.0);
  _placeNumber( true, _CON_RAD, 1.0);
  _placeNumber( true, _CON_grad, 2.0);
  _placeNumber( true, _CON_GRAD, 2.0);
  _placeNumber( true, _CON_High, 1);
  _placeNumber( true, _CON_Low, 0);
  _varAvailble = (int64_t *)_getDataPtr( _placeNumber( true, _CON_varMemory));
  _prgAvailble = (int64_t *)_getDataPtr( _placeNumber( true, _CON_prgMemory));
  _standard_top = _const_top; // the constants above cannot be removed or wriitten to from BASIC
  setMemoryAvailable();
  #ifdef __DEBUG
  Serial.print( "Placed all standard constants, standard top: ");
  Serial.println( _standard_top);
  Serial.print( "Available variables memory: ");
  Serial.println( getMemoryAvailable());
  #endif
}

//
// List available variables or constants
//
VariableToken Variables::_getNextVar( VariableToken vt){
  if( vt < 2) return 0;
  if( vt >= VARIABLE_SPACE) return 0;
  byte vtype = getVarType(vt);
  if( !vtype) return 0; // undefined type;
  byte nameLen = getVarNameLength(vt);
  uint16_t total_size = getTotalSize(vt);
  uint16_t varLen = _getVarLength( nameLen, vtype, total_size);
  return vt + varLen;
}
VariableToken Variables::getNextVar( VariableToken vt){
  vt = _getNextVar( vt);
  if( vt < 2) return 0;
  if( vt >= VARIABLE_SPACE) return 0;
  if( _var_bottom < vt && vt-2 < _const_top ) return 0;
  return vt;
}
uint16_t Variables::getTotalSize( VariableToken vt){
  if( getVarType( vt) == VARTYPE_NUMBER) return 1;
  uint16_t *ptr = (uint16_t *)_getVarBlockPtr( vt);
  return *ptr;
}
uint16_t Variables::getRowSize( VariableToken vt){
  switch( getVarType( vt)){
    case VARTYPE_NUMBER:
      return 1;
    case VARTYPE_STRING:
    case VARTYPE_VECTOR:
      return getTotalSize( vt);
    default:
      break;
  }
  uint16_t *ptr = (uint16_t *)_getVarBlockPtr( vt);
  return ptr[1];
}
uint16_t Variables::getColumnSize( VariableToken vt){
  if( getVarType( vt) != VARTYPE_MATRIX) return 1;
  uint16_t *ptr = (uint16_t *)_getVarBlockPtr( vt);
  return ptr[0]/ptr[1];
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
  return nameEndsWith(vt) == _PERCENT_;
}

//
// Setting values
//
int64_t Variables::_limitHuge( double v){
  if( v > _HUGE_POS_INTEGER_) return _HUGE_POS_INTEGER_;
  if( v < _HUGE_NEG_INTEGER_) return _HUGE_NEG_INTEGER_;
  return (int64_t)v;
}
void Variables::setValueReal( VariableToken vt, double v, uint16_t i, uint16_t j){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;
  if( isNameBASICInteger(vt)){
    int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
    *ptri = _limitHuge( v);
    return;
  }
  double *ptrd = (double *)_getDataPtr( vt, i, j);
  *ptrd = v;
}
void Variables::setValueInteger( VariableToken vt, int64_t v, uint16_t i, uint16_t j){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;
  if( isNameBASICInteger(vt)){
    int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
    *ptri = v;
    return;
  }
  double *ptrd = (double *)_getDataPtr( vt, i, j);
  *ptrd = (double)v;
}
void Variables::setValueString( VariableToken vt, const char *v){
  if( vt < 2) return;
  if( getVarType(vt) != VARTYPE_STRING) return;
  uint16_t totalSize = getTotalSize(vt);
  char *ptr = (char *)_getDataPtr( vt);
  strncat2(ptr, v, totalSize);  
}
void Variables::setValueRealArray( VariableToken vt, double v){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;  
  if( getVarType(vt) == VARTYPE_NUMBER){
    setValueReal( vt, v);
    return;  
  }
  uint16_t totalSize = getTotalSize(vt);
  if( isNameBASICInteger(vt)){
    int64_t *ptri = (int64_t *)_getDataPtr( vt, 0, 0);
    int64_t tmp = _limitHuge( v);
    for( uint16_t i=0; i<totalSize; i++) *ptri++ = tmp;
    return;
  }
  double *ptrd = (double *)_getDataPtr( vt, 0, 0);
  for( uint16_t i=0; i<totalSize; i++) *ptrd++ = v;
}
void Variables::setValueIntegerArray( VariableToken vt, int64_t v){
  if( vt < 2) return;
  if( getVarType(vt) == VARTYPE_STRING) return;
  if( getVarType(vt) == VARTYPE_NUMBER){
    setValueInteger( vt, v);
    return;  
  }
  uint16_t *total_length = (uint16_t *)_getVarBlockPtr( vt);
  if( isNameBASICReal(vt)){
    double *ptrd = (double *)_getDataPtr( vt, 0, 0);
    double tmp = (double)v;
    for( uint16_t i=0; i<*total_length; i++) *ptrd++ = tmp;
    return;
  }
  int64_t *ptri = (int64_t *)_getDataPtr( vt, 0, 0);
  for( uint16_t i=0; i<*total_length; i++) *ptri++ = v;
}

//
// Return values in different forms
// Note that the variable naming follows the BASIC convention:
// % - integers
// $ - strings
//
double Variables::realValue( VariableToken vt, uint16_t i, uint16_t j){
  if( vt < 2) return 0.0;
  if( getVarType(vt) == VARTYPE_STRING) return 0.0;
  byte * ptr = _getDataPtr( vt, i, j);
  if( isNameBASICInteger(vt)){
    int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
    return (double)ptri[0];
  }
  double *ptrd = (double *)_getDataPtr( vt, i, j);
  return *ptrd;
}
int64_t Variables::integerValue( VariableToken vt, uint16_t i, uint16_t j){
  if( vt < 2) return 0L;
  if( getVarType(vt) == VARTYPE_STRING) return 0L;
  if( isNameBASICInteger(vt)){
    int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
    return ptri[0];
  }
  double *ptrd = (double *)_getDataPtr( vt, i, j);
  return (int64_t)(*ptrd);
}
byte * Variables::stringValue( VariableToken vt){
  if( vt < 2) return NULL;
  if( getVarType(vt) != VARTYPE_STRING) return NULL;
  return _getDataPtr( vt);  
}

//
// Looks for constants and variables;
// Returns zero if not found or if a keyword
//
VariableToken Variables::findByName( const char *name){
  if( strlen(name) == 0) return 0;
  if( _kwds->isKeyword( (byte *)name)) return 0;
  VariableToken vt = _findConstantByName( name);
  if( vt > 0) return vt;
  return _findVariableByName( name);
}
VariableToken Variables::_findConstantByName( const char *name){
  if( _const_top >= VARIABLE_SPACE) return 0;
  VariableToken vt = getFirstConst();
  const char *ptr = (const char*)_buffer + vt;
  while(true){
    if( strcmp( name, ptr) == 0) return vt;
    vt = _getNextVar( vt);
    if( vt == 0 || vt>=VARIABLE_SPACE) break; // no more constants
    ptr = (const char*)_buffer + vt;
  }
  return 0;
}
VariableToken Variables::_findVariableByName( const char *name){
  if( _var_bottom == 0) return 0;
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
    vt = _getNextVar( vt);
    if( vt == 0 || vt>=_var_bottom) break; // no more variables
    ptr = (const char*)_buffer + vt;
  }
  return 0;
}
byte *Variables::findDataPtr( const char * name, uint16_t i, uint16_t j){
  VariableToken vt = findByName( name);
  if( !vt) return NULL;
  return _getDataPtr( vt, i, j);
}

//
// Places new variable in either var or const stack
// Returns ID or 0 if not enough memory
//
VariableToken Variables::getOrCreateNumber( bool asConstant, byte *name){
   // redeclaration of keywords not allowed
  if( _kwds->isKeyword( (byte *)name)) return 0; 
  VariableToken vt = findByName( name);
  if( vt){ // exists
    return vt;
  }
  vt = _placeNumber( asConstant, (const char *)name);
  *_varAvailble = (int64_t)getMemoryAvailable();
  return vt;
}

void Variables::_removeVariable( VariableToken vt){
  VariableToken vt2 = _getNextVar( vt);
  if( vt2>_var_bottom){
    _var_bottom = vt-2;
    return;
  } 
  byte *dest = _buffer + vt - 2;
  byte *src = _buffer + vt2 - 2;
  for( uint16_t i=vt2-2; i<_var_bottom; i++)
    *dest++ = *src++;
  _var_bottom -= (vt2 - vt);
  return;
}
void Variables::_removeConstant( VariableToken vt){
  VariableToken vt2 = _getNextVar( vt);  
  if( vt-2<=_const_top){
    _const_top = vt2-2;
    return;
  } 
  byte *dest = _buffer + vt2 - 3;
  byte *src = _buffer + vt - 3;
  for( uint16_t i=vt-3; i>=_const_top; i--)
    *dest-- = *src--;
  _const_top += (vt2 - vt);
  return;
}
void Variables::removeByToken( VariableToken vt){
  if( isUnremovable( vt)) return; // read-only constant or unremovable variable
  if( isConstant( vt)) _removeConstant( vt);
  else _removeVariable(vt);
  *_varAvailble = (int64_t)getMemoryAvailable();
}
void Variables::removeByName( const char *name){
  VariableToken vt = findByName(name);
  removeByToken( vt);
}

//
// Stack handling methods
//
void Variables::swapRPNXY(){
  double tmp = _rpnStack[0];
  _rpnStack[0] = _rpnStack[1];
  _rpnStack[1] = tmp;
}
void Variables::popRPNStack(byte start){
  for(byte i=start; i<RPN_STACK; i++) _rpnStack[i-1] = _rpnStack[i];
}
void Variables::pushRPNStack(){
  for(byte i=RPN_STACK-1; i>0; i--) _rpnStack[i] = _rpnStack[i-1];
}
void Variables::pushRPNStack( double v){
  pushRPNStack();
  *_rpnStack = v;
}
void Variables::rollRPNStack(){
  pushRPNStack( _rpnStack[RPN_STACK-1]);
}

double Variables::getConvertedAngle( double a){
  return a * _angleConversions[ (uint16_t)(*_amode)];
}
double Variables::getUnconvertedAngle( double a){
  if( isnan(a)) return a;
  return a / _angleConversions[(uint16_t)(*_amode)];
}
const char *Variables::getAMODEString(){
  return _RPN_AMODE_Table[ getAngleMode()];
}

//
// Private methods
//

//
// Estimates variable length
//
uint16_t Variables::_getVarLength( uint16_t nameLen, byte vtype,
                                 uint16_t total_size){
  nameLen += 3;
  switch( vtype){
    case VARTYPE_NUMBER:
      // +8 for double or int64 value
      nameLen += sizeof(double);
      break;
    case VARTYPE_VECTOR:
      nameLen += sizeof(uint16_t) + total_size * sizeof(double);
      break;
    case VARTYPE_MATRIX:
      nameLen += 2 * sizeof(uint16_t) + total_size * sizeof(double);
      break;
    case VARTYPE_STRING:
      nameLen += sizeof(uint16_t) + total_size + 1;
      break;
    default:
      nameLen = 0;
      break;
  }
  return nameLen;
}

//
// points to the data, including strings and arrays
//
byte * Variables::_getLengthBlockPtr( VariableToken vt){
  if( vt < 2) return NULL;
  if( vt >= VARIABLE_SPACE) return NULL;
  return (byte *)_buffer + vt - 2;
}
byte * Variables::_getVarBlockPtr( VariableToken vt){
  byte *ptr = _getLengthBlockPtr(vt);
  if(!ptr) return NULL;
  return ptr + ptr[1] + 3;
}
byte * Variables::_getDataPtr( VariableToken vt, uint16_t i, uint16_t j){
  byte *ptr = _getVarBlockPtr( vt);
  if( !ptr) return NULL;
  byte tp = _buffer[vt-2]; 
  if( tp == VARTYPE_NONE) return NULL;
  if( tp == VARTYPE_NUMBER) return ptr;
  uint16_t *size_ptr = (uint16_t*)ptr;
  uint16_t total_size = size_ptr[0];
  if( i>=total_size) i = 0;
  ptr += sizeof( uint16_t);
  if( tp == VARTYPE_STRING) return ptr + i;
  if( tp == VARTYPE_VECTOR) return ptr + i * sizeof(double);
  ptr += sizeof( uint16_t);
  uint16_t row_size = size_ptr[1];
  uint16_t column_size = total_size / row_size;
  if( i>=row_size) j = row_size-1;
  if( j>=column_size) j = column_size-1;
  i = i*row_size + j;
  if( i>=total_size) i = 0;
  return ptr + i * sizeof(double);
}

//
// Allocates space for the new variable; if no memory, returns 0
//
VariableToken Variables::_allocateVarSpace( bool isConst,
    uint16_t nameLen, byte varType, uint16_t totalSize){
  uint16_t l = _getVarLength( nameLen, varType, totalSize);
  if( l >= getMemoryAvailable()) return 0;
  if( isConst){
    _const_top -= l;
    return _const_top + 2;
  }
  VariableToken vt = _var_bottom + 2;
  _var_bottom += l;
  return vt;
}
VariableToken Variables::_placeVarName( bool isConst, const char *name,
    byte varType, uint16_t totalSize){
  uint16_t nameLen = strlen( name);
  if( !nameLen) return 0;
  if( nameLen >= 254) nameLen = 254; 
  VariableToken vt = _allocateVarSpace( isConst, nameLen, varType, totalSize);
  if( !vt) return 0;
  byte *ptr = _getLengthBlockPtr(vt);
  *ptr++ = varType;
  *ptr++ = (byte)nameLen;
  strncat2( (char *)ptr, name, nameLen+1);
  return vt;
}
VariableToken Variables::_placeNumber( bool isConst,
    const char *name, double value){
  VariableToken vt = _placeVarName( isConst, name, VARTYPE_NUMBER, 1);
  if( !vt) return 0;
  setValueReal( vt, value);
  return vt;
}
VariableToken Variables::_placeVector( bool isConst,
    const char *name, uint16_t length, double value){
  if( length == 0) return 0;
  VariableToken vt = _placeVarName( isConst, name, VARTYPE_VECTOR, length);
  if( !vt) return 0;
  uint16_t *ptr = (uint16_t *)_getVarBlockPtr( vt);
  *ptr = length;
  setValueRealArray( vt, value);
  return vt;
}
VariableToken Variables::_placeMatrix( bool isConst,
    const char *name, uint16_t rows, uint16_t cols, double value){
  uint16_t totalSize = rows*cols;
  if( totalSize == 0) return 0;
  VariableToken vt = _placeVarName( isConst, name, VARTYPE_MATRIX, totalSize);
  if( !vt) return 0;
  uint16_t *ptr = (uint16_t *)_getVarBlockPtr( vt);
  *ptr++ = totalSize;
  *ptr = cols;
  setValueRealArray( vt, value);
  return vt;
}
VariableToken Variables::_placeString( bool isConst,
    const char *name, uint16_t length, const char* value){
  if( length == 0) return 0;
  VariableToken vt = _placeVarName( isConst, name, VARTYPE_STRING, length);
  if( !vt) return 0;
  uint16_t *ptr = (uint16_t *)_getVarBlockPtr( vt);
  *ptr++ = length;
  char *dataPtr = (char *)ptr;
  if( value) strncat2( dataPtr, value, length-1);
  else *dataPtr = _NUL_;
  return vt;
}

void Variables::clearRPNSum(){
  *nmean = 0.0;
  *mean = 0.0;
  *stdev = 1.0;
  sumsq = 0.0;
  variance = 0.0;
}

void Variables::clearRPNSumXY(){
  *nmeanXY = 0.0;
  *meanX = 0.0;
  *stdevX = 1.0;
  sumsqX = 0.0;
  varianceX = 0.0;
  *meanY = 0.0;
  *stdevY = 0.0;
  sumsqY = 0.0;
  varianceY = 0.0;
  sumsqXY = 0.0;
  varianceXY = 0.0;
  sumsqYX = 0.0;
  varianceYX = 0.0;
  *gain = 1.0;
  *offset = 0.0;
  rXY = 0.0;
  stErrXY = 0.0;
}

void Variables::addSample2RPNSum( double v){
  Serial.println("Sum called");
  double pMean = *mean;
  *nmean += 1.0;
  *mean += (v - *mean) / (*nmean);
  sumsq += (v - *mean) * (v - pMean);
  sumsq = abs(sumsq);
  variance = sumsq / *nmean;
  *stdev = sqrt( (*nmean >= 3.0)? sumsq / (*nmean - 1.0): variance); 
}

void Variables::addSample2RPNSumXY( double x, double y){
  *nmeanXY += 1.0;
  double pMeanX = *meanX;
  *meanX += (x - *meanX) / (*nmeanXY);
  sumsqX += (x - *meanX) * (x - pMeanX);
  sumsqX = abs(sumsqX);
  varianceX = sumsqX / *nmeanXY;
  *stdevX = sqrt( (*nmeanXY >= 3.0)? sumsqX / (*nmeanXY - 1.0): varianceX); 
  double pMeanY = *meanY;
  *meanY += (y - *meanY) / (*nmeanXY);
  sumsqY += (y - *meanY) * (y - pMeanY);
  sumsqY = abs(sumsqY);
  varianceY = sumsqY / *nmeanXY;
  *stdevY = sqrt( (*nmeanXY >= 3.0)? sumsqY / (*nmeanXY - 1.0): varianceY);
  sumsqXY += (x - *meanX)*( y - pMeanY);
  sumsqYX += (x - pMeanX)*( y - *meanY);
  varianceXY = sumsqXY / *nmeanXY;
  varianceYX = sumsqYX / *nmeanXY;
  if( *nmeanXY < 0.5){
    *gain = 0.0;
    *offset = 0.0;
    return;
  }
  if( *nmeanXY < 1.5){
    *gain = 1.0;
    *offset = 0.0;
    return;
  }
  double r_num = (varianceXY + varianceYX) * 0.5;
  double e_num = 1.0;
  *gain = 0.0;
  if( abs( varianceX) >= NEAR_ZERO){
    *gain = r_num / varianceX;
    e_num = varianceY / varianceX;
  }
  *offset = (*meanY) - (*gain) * (*meanX);
  double r_den = sqrt( varianceX * varianceY);
  rXY = 0.0;
  if( r_den >= NEAR_ZERO){
    rXY = r_num / r_den;
    if( rXY > 1.0) rXY = 1.0;
    if( rXY < -1.0) rXY = -1.0;
  }
  stErrXY = 0.0;
  if( *nmeanXY > 2.5)
    stErrXY = sqrt((1.0 - rXY*rXY) * e_num / (*nmeanXY - 2.0));
}

//
// Simple gain-offset conversion
//
byte Variables::_convert1_( double *args, double *rets, bool isRPN,
    double Gain, double Offset){
  mathError = _ERROR_;
  double tmp = Gain * args[0] + Offset;
  return _Universal_Mantra_( isRPN, tmp, rets, 0);
}

//
// Inverse conversion
//
byte Variables::_convert2_( double *args, double *rets, bool isRPN,
    double Gain, double Offset1, double Offset2){
  mathError = _ERROR_;
  double tmp = args[0] + Offset1;
  if( tmp <= 0.0){
    if( isRPN) setScrMessage( _RPN_Error_Argument);
    return _REQUEST_REDRAW_MSG;
  }
  tmp = Gain/tmp + Offset2;
  return _Universal_Mantra_( isRPN, tmp, rets, 0);
}

//
// Standard proceedings for RPN and non-RPN operation completion
//
byte Variables::_RPN_Mantra_( double value, byte pops){
  if( isnan(value)){
    setScrMessage( _RPN_Error_NaN);
    return _REQUEST_REDRAW_MSG;
  }
  saveRPNPrev();
  for( byte i=0; i<pops; i++) popRPNStack();
  _rpnStack[0] = value;
  mathError = _NO_ERROR_;
  return _REQUEST_DO_IOM + (pops? 3: 1);
}
byte Variables::_nonRPN_Mantra_( double value, double *rets){
  if( isnan(value)) return _REQUEST_REDRAW_MSG;
  rets[0] = value;
  mathError = _NO_ERROR_;
  return 1;
}
