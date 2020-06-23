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
static bool _operator_RESTORE_( Lexer *lex){
  return lex->operator_RESTORE();
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
      return operator_RESTORE_Vars();
    case _OPR_CONST_KW:
      return operator_RESTORE_Const();
    default:
      _skipToNextOperator( _lexer_position);
      break;
  }
  return true; 
}

bool Lexer::operator_RESTORE_Vars(){
  Serial.println( "RESTORE VARS not implemented");
  // _ignore_Blanks();
  // _lexer_position = _epar->filenameParser.parse( _lexer_position);
  // bool resError = false;
  // if( _epar->filenameParser.result)
  //   resError = _sdm->openDataFileWrite( (char *)_epar->filenameParser.Name());    
  // else{
  //   resError = _sdm->openDataFileWrite( (char *)LEX_Standard_Variables);    
  // }
  // if( resError){
  //   if( _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
  //   return true;
  // }
  // int16_t lineNumber = 10;
  // VariableToken vt = _vars->getFirstVar();
  // NumberParser *npar = &(_epar->numberParser);
  // char *buff = (char *)_io_buffer;
  // while( vt){
  //   lineNumber = _writeVariable( buff, "%05u LET ", lineNumber, vt, _vars, npar, _sdm);
  //   vt = _vars->getNextVar( vt);
  // }
  // _sdm->closeFile();
  // _mbox->setLabel( ( _sdm->LastError == NULL)?
  //     LEX_Message_Saved: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}

bool Lexer::operator_RESTORE_Const(){
  Serial.println( "RESTORE CONST not implemented");
  // _ignore_Blanks();
  // _lexer_position = _epar->filenameParser.parse( _lexer_position);
  // bool resError = false;
  // if( _epar->filenameParser.result)
  //   resError = _sdm->openDataFileWrite( (char *)_epar->filenameParser.Name());    
  // else{
  //   resError = _sdm->openDataFileWrite( (char *)LEX_Standard_Constants);    
  // }
  // if( resError){
  //   if( _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
  //   return true;
  // }
  // int16_t lineNumber = 10;
  // VariableToken vt = _vars->getFirstConst();
  // NumberParser *npar = &(_epar->numberParser);
  // char *buff = (char *)_io_buffer;
  // while( vt){
  //   lineNumber = _writeVariable( buff, "%05u CONST ", lineNumber, vt, _vars, npar, _sdm);
  //   vt = _vars->getNextVar( vt);
  //   if( _vars->isReadOnly( vt)) break;
  // }
  // _sdm->closeFile();
  // _mbox->setLabel( ( _sdm->LastError == NULL)?
  //     LEX_Message_Saved: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}

// void SDManager::storeConstants(){
//   if(!SDInserted) return;
//   if(!SDMounted) return;
//   File file = SD.open( RPN_SaveConstantsFile, FILE_WRITE);
//   if(!file){
//     #ifdef __DEBUG
//     Serial.println("Failed to open constants file for writing");
//     #endif
//     _mbox->setLabel(SD_Error_WriteFailed);
//     return;
//   }
//   VariableToken vt = _vars->getFirstConst();
//   size_t lineNumber = 10;
//   char *ptr = (char *)_io_buffer;
//   int offset, end_point;
//   #ifdef __DEBUG
//   Serial.println("Saving constants:");
//   #endif
//   while( !_vars->isReadOnly( vt)){
//     _writeVariable( &file, ConstantFileFormat, lineNumber, vt);
//     vt = _vars->getNextVar( vt);
//     if( !vt) break;
//     lineNumber += 10;
//   }
//   file.close();
// }


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
//   _currentFile.close();
// }
