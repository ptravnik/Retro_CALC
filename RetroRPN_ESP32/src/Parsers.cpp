//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Parsers.hpp"

//#define __DEBUG

const byte _hexadigits[] PROGMEM = 
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'x'};
const byte _multiplier_Prefixes[] PROGMEM = 
  { 'f', 'p', 'n', 'u', 'm', 'c', '%', 'd', 'k', 'M', 'G', 'T', 'P'};
const int8_t _multiplier_Values[] PROGMEM = 
  { -15, -12,  -9,  -6,  -3,  -2,  -2,  -1,   3,   6,   9,  12,  15};
const char _not_a_Number[] PROGMEM = "Not a number";

//
// Number parser components
//
double NumberParser::realValue(){
  switch(result){
    case _RESULT_REAL_:
      return _dValue;
    case _RESULT_INTEGER_:
      return (double)_iValue;
    default:
      break;
  }
  return 0.0;
}
int64_t NumberParser::integerValue(){
  switch(result){
    case _RESULT_REAL_:
      if( _dValue >= 8e18) return _HUGE_POS_INTEGER_;
      if( _dValue <= -8e18) return _HUGE_NEG_INTEGER_;
      return (int64_t)round( _dValue);
    case _RESULT_INTEGER_:
      return _iValue;
    default:
      break;
  }
  return 0;
}

void NumberParser::negate(){
  switch(result){
    case _RESULT_REAL_:
      _dValue = -_dValue;
      return;
    case _RESULT_INTEGER_:
      _iValue = -_iValue;
      return;
    default:
      break;
  }
}

//
// Converts double or int properly
// Note that the buffer length must be sufficient to accommodate at least 
// the sign, 9 characters of the number, 6 characters of the exponent and 
// the final zero. That is 17 characters.
// For the long int, it can be up to 20 decimals, sign and _NUL_
// Returns a pinter to the end of converted string.
//
byte *NumberParser::stringValue( byte *ptr, byte max_len){
  // use internal
  if(ptr == NULL){
    ptr = _numString;
    max_len = _NUMBER_LENGTH_;
  }
  switch(result){
    case _RESULT_INTEGER_ :
      return _convertInt(ptr, max_len);
    case _RESULT_REAL_    :
      return _convertDouble(_dValue, ptr, max_len);
    default:
      strncat2( (char *)ptr, _not_a_Number, max_len);
      break;
  }
  return ptr;
}
byte *NumberParser::stringValue( double n, byte *ptr, byte max_len){
  // use internal
  if(ptr == NULL){
    ptr = _numString;
    max_len = _NUMBER_LENGTH_;
    _convertDouble(n, ptr, max_len);
    return _numString;
  }
  return _convertDouble(n, ptr, max_len);
}

byte *NumberParser::_convertInt( byte *ptr, byte max_len){
  if( _iValue == 0){
    *ptr = _ZERO_;
    ptr[1] = _NUL_;
    return ptr;
  }
  bool sign_negative = _iValue < 0;
  int64_t tmp = _iValue;
  if( sign_negative) tmp = -tmp;
  ptr[max_len-1] = _NUL_;
  byte i = max_len;
  while(tmp){
    ptr[--i] = tmp % 10;
    tmp /= 10;    
  }
  if(sign_negative) ptr[--i] = '-';
  for(byte j=0; j<max_len; j++){
    ptr[j] = ptr[i];
    if( !ptr[i++]) break;  
  }
  return ptr;
}

byte *NumberParser::_convertDouble( double n, byte *ptr, byte max_len){
  size_t i;
  if( n<0.0){
    *ptr++ = '-';
    n=-n;
  }
  if( n < 1e-300){
    *ptr++ = '0';
    *ptr = _NUL_;
    return ptr;
  }
  *ptr = _NUL_;

  // scientific format
  if( force_sci || n>=1e7 || n<0.1){
    int exponent = 0;
    while( n>=10.0){
      n *= 0.1;
      exponent++;
    }
    while( n<1.0){
      n *= 10;
      exponent--;
    }
    dtostrf( n, precision+2, precision, (char *)ptr);
    i = strlen( ptr);
    snprintf( (char *)(ptr+i), MAXIMUM_NUMBER_LENGTH-i, "e%+04d", exponent);
    return ptr + strlen( ptr);
  }

  // normal decimal format
  dtostrf( n, precision+2, precision, (char *)ptr);

  // check if the trailing zeros can be discarded
  i = strlen(ptr)-1;
  while( i>0 && ptr[i] == _ZERO_) i--;
  if( ptr[i] == _DECIMAL_) i--;
  ptr += i+1;
  *ptr = _NUL_;
  return ptr;
}

