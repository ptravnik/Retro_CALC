//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Lexer.hpp"

//#define __DEBUG

const char RPN_SaveVariables[] PROGMEM = "/_RPN_SaveVars.bin";
const char RPN_SaveConstants[] PROGMEM = "/_RPN_SaveConstants.bas";
const char RPN_SaveProgram[] PROGMEM = "/_RPN_SaveProgram.bin";
const char RPN_SaveStatus[] PROGMEM = "/_RPN_SaveStatus.txt";
const char RPN_AutoexecFile[] PROGMEM = "/autotest.bas";

const char LEX_Error_OutOfMemory[] PROGMEM = "Err: Out of memory";
const char LEX_Error_NoSDCard[] PROGMEM = "Err: No card";
const char LEX_Warning_ReadOnly[] PROGMEM = "Warn: Read-only";
const char LEX_Warning_NoSuchFolder[] PROGMEM = "Warn: Not found";
const char LEX_Message_VariableList[] PROGMEM = "Variables:";
const char LEX_Message_ConstantsList[] PROGMEM = "Constants:";
const char LEX_Message_ProgMemory[] PROGMEM = "Program Memory:";
const char LEX_Message_VarsMemory[] PROGMEM = "Variable Memory:";
const char LEX_Message_Loaded[] PROGMEM = "Loaded";
const char LEX_Message_Saved[] PROGMEM = "Saved";
const char LEX_Message_Deleted[] PROGMEM = "Deleted";
const char LEX_Message_Created[] PROGMEM = "Created";
const char LEX_Message_Restored[] PROGMEM = "Restored";
const char LEX_Message_SumUpdated[] PROGMEM = "Sum Updated";
const char LEX_Message_N[] PROGMEM = "N:";
const char LEX_Message_stDev[] PROGMEM = "StDev:";
const char LEX_Message_Mean[] PROGMEM = "Mean:";
const char LEX_Message_StdError[] PROGMEM = "Std Error:";
const char LEX_Message_Gain[] PROGMEM = "Gain:";
const char LEX_Message_Offset[] PROGMEM = "Offset:";
const char LEX_Message_NewProgram[] PROGMEM = "New program";
const char LEX_Message_FolderSet[] PROGMEM = "Folder set";

const char LEX_Standard_Variables[] PROGMEM = "/_VARS_.bas";
const char LEX_Standard_Constants[] PROGMEM = "/_CONST_.bas";

const char LEX_StorageMarker_Current_UI[] PROGMEM = "# Current_UI = ";
const char LEX_StorageMarker_Command[] PROGMEM = "# Command = ";
const char LEX_StorageMarker_Command_Prev[] PROGMEM = "# Command_Prev = ";
const char LEX_StorageMarker_Cursor_Column[] PROGMEM = "# Cursor_Column = ";


// add operator includes here
#include "operator_AMODE.hpp"
#include "operator_CLEAR.hpp"

static bool _operator_CLI_( Lexer *lex){
  if( lex->currentUI != UI_CONSOLE) lex->nextUI = UI_CONSOLE;
  lex->_skipToNextOperator();
  return true;
}

#include "operator_CONST.hpp"

#include "operator_DATA.hpp"
#include "operator_DELETE.hpp"
#include "operator_DIRECTORY.hpp"

static bool _operator_EDIT_( Lexer *lex){
  if( lex->currentUI != UI_EDITOR) lex->nextUI = UI_EDITOR;
  lex->_skipToNextOperator();
  return true;
}

static bool _operator_FMAN_( Lexer *lex){
  if( lex->currentUI != UI_FILEMAN) lex->nextUI = UI_FILEMAN;
  lex->_skipToNextOperator();
  return true;
}

#include "operator_LET.hpp"
#include "operator_LIST.hpp"
#include "operator_LOAD.hpp"
#include "operator_MEM.hpp"
#include "operator_MKDIR.hpp"
#include "operator_NEW.hpp"
#include "operator_PUSH.hpp"
#include "operator_REM.hpp"
#include "operator_RESTORE.hpp"

