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
// Lists directory content to terminal
//
static bool _operator_DIR_( Lexer *lex){
  return lex->operator_DIR();
}

bool Lexer::operator_DIR(){
  #ifdef __DEBUG
  Serial.println("DIR called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  if( _epar->filenameParser.result) _sdm->listFolder( (char *)_epar->filenameParser.Name());
  else _sdm->listFolder( _vars->currentDir);
  _skipToEOL(_lexer_position);
  return true;
}

//
// Sets new directory (changed keyword to "CD")
//
static bool _operator_DIRECTORY_( Lexer *lex){
  return lex->operator_DIRECTORY();
}

bool Lexer::operator_DIRECTORY(){
  #ifdef __DEBUG
  Serial.println("CD called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  if( !_epar->filenameParser.result || !_sdm->checkEntityExists( (char *)_epar->filenameParser.Name())){
    _mbox->setLabel( LEX_Warning_NoSuchFolder);
    strncat2( _vars->currentDir, "/", CURRENT_DIR_LEN);
    _skipToEOL(_lexer_position);
  return true;
  }
  strncat2( _vars->currentDir, (char *)_epar->filenameParser.Name(), CURRENT_DIR_LEN);
  _skipToEOL(_lexer_position);
  return true;
}