byte *NumberParser::stringHex( double n, byte *ptr, byte max_len){
  _dValue = n;
  result = _RESULT_REAL_;
  return stringHex( ptr, max_len);
}

byte *NumberParser::stringHex( byte *ptr, byte max_len){
  // use internal
  if(ptr == NULL){
    ptr = _numString;
    max_len = _NUMBER_LENGTH_;
  }
  *ptr++ = _ZERO_;
  *ptr++ = _hexadigits[16];
  int64_t n = integerValue();
  if( n == 0){ // show at least a zero!
    *ptr++ = _hexadigits[0];
    *ptr++ = _hexadigits[0];
    *ptr = _NUL_;
    return ptr;      
  }  
  uint64_t tmp = (n<0)? 0xFFFFFFFFFFFFFFFE - (uint64_t)n: n;
  if( tmp == 0){ // show at least a zero!
    *ptr++ = _hexadigits[0];
    *ptr++ = _hexadigits[0];
    *ptr = _NUL_;
    return ptr;      
  }
  // a 64-bit integer may contain 16 hex numbers plus the trailing zero
  int8_t i = 16;
  ptr[i--] = _NUL_;
  while(tmp > 0 && i>=0){
    ptr[i--] = _hexadigits[ tmp & 0x0f];
    tmp >>= 4; 
  }
  i++;
  while(true){ // shift to shorten the number
    *ptr = ptr[i];
    if(*ptr == _NUL_) break;  
    ptr++;
  }
  return ptr;
}

//
// Attempts to parse a number
// The pointer returned is past the number if valid
// or at the origin if not parsed correctly
//
byte *NumberParser::parse( byte *str){
  #ifdef __DEBUG
  Serial.print("Number parsing: [");
  Serial.print((char*)str);
  Serial.println("]");
  #endif  
  byte logical_position = 0; // 0-whole, 1-decmal, 2-exponent, 3-exp number
  result = _RESULT_UNDEFINED_;
  byte *ptr = ignore_Blanks( str);
  if( !IsDigitOrDecimal(*ptr)) return ptr;
  str = ptr;
  _dValue = 0.0;           
  _iValue = 0L;
  int8_t nExp = 0;
  
  // check if hexadecimal
  if( *str == _ZERO_ && str[1] == 'x' && convertHex(str[2])<16)
    return _parseHex( str);

  // parse the integer part  
  while( isDigit(*ptr)){
    _iValue *= 10;
    _iValue += (*ptr++)-_ZERO_;
  }

  // a number may end on a decimal point, E,
  // an engineering multiplier, or number end
  switch(*ptr){
    case _NUL_:
      result = _RESULT_INTEGER_;
      return ptr;
    case '.':
      _processMultiplier( ++ptr, _iValue, 0);
      logical_position = 1;
      break;
    case 'e':
    case 'E':
      _processMultiplier( ++ptr, _iValue, 0);
      logical_position = 2;
      break;
    default:
      nExp = _locateMultiplier(*ptr);
      if( nExp != 0)
        return _processMultiplier( ++ptr, _iValue, nExp);
      if( !IsNumberTerm(*ptr)) return str; // parsing failed!
      result = _RESULT_INTEGER_;
      return ptr;   
  }

  // process decimals;
  // the allowed are digits, E,
  // an engineering multiplier, or number end
  // the second decimal point causes an error
  double mult = 0.1;
  while( logical_position == 1){
    switch(*ptr){
      case _NUL_:
        result = _RESULT_REAL_;
        return ptr;
      case '.': // second decimal point is not allowed
        result = _RESULT_UNDEFINED_;
        _dValue = 0.0;           
        return str;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        _dValue += mult * ((*ptr++)-_ZERO_);           
        mult /= 10.0;
        break;
      case 'e':
      case 'E':
        logical_position = 2;
        ptr++;
        break;
      default:
        nExp = _locateMultiplier(*ptr);
        if( nExp != 0)
          return _processMultiplier( ++ptr, _dValue, nExp);
        if( !IsNumberTerm(*ptr)){
          result = _RESULT_UNDEFINED_;
          _dValue = 0.0;
          return str; // parsing failed!
        }           
        return ptr;   
    }      
  }
  
  // after E, must be only +, -, or digit
  bool exponent_negative = false;
  switch(*ptr){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;    
    case '-':
      exponent_negative = true;
    case '+':
      ptr++;
      if( !IsDigit(*ptr)){
        result = _RESULT_UNDEFINED_;
        _dValue = 0.0;
        return str; // parsing failed!      
      }
      break;
    default:
      result = _RESULT_UNDEFINED_;
      _dValue = 0.0;
      return str; // parsing failed!
  }

  // finally, the exponent digits
  #ifdef __DEBUG
  Serial.println("Exponents:");
  #endif
  uint16_t exponent_value = 0;
  while( isDigit(*ptr)){
    if( exponent_value <= 300){
      exponent_value *= 10;
      exponent_value += (*ptr)-_ZERO_;
      #ifdef __DEBUG
      Serial.println(exponent_value);
      #endif
    }
    ptr++;
  }
  if( exponent_value > 300)
    exponent_value = 300;
  if( exponent_negative)
    _dValue /= pow( 10.0, exponent_value);
  else
    _dValue *= pow( 10.0, exponent_value);
  return ptr; // parsing success
}

