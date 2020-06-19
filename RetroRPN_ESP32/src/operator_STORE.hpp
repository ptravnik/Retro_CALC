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
// Stores variables or constants TODO
//
static bool _operator_STORE_( Lexer *lex){
  return lex->operator_STORE();
}
static void _storeFake( size_t lFrom, size_t lTo=MAX_LINE_NUMBER){
    Serial.print("listing program form ");
    Serial.print( lFrom);
    Serial.print(" to ");
    Serial.println( lTo); 
}
bool Lexer::operator_STORE(){
  #ifdef __DEBUG
  Serial.println("STORE called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  byte *tmpptr = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    _sdm->saveState();
    _mbox->setLabel( LEX_Message_Saved);
    _skipToNextOperator( _lexer_position);
    return true; 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_VARS_KW:
      return operator_STORE_Vars( false);
    case _OPR_CONST_KW:
      return operator_STORE_Vars( true);
    default:
      _skipToNextOperator( _lexer_position);
      break;
  }
  return true; 
}

bool Lexer::operator_STORE_Vars(bool constants){
  // // TODO: get the file name
  // Serial.println("STORE VARS and STORE CONST to a file not yet implemented!");
  // if( constants) _sdm->storeConstants();
  // else _sdm->storeVariables();
  // _skipToNextOperator( _lexer_position);
  // return true;
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
    _sdm->writeln( (char *)_code->getLineString( pl, true));
    pl = _code->getNextLine( pl);
  }
  _sdm->closeProgramFile();
  _mbox->setLabel( ( _sdm->LastError == NULL)?
      LEX_Message_Loaded: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}

//
// reads BASIC file from disk and performs immediate evaluation
// for constants only
// TODO: This is a Kludge - should be integrated with a parser
//
// void SDManager::readBASICConstantFile( bool constants, const char *name){
//   if( openProgramFileRead( name)) return;
//   byte *buff = (byte *)malloc(INPUT_COLS_UTF);
//   if( buff == NULL){
//     LastError = SD_Error_OutOfMemory;
//     return;
//   }
//   while( true){
//     if( !readln( buff)) break;

//     // Skip the line number and check if the line starts with CONST
//     byte *ptr = _io_buffer;
//     while( IsDigit( *ptr)) ptr++;
//     ptr = _kwds->parse( ptr);
//     if( _kwds->lastKeywordFound == NULL) continue;
//     if( constants && _kwds->lastKeywordFound->id != _OPR_CONST_KW) continue;
//     if( !constants && _kwds->lastKeywordFound->id != _OPR_LET_KW) continue;
//     ptr = _epar->nameParser.parse( ptr);
//     if( !_epar->nameParser.result) continue;
//     VariableToken vt = _vars->findByName( _epar->nameParser.Name());
//     if( vt && _vars->isReadOnly( vt)) continue;
//     ptr = ignore_Blanks( ptr);
//     if( *ptr++ != '=') continue;
//     ptr = ignore_Blanks( ptr);
//     ptr = _epar->numberParser.parse( ptr);
//     switch( _epar->numberParser.result){
//       case _RESULT_INTEGER_:
//         vt = _vars->getOrCreateNumber( constants, _epar->nameParser.Name());
//         _vars->setValueReal( vt, _epar->numberParser.integerValue());
//         break;
//       case _RESULT_REAL_:
//         vt = _vars->getOrCreateNumber( constants, _epar->nameParser.Name());
//         _vars->setValueReal( vt, _epar->numberParser.realValue());
//         break;
//       default:
//         break;
//     }
//   }
//   free(buff);
//   _currentProgramFile.close();
// }
