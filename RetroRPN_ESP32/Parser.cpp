//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "Parser.hpp"
#include "CP1251_mod.h"

//#define __DEBUG

const byte _hexadigits[] PROGMEM = 
   { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'x'};
const byte _multiplier_Prefixes[] PROGMEM = 
   { 'f', 'p', 'n', 'u', 'm', 'c', 'd', 'k', 'M', 'G', 'T', 'P'};
const int8_t _multiplier_Values[] PROGMEM = 
   { -15, -12,  -9,  -6,  -3,  -2,  -1,   3,   6,   9,  12,  15};
const char _not_a_Number[] PROGMEM = "Not a number";

//
// Number parser components
//
double NumberParser::realValue(){
  switch(result){
    case _REAL_:
      return _dValue;
    case _INTEGER_:
      return (double)_iValue;
    default:
      break;
  }
  return 0.0;
}
int64_t NumberParser::integerValue(){
  switch(result){
    case _REAL_:
      if( _dValue >= 8e18) return _HUGE_POS_INTEGER_;
      if( _dValue <= -8e18) return _HUGE_NEG_INTEGER_;
      return (int64_t)round( _dValue);
    case _INTEGER_:
      return _iValue;
    default:
      break;
  }
  return 0;
}

void NumberParser::negate(){
  switch(result){
    case _REAL_:
      _dValue = -_dValue;
      return;
    case _INTEGER_:
      _iValue = -_iValue;
      return;
    default:
      break;
  }
}

//
// Converts double or int properly
// Note that the buffer length must be sufficient to acommodate at least 
// the sign, 9 characters of the number, 6 characters of the exponent and 
// the final zero. That is 17 characters.
// For the long int, it can be up to 20 decimals, sign and _NUL_
// Returns a ponter to the end of converted string.
//
byte *NumberParser::stringValue( byte *ptr, byte max_len){
  // use internal
  if(ptr == NULL){
    ptr = _numString;
    max_len = _NUMBER_LENGTH_;
  }
  switch(result){
    case _INTEGER_ :
      return _convertInt(ptr, max_len);
    case _REAL_    :
      return _convertDouble(_dValue, ptr, max_len);
    default:
      strncpy( (char *)ptr, _not_a_Number, max_len);
      break;
  }
  return ptr;
}
byte *NumberParser::stringValue( double n, byte *ptr, byte max_len){
  // use internal
  if(ptr == NULL){
    ptr = _numString;
    max_len = _NUMBER_LENGTH_;
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
  result = _REAL_;
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
  result = _NOT_A_NUMBER_;
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
  // an engineering mulitiplier, or number end
  switch(*ptr){
    case _NUL_:
      result = _INTEGER_;
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
      result = _INTEGER_;
      return ptr;   
  }

  // process decimals;
  // the allowed are digits, E,
  // an engineering mulitiplier, or number end
  // the second decimal point causes an error
  double mult = 0.1;
  while( logical_position == 1){
    switch(*ptr){
      case _NUL_:
        result = _REAL_;
        return ptr;
      case '.': // second decimal point is not allowed
        result = _NOT_A_NUMBER_;
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
          result = _NOT_A_NUMBER_;
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
        result = _NOT_A_NUMBER_;
        _dValue = 0.0;
        return str; // parsing failed!      
      }
      break;
    default:
      result = _NOT_A_NUMBER_;
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
  result = _REAL_;
  _dValue = pow( 10, mult) * v;
  return ptr;  
}
byte *NumberParser::_processMultiplier(byte *ptr, int64_t v, int8_t mult){
  result = _REAL_;
  _dValue = pow( 10, mult) * v;
  _iValue = 0;
  return ptr;  
}
int8_t NumberParser::_locateMultiplier(byte b){
  for( byte i=0; i<12; i++){
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
  result = _INTEGER_;
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

//
// Checks if the value at text position is character c
// Used for finding commas, new lines and such
//
bool ExpressionParser::_validate_NextCharacter( byte c){
  _expression_error = _expression_error && (*_parser_position != c);
  if( _expression_error) return true;
  _parser_position++;
  _ignore_Blanks();
  return false;  
}

//
// Processes a bracket pair (less the opening bracket) or list member 
//
bool ExpressionParser::_parse_ListMember( byte terminator){
  _parser_position = parse(_parser_position);
  if( numberParser.result == _NOT_A_NUMBER_) return true;
  if( _validate_NextCharacter( terminator)) return true;
  _ignore_Blanks();
  return false;
}

byte *ExpressionParser::parse(byte *str){
  #ifdef __DEBUG
  Serial.print("Parsing: [");
  Serial.print((char*)str);
  Serial.println("]");
  #endif  

  result = _NOT_A_NUMBER_;
  _expression_error = false;
  _parser_position = str;
  _ignore_Blanks();

  // this is a kludge to test RPN screen TODO: unkludge!
  if( *_parser_position == '#'){
    nameParser._reset_name();
    result = _STRING_;
    return _parser_position; 
  }

  // names evaluation
  if( IsNameStarter( *_parser_position) ){
    _parser_position = nameParser.parse(_parser_position);
    if(!nameParser.result) return _parser_position;
    #ifdef __DEBUG
    Serial.print("Found keyword: ");
    Serial.println((char *)nameParser.Name());
    #endif
    lastMathFunction = mathFunctions.getFunction(nameParser.Name());
    if( lastMathFunction == NULL){
      #ifdef __DEBUG
      Serial.println("This name is indefined!");
      #endif
      result = _STRING_;
      return _parser_position;
    }
    #ifdef __DEBUG
    Serial.print("Located function: ");
    Serial.println(lastMathFunction->name0);
    #endif
    MathFunction *mfptr = lastMathFunction; // could be a recursive call!
    if(_parse_FunctionArguments(lastMathFunction)){
      result = _STRING_;
      return _parser_position;
    }
    double *tmp = mathFunctions.Compute( mfptr, _args);
    #ifdef __DEBUG
    Serial.print("Computation returned: ");
    Serial.println(*tmp);
    #endif
    numberParser.setValue(*tmp);
    result = numberParser.result;
    return _parser_position;
  }
  
  // Unary + and unary - by recursive calls such as 5 * -2; not sure is this should be allowed
  if( _check_NextToken( '+')){
    parse(_parser_position);
    _expression_error = _expression_error || !IsEndStatement( *_parser_position);
    if(_expression_error) return _parser_position;
    result = numberParser.result;
    return _parser_position;
  }
  if( _check_NextToken( '-')){
    parse(_parser_position);
    _expression_error = _expression_error || !IsEndStatement( *_parser_position);
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

bool ExpressionParser::_parse_FunctionArguments(MathFunction *mf){
  _ignore_Blanks();
  if(mf->nArgs == 0) return false;
  if(!_check_NextToken( '(')) return true;
  for( byte i=0; i<mf->nArgs-1; i++){
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
  _args[mf->nArgs-1] = numberParser.realValue();
  #ifdef __DEBUG
  Serial.print("Argument [");
  Serial.print( mf->nArgs-1);
  Serial.print("] parsed: ");
  Serial.println(_args[mf->nArgs-1]);
  #endif
  return false;
}
