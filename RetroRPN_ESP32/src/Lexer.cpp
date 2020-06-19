//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Lexer.hpp"

//#define __DEBUG

const char LEX_Error_OutOfMemory[] PROGMEM = "Err: Out of memory";
const char LEX_Warning_ReadOnly[] PROGMEM = "Warn: Read-only";
const char LEX_Message_VariableList[] PROGMEM = "Variables:";
const char LEX_Message_ConstantsList[] PROGMEM = "Constants:";
const char LEX_Message_ProgMemory[] PROGMEM = "Program Memory:";
const char LEX_Message_VarsMemory[] PROGMEM = "Variable Memory:";
const char LEX_Message_Loaded[] PROGMEM = "Loaded";
const char LEX_Message_Saved[] PROGMEM = "Saved";
const char LEX_Message_SumUpdated[] PROGMEM = "Sum Updated";
const char LEX_Message_N[] PROGMEM = "N:";
const char LEX_Message_stDev[] PROGMEM = "StDev:";
const char LEX_Message_Mean[] PROGMEM = "Mean:";
const char LEX_Message_StdError[] PROGMEM = "Std Error:";
const char LEX_Message_Gain[] PROGMEM = "Gain:";
const char LEX_Message_Offset[] PROGMEM = "Offset:";
const char LEX_Message_NewProgram[] PROGMEM = "New program";

const char LEX_Standard_Variables[] PROGMEM = "/_VARS_.bas";
const char LEX_Standard_Constants[] PROGMEM = "/_CONST_.bas";

// add operator includes here
#include "operator_AMODE.hpp"
#include "operator_CLEAR.hpp"

static bool _operator_CLI_( Lexer *lex){
  if( lex->currentUI != UI_CONSOLE) lex->nextUI = UI_CONSOLE;
  lex->_skipToNextOperator();
  return true;
}

#include "operator_CONST.hpp"

static bool _operator_FMAN_( Lexer *lex){
  if( lex->currentUI != UI_FILEMAN) lex->nextUI = UI_FILEMAN;
  lex->_skipToNextOperator();
  return true;
}

#include "operator_LET.hpp"
#include "operator_LIST.hpp"
#include "operator_LOAD.hpp"
#include "operator_MEM.hpp"
#include "operator_NEW.hpp"
#include "operator_PUSH.hpp"
#include "operator_REM.hpp"

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
  _kwds->getKeywordById( _OPR_CLEAR_KW)->operator_ptr = (void *)_operator_CLEAR_;
  _kwds->getKeywordById( _OPR_CLI_KW)->operator_ptr = (void *)_operator_CLI_;
  _kwds->getKeywordById( _OPR_CONST_KW)->operator_ptr = (void *)_operator_CONST_;
  _kwds->getKeywordById( _OPR_FMAN_KW)->operator_ptr = (void *)_operator_FMAN_;
  _kwds->getKeywordById( _OPR_LET_KW)->operator_ptr = (void *)_operator_LET_;
  _kwds->getKeywordById( _OPR_LIST_KW)->operator_ptr = (void *)_operator_LIST_;
  _kwds->getKeywordById( _OPR_LOAD_KW)->operator_ptr = (void *)_operator_LOAD_;
  _kwds->getKeywordById( _OPR_MEM_KW)->operator_ptr = (void *)_operator_MEM_;
  _kwds->getKeywordById( _OPR_NEW_KW)->operator_ptr = (void *)_operator_NEW_;
  _kwds->getKeywordById( _OPR_REM_KW)->operator_ptr = (void *)_operator_REM_;
  _kwds->getKeywordById( _OPR_REMALT_KW)->operator_ptr = (void *)_operator_REM_;
  _kwds->getKeywordById( _OPR_PUSH_KW)->operator_ptr = (void *)_operator_PUSH_;
  _kwds->getKeywordById( _OPR_RPN_KW)->operator_ptr = (void *)_operator_RPN_;
  _kwds->getKeywordById( _OPR_RUN_KW)->operator_ptr = (void *)_operator_RUN_;
  _kwds->getKeywordById( _OPR_SAVE_KW)->operator_ptr = (void *)_operator_SAVE_;
  _kwds->getKeywordById( _OPR_STORE_KW)->operator_ptr = (void *)_operator_STORE_;
  _kwds->getKeywordById( _OPR_SUM_KW)->operator_ptr = (void *)_operator_SUM_;
  _kwds->getKeywordById( _OPR_SUMXY_KW)->operator_ptr = (void *)_operator_SUMXY_;
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
// // Checks if the value at text position is one of operations
// // Used for finding ** and such
// //
// bool ExpressionParser::_validate_NextOperation( const char *op1){
//   if( _expression_error) return true;
//   if( startsWithC(_parser_position, op1)){
//     _parser_position += strlen(op1);
//     _ignore_Blanks();
//     return false;
//   }
//   return true;
// }
// bool ExpressionParser::_validate_NextOperation( const char *op1, const char *op2){
//   if( _expression_error) return true;
//   if( startsWithC(_parser_position, op1)){
//     _parser_position += strlen(op1);
//     _ignore_Blanks();
//     return false;
//   }
//   if( startsWithC(_parser_position, op2)){
//     _parser_position += strlen(op2);
//     _ignore_Blanks();
//     return false;
//   }
//   return true;
// }

