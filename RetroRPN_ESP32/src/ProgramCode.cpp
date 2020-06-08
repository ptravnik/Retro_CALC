//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "ProgramCode.hpp"

//#define __DEBUG

void ProgramCode::init( void *components[]){
  _kwds = (Keywords *)components[UI_COMP_Keywords];
  _vars = (Variables *)components[UI_COMP_Variables];
  _epar = (ExpressionParser *)components[UI_COMP_ExpressionParser];
}

void ProgramCode::clearProgram(){
  _program_bottom = 0;
  currentLine = 0;
  lastLine = 0;
}

//
// Returns true if an error or no more memory
//
bool ProgramCode::addLine( byte *line){
  byte *ptr = _epar->numberParser.parse( line);
  if( *ptr == _SP_) ptr++;
  if( _epar->numberParser.result != _RESULT_INTEGER_ ) return true;
  int32_t lineNum = (int32_t)_epar->numberParser.integerValue();
  if( lineNum > MAX_LINE_NUMBER) return true;
  if( lineNum <= lastLine) return true;
  size_t lineLen = strlen( ptr);
  if( lineLen + 5 >= getMemoryAvailable()) return true;
  uint16_t *linePtr = (uint16_t *)getBottom();
  linePtr[0] = (uint16_t)lineNum;
  linePtr[1] = (uint16_t)lineLen;
  strcpy( (char*)getBottom() + 2*sizeof(uint16_t), (const char*)ptr);
  #ifdef __DEBUG
  Serial.print("Placed at ");
  Serial.print( _program_bottom);
  Serial.print(" #");
  Serial.print( linePtr[0]);
  Serial.print(" (");
  Serial.print( linePtr[1]);
  Serial.print(" bytes): ");
  Serial.println( (char *)ptr);
  #endif
  _program_bottom += 2*sizeof( uint16_t) + lineLen + 1;
  lastLine = lineNum;
  _vars->setMemoryAvailable( getMemoryAvailable());
  return false;
}

ProgramLine ProgramCode::getFirstLine(){
  ProgramLine pl;
  if( _program_bottom == 0) return pl;
  uint16_t *ptr = (uint16_t *)_buffer; 
  pl.lineNumber = ptr[0];
  pl.line = _buffer + 2*sizeof(uint16_t);
  return pl;
}

ProgramLine ProgramCode::getNextLine( ProgramLine pl){
  ProgramLine pl2;
  if( pl.line == NULL) return pl2;
  uint16_t *ptr = (uint16_t *)(pl.line) - 1;
  pl2.line = pl.line + *ptr + 1 + 2*sizeof(uint16_t);
  if( pl2.line-_buffer > _program_bottom){
    pl2.line = NULL;
    return pl2;
  }
  ptr = (uint16_t *)(pl2.line) - 2;
  pl2.lineNumber = *ptr;
  return pl2;
}

// byte *getLineString( ProgramLine pl, byte *buffer){

// }


// //
// // Removes variables from either var o const stack
// //
// VariableToken Variables::removeVariable( const char *name){
//   VariableToken vt = findVariableByName(name);
//   if(vt < _standard_bottom) return 0; // standard cannot be deleted
//   VariableToken vt2 = _getNextVar( vt);  
//   byte *dest = _buffer + vt - 2;
//   byte *src = _buffer + vt2 - 2;
//   for( size_t i=vt2-2; i<_var_bottom; i++)
//     *dest++ = *src++;
//   _var_bottom -= (vt2 - vt);
//   return vt;
// }
// VariableToken Variables::removeConstant( const char *name){
//   VariableToken vt = findConstantByName(name);
//   if(vt >= _standard_top) return 0; // standard cannot be deleted
//   VariableToken vt2 = _getNextVar( vt);  
//   byte *dest = _buffer + vt2 - 2;
//   byte *src = _buffer + vt - 2;
//   for( size_t i=vt-2; i>=_const_top; i--)
//     *dest-- = *src--;
//   _const_top += (vt2 - vt);
//   return vt;
// }