byte *NumberParser::_processMultiplier(byte *ptr, double v, int8_t mult){
  result = _RESULT_REAL_;
  _dValue = pow( 10, mult) * v;
  return ptr;  
}
byte *NumberParser::_processMultiplier(byte *ptr, int64_t v, int8_t mult){
  result = _RESULT_REAL_;
  _dValue = pow( 10, mult) * v;
  _iValue = 0;
  return ptr;  
}
int8_t NumberParser::_locateMultiplier(byte b){
  for( byte i=0; i<13; i++){
    if( _multiplier_Prefixes[i] == b)
        return _multiplier_Values[i];
  }
  return 0; 
}
byte *NumberParser::_parseHex( byte *str){
  // skip first two digits
  byte *ptr = str+2;
  byte tmp = convertHex(*ptr); 
  while( tmp < 16){
    _iValue <<= 4;
    _iValue += tmp;
    ptr++;
    tmp = convertHex(*ptr);
  }
  if( !IsNumberTerm(*ptr)){
    _iValue = 0;
    return str;
  }
  result = _RESULT_INTEGER_;
  return ptr;
}

//
// Attempts to parse a name, such as an identifier
// The pointer returned is past the name if valid
// or at the origin if not parsed correctly
//
byte *NameParser::parse( byte *str){
  #ifdef __DEBUG
  Serial.print("Name parsing: [");
  Serial.print((char*)str);
  Serial.println("]");
  #endif
  _reset_name();
  byte *ptr = str;
  
  // skip empty space
  while( IsSpacer(*ptr)) ptr++;
  if( !IsNameStarter(*ptr))
    return ptr;
  str = ptr;

  while( IsNameCharacter(*ptr)){
    // dollar or percent must be the last character
    if( *ptr == _DOLLAR_ || *ptr == _PERCENT_){
      _add_char_to_name( ptr++);
      break;
    }
    if(_name_position < _MAX_IDENTIFIER_) 
      _add_char_to_name( ptr++);
  }
  result = true;
  if( *ptr == _NUL_ || IsNameTerm( *ptr))
    return ptr;
  _reset_name();
  return str;
}
inline byte NameParser::VarType(){
  if( _name_position==0) return VARTYPE_NONE;
  byte lc = _name[ _name_position-1];
  if( lc == _DOLLAR_) return VARTYPE_STRING;
  return VARTYPE_NUMBER;
}

