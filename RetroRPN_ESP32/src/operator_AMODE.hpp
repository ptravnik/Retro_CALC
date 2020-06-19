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
// Sets the angle mode for trigonometry
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
  byte nParsed = _parseList( 1);
  if( nParsed >=1 ){
    _vars->setAngleMode( (byte)_listValues[0]);
  }
  else{
    _vars->setAngleMode( (byte)_vars->getRPNRegister());
    _vars->popRPNStack();
    _rsb->setStackRedrawAll();
  }
  _mbox->setLabel( _vars->getAMODEString());
  result = _RESULT_EXECUTED_;
  _skipToNextOperator( _lexer_position);
  return true;
}
