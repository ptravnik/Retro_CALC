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
// Restores variables or constants
//
static bool _operator_RESTORE_( Lexer *lex){
  return lex->operator_RESTORE();
}
static bool _operator_APPEND_( Lexer *lex){
  return lex->operator_APPEND();
}

bool Lexer::operator_RESTORE(){
  #ifdef __DEBUG
  Serial.println("RESTORE called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  _lexer_position = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    _vars->restoreReadPosition();
    _skipToNextOperator( _lexer_position);
    return true; 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_VARS_KW:
      return operator_RESTORE_Vars( true);
    case _OPR_CONST_KW:
      return operator_RESTORE_Const( true);
    default:
      _skipToNextOperator( _lexer_position);
      break;
  }
  return true; 
}

bool Lexer::operator_APPEND(){
  #ifdef __DEBUG
  Serial.println("APPEND called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  _lexer_position = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    // TODO: append program file here
    operator_RESTORE_Vars( false);
    return true; 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_VARS_KW:
      return operator_RESTORE_Vars( false);
    case _OPR_CONST_KW:
      return operator_RESTORE_Const( false);
    default:
      _skipToNextOperator( _lexer_position);
      break;
  }
  return true; 
}

bool Lexer::operator_RESTORE_Vars( bool remove){
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  if( remove)_vars->removeAllVariables();
  readBASICConstantFile( _epar->filenameParser.result?
      (const char*)_epar->filenameParser.Name(): RPN_SaveConstants);    
  _mbox->setLabel( ( _sdm->LastError != NULL)?
      _sdm->LastError: LEX_Message_Restored);
  _skipToEOL(_lexer_position);
  return true;
}

bool Lexer::operator_RESTORE_Const( bool remove){
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  if( remove) _vars->removeAllConstants();
  readBASICConstantFile( _epar->filenameParser.result?
      (const char*)_epar->filenameParser.Name(): RPN_SaveConstants);    
  _mbox->setLabel( ( _sdm->LastError != NULL)?
      _sdm->LastError: LEX_Message_Restored);
  _skipToEOL(_lexer_position);
  return true;
}

//
// reads BASIC file from disk and performs immediate evaluation
// for constants only TODO: use empty prog space for parsing
//
void Lexer::readBASICConstantFile( const char *name){
  byte *buff = (byte *)malloc( INPUT_COLS);
  if( buff == NULL){
    _mbox->setLabel(LEX_Error_OutOfMemory);
    return;
  }
  if( _sdm->openDataFileRead( name)){
    free( buff);
    return;
  }
  while( true){
    if( _sdm->readString( buff, INPUT_COLS) == 0) break;
    #ifdef __DEBUG
    Serial.println ((char *)buff);
    #endif
    byte *ptr = buff;
    while( *ptr && IsDigit( *ptr)) ptr++;
    parse( ptr);
  }
  free(buff);
  _sdm->closeFile();
}
