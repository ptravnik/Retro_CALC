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
// Pushes value int stack
//
static bool _operator_PUSH_( Lexer *lex){
  return lex->operator_PUSH();
}
bool Lexer::operator_PUSH(){
  #ifdef __DEBUG
  Serial.println("PUSH called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  // parse expression
  _lexer_position = _epar->parseAlgebraic(_lexer_position);
  switch( _epar->result ){
    case _RESULT_INTEGER_:
    case _RESULT_REAL_:
      _vars->saveRPNPrev();
      _vars->pushRPNStack( _epar->numberParser.realValue());
      break;
    default:
      _vars->pushRPNStack();
      break;
  }
  _rsb->setStackRedrawAll();
  _rsb->updateIOM( true);
  _skipToNextOperator( _lexer_position);
  return true;
}