// //
// // returns variable name's offset or 0 if not found;
// // note that the actual variable starts two bytes earlier
// //
// VariableToken Variables::findVariableByName( const char *name){
//   if( _var_bottom == 0) return 0;
//   if( strlen(name) == 0) return 0;
//   VariableToken vt = getFirstVar();
//   const char *ptr = (const char*)_buffer + vt;
//   #ifdef __DEBUG
//   Serial.print( "Looking for: ");
//   Serial.println(name);
//   #endif
//   while(true){
//     #ifdef __DEBUG
//     Serial.print( "Checking: ");
//     Serial.println(ptr);
//     #endif
//     if( strcmp( name, ptr) == 0) return vt;
//     vt = _getNextVar( vt);
//     if( vt == 0 || vt>=_var_bottom) break; // no more variables
//     ptr = (const char*)_buffer + vt;
//   }
//   return 0;
// }
// VariableToken Variables::findConstantByName( const char *name){
//   if( _const_top >= VARIABLE_SPACE) return 0;
//   if( strlen(name) == 0) return 0;
//   VariableToken vt = getFirstConst();
//   const char *ptr = (const char*)_buffer + vt;
//   while(true){
//     if( strcmp( name, ptr) == 0) return vt;
//     vt = _getNextVar( vt);
//     if( vt == 0 || vt>=VARIABLE_SPACE) break; // no more constants
//     ptr = (const char*)_buffer + vt;
//   }
//   return 0;
// }

// //
// // Creates a variable or a constant;
// // creation of runtime variables in which the name matches one of the
// // keywords is not allowed 
// //
// VariableToken Variables::getOrCreate( bool asConstant, byte *name,
//   byte type, size_t row_size, size_t column_size){
//   if( _kwds->getKeyword( name) != NULL) return 0;
//   VariableToken vt = asConstant?
//     findConstantByName( name):
//     findVariableByName( name);
//   if( vt != 0) return vt;
//   return asConstant?
//     placeNewConstant( name, type, row_size, column_size):
//     placeNewVariable( name, type, row_size, column_size);
// }

// VariableToken Variables::getNextVar( VariableToken vt){
//   vt = _getNextVar( vt);
//   if (vt >= VARIABLE_SPACE) return 0;
//   if (_var_bottom < vt && vt+1 < _const_top) return 0;
//   return vt;
// }

// VariableToken Variables::_getNextVar( VariableToken vt){
//   if( vt < 2) return 0;
//   if( vt >= VARIABLE_SPACE) return 0;
//   byte vtype = _buffer[vt-2];
//   byte nameLen = _buffer[vt-1];
//   size_t offset = vt + nameLen + 1;
//   size_t *size_ptr = (size_t *)(_buffer + offset);
//   switch(vtype){
//     case VARTYPE_NUMBER:
//       offset += sizeof(double);
//       break;
//     case VARTYPE_VECTOR:
//       offset += sizeof(size_t);
//       offset += *size_ptr * sizeof(double);
//       break;
//     case VARTYPE_MATRIX:
//       offset += sizeof(size_t);
//       offset += sizeof(size_t);
//       offset += *size_ptr * sizeof(double);
//       break;
//     case VARTYPE_STRING:
//       offset += sizeof(size_t);
//       offset += *size_ptr + 1;
//       break;
//     default:
//       return 0;  
//   }
//   return offset + 2;  
// }

// //
// // By the name convention, BASIC names end with $ if a string or
// // with a % if an integer; not sure if this convention should
// // be supported in the future, but leave for now (TODO).
// //
// byte Variables::nameEndsWith(VariableToken vt){
//   return _buffer[vt + getVarNameLength( vt) - 1];
// }
// bool Variables::isNameBASICReal( VariableToken vt){
//   byte b = nameEndsWith(vt);
//   return b != _DOLLAR_ && b != _PERCENT_;
// }
// bool Variables::isNameBASICString( VariableToken vt){
//   return nameEndsWith(vt) == _DOLLAR_;
// }
// bool Variables::isNameBASICInteger( VariableToken vt){
//   return _buffer[ vt + getVarNameLength( vt) - 1] == _PERCENT_;
// }

