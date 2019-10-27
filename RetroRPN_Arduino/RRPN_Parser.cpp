#include "Arduino.h"
#include "RRPN_Parser.h"

static const byte RRPN_INFINITY_MSG[]       PROGMEM = "Infinity";

//
// Converts a double number into a string
// Note that the buffer length must be sufficient to acommodate at least 
// the sign, 9 characters of the number, 6 characters of the exponent and zero
// That is 17 characters
//
char *RRPN_Parser::DoubleToString( double n, char *buff){
  int exponent = 0;
  char *ptr = buff;

  // a negative number 
  if( n<0.0){
    *ptr++ = '-';
    *ptr = '\0';
    n=-n;
  }

  // a very small number
  if( n < 1e-30){
    *ptr++ = '0';
    *ptr = '\0';
    return buff;
  }

  // a very large number
  if( n > 1e38){
    strcpy_P( ptr, RRPN_INFINITY_MSG);
    return buff;
  }

  // scientific format
  if( n>9999999.0 || n<1.0){
    while( n>=10.0){
      n *= 0.1;
      exponent++;
    }
    while( n<1.0){
      n *= 10;
      exponent--;
    }
    dtostrf( n, decimal_places+2, decimal_places, ptr);
    ptr = buff + strlen( buff);
    snprintf( ptr, 6, "e%+03d", exponent);
    return buff;
  }

  // normal decimal format
  n *= 2.0;
  n /= 2.0;
  dtostrf( n, decimal_places+2, decimal_places, ptr);

  // check if the trailing zeros can be discarded
  ptr = buff + strlen(buff)-1;
  while( ptr>buff && *ptr == '0') ptr--;
  if( *ptr == '.') ptr--;
  ptr[1] = '\0';
  return buff;
}


//
// Converts a string to a double number
//
double RRPN_Parser::StringToDouble( char *buff, bool leading_sign=true){
  bool sign = true;
  parser_Position = buff;
  if( leading_sign && *buff == '+'){
    parser_Position = buff+1;
  }
  if( leading_sign && *buff == '-'){
    sign = false;
    parser_Position = buff+1;
  }
#ifdef RRPN_PARSER_DEBUG_
  Serial.print( "Parser entry: ");
  Serial.println(parser_Position);
#endif  
  expression_Error = false;
  int entry_logical_position = 0; // 0-whole, 1-decmal, 2-fraction, 3-exponent, 4-exp sign, 5-exp number
  double decimal_multiplier = 0.1;
  bool exponent_minus = false;
  int exponent_value = 0;
  double result = 0.0;
  double tmp;
  while( true){
    switch( *parser_Position){
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
        tmp = (double)((*parser_Position) - '0');
        if( entry_logical_position == 0){
          result *= 10.0;
          result += tmp;
          break;          
        }
        if( entry_logical_position == 1){
          entry_logical_position++;
          result += decimal_multiplier * tmp;
          decimal_multiplier *= 0.1;
          break;          
        }
        if( entry_logical_position == 2){
          result += decimal_multiplier * tmp;
          decimal_multiplier *= 0.1;
          break;          
        }
        if( entry_logical_position == 3 || entry_logical_position == 4){
          entry_logical_position = 5;
        }
        exponent_value *= 10;
        exponent_value += (*parser_Position) - '0';
        break;
      case '.':
        if( entry_logical_position == 0){
          entry_logical_position = 1;
          break;          
        }
        expression_Error = true;
        return 0.0;
      case 'e':
        if( entry_logical_position < 3){
          entry_logical_position = 3;
          break;          
        }
        expression_Error = true;
        return 0.0;
      case '+':
      case '-':
        if( entry_logical_position < 3){ // probably next exporession
          return _process_Sign(_process_Double( result, exponent_value, exponent_minus), sign);
        }
        if( entry_logical_position == 3){
          entry_logical_position++;
          exponent_minus = (*parser_Position == '-');
          break;          
        }
        expression_Error = true;
        return 0.0;
      case 'f':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, -15), sign);
      case 'p':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, -12), sign);
      case 'n':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, -9), sign);
      case 'u':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, -6), sign);
      case 'm':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, -3), sign);
      case 'c':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, -2), sign);
      case 'd':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, -2), sign);
      case 'k':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, 3), sign);
      case 'M':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, 6), sign);
      case 'G':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, 9), sign);
      case 'T':
        return _process_Sign( _process_Multiplier( result, exponent_value, exponent_minus, 12), sign);
      default:
        return _process_Sign( _process_Double( result, exponent_value, exponent_minus), sign);
    }
    parser_Position++;
  }
}

double RRPN_Parser::_process_Double( double result, int exponent_value, bool exponent_minus){
  
  // TODO precision checks
  if( exponent_minus) exponent_value = -exponent_value; 
  while( exponent_value>0 && result<3.4e38){
    result *= 10.0;
    exponent_value--;
  }
  while( exponent_value<0){
      result *= 0.1;
      exponent_value++;
  }
  return result; 
}

double RRPN_Parser::_process_Multiplier( double result, int exponent_value, bool exponent_minus, int mult){
  parser_Position++;
  _ignore_Blanks();
  if( exponent_minus) exponent_value = -exponent_value;
  return _process_Double( result, mult+exponent_value, false);
}