//
// Gets the filename string into list memory
//
byte *FilenameParser::parse( byte *str){
  _reset_name();
  _locateTerminator( str);
  if( !IsOperatorTerm( *_parser_end)) return _parser_start;
  size_t len = _parser_end - _parser_start;
  if( len == 0 ) return _parser_start;

  Serial.print("Given: ");
  Serial.println( (char*)str);  

  // absolute directory
  if( *_parser_start == '/'){
    strncat2( (char*)_name, (const char *)_parser_start, INPUT_COLS+1);
    Serial.print("Absolute name: ");
    Serial.println( (char*)_name);  
    result = true;
    return _parser_end;
  }

  // Not absolute - start from existing
  strncat2( (char*)_name, (const char *)_vars->currentDir, INPUT_COLS-1);

  // up one directory
  if( IsToken( _parser_start, "../", false)){
    while( IsToken( _parser_start, "../", false)){
      _backpedalDirectory();
      _parser_start += 3;
    }
  }

  // up one directory
  if( IsToken( _parser_start, "..", true)){
    _backpedalDirectory();
    _parser_start = _parser_end;
  }

  // use the same directory
  if( IsToken( _parser_start, "./", false)){
    _parser_start += 2;
  }

  len = _parser_end - _parser_start;
  if( len == 0 ){
    result = true;
    return _parser_start;
  }
  int l2 = strlen(_name);
  if( l2<=0 || _name[l2-1] != '/') strcat( (char*)_name, "/");
  len += l2;
  strncat( (char*)_name, (const char *)_parser_start, INPUT_COLS - len);
  #ifdef __DEBUG
  Serial.print("Name set to: ");
  Serial.println( (char*)_name);
  #endif
  result = true;
  return _parser_end;
}

//
// Locates a valid terminator for filename
//
void FilenameParser::_locateTerminator( byte *start){
  _parser_start = start;
  _parser_end = _parser_start;
  if( !IsFilenameStarter( *_parser_end)) return;
  while( IsFilenameCharacter( *_parser_end )) _parser_end++;
}

//
// Locates a previous directory marker and chops the string at it
//
void FilenameParser::_backpedalDirectory(){
  Serial.println( "Directory before: ");
  Serial.println( (char *)_name);
  int16_t end = strlen( _name)-1;
  if( end>0 && _name[end] == '/') end--;
  while( end>0 && _name[end] != '/') end--;
  if( end<=0) return;
  _name[end] = _NUL_;
  Serial.println( "Directory after backpedal:");
  Serial.println( (char *)_name);
}

//
// Inits ExpressionParser
//
void ExpressionParser::init(void *components[]){
  _kwds = (Keywords *)components[UI_COMP_Keywords];
  _vars = (Variables *)components[UI_COMP_Variables];
  _funs = (Functions *)components[UI_COMP_Functions];
  filenameParser.init( _vars);
}

//
// Checks if the value at text position is character c
// Used for finding commas, new lines and such
//
bool ExpressionParser::_validate_NextCharacter( byte c){
  _expression_error = _expression_error || (*_parser_position != c);
  if( _expression_error) return true;
  _parser_position++;
  _ignore_Blanks();
  return false;  
}

//
// Checks if the value at text position is one of operations
// Used for finding ** and such
//
bool ExpressionParser::_validate_NextOperation( const char *op1){
  if( _expression_error) return true;
  if( startsWithC(_parser_position, op1)){
    _parser_position += strlen(op1);
    _ignore_Blanks();
    return false;
  }
  return true;
}
bool ExpressionParser::_validate_NextOperation( const char *op1, const char *op2){
  if( _expression_error) return true;
  if( startsWithC(_parser_position, op1)){
    _parser_position += strlen(op1);
    _ignore_Blanks();
    return false;
  }
  if( startsWithC(_parser_position, op2)){
    _parser_position += strlen(op2);
    _ignore_Blanks();
    return false;
  }
  return true;
}

//
// Processes a bracket pair (less the opening bracket) or list member 
//
bool ExpressionParser::_parse_ListMember( byte terminator){
  _parse_Expression_Logic();
  if( numberParser.result == _RESULT_UNDEFINED_) return true;
  if( _validate_NextCharacter( terminator)) return true;
  _ignore_Blanks();
  return false;
}

