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
static bool _operator_LOAD_( Lexer *lex){
  return lex->operator_LOAD();
}

bool Lexer::operator_LOAD(){
  // TODO: if this called from a running program, need to stop gracefully
  // For now, LOAD is only allowed from parseInteractive.
  isRunning = false;
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  bool rs = _sdm->openProgramFileRead( _epar->filenameParser.result?
      (char *)_epar->filenameParser.Name(): NULL);
  if( rs){
    if( _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
    return true;
  }
  _vars->removeAllVariables();
  _code->clearProgram();
  while( true){
    if( !_sdm->readln(_io_buffer)) break;
    if( _code->addLine( _io_buffer)) break;
  }
  _sdm->closeFile();
  _vars->setPrgCounter( _code->getFirstLine().lineNumber);
  _mbox->setLabel( ( _sdm->LastError == NULL)?
      LEX_Message_Loaded: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}

void Lexer::_loadBASICFile( const char *name, bool ignoreErrors){
  if( _sdm->openProgramFileRead( name)){
    if( !ignoreErrors && _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
    return;
  }
  while( true){
    if( !_sdm->readln(_io_buffer)) break;
    if( _code->addLine( _io_buffer)) break;
  }
  _sdm->closeFile();
  _vars->setPrgCounter( _code->getFirstLine().lineNumber);
}
