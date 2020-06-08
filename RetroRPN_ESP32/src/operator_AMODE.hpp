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
static bool _operator_AMODE_( Lexer *lex){
  return lex->operator_AMODE();
}
bool Lexer::operator_AMODE(){
  #ifdef __DEBUG
  Serial.println("AMODE called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  // parse expression
  _lexer_position = _epar->parse(_lexer_position);
  switch( _epar->result ){
    case _RESULT_INTEGER_:
    case _RESULT_REAL_:
      _vars->setAngleMode( (byte)_epar->numberParser.integerValue());
      break;
    default:
      _vars->setAngleMode( 0);
      break;
  }
  _mbox->setLabel( _vars->getAMODEString());
  _skipToNextOperator( _lexer_position);
  return true;
}
