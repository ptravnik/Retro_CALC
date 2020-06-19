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
// Reports memory usage
//
static bool _operator_MEM_( Lexer *lex){
  return lex->operator_MEM();
}
bool Lexer::operator_MEM(){
  #ifdef __DEBUG
  Serial.println("MEM called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _iom->sendStringUTF8Ln( LEX_Message_ProgMemory);
  snprintf( (char *)_io_buffer, INPUT_COLS, "  %d / %d", _code->getMemoryAvailable(), BASIC_SPACE);
  _iom->sendStringUTF8Ln( _io_buffer);
  _iom->sendStringUTF8Ln( LEX_Message_VarsMemory);
  snprintf( (char *)_io_buffer, INPUT_COLS, "  %d / %d", _vars->getMemoryAvailable(), VARIABLE_SPACE);
  _iom->sendStringUTF8Ln( _io_buffer);
  _skipToNextOperator( _lexer_position);
  return true;
}
