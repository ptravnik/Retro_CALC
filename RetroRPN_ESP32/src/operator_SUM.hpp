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
static bool _operator_SUM_( Lexer *lex){
  return lex->operator_SUM();
}
bool Lexer::operator_SUM(){
  #ifdef __DEBUG
  Serial.println("SUM called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  // parse expression
  _lexer_position = _epar->parse(_lexer_position);
  switch( _epar->result ){
    case _RESULT_INTEGER_:
    case _RESULT_REAL_:
      _vars->addSample2RPNSum( _epar->numberParser.realValue());
      break;
    default:
      _vars->saveRPNPrev();
      _vars->addSample2RPNSum( _vars->getRPNRegister());
      _vars->popRPNStack();
      break;
  }
  _vars->pushRPNStack( _vars->nmean[0]);
  _vars->pushRPNStack( _vars->stdev[0]);
  _vars->pushRPNStack( _vars->mean[0]);
  _rsb->setStackRedrawAll();
  _rsb->setRPNLabel( 2, LEX_Message_N);
  _rsb->setRPNLabel( 1, LEX_Message_stDev);
  _rsb->setRPNLabel( 0, LEX_Message_Mean);
  _rsb->updateIOM();
  _mbox->setLabel( LEX_Message_SumUpdated);
  _skipToNextOperator( _lexer_position);
  return true;
}
