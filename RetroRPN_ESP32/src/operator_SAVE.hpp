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
// Saves program code
//
static bool _operator_SAVE_( Lexer *lex){
  return lex->operator_SAVE();
}

bool Lexer::operator_SAVE(){
  #ifdef __DEBUG
  Serial.println("SAVE called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  // TODO: if this called from a running program, need to stop gracefully
  // For now, SAVE is only allowed from parseInteractive.
  isRunning = false;
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  bool rs = _sdm->openProgramFileWrite( _epar->filenameParser.result?
      (char *)_epar->filenameParser.Name(): NULL); 
  if( rs){
    if( _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
    return true;
  }
  ProgramLine pl = _code->getFirstLine();
  while( true){
    if( pl.line == NULL) break;
    _sdm->print( (char *)_code->getLineString( pl, true));
    pl = _code->getNextLine( pl);
  }
  _sdm->closeFile();
  _mbox->setLabel( ( _sdm->LastError == NULL)?
      LEX_Message_Saved: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}