//
// Main parsing entry
//
byte *ExpressionParser::parse(byte *str){
  #ifdef __DEBUG
  Serial.print("Parsing: [");
  Serial.print((char*)str);
  Serial.println("]");
  #endif

  result = _RESULT_UNDEFINED_;
  _expression_error = false;
  _parser_position = str;
  _ignore_Blanks();
  byte *ptr = _bracket_Check();
  if( _expression_error) return ptr;

  // this is a kludge to test RPN screen TODO: unkludge!
  if( *_parser_position == '#'){
    nameParser._reset_name();
    result = _RESULT_STRING_;
    return _parser_position; 
  }

  _parse_Expression_Logic();
  // Left here for debugging; each level represents priority
  //_parse_Expression_NOT();
  //_parse_Expression_Comparison();
  //_parse_Expression_Add_Sub();
  //_parse_Expression_Mult_Div();
  //_parse_Expression_Power();
  if( _expression_error) result = _RESULT_UNDEFINED_;
  return _parser_position;
}

//
// Processes logic left to right
//
byte *ExpressionParser::_parse_Expression_Logic(){
  double a,b;
  byte *ptr = _parse_Expression_NOT();
  if(_expression_error) return ptr;
  a = numberParser.realValue();
  while(true){
    _ignore_Blanks();
    if( !_validate_NextOperation( "and ", "AND ")){
      ptr = _parse_Expression_NOT();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a>0.5 && b>0.5)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( "or ", "OR ")){
      ptr = _parse_Expression_NOT();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a>0.5 || b>0.5)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( "nor ", "NOR ")){
      ptr = _parse_Expression_NOT();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a>0.5 || b>0.5)? 0.0: 1.0;
      continue;
    }
    if( !_validate_NextOperation( "xor ", "XOR ")){
      ptr = _parse_Expression_NOT();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = ((a>0.5 && b>0.5) || (a<0.5 && b<0.5))? 0.0: 1.0;
      continue;
    }
    break;
  }
  numberParser.setValue( a);
  return _parser_position;
}

//
// Processes logical not left to right
//
byte *ExpressionParser::_parse_Expression_NOT(){
  double a;
  byte *ptr;
  while(true){
    _ignore_Blanks();
    if( !_validate_NextOperation( "not ", "NOT ")){
      ptr = _parse_Expression_NOT();
      if(_expression_error) return ptr;
      a = (numberParser.realValue()>0.5)? 0.0: 1.0;
      break;
    }
    if( !_validate_NextOperation( "and ", "AND ")){
      _expression_error = true;
      return _parser_position;
    }
    if( !_validate_NextOperation( "or ", "OR ")){
      _expression_error = true;
      return _parser_position;
    }
    if( !_validate_NextOperation( "nor ", "NOR ")){
      _expression_error = true;
      return _parser_position;
    }
    if( !_validate_NextOperation( "xor ", "XOR ")){
      _expression_error = true;
      return _parser_position;
    }
    ptr = _parse_Expression_Comparison();
    if(_expression_error) return ptr;
    a = numberParser.realValue();
    break;
  }
  numberParser.setValue( a);
  return _parser_position;
}

//
// Processes comparisons left to right
//
byte *ExpressionParser::_parse_Expression_Comparison(){
  double a,b;
  byte *ptr = _parse_Expression_Add_Sub();
  if(_expression_error) return ptr;
  a = numberParser.realValue();
  while(true){
    _ignore_Blanks();
    if( !_validate_NextOperation( "!=", "<>")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a!=b)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( "<=")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a<=b)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( ">=")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a>=b)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( "<")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a<b)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( ">")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a>b)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( "==")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a==b)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( "is not ", "IS NOT ")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a!=b)? 1.0: 0.0;
      continue;
    }
    if( !_validate_NextOperation( "is ", "IS ")){
      ptr = _parse_Expression_Add_Sub();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      a = (a==b)? 1.0: 0.0;
      continue;
    }
    break;
  }
  numberParser.setValue( a);
  return _parser_position;
}

//
// Processes addition and subtraction left to right
//
byte *ExpressionParser::_parse_Expression_Add_Sub(){
  double a,b;
  byte *ptr = _parse_Expression_Mult_Div();
  if(_expression_error) return ptr;
  a = numberParser.realValue();
  while(true){
    _ignore_Blanks();
    if(*_parser_position == '+' && _parser_position[1] != '+') {
      _parser_position++;
      ptr = _parse_Expression_Mult_Div();
      if(_expression_error) return ptr;
      a += numberParser.realValue();
      continue;
    }
    if(*_parser_position == '-' && _parser_position[1] != '-') {
      _parser_position++;
      ptr = _parse_Expression_Mult_Div();
      if(_expression_error) return ptr;
      a -= numberParser.realValue();
      continue;
    }
    break;
  }
  numberParser.setValue( a);
  return _parser_position;
}

