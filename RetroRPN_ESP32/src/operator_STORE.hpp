//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

// This include is for convenience of coding; comment out
//#include "Lexer.hpp"

//
// Lists programs, variables or constants
//
static bool _operator_STORE_( Lexer *lex){
  return lex->operator_STORE();
}
static void _storeFake( size_t lFrom, size_t lTo=MAX_LINE_NUMBER){
    Serial.print("listing program form ");
    Serial.print( lFrom);
    Serial.print(" to ");
    Serial.println( lTo); 
}
bool Lexer::operator_STORE(){
  #ifdef __DEBUG
  Serial.println("STORE called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  byte *tmpptr = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    _sdm->saveState();
    _mbox->setLabel( LEX_Message_Saved);
    _skipToNextOperator( _lexer_position);
    return true; 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_VARS_KW:
      return operator_STORE_Vars( false);
    case _OPR_CONST_KW:
      return operator_STORE_Vars( true);
    default:
      _skipToNextOperator( _lexer_position);
      break;
  }
  return true; 
}

bool Lexer::operator_STORE_Vars(bool constants){
  // TODO: get the file name
  Serial.println("STORE VARS and STORE CONST to a file not yet implemented!");
  if( constants) _sdm->storeConstants();
  else _sdm->storeVariables();
  _skipToNextOperator( _lexer_position);
  return true;
}
