//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Keywords.hpp"

//#define __DEBUG

//#define _OPR_REM_KW        0
const char _OPR_REM[] PROGMEM = "REM";
const char _OPR_rem[] PROGMEM = "rem";
//#define _OPR_REMALT_KW     1
const char _OPR_REMALT[] PROGMEM = "#";
//#define _OPR_CONST_KW      2
const char _OPR_CONST[] PROGMEM = "CONST";
const char _OPR_const[] PROGMEM = "const";
//#define _OPR_LET_KW        3
const char _OPR_LET[] PROGMEM = "LET";
const char _OPR_let[] PROGMEM = "let";
//#define _OPR_CLEAR_KW      4
const char _OPR_CLEAR[] PROGMEM = "CLEAR";
const char _OPR_clear[] PROGMEM = "clear";
//#define _OPR_VARS_KW       5
const char _OPR_VARS[] PROGMEM = "VARS";
const char _OPR_vars[] PROGMEM = "vars";
//#define _OPR_STORE_KW      6
const char _OPR_STORE[] PROGMEM = "STORE";
const char _OPR_store[] PROGMEM = "store";
//#define _OPR_LIST_KW       7
const char _OPR_LIST[] PROGMEM = "LIST";
const char _OPR_list[] PROGMEM = "list";
//#define _OPR_RESTORE_KW    8
const char _OPR_RESTORE[] PROGMEM = "RESTORE";
const char _OPR_restore[] PROGMEM = "restore";
//#define _OPR_APPEND_KW     9
const char _OPR_APPEND[] PROGMEM = "APPEND";
const char _OPR_append[] PROGMEM = "append";
//#define _OPR_DIM_KW       10
const char _OPR_DIM[] PROGMEM = "DIM";
const char _OPR_dim[] PROGMEM = "dim";
//#define _OPR_VECTOR_KW    11
const char _OPR_VECTOR[] PROGMEM = "VECTOR";
const char _OPR_vector[] PROGMEM = "vector";
//#define _OPR_MATRIX_KW    12
const char _OPR_MATRIX[] PROGMEM = "MATRIX";
const char _OPR_matrix[] PROGMEM = "matrix";
//#define _OPR_PROGRAM_KW    13
const char _OPR_PROGRAM[] PROGMEM = "PROGRAM";
const char _OPR_program[] PROGMEM = "program";
//#define _OPR_NEW_KW    14
const char _OPR_NEW[] PROGMEM = "NEW";
const char _OPR_new[] PROGMEM = "new";
//#define _OPR_LOAD_KW    15
const char _OPR_LOAD[] PROGMEM = "LOAD";
const char _OPR_load[] PROGMEM = "load";
//#define _OPR_SAVE_KW    16
const char _OPR_SAVE[] PROGMEM = "SAVE";
const char _OPR_save[] PROGMEM = "save";
//#define _OPR_CHAIN_KW    17
const char _OPR_CHAIN[] PROGMEM = "CHAIN";
const char _OPR_chain[] PROGMEM = "chain";

void Keywords::init(){
    _addKeyword( _OPR_REM, _OPR_rem);           // 0
    _addKeyword( _OPR_REMALT, _OPR_REMALT);     // 1
    _addKeyword( _OPR_CONST, _OPR_const);       // 2
    _addKeyword( _OPR_LET, _OPR_let);           // 3
    _addKeyword( _OPR_CLEAR, _OPR_clear);       // 4
    _addKeyword( _OPR_VARS, _OPR_vars);         // 5
    _addKeyword( _OPR_STORE, _OPR_store);       // 6
    _addKeyword( _OPR_LIST, _OPR_list);         // 7
    _addKeyword( _OPR_RESTORE, _OPR_restore);   // 8
    _addKeyword( _OPR_APPEND, _OPR_append);     // 9
    _addKeyword( _OPR_DIM, _OPR_dim);           // 10
    _addKeyword( _OPR_VECTOR, _OPR_vector);     // 11
    _addKeyword( _OPR_MATRIX, _OPR_matrix);     // 12
    _addKeyword( _OPR_PROGRAM, _OPR_program);   // 13
    _addKeyword( _OPR_NEW, _OPR_new);           // 14
    _addKeyword( _OPR_LOAD, _OPR_load);         // 15
    _addKeyword( _OPR_SAVE, _OPR_save);         // 16
    _addKeyword( _OPR_CHAIN, _OPR_chain);       // 17
    //_addKeyword( Add here, Add here);         // 18
};

Keyword *Keywords::getKeyword(byte *str){
  for(size_t i=0; i<_OPERATOR_COUNT; i++){
    lastKeywordFound  = _keywords + i;
    if( IsKeyword(str, lastKeywordFound->name0)) return lastKeywordFound;
    if( IsKeyword(str, lastKeywordFound->name1)) return lastKeywordFound;
  }
  lastKeywordFound = NULL;
  return NULL; // not found
}

byte *Keywords::parse( byte *str){
  lastKeywordFound = NULL;
  byte tmp;
  byte *ptr = str;

  // skip empty space
  while( IsSpacer(*ptr)) ptr++;
  if( !IsNameStarter(*ptr)) return ptr;
  str = ptr;

  while( IsNameCharacter(*ptr)){
    // dollar or percent must be the last character
    if( *ptr == _DOLLAR_ || *ptr == _PERCENT_){
      ptr++;
      break;
    }
    ptr++;
  }
  tmp = *ptr; // preserve symbol
  *ptr = _NUL_;
  if( getKeyword(str) != NULL) str = ptr;
  *ptr = tmp; // restore symbol
  return str;
}

void Keywords::_addKeyword( const char *name0, const char *name1, void *method){
  if( _id >= _OPERATOR_COUNT) return;
  Keyword *tmp = _keywords + _id;
  tmp->id = _id++;
  tmp->name0 = name0;
  tmp->name1 = name1;
  tmp->method = method;
  #ifdef __DEBUG
  Serial.print( tmp->id+1);
  Serial.print(" added operator keyword ");
  Serial.print( tmp->name0);
  Serial.print(", AKA ");
  Serial.print( tmp->name1);
  Serial.println( ".");
  #endif
}
