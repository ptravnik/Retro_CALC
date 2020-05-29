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
static bool _operator_LOAD_( Lexer *lex){
  return lex->operator_LOAD();
}
static void _loadFake( size_t lFrom, size_t lTo=MAX_LINE_NUMBER){
    Serial.print("listing program form ");
    Serial.print( lFrom);
    Serial.print(" to ");
    Serial.println( lTo); 
}
bool Lexer::operator_LOAD(){
  #ifdef __DEBUG
  Serial.println("LOAD called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  byte *tmpptr = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    return operator_LOAD_Program(); 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_VARS_KW:
      return operator_LOAD_Vars( false);
    case _OPR_CONST_KW:
      return operator_LOAD_Vars( true);
    default:
    break;
  }
  return operator_LOAD_Program(); 
}

bool Lexer::operator_LOAD_Program(){
  // TODO
  Serial.println("LOAD PROGRAM not yet implemented (Program class undefined)");
  byte *ptr = _epar->parse( _lexer_position);
  size_t lFrom = 0;
  if( _epar->result != _RESULT_INTEGER_){
    _loadFake( lFrom);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  else{
    _lexer_position = ptr;
    _ignore_Blanks();
    lFrom = clipLineNumber(_epar->numberParser.integerValue());
  }
  if( _validate_NextCharacter( _COMMA_)){
    _loadFake( lFrom);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  ptr = _epar->parse( _lexer_position);
  if( _epar->result == _RESULT_INTEGER_){
    _loadFake( lFrom, clipLineNumber(_epar->numberParser.integerValue(), lFrom));
    _skipToNextOperator( ptr);
    return true;
  }
  _loadFake( lFrom);
  _skipToNextOperator( ptr);
  return true;
}

bool Lexer::operator_LOAD_Vars(bool constants){
  // TODO
  Serial.println("LOAD VARS and LOAD CONST for LCD are not yet implemented!");
  _iom->sendLn();
  _iom->sendStringUTF8Ln( constants?
      LEX_Message_ConstantsList:
      LEX_Message_VariableList);
  VariableToken vt = constants?
      _vars->getFirstConst():
      _vars->getFirstVar();
  while( vt){
    _iom->sendStringLn( _vars->getVarName( vt));
    vt = _vars->getNextVar( vt);
  }
  _skipToNextOperator( _lexer_position);
  return true;
}