static bool _operator_RPN_( Lexer *lex){
  if( lex->currentUI != UI_RPNCALC) lex->nextUI = UI_RPNCALC;
  lex->_skipToNextOperator();
  return true;
}

#include "operator_RUN.hpp"
#include "operator_SAVE.hpp"
#include "operator_STORE.hpp"
#include "operator_SUM.hpp"
#include "operator_SUMXY.hpp"
#include "operator_TYPE.hpp"

//
// Inits Lexer
//
void Lexer::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _kwds = (Keywords *)components[UI_COMP_Keywords];
  _vars = (Variables *)components[UI_COMP_Variables];
  _code = (ProgramCode *)components[UI_COMP_ProgramCode];
  _funs = (Functions *)components[UI_COMP_Functions];
  _epar = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _sdm = (SDManager *)components[UI_COMP_SDManager];
  _rsb = (RPNStackBox *)components[UI_COMP_RPNBox];
  _mbox = (MessageBox *)components[UI_COMP_MessageBox];
  _clb = (CommandLine *)components[UI_COMP_CommandLine];
  _io_buffer = _iom->getIOBuffer();
  _kwds->getKeywordById( _OPR_AMODE_KW)->operator_ptr = (void *)_operator_AMODE_;
  _kwds->getKeywordById( _OPR_APPEND_KW)->operator_ptr = (void *)_operator_APPEND_; // desc in oprerator_RESTORE.hpp
  _kwds->getKeywordById( _OPR_CLEAR_KW)->operator_ptr = (void *)_operator_CLEAR_;
  _kwds->getKeywordById( _OPR_CLI_KW)->operator_ptr = (void *)_operator_CLI_;
  _kwds->getKeywordById( _OPR_CONST_KW)->operator_ptr = (void *)_operator_CONST_;
  _kwds->getKeywordById( _OPR_DATA_KW)->operator_ptr = (void *)_operator_DATA_;
  _kwds->getKeywordById( _OPR_DELETE_KW)->operator_ptr = (void *)_operator_DELETE_;
  _kwds->getKeywordById( _OPR_DIR_KW)->operator_ptr = (void *)_operator_DIR_;
  _kwds->getKeywordById( _OPR_DIRECTORY_KW)->operator_ptr = (void *)_operator_DIRECTORY_;
  _kwds->getKeywordById( _OPR_EDIT_KW)->operator_ptr = (void *)_operator_EDIT_;
  _kwds->getKeywordById( _OPR_FMAN_KW)->operator_ptr = (void *)_operator_FMAN_;
  _kwds->getKeywordById( _OPR_LET_KW)->operator_ptr = (void *)_operator_LET_;
  _kwds->getKeywordById( _OPR_LIST_KW)->operator_ptr = (void *)_operator_LIST_;
  _kwds->getKeywordById( _OPR_LOAD_KW)->operator_ptr = (void *)_operator_LOAD_;
  _kwds->getKeywordById( _OPR_MEM_KW)->operator_ptr = (void *)_operator_MEM_;
  _kwds->getKeywordById( _OPR_MKDIR_KW)->operator_ptr = (void *)_operator_MKDIR_;
  _kwds->getKeywordById( _OPR_NEW_KW)->operator_ptr = (void *)_operator_NEW_;
  _kwds->getKeywordById( _OPR_REM_KW)->operator_ptr = (void *)_operator_REM_;
  _kwds->getKeywordById( _OPR_REMALT_KW)->operator_ptr = (void *)_operator_REM_;
  _kwds->getKeywordById( _OPR_PUSH_KW)->operator_ptr = (void *)_operator_PUSH_;
  _kwds->getKeywordById( _OPR_RESTORE_KW)->operator_ptr = (void *)_operator_RESTORE_;
  _kwds->getKeywordById( _OPR_RPN_KW)->operator_ptr = (void *)_operator_RPN_;
  _kwds->getKeywordById( _OPR_RUN_KW)->operator_ptr = (void *)_operator_RUN_;
  _kwds->getKeywordById( _OPR_SAVE_KW)->operator_ptr = (void *)_operator_SAVE_;
  _kwds->getKeywordById( _OPR_STORE_KW)->operator_ptr = (void *)_operator_STORE_;
  _kwds->getKeywordById( _OPR_SUM_KW)->operator_ptr = (void *)_operator_SUM_;
  _kwds->getKeywordById( _OPR_SUMXY_KW)->operator_ptr = (void *)_operator_SUMXY_;
  _kwds->getKeywordById( _OPR_TYPE_KW)->operator_ptr = (void *)_operator_TYPE_;
}