// //
// // Processes a bracket pair (less the opening bracket) or list member 
// //
// bool ExpressionParser::_parse_ListMember( byte terminator){
//   _parse_Expression_Logic();
//   if( numberParser.result == _RESULT_UNDEFINED_) return true;
//   if( _validate_NextCharacter( terminator)) return true;
//   _ignore_Blanks();
//   return false;
// }

// //
// // Processes logic left to right
// //
// byte *ExpressionParser::_parse_Expression_Logic(){
//   double a,b;
//   byte *ptr = _parse_Expression_NOT();
//   if(_expression_error) return ptr;
//   a = numberParser.realValue();
//   while(true){
//     _ignore_Blanks();
//     if( !_validate_NextOperation( "and ", "AND ")){
//       ptr = _parse_Expression_NOT();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a>0.5 && b>0.5)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "or ", "OR ")){
//       ptr = _parse_Expression_NOT();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a>0.5 || b>0.5)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "nor ", "NOR ")){
//       ptr = _parse_Expression_NOT();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a>0.5 || b>0.5)? 0.0: 1.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "xor ", "XOR ")){
//       ptr = _parse_Expression_NOT();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = ((a>0.5 && b>0.5) || (a<0.5 && b<0.5))? 0.0: 1.0;
//       continue;
//     }
//     break;
//   }
//   numberParser.setValue( a);
//   return _parser_position;
// }

// //
// // Processes logical not left to right
// //
// byte *ExpressionParser::_parse_Expression_NOT(){
//   double a;
//   byte *ptr;
//   while(true){
//     _ignore_Blanks();
//     if( !_validate_NextOperation( "not ", "NOT ")){
//       ptr = _parse_Expression_NOT();
//       if(_expression_error) return ptr;
//       a = (numberParser.realValue()>0.5)? 0.0: 1.0;
//       break;
//     }
//     if( !_validate_NextOperation( "and ", "AND ")){
//       _expression_error = true;
//       return _parser_position;
//     }
//     if( !_validate_NextOperation( "or ", "OR ")){
//       _expression_error = true;
//       return _parser_position;
//     }
//     if( !_validate_NextOperation( "nor ", "NOR ")){
//       _expression_error = true;
//       return _parser_position;
//     }
//     if( !_validate_NextOperation( "xor ", "XOR ")){
//       _expression_error = true;
//       return _parser_position;
//     }
//     ptr = _parse_Expression_Comparison();
//     if(_expression_error) return ptr;
//     a = numberParser.realValue();
//     break;
//   }
//   numberParser.setValue( a);
//   return _parser_position;
// }

// //
// // Processes comparisons left to right
// //
// byte *ExpressionParser::_parse_Expression_Comparison(){
//   double a,b;
//   byte *ptr = _parse_Expression_Add_Sub();
//   if(_expression_error) return ptr;
//   a = numberParser.realValue();
//   while(true){
//     _ignore_Blanks();
//     if( !_validate_NextOperation( "!=", "<>")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a!=b)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "<=")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a<=b)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( ">=")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a>=b)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "<")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a<b)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( ">")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a>b)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "==")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a==b)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "is not ", "IS NOT ")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a!=b)? 1.0: 0.0;
//       continue;
//     }
//     if( !_validate_NextOperation( "is ", "IS ")){
//       ptr = _parse_Expression_Add_Sub();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       a = (a==b)? 1.0: 0.0;
//       continue;
//     }
//     break;
//   }
//   numberParser.setValue( a);
//   return _parser_position;
// }

