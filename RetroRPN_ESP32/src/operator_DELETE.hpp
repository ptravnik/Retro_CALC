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
// Deletes files or folders
//
static bool _operator_DELETE_( Lexer *lex){
  return lex->operator_DELETE();
}

bool Lexer::operator_DELETE(){
  #ifdef __DEBUG
  Serial.println("DELETE called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  if( !_epar->filenameParser.result){
    _mbox->setLabel( LEX_Warning_NoSuchFolder);
    return false;
  }
  _sdm->deleteEntity( (char *)_epar->filenameParser.Name());
  if( _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
  else _mbox->setLabel( LEX_Message_Deleted);
  _sdm->checkRootExists();
  _skipToEOL(_lexer_position);
  return true;
}