// //
// // Return values in different forms
// // Note that the variable naming follows the BASIC convention:
// // % - integers
// // $ - strings
// //
// double Variables::realValue( VariableToken vt, size_t i, size_t j){
//   if( vt < 2) return 0.0;
//   if( getVarType(vt) == VARTYPE_STRING) return 0.0;
//   if( isNameBASICReal(vt)){
//     double *ptrd = (double *)_getDataPtr( vt, i, j);
//     return *ptrd;
//   }
//   int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
//   return (double)*ptri;
// }
// int64_t Variables::integerValue( VariableToken vt, size_t i, size_t j){
//   if( vt < 2) return 0L;
//   if( getVarType(vt) == VARTYPE_STRING) return 0L;
//   if( isNameBASICReal(vt)){
//     double *ptrd = (double *)_getDataPtr( vt, i, j);
//     return _limitHuge( *ptrd);
//   }
//   int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
//   return *ptri;
// }
// byte * Variables::stringValue( VariableToken vt){
//   if( vt < 2) return NULL;
//   if( getVarType(vt) != VARTYPE_STRING) return NULL;
//   return _getDataPtr( vt);  
// }

// //
// // Setting values
// //
// void Variables::setValue( VariableToken vt, double v, size_t i, size_t j){
//   if( vt < 2) return;
//   if( getVarType(vt) == VARTYPE_STRING) return;
//   if( isNameBASICReal(vt)){
//     double *ptrd = (double *)_getDataPtr( vt, i, j);
//     *ptrd = v;
//     return;
//   }
//   int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
//   *ptri = _limitHuge( v);
// }
// void Variables::setValue( VariableToken vt, int64_t v, size_t i, size_t j){
//   if( vt < 2) return;
//   if( getVarType(vt) == VARTYPE_STRING) return;
//   if( isNameBASICReal(vt)){
//     double *ptrd = (double *)_getDataPtr( vt, i, j);
//     *ptrd = (double)v;
//     return;
//   }
//   int64_t *ptri = (int64_t *)_getDataPtr( vt, i, j);
//   *ptri = v;
// }
// void Variables::setValue( VariableToken vt, const char *v){
//   if( vt < 2) return;
//   if( getVarType(vt) != VARTYPE_STRING) return;
//   char *ptr = (char *)_getVarBlockPtr( vt);
//   size_t *size_ptr = (size_t *)ptr;
//   strncpy(ptr+sizeof(size_t), v, *size_ptr);  
// }
// void Variables::setVector( VariableToken vt, double v){
//   if( vt < 2) return;
//   if( getVarType(vt) == VARTYPE_STRING) return;  
//   if( getVarType(vt) == VARTYPE_NUMBER){
//     setValue( vt, v);
//     return;  
//   }
//   size_t *total_length = (size_t *)_getVarBlockPtr( vt);
//   if( isNameBASICReal(vt)){
//     double *ptrd = (double *)_getDataPtr( vt, 0, 0);
//     for( size_t i=0; i<*total_length; i++) *ptrd++ = v;
//     return;
//   }
//   int64_t *ptri = (int64_t *)_getDataPtr( vt, 0, 0);
//   int64_t tmp = _limitHuge( v);
//   for( size_t i=0; i<*total_length; i++) *ptri++ = tmp;
// }
// void Variables::setVector( VariableToken vt, int64_t v){
//   if( vt < 2) return;
//   if( getVarType(vt) == VARTYPE_STRING) return;
//   if( getVarType(vt) == VARTYPE_NUMBER){
//     setValue( vt, v);
//     return;  
//   }
//   size_t *total_length = (size_t *)_getVarBlockPtr( vt);
//   if( isNameBASICReal(vt)){
//     double *ptrd = (double *)_getDataPtr( vt, 0, 0);
//     double tmp = (double)v;
//     for( size_t i=0; i<*total_length; i++) *ptrd++ = tmp;
//     return;
//   }
//   int64_t *ptri = (int64_t *)_getDataPtr( vt, 0, 0);
//   for( size_t i=0; i<*total_length; i++) *ptri++ = v;
// }