//
// Processes multiplication and division left to right
//
byte *ExpressionParser::_parse_Expression_Mult_Div(){
  double a,b;
  int64_t c;
  byte *ptr = _parse_Expression_Power();
  if(_expression_error) return ptr;
  a = numberParser.realValue();
  while(true){
    _ignore_Blanks();
    if(*_parser_position == '*' && _parser_position[1] != '*') {
      _parser_position++;
      ptr = _parse_Expression_Power();
      if(_expression_error) return ptr;
      a *= numberParser.realValue();
      continue;
    }
    if( !_validate_NextOperation( "//")){
      ptr = _parse_Expression_Power();
      if(_expression_error) return ptr;
      c = numberParser.integerValue();
      _expression_error = (c == 0);
      if(_expression_error) return ptr;
      a = (double)((int64_t)a / c);
      continue;
    }
    if( !_validate_NextOperation( "/%")){
      ptr = _parse_Expression_Power();
      if(_expression_error) return ptr;
      c = numberParser.integerValue();
      _expression_error = (a > _HUGE_POS_INTEGER_) || (a < _HUGE_NEG_INTEGER_) || (c == 0);
      if(_expression_error) return ptr;
      a = (double)((int64_t)a % c);
      continue;
    }
    if(*_parser_position == '/' && _parser_position[1] != '/') {
      _parser_position++;
      ptr = _parse_Expression_Power();
      if(_expression_error) return ptr;
      b = numberParser.realValue();
      _expression_error = (b == 0.0);
      if(_expression_error) return ptr;
      a /= b;
      continue;
    }
    break;
  }
  numberParser.setValue( a);
  return _parser_position;
}

//
// Processes power left to right
//
byte *ExpressionParser::_parse_Expression_Power(){
  double a,b;
  byte *ptr = _parse_Expression_Value();
  if(_expression_error) return ptr;
  a = numberParser.realValue();
  while(true){
    _ignore_Blanks();
    if(_validate_NextOperation( "^", "**")){
      numberParser.setValue( a); 
      break;
    }
    ptr = _parse_Expression_Value();
    if(_expression_error) return ptr;
    b = numberParser.realValue();
    a = pow(a, b);
    _expression_error = isnan( a);
    if( _expression_error) break;
  }
  return _parser_position;
}

//
// Processes expression for value
//
byte *ExpressionParser::_parse_Expression_Value(){
  // names evaluation
  _ignore_Blanks();
  if( IsNameStarter( *_parser_position) ){
    _parser_position = nameParser.parse(_parser_position);
    if(!nameParser.result) return _parser_position;
    #ifdef __DEBUG
    Serial.print("Keyword found (0): ");
    Serial.println((char *)nameParser.Name());
    #endif
    lastMathFunction = _funs->getFunction(nameParser.Name());
    if( lastMathFunction == NULL){
      #ifdef __DEBUG
      Serial.println("This name is indefined!");
      #endif
      result = _RESULT_STRING_;
      return _parser_position;
    }
    #ifdef __DEBUG
    Serial.print("Function keyword found: ");
    Serial.println( (char *)_kwds->getKeywordById( lastMathFunction->kwid)->name0);
    #endif
    Function *mfptr = lastMathFunction; // could be a recursive call!
    double _args[3]; // kept on system stack
    if(_parse_FunctionArguments(lastMathFunction, _args)){
      result = _RESULT_STRING_;
      return _parser_position;
    }
    #ifdef __DEBUG
    Serial.print("Argument(s):");
    for( byte i=0; i<lastMathFunction->nArgs; i++){
      Serial.print(" ");
      Serial.print(_args[i]);
    }
    Serial.println();
    #endif    
    if( _funs->Compute( mfptr, _args)){
      result = _RESULT_STRING_;
      return _parser_position;  
    }
    #ifdef __DEBUG
    Serial.print("Computation returned: ");
    Serial.println(_funs->_rets[0]);
    #endif
    numberParser.setValue(_funs->_rets[0]);
    result = numberParser.result;
    return _parser_position;
  }
  
  // Unary + and unary - by recursive calls such as 5 * -2; not sure is this should be allowed
  if( _check_NextToken( '+')){
    _parse_Expression_Logic();
    if(_expression_error) return _parser_position;
    result = numberParser.result;
    return _parser_position;
  }
  if( _check_NextToken( '-')){
    _parse_Expression_Logic();
    if(_expression_error) return _parser_position;
    numberParser.negate();
    result = numberParser.result;
    return _parser_position;
  }

  // opening bracket - evaluate inside, get a pair braket
  if( _check_NextToken( '(')){
    if( _parse_ListMember( ')')) return _parser_position;
    result = numberParser.result;
    return _parser_position;
  }
  
  // not a bracket - leftovers
  _parser_position = numberParser.parse(_parser_position);
  result = numberParser.result;
  return _parser_position;
}

