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
// Loads program code from file
//
static bool _operator_MKDIR_( Lexer *lex){
  return lex->operator_MKDIR();
}

bool Lexer::operator_MKDIR(){
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  if( !_epar->filenameParser.result) return true;
  _sdm->createFolder((char *)_epar->filenameParser.Name());
  _mbox->setLabel( ( _sdm->LastError == NULL)?
      LEX_Message_Created: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}
