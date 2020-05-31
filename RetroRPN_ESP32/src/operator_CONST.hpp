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
// Creates a constant
//
static bool _operator_CONST_( Lexer *lex){
  return lex->operator_CONST();
}
bool Lexer::operator_CONST(){
  #ifdef __DEBUG
  Serial.println("CONST called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  byte *ptr = _epar->nameParser.parse( _lexer_position);
  if( !_epar->nameParser.result){
    _skipToNextOperator( ptr);
     return true;
  }
  lastVariable = _vars->getOrCreateNumber( true, _epar->nameParser.Name());
  if( lastVariable == 0){
    _mbox->setLabel(LEX_Error_OutOfMemory);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  if( _vars->isReadOnly(lastVariable)){
    _skipToNextOperator( _lexer_position);
    return true;
  }
  _lexer_position = ptr; // name found, looking for assignment
  if( !_findAssignment()){ // no assignment take from stack
    _vars->setValueReal( lastVariable, _vars->getRPNRegister());
    _skipToNextOperator( _lexer_position);
     return true;
  }
  // parse expression
  _lexer_position = _epar->parse(_lexer_position);
  switch( _epar->result ){
    case _RESULT_INTEGER_:
      _vars->setValueInteger( lastVariable, _epar->numberParser.integerValue());
      break;
    case _RESULT_REAL_:
      _vars->setValueReal( lastVariable, _epar->numberParser.realValue());
      break;
    default:
      _vars->setValueReal( lastVariable, 0.0);
      break;
  }
  _skipToNextOperator( _lexer_position);
  return true;
}