//
// Main parsing entry for parsing a line of command or code
// It ignores comments and separates string into operators
//
byte *Lexer::parse(byte *str){
  #ifdef __DEBUG
  Serial.print("Parsing: [");
  Serial.print((char*)str);
  Serial.println("]");
  #endif
  
  result = _RESULT_UNDEFINED_;
  lastKeyword = NULL;
  lastVariable = 0;
  _lexer_position = str;

  // Quirk of standard BASIC: strings like "10 REMComment" are treated like comments
  // TODO: decide, if this is an excessive requirement;
  // e.g. should the user fix the program into "10 REM Comment"?
  if( IsToken( str, "REM", false)) return _skipToEOL( str);

  // if the line is a comment, consider it processed
  _ignore_Blanks();
  if( IsToken( str, "#", false))  return _skipToEOL( str);

  //
  // Separate substrings and process one-by-one;
  // the operators are separated by colons (:)
  // everything after hash (#) is a comment
  //
  while( *_lexer_position && *_lexer_position != '#'){
    _parseOperator();
    if( result == _RESULT_UNDEFINED_) break;
    _check_NextToken( ':');
  }
  return _lexer_position; 
}

//
// Skips blanks to next token, repositions the pointer
//
bool Lexer::_check_NextToken( byte c){
  _ignore_Blanks();
  if( *_lexer_position != c ) return false;
  _lexer_position++;
  _ignore_Blanks();
  return true;
};
bool Lexer::_peek_NextToken( byte c){
  _ignore_Blanks();
  if( *_lexer_position != c ) return false;
  return true;
};


//
// Parses one operator
//
void Lexer::_parseOperator(){
  lastKeyword = NULL;
  lastVariable = 0;
  byte *tmpptr = _lexer_position;
  _lexer_position = _epar->nameParser.parse(_lexer_position); // starts with a name?
  if(_epar->nameParser.result){
    #ifdef __DEBUG
    Serial.print("name found: ");
    Serial.println((char*)_epar->nameParser.Name());
    #endif
    lastKeyword = _kwds->getKeyword(_epar->nameParser.Name());
    if( _processKeyword()) return; // name is a function?
    if( _processVariable()) return; // name is an assignment?
  }

  // direct expression evaluation
  _lexer_position = tmpptr;
  #ifdef __DEBUG
  Serial.print("Direct eval here! |");
  Serial.println((char *)_lexer_position);
  #endif
  _lexer_position = _epar->parse(_lexer_position);
  result = _epar->result;
}
  
//
// Returns true if keyword is recognized and processed
//
bool Lexer::_processKeyword(){
  if( lastKeyword == NULL) return false;
  if( lastKeyword->operator_ptr == NULL) return false;
  #ifdef __DEBUG
  Serial.print("Processing ");
  Serial.print( lastKeyword->name1);
  Serial.print(" ");
  Serial.println(lastKeyword->argumentType);
  #endif
  if(_peek_NextToken( '(')) return false;
  if( lastKeyword->argumentType<0){
    bool (*myOperator)(Lexer *) = (bool (*)(Lexer *))(lastKeyword->operator_ptr);
    return myOperator( this);
  }
  _processRPNKeyword( lastKeyword);
  _skipToNextOperator( _lexer_position);
  return true;
}

