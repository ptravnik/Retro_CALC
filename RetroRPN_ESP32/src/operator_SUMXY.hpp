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
// 2-D statistics
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
    _vars->setRPNRegister( _vars->stErrXY, 2);
  }
  _vars->setRPNLabelZ( LEX_Message_StdError);
  _vars->setRPNLabelY( LEX_Message_Gain);
  _vars->setRPNLabelX( LEX_Message_Offset);
  _vars->setScrMessage( LEX_Message_SumUpdated);
  _mbox->setRedrawRequired();
  _rsb->setStackRedrawAll();
  _rsb->setLabelRedrawAll();
  _skipToNextOperator( _lexer_position);
  return true;
}