// //
// // Estimates variable length
// //
// size_t Variables::_getVarLength( size_t nameLen, byte type,
//                                  size_t total_size){
//   nameLen += 3;
//   switch( type){
//     case VARTYPE_NUMBER:
//       // +8 for double or int64 value
//       return nameLen + sizeof(double);
//     case VARTYPE_VECTOR:
//       return nameLen + sizeof(size_t) + total_size * sizeof(double);
//     case VARTYPE_MATRIX:
//       return nameLen + 2 * sizeof(size_t) + total_size * sizeof(double);
//     case VARTYPE_STRING:
//       return nameLen + sizeof(size_t) + total_size + 1;
//     default:
//       break;
//   }
//   return nameLen;
// }

// //
// // points to the data, including strings and arrays
// //
// byte * Variables::_getVarBlockPtr( VariableToken vt){
//   if( vt < 2) return NULL;
//   return _buffer + vt + getVarNameLength(vt) + 1;
// }
// byte * Variables::_getDataPtr( VariableToken vt, size_t i, size_t j){
//   if( vt < 2) return NULL;
//   byte *ptr = _getVarBlockPtr( vt);
//   size_t *size_ptr = (size_t *)ptr;
//   size_t row_size;
//   switch(getVarType(vt)){
//     case VARTYPE_NUMBER:
//       #ifdef __DEBUG
//       Serial.print("Number offset: ");
//       Serial.println((size_t)(ptr-_buffer+2));
//       #endif
//       return ptr;
//     case VARTYPE_STRING:
//       #ifdef __DEBUG
//       Serial.print("String offset: ");
//       Serial.println((size_t)(ptr-_buffer+2));
//       #endif
//       size_ptr++;
//       return (byte *)size_ptr;
//     case VARTYPE_MATRIX:
//       ptr += sizeof( size_t);
//       row_size = size_ptr[1];
//       if( i >= row_size) i = row_size - 1;
//       i += j * row_size;
//     case VARTYPE_VECTOR: // fall-through
//       ptr += sizeof( size_t);
//       if( i >= *size_ptr) i = *size_ptr - 1;
//       ptr += i << 3;
//       #ifdef __DEBUG
//       Serial.print("Vector offset: ");
//       Serial.println((size_t)(ptr-_buffer+2));
//       #endif
//       return ptr;
//     default:
//       break;
//   }
//   return NULL;
// }

// //
// // Limits huge integers
// //
// int64_t Variables::_limitHuge( double v){
//   if( v > _HUGE_POS_INTEGER_) return _HUGE_POS_INTEGER_;
//   if( v < _HUGE_NEG_INTEGER_) return _HUGE_NEG_INTEGER_;
//   return v;
// }

// void Variables::_placeVar( byte *ptr, const char *name, byte type, size_t nameLen, size_t total_size, size_t row_size){
//   *ptr++ = type;
//   *ptr++ = (byte)nameLen;
//   size_t *ptr2 = (size_t *)(strncpy( (char *)ptr, name, nameLen) + nameLen + 1);
//   if( type == VARTYPE_NUMBER) return;
//   *ptr2 = total_size;
//   if( type == VARTYPE_MATRIX) ptr2[1] = row_size;
// }

// size_t Variables::_limitName( const char *name){
//   size_t nameLen = strlen( name);
//   if( nameLen>254) return 254;
//   return nameLen;
// }

// void Variables::swapRPNXY(){
//   double tmp = _rpnStack[0];
//   _rpnStack[0] = _rpnStack[1];
//   _rpnStack[1] = tmp;
// }

// double Variables::getConvertedAngle( double a){
//   return a * _angleConversions[ (size_t)(*_amode)];
// }

// double Variables::getUnconvertedAngle( double a){
//   if( isnan(a)) return a;
//   return a / _angleConversions[(size_t)(*_amode)];
// }