// //
// // Processes addition and subtraction left to right
// //
// byte *ExpressionParser::_parse_Expression_Add_Sub(){
//   double a,b;
//   byte *ptr = _parse_Expression_Mult_Div();
//   if(_expression_error) return ptr;
//   a = numberParser.realValue();
//   while(true){
//     _ignore_Blanks();
//     if(*_parser_position == '+' && _parser_position[1] != '+') {
//       _parser_position++;
//       ptr = _parse_Expression_Mult_Div();
//       if(_expression_error) return ptr;
//       a += numberParser.realValue();
//       continue;
//     }
//     if(*_parser_position == '-' && _parser_position[1] != '-') {
//       _parser_position++;
//       ptr = _parse_Expression_Mult_Div();
//       if(_expression_error) return ptr;
//       a -= numberParser.realValue();
//       continue;
//     }
//     break;
//   }
//   numberParser.setValue( a);
//   return _parser_position;
// }

// //
// // Processes multiplication and division left to right
// //
// byte *ExpressionParser::_parse_Expression_Mult_Div(){
//   double a,b;
//   int64_t c;
//   byte *ptr = _parse_Expression_Power();
//   if(_expression_error) return ptr;
//   a = numberParser.realValue();
//   while(true){
//     _ignore_Blanks();
//     if(*_parser_position == '*' && _parser_position[1] != '*') {
//       _parser_position++;
//       ptr = _parse_Expression_Power();
//       if(_expression_error) return ptr;
//       a *= numberParser.realValue();
//       continue;
//     }
//     if( !_validate_NextOperation( "//")){
//       ptr = _parse_Expression_Power();
//       if(_expression_error) return ptr;
//       c = numberParser.integerValue();
//       _expression_error = (c == 0);
//       if(_expression_error) return ptr;
//       a = (double)((int64_t)a / c);
//       continue;
//     }
//     if(*_parser_position == '/' && _parser_position[1] != '/') {
//       _parser_position++;
//       ptr = _parse_Expression_Power();
//       if(_expression_error) return ptr;
//       b = numberParser.realValue();
//       _expression_error = (b == 0.0);
//       if(_expression_error) return ptr;
//       a /= b;
//       continue;
//     }
//     if( !_validate_NextOperation( "%")){
//       ptr = _parse_Expression_Power();
//       if(_expression_error) return ptr;
//       c = numberParser.integerValue();
//       _expression_error = (a > _HUGE_POS_INTEGER_) || (a < _HUGE_NEG_INTEGER_) || (c == 0);
//       if(_expression_error) return ptr;
//       a = (double)((int64_t)a % c);
//       continue;
//     }
//     break;
//   }
//   numberParser.setValue( a);
//   return _parser_position;
// }

// //
// // Processes power left to right
// //
// byte *ExpressionParser::_parse_Expression_Power(){
//   double a,b;
//   byte *ptr = _parse_Expression_Value();
//   if(_expression_error) return ptr;
//   a = numberParser.realValue();
//   while(true){
//     _ignore_Blanks();
//     if(_validate_NextOperation( "^", "**")){
//       numberParser.setValue( a); 
//       break;
//     }
//     ptr = _parse_Expression_Value();
//     if(_expression_error) return ptr;
//     b = numberParser.realValue();
//     a = pow(a, b);
//   }
//   return _parser_position;
// }


// bool ExpressionParser::_parse_FunctionArguments(Function *mf, double *_args){
//   _ignore_Blanks();
//   if(mf->nArgs == 0) return false;
//   if(!_check_NextToken( '(')) return true;
//   for( byte i=0; i<mf->nArgs-1; i++){
//     if( _parse_ListMember( ',')) return true;
//     _args[i] = numberParser.realValue();
//     #ifdef __DEBUG
//     Serial.print("Argument [");
//     Serial.print( i);
//     Serial.print("] parsed: ");
//     Serial.println(_args[0]);
//     #endif
//   }
//   if( _parse_ListMember( ')')) return true;
//   _args[mf->nArgs-1] = numberParser.realValue();
//   #ifdef __DEBUG
//   Serial.print("Argument [");
//   Serial.print( mf->nArgs-1);
//   Serial.print("] parsed: ");
//   Serial.println(_args[mf->nArgs-1]);
//   #endif
//   return false;
// }

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