bool ExpressionParser::_parse_FunctionArguments(Function *f, double *_args){
  _ignore_Blanks();
  if(f->nArgs == 0) return false;
  if(!_check_NextToken( '(')) return true;
  for( byte i=0; i<f->nArgs-1; i++){
    if( _parse_ListMember( ',')) return true;
    _args[i] = numberParser.realValue();
    #ifdef __DEBUG
    Serial.print("Argument [");
    Serial.print( i);
    Serial.print("] parsed: ");
    Serial.println(_args[0]);
    #endif
  }
  if( _parse_ListMember( ')')) return true;
  _args[f->nArgs-1] = numberParser.realValue();
  #ifdef __DEBUG
  Serial.print("Argument [");
  Serial.print( f->nArgs-1);
  Serial.print("] parsed: ");
  Serial.println(_args[f->nArgs-1]);
  #endif
  return false;
}

bool ExpressionParser::_parse_VariableArguments(VariableToken vt){
  _ignore_Blanks();
  _martixRow = 0;
  _martixColumn = 0;
  switch( _vars->getVarType(vt)){
    case VARTYPE_NUMBER:
      _expression_error = *_parser_position == '[' || *_parser_position == '('; 
      return _expression_error;
    case VARTYPE_VECTOR:
    case VARTYPE_STRING:
      if( *_parser_position != '[' && *_parser_position != '(') return false;
      if( *_parser_position == '['){
        _parser_position++;
        if( _parse_ListMember( ']')) return true;
      }
      if( *_parser_position == '('){
        _parser_position++;
        if( _parse_ListMember( ')')) return true;
      }
      if( numberParser.result != _RESULT_INTEGER_) return true;
      _martixColumn = (int)numberParser.integerValue();
      return false;
    default:
      if( *_parser_position != '[' && *_parser_position != '(') return false;
      if( *_parser_position == '['){
        _parser_position++;
        if( _parse_ListMember( ',')) return true;
        if( numberParser.result != _RESULT_INTEGER_) return true;
        _martixRow = (int)numberParser.integerValue();
        if( _parse_ListMember( ']')) return true;
      }
      if( *_parser_position == '('){
        _parser_position++;
        if( _parse_ListMember( ',')) return true;
        if( numberParser.result != _RESULT_INTEGER_) return true;
        _martixRow = (int)numberParser.integerValue();
        if( _parse_ListMember( ')')) return true;
      }
      if( numberParser.result != _RESULT_INTEGER_) return true;
      _martixColumn = (int)numberParser.integerValue();
      return false;
  }
}

//
// Returns true if unmatched brackets
//
byte *ExpressionParser::_bracket_Check(){
  byte *ptr = _parser_position;
  int8_t cRound = 0;
  int8_t cSquare = 0;
  int8_t cCurled = 0;
  while( *ptr){
    if( *ptr == '(') cRound++;
    if( *ptr == ')') cRound--;
    if( *ptr == '[') cSquare++;
    if( *ptr == ']') cSquare--;
    if( *ptr == '{') cCurled++;
    if( *ptr == '}') cCurled--;
    _expression_error = cRound<0 || cSquare<0 || cCurled<0;
    if( _expression_error) return ptr;
    ptr++; 
  }
  _expression_error = cRound!=0 || cSquare!=0 || cCurled!=0;
  return ptr;
}
