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
// Stores variables or constants
//
static bool _operator_STORE_( Lexer *lex){
  return lex->operator_STORE();
}

static void _writeLineStarter( char *buff, const char *format,
    int16_t lineNumber, byte *varName, SDManager *sdm){
  int n = snprintf( buff, INPUT_COLS, format, lineNumber);
  convertToUTF8( buff+n, varName, INPUT_COLS-n);
  sdm->print( buff, false);
}

static void _writeScalar( char *buff, double value, NumberParser *npar, SDManager *sdm){
  sdm->print( " = ", false);
  npar->stringValue( value, (byte*)buff);
  sdm->print( buff);
}

static void _writeIndex( char *buff, int value, SDManager *sdm){
  snprintf( buff, INPUT_COLS, "[%d]", value);
  sdm->print( buff, false);
}

static void _writeDoubleIndex( char *buff, int value1, int value2, SDManager *sdm){
  snprintf( buff, INPUT_COLS, "[%d,%d]", value1, value2);
  sdm->print( buff, false);
}

static void _writeString( char *buff, byte *str, SDManager *sdm){
  snprintf( buff, INPUT_COLS, " = \"");
  sdm->print( buff, false);
  convertToUTF8( buff, str, INPUT_COLS);
  sdm->print( buff, false);
  sdm->print( "\"");
}

static uint16_t _writeVariable( char *buff, const char *format, uint16_t lineNumber,
    VariableToken vt, Variables *vars, NumberParser *npar, SDManager *sdm){
  size_t xsize, ysize; 
  switch( vars->getVarType( vt)){
    case VARTYPE_NUMBER:
      _writeLineStarter( buff, format, lineNumber, vars->getVarName(vt), sdm);
      _writeScalar( buff, vars->realValue(vt), npar, sdm);
      lineNumber += PROGLINE_INCREMENT;
      break;
    case VARTYPE_VECTOR:
      xsize = vars->getTotalSize( vt);
      for( int i=0; i<xsize; i++){
        _writeLineStarter( buff, format, lineNumber, vars->getVarName(vt), sdm);
        _writeIndex( buff, i, sdm);
        _writeScalar( buff, vars->realValue(vt, i), npar, sdm);
        lineNumber += PROGLINE_INCREMENT;
      }
      break;
    case VARTYPE_MATRIX:
      xsize = vars->getRowSize( vt);
      ysize = vars->getColumnSize( vt);
      for( size_t j; j<ysize; j++){
        for( size_t i; i<xsize; i++){
          _writeLineStarter( buff, format, lineNumber, vars->getVarName(vt), sdm);
          _writeDoubleIndex( buff, j, i, sdm);
          _writeScalar( buff, vars->realValue(vt, i, j), npar, sdm);
          lineNumber += PROGLINE_INCREMENT;
        }
      }
      break;
    case VARTYPE_STRING:
      _writeLineStarter( buff, format, lineNumber, vars->getVarName(vt), sdm);
      _writeString( buff, vars->stringValue( vt), sdm);
      lineNumber += 10;
      break;
    default:
      _writeLineStarter( buff, format, lineNumber, vars->getVarName(vt), sdm);
      _writeString( buff, (byte*)"None", sdm);
      lineNumber += PROGLINE_INCREMENT;
      break;
  }
  return lineNumber;
}

bool Lexer::operator_STORE(){
  //#ifdef __DEBUG
  Serial.println("STORE called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  //#endif
  _ignore_Blanks();
  _lexer_position = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    Serial.println("No keyword");
    _sdm->saveState();
    _mbox->setLabel( LEX_Message_Saved);
    _skipToNextOperator( _lexer_position);
    return true; 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_VARS_KW:
      Serial.println("Keyword VARS");
      return operator_STORE_Vars();
    case _OPR_CONST_KW:
      Serial.println("Keyword CONST");
      return operator_STORE_Const();
    default:
      _skipToNextOperator( _lexer_position);
      break;
  }
  return true; 
}

bool Lexer::operator_STORE_Vars(){
  Serial.println("STORE VARS called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  bool resError = _sdm->openDataFileWrite( _epar->filenameParser.result?
      (char *)_epar->filenameParser.Name(): (char *)LEX_Standard_Variables);
  if( resError){
    if( _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
    return true;
  }
  int16_t lineNumber = 10;
  VariableToken vt = _vars->getFirstVar();
  NumberParser *npar = &(_epar->numberParser);
  char *buff = (char *)_io_buffer;
  while( vt){
    lineNumber = _writeVariable( buff, "%05u LET ", lineNumber, vt, _vars, npar, _sdm);
    vt = _vars->getNextVar( vt);
  }
  _sdm->closeFile();
  _mbox->setLabel( ( _sdm->LastError == NULL)?
      LEX_Message_Saved: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}

bool Lexer::operator_STORE_Const(){
  Serial.println("STORE CONST called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  _ignore_Blanks();
  _lexer_position = _epar->filenameParser.parse( _lexer_position);
  bool resError = _sdm->openDataFileWrite( _epar->filenameParser.result?
      (char *)_epar->filenameParser.Name(): (char *)LEX_Standard_Constants);
  if( resError){
    if( _sdm->LastError != NULL) _mbox->setLabel( _sdm->LastError);
    return true;
  }
  int16_t lineNumber = 10;
  VariableToken vt = _vars->getFirstConst();
  NumberParser *npar = &(_epar->numberParser);
  char *buff = (char *)_io_buffer;
  while( vt){
    lineNumber = _writeVariable( buff, "%05u CONST ", lineNumber, vt, _vars, npar, _sdm);
    vt = _vars->getNextVar( vt);
    if( _vars->isReadOnly( vt)) break;
  }
  _sdm->closeFile();
  _mbox->setLabel( ( _sdm->LastError == NULL)?
      LEX_Message_Saved: _sdm->LastError);
  _skipToEOL(_lexer_position);
  return true;
}