bool Lexer::_processRPNKeyword( Keyword *kwd){
  byte ret = _funs->ComputeRPN( kwd);
  #ifdef __DEBUG
  Serial.print("Method returned: ");
  Serial.println( ret);
  #endif
  if( ret & 0x80) _mbox->setRedrawRequired();
  if( ret & 0x40) _rsb->setLabelRedrawAll();
  if( ret & 0x20) _rsb->resetRPNLabels();
  if( ret & 0x10) _rsb->updateIOM();
  switch( ret & 0x0F){
    case 0:
      break;
    case 2:
      _rsb->setStackRedrawRequired( 1);
    case 1:
      _rsb->setStackRedrawRequired( 0);
      break;
    default:
      _rsb->setStackRedrawAll();
      break;  
  }
  return true;
}
bool Lexer::processRPNKeywordByID( int16_t id, bool refresh){
  Keyword *kwd = _kwds->getKeywordById( id);
  if( !kwd) return false; // no such id
  _processRPNKeyword( kwd);
  //TODO: make silent if not in RPN mode (also check _processRPNKeyword)
  //_rsb->updateIOM( refresh);
}

//
// Returns true if variable is recognized and assigned
//
bool Lexer::_processVariable( bool asConstant){
  if( lastKeyword != NULL) return false; 
  byte *ptr = _lexer_position;
  if( !_findAssignment()){ // no assignment
    _lexer_position = ptr;
     return false;
  }
  lastVariable = _vars->getOrCreateNumber( asConstant, _epar->nameParser.Name());
  if( lastVariable == 0){
    _mbox->setLabel(LEX_Error_OutOfMemory);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  if( _vars->isReadOnly( lastVariable)){
    _mbox->setLabel(LEX_Warning_ReadOnly);
    _skipToNextOperator( _lexer_position);
    return true;
  }

  // parse from assignment
  _lexer_position = _epar->parse(_lexer_position);
  switch( _epar->result ){
    case _RESULT_INTEGER_:
      _vars->setValueInteger( lastVariable, _epar->numberParser.integerValue());
      break;
    case _RESULT_REAL_:
      _vars->setValueReal( lastVariable, _epar->numberParser.realValue());
      break;
    default:
      _vars->setValueReal( lastVariable, 0.0);
      break;
  }
  _skipToNextOperator( _lexer_position);
  return true;
}

//
// Locates assignment (=); if not found returns false
// _lexer_position is set to the next symbol after the assignment
//
bool Lexer::_findAssignment(){
  _ignore_Blanks();
  #ifdef __DEBUG
  Serial.print("looking for assignment: ");
  Serial.println((char*)_lexer_position);
  #endif
  if(*_lexer_position == '=' && _lexer_position[1] != '='){
    _lexer_position++;
    return true;
    }
  return false;
}

byte *Lexer::_skipToEOL( byte *str){
  if( str == NULL) str = _lexer_position;
  if( result==_RESULT_UNDEFINED_) result = _RESULT_EXECUTED_;
  _lexer_position = str + strlen(str);
  return _lexer_position;
}

byte *Lexer::_skipToNextOperator( byte *str){
  if( str == NULL) str = _lexer_position;
  if( result==_RESULT_UNDEFINED_) result = _RESULT_EXECUTED_;
  while( *str && *str != _COLON_) str++;
  if( *str == _COLON_) str++;
  _lexer_position = str;
  return _lexer_position;
}

//
// Checks if the value at text position is character c
// Used for finding commas, new lines and such
//
bool Lexer::_validate_NextCharacter( byte c){
  _ignore_Blanks();
  bool tmp = *_lexer_position != c;
  if( tmp) return true;
  _lexer_position++;
  _ignore_Blanks();
  return false;  
}

//
// Parses a list separated by commas
// TODO: this is a kludge; change into the normal list parser
//
byte Lexer::_parseList( byte maxVal){
  for( byte i=0, j=0; i<10; i++){
    _lexer_position = _epar->parse(_lexer_position);
    switch( _epar->result ){
      case _RESULT_INTEGER_:
      case _RESULT_REAL_:
      _listValues[j++] = _epar->numberParser.realValue();
      if(j>= maxVal) return j;
      break;
    default:
      return j;
    }
    if( _validate_NextCharacter(',')) return j;
  }
  return maxVal;
}

// //
// // Returns true if unmatched brackets
// //
// byte *ExpressionParser::_bracket_Check(){
//   byte *ptr = _parser_position;
//   int8_t bracket_count = 0;
//   while( *ptr){
//     if( *ptr == '(') bracket_count++;
//     if( *ptr == ')') bracket_count--;
//     _expression_error = bracket_count<0;
//     if( _expression_error) return ptr;
//     ptr++; 
//   }
//   _expression_error = bracket_count != 0;
//   return ptr;
// }

void Lexer::loadState(){
  size_t loadedSize = _sdm->loadBinary( RPN_SaveVariables, _vars->_buffer, _vars->_var_bottom, _vars->_const_top - 2);
  if( _vars->_var_bottom < loadedSize) _vars->_var_bottom = loadedSize;
  readBASICConstantFile( RPN_SaveConstants); // may use program space; should be called before loadBinbary
  _code->_program_bottom = _sdm->loadBinary( RPN_SaveProgram, _code->_buffer, 5, BASIC_SPACE-4);
  if( _code->_program_bottom <= 0) _loadBASICFile( RPN_AutoexecFile, true);

  byte *buff = (byte *)malloc( INPUT_COLS);
  byte *ptr;
  if( buff == NULL){
    _mbox->setLabel(LEX_Error_OutOfMemory);
    return;
  }
  if( _sdm->openDataFileRead( RPN_SaveStatus)){
    free( buff);
    return;
  }
  while( true){
    if( _sdm->readString( buff, INPUT_COLS) == 0) break;
    #ifdef __DEBUG
    Serial.println ((char *)buff);
    #endif
    if( IsToken( buff, LEX_StorageMarker_Current_UI, false)){
      ptr = buff + strlen( LEX_StorageMarker_Current_UI);
      _epar->numberParser.parse( ptr);
      if( _epar->numberParser.result == _RESULT_INTEGER_ || _epar->numberParser.result == _RESULT_REAL_)
        currentUI = (byte)_epar->numberParser.integerValue();
      if( currentUI < UI_RPNCALC || currentUI > UI_CONSOLE) currentUI = UI_RPNCALC;
      continue;
    }
    if( IsToken( buff, LEX_StorageMarker_Command, false)){
      ptr = buff + strlen( LEX_StorageMarker_Command);
      strncat2( (char *)_clb->_input, (const char *)ptr, INPUT_COLS);
      continue;
    }
    if( IsToken( buff, LEX_StorageMarker_Command_Prev, false)){
      ptr = buff + strlen( LEX_StorageMarker_Command_Prev);
      strncat2( (char *)_clb->_inputPrevious, (const char *)ptr, INPUT_COLS);
      continue;
    }
    if( IsToken( buff, LEX_StorageMarker_Cursor_Column, false)){
      ptr = buff + strlen( LEX_StorageMarker_Cursor_Column);
      _epar->numberParser.parse( ptr);
      if( _epar->numberParser.result == _RESULT_INTEGER_ || _epar->numberParser.result == _RESULT_REAL_)
        _clb->cursor_column = (uint16_t)_epar->numberParser.integerValue();
      continue;
    }
  }
  size_t linp = strlen(_clb->_input);
  if( _clb->cursor_column > linp) _clb->cursor_column = linp;
  free(buff);
  _sdm->closeFile();
}

void Lexer::saveState(){
  if(!_sdm->SDInserted) return;
  if(!_sdm->SDMounted) return;
  _sdm->saveBinary( RPN_SaveVariables, _vars->_buffer, _vars->_var_bottom);
  _sdm->saveBinary( RPN_SaveProgram, _code->_buffer, _code->_program_bottom);
  operator_STORE_Const( RPN_SaveConstants);
  if( _sdm->openDataFileWrite( RPN_SaveStatus)) return;
  if( _sdm->writeSettingNumber( LEX_StorageMarker_Current_UI, (double)currentUI)) return;
  if( _sdm->writeSettingString( LEX_StorageMarker_Command, _clb->_input)) return;
  if( _sdm->writeSettingString( LEX_StorageMarker_Command_Prev, _clb->_inputPrevious)) return;
  if( _sdm->writeSettingNumber( LEX_StorageMarker_Cursor_Column, _clb->cursor_column)) return;
  _sdm->closeFile(); // if file has not been closed due to an error;
}
