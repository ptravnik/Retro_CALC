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
static bool _operator_SUMXY_( Lexer *lex){
  return lex->operator_SUMXY();
}
bool Lexer::operator_SUMXY(){
  #ifdef __DEBUG
  Serial.println("SUM called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  // parse expression
  byte nArgs = _parseList( 2);
  if( nArgs == 2){
    _vars->addSample2RPNSumXY( _listValues[0], _listValues[1]);
    _vars->pushRPNStack( _vars->stErrXY);
    _vars->pushRPNStack( _vars->gain[0]);
    _vars->pushRPNStack( _vars->offset[0]);
  }
  else{
    _vars->addSample2RPNSumXY( _vars->getRPNRegister(1), _vars->getRPNRegister(0));
    _vars->pushRPNStack( _vars->offset[0]);
    _vars->setRPNRegister( _vars->gain[0], 1);
    _vars->setRPNRegister( _vars->offset[0], 2);
  }
  _rsb->setStackRedrawAll();
  _rsb->setRPNLabel( 2, LEX_Message_StdError);
  _rsb->setRPNLabel( 1, LEX_Message_Gain);
  _rsb->setRPNLabel( 0, LEX_Message_Offset);
  _rsb->updateIOM();
  _mbox->setLabel( LEX_Message_SumUpdated);
  _skipToNextOperator( _lexer_position);
  return true;
}
