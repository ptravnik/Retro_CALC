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
// Rudimentary RUN; TODO
//
static bool _operator_RUN_( Lexer *lex){
  return lex->operator_RUN();
}
bool Lexer::operator_RUN(){
  #ifdef __DEBUG
  Serial.println("RUN called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  Serial.println();
  Serial.println("Run called:");
  byte *ptr = _lexer_position;
  char *buff = (char *)_iom->getIOBuffer();
  ProgramLine pl = _code->getFirstLine();
  _vars->setPrgCounter( pl.lineNumber);
  while( pl.line){
    if( _iom->scan() == 10){
      Serial.println("HALT!");
      while( _iom->scan());
      break;
    };
    int n = snprintf( buff, INPUT_COLS, "%05u ", pl.lineNumber);
    convertToUTF8( buff+n, pl.line, INPUT_COLS-n);
    _iom->sendStringUTF8Ln( buff);
    _vars->setPrgCounter( pl.lineNumber);
    parse( pl.line);
    switch( result){
      case _RESULT_EXECUTED_:
        snprintf( buff, INPUT_COLS, "OK");
        break;
      case _RESULT_INTEGER_:
        n = snprintf( buff, INPUT_COLS, "Result integer: ");
        _epar->numberParser.stringValue( (byte *)buff + n, INPUT_COLS-n);
        break;
      case _RESULT_REAL_:
        n = snprintf( buff, INPUT_COLS, "Result real: ");
        _epar->numberParser.stringValue( (byte *)buff + n, INPUT_COLS-n);
        break;
      case _RESULT_STRING_:
        n = snprintf( buff, INPUT_COLS, "Unknown: ");
        convertToUTF8( buff+n, pl.line, INPUT_COLS-n);
        break;
      defeult:
        snprintf( buff, INPUT_COLS, "Result undefined");
        break;
    }
    _iom->sendStringUTF8Ln( buff);      
    pl = _code->getNextLine( pl);
  }
  Serial.println("Run completed");
  _skipToEOL( ptr);
  return true;
}
