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
// Lists programs, variables or constants
//
static bool _operator_LIST_( Lexer *lex){
  return lex->operator_LIST();
}

bool Lexer::operator_LIST(){
  #ifdef __DEBUG
  Serial.println("LIST called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  byte *tmpptr = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    return operator_LIST_Program(); 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_VARS_KW:
      return operator_LIST_Vars( false);
    case _OPR_CONST_KW:
      return operator_LIST_Vars( true);
    default:
      return operator_LIST_Program(); 
  }
}

bool Lexer::operator_LIST_Program(){
  // TODO
  Serial.println("LIST PROGRAM:");
  byte nParsed = _parseList( 2);
  uint16_t lFrom = 0;
  uint16_t lTo = MAX_LINE_NUMBER;
  switch(nParsed){
    case 2:
      lTo = (uint16_t)floor( _listValues[1]);
    case 1:
      lFrom = (uint16_t)floor( _listValues[0]);
    default:
      break;
  }
  //char *buff = (char *)_iom->getIOBuffer();
  ProgramLine pl = _code->getFirstLine();
  while( true){
    if( pl.line == NULL) break;
    if( pl.lineNumber > lTo) break;
    if( pl.lineNumber < lFrom){
      pl = _code->getNextLine( pl);
      continue;
    }
    //int n = snprintf( buff, INPUT_COLS, "%05u ", pl.lineNumber);
    //convertToUTF8( buff+n, pl.line, INPUT_COLS-n);
    _iom->sendStringUTF8Ln( _code->getLineString( pl, true));
    pl = _code->getNextLine( pl);
  }
  _skipToNextOperator( _lexer_position);
  return true;
}

bool Lexer::operator_LIST_Vars(bool constants){
  // TODO
  Serial.println("LIST VARS and LIST CONST for LCD are not yet implemented!");
  _iom->sendLn();
  _iom->sendStringUTF8Ln( constants?
      LEX_Message_ConstantsList:
      LEX_Message_VariableList);
  VariableToken vt = constants?
      _vars->getFirstConst():
      _vars->getFirstVar();
  while( vt){
    char *ptr = convertToUTF8( (char *)_io_buffer, _vars->getVarName( vt), INPUT_COLS);
    switch( _vars->getVarType(vt)){
      case VARTYPE_VECTOR:
        snprintf( ptr, INPUT_COLS - _vars->getVarNameLength( vt), "[%d]", _vars->getTotalSize(vt));
        break;
      case VARTYPE_MATRIX:
        snprintf( ptr, INPUT_COLS - _vars->getVarNameLength( vt), "[%d,%d]", _vars->getRowSize(vt),_vars->getColumnSize(vt));
        break;
      case VARTYPE_STRING:
        snprintf( ptr, INPUT_COLS - _vars->getVarNameLength( vt), "[%d] (string)", _vars->getTotalSize(vt));
        break;
      default:
        break;
    }
    _iom->sendStringUTF8Ln( (char *)_io_buffer);
    vt = _vars->getNextVar( vt);
  }
  _skipToNextOperator( _lexer_position);
  return true;
}
