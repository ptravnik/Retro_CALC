//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Lexer.hpp"

#define __DEBUG

//
// Inits Lexer
//
void Lexer::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _kwds = (Keywords *)components[UI_COMP_Keywords];
  _vars = (Variables *)components[UI_COMP_Variables];
  _funs = (Functions *)components[UI_COMP_Functions];
  _epar = (ExpressionParser *)components[UI_COMP_ExpressionParser];
 
  //_lcd = (LCDManager *)components[UI_COMP_LCDManager];
  //_sdm = (SDManager *)components[UI_COMP_SDManager];
  //_rsb = (RPNStackBox *)components[UI_COMP_RPNBox];
  //_mbox = (MessageBox *)components[UI_COMP_MessageBox];
  //_clb = (CommandLine *)components[UI_COMP_CommandLine];
  _io_buffer = _iom->getIOBuffer();
}

// //
// // Checks if the value at text position is character c
// // Used for finding commas, new lines and such
// //
// bool ExpressionParser::_validate_NextCharacter( byte c){
//   _expression_error = _expression_error || (*_parser_position != c);
//   if( _expression_error) return true;
//   _parser_position++;
//   _ignore_Blanks();
//   return false;  
// }

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
//   if( numberParser.result == _NOT_A_NUMBER_) return true;
//   if( _validate_NextCharacter( terminator)) return true;
//   _ignore_Blanks();
//   return false;
// }

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

  //  result = _NOT_A_NUMBER_;
  //  _expression_error = false;
  //  _parser_position = str;
  //  _ignore_Blanks();
  //  byte *ptr = _bracket_Check();
  //  if( _expression_error) return ptr;

  //  // this is a kludge to test RPN screen TODO: unkludge!
  //  if( *_parser_position == '#'){
  //    nameParser._reset_name();
  //    result = _STRING_;
  //    return _parser_position; 
  //  }

  //  _parse_Expression_Logic();
//   //_parse_Expression_NOT();
//   //_parse_Expression_Comparison();
//   //_parse_Expression_Add_Sub();
//   //_parse_Expression_Mult_Div();
//   //_parse_Expression_Power();
//   if( _expression_error) result = _NOT_A_NUMBER_;
//   return _parser_position;
}

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

// //
// // Processes expression for value
// //
// byte *ExpressionParser::_parse_Expression_Value(){
//   // names evaluation
//   _ignore_Blanks();
//   if( IsNameStarter( *_parser_position) ){
//     _parser_position = nameParser.parse(_parser_position);
//     if(!nameParser.result) return _parser_position;
//     #ifdef __DEBUG
//     Serial.print("Found keyword: ");
//     Serial.println((char *)nameParser.Name());
//     #endif
//     lastMathFunction = mathFunctions->getFunction(nameParser.Name());
//     if( lastMathFunction == NULL){
//       #ifdef __DEBUG
//       Serial.println("This name is indefined!");
//       #endif
//       result = _STRING_;
//       return _parser_position;
//     }
//     #ifdef __DEBUG
//     Serial.print("Located function: ");
//     Serial.println(lastMathFunction->name0);
//     #endif
//     Function *mfptr = lastMathFunction; // could be a recursive call!
//     double _args[3]; // kept on system stack
//     if(_parse_FunctionArguments(lastMathFunction, _args)){
//       result = _STRING_;
//       return _parser_position;
//     }
//     #ifdef __DEBUG
//     Serial.print("Function: ");
//     Serial.print(mfptr->name1);
//     for( byte i=0; i<3; i++){
//       Serial.print(" ");
//       Serial.print(_args[i]);
//     }
//     #endif    
//     double *tmp = mathFunctions->Compute( mfptr, _args);
//     #ifdef __DEBUG
//     Serial.print("Computation returned: ");
//     Serial.println(*tmp);
//     #endif
//     numberParser.setValue(*tmp);
//     result = numberParser.result;
//     return _parser_position;
//   }
  
//   // Unary + and unary - by recursive calls such as 5 * -2; not sure is this should be allowed
//   if( _check_NextToken( '+')){
//     //Serial.println("Arrived into unary plus:");
//     //Serial.println((const char *)_parser_position);    
//     _parse_Expression_Logic();
//     if(_expression_error) return _parser_position;
//     result = numberParser.result;
//     return _parser_position;
//   }
//   if( _check_NextToken( '-')){
//     //Serial.println("Arrived into negation:");
//     //Serial.println((const char *)_parser_position);    
//     _parse_Expression_Logic();
//     if(_expression_error) return _parser_position;
//     numberParser.negate();
//     result = numberParser.result;
//     return _parser_position;
//   }

//   // opening bracket - evaluate inside, get a pair braket
//   if( _check_NextToken( '(')){
//     //Serial.println("Arrived into brackets:");
//     //Serial.println((const char *)_parser_position);    
//     if( _parse_ListMember( ')')) return _parser_position;
//     result = numberParser.result;
//     return _parser_position;
//   }
  
//   // not a bracket - leftovers
//   _parser_position = numberParser.parse(_parser_position);
//   result = numberParser.result;
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
