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
// Implements comments
//
static bool _operator_REM_( Lexer *lex){
  return lex->operator_REM();
}
bool Lexer::operator_REM(){
  #ifdef __DEBUG
  Serial.println("REM called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _skipToEOL( _lexer_position);
  return true;
}
