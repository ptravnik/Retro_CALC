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
// Clears program code and variables. Constants intact.
//
static bool _operator_NEW_( Lexer *lex){
  return lex->operator_NEW();
}

bool Lexer::operator_NEW(){
  #ifdef __DEBUG
  Serial.println("NEW called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  isRunning = false;
  _vars->removeAllVariables();
  _code->clearProgram();
  _mbox->setLabel( LEX_Message_NewProgram);
  return _skipToEOL();
}
