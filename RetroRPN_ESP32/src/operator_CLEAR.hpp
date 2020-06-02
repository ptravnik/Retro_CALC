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
// Clears programs, variables or constants
//
static bool _operator_CLEAR_( Lexer *lex){
  return lex->operator_CLEAR();
}
static void _clearFake( size_t lFrom, size_t lTo=MAX_LINE_NUMBER){
    Serial.print("clearing program form ");
    Serial.print( lFrom);
    Serial.print(" to ");
    Serial.println( lTo); 
}
bool Lexer::operator_CLEAR(){
  #ifdef __DEBUG
  Serial.println("CLEAR called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  if( IsToken( _lexer_position, "STACK", true) || IsToken( _lexer_position, "stack", true)){
    _vars->clearRPNStack();
    _rsb->setStackRedrawAll();
    _skipToEOL( _lexer_position);
    return true;
  }
  _lexer_position = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    return operator_CLEAR_Vars( false); 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_PROGRAM_KW:
      return operator_CLEAR_Program();
    case _OPR_CONST_KW:
      return operator_CLEAR_Vars( true);
    default:
    break;
  }
  return operator_CLEAR_Program(); 
}

bool Lexer::operator_CLEAR_Program(){
  // TODO
  Serial.println("CLEAR PROGRAM not yet implemented (Program class undefined)");
  byte *ptr = _epar->parse( _lexer_position);
  size_t lFrom = 0;
  if( _epar->result != _RESULT_INTEGER_){
    _clearFake( lFrom);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  else{
    _lexer_position = ptr;
    _ignore_Blanks();
    lFrom = clipLineNumber(_epar->numberParser.integerValue());
  }
  if( _validate_NextCharacter( _COMMA_)){
    _clearFake( lFrom);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  ptr = _epar->parse( _lexer_position);
  if( _epar->result == _RESULT_INTEGER_){
    _clearFake( lFrom, clipLineNumber(_epar->numberParser.integerValue(), lFrom));
    _skipToNextOperator( ptr);
    return true;
  }
  _clearFake( lFrom);
  _skipToNextOperator( ptr);
  return true;
}

bool Lexer::operator_CLEAR_Vars(bool constants){
  _ignore_Blanks();
  #ifdef __DEBUG
  Serial.println("CLEAR vars called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  byte *ptr = _lexer_position;
  if( *ptr == '*' && ptr[1] == '*' && ptr[2] == '*'){
    _skipToNextOperator( ptr);
    if( constants) _vars->removeAllConstants();
    else _vars->removeAllVariables();
    return true;
  }
  ptr = _epar->nameParser.parse( _lexer_position);
  _skipToNextOperator( ptr);
  if( !_epar->nameParser.result) return true;
  _vars->removeByName( _epar->nameParser.Name());
  return true;
}
