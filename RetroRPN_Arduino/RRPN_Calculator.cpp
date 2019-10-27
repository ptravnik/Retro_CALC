#include "Arduino.h"
#include "RRPN_Calculator.h"

static const byte RRPN_STACK_LABELS[] PROGMEM = {'X', ':', 0, 'Y', ':', 0, 'Z', ':', 0};

//
// Console connection:
// allocates memory for PROGMEM reading buffer
// returns the next available slot in xram
//
void *RRPN_Calculator::init( void *xram, LCD_Console *terminal){
  char c;
  const char *ptr_in = RRPN_STACK_LABELS;
  volatile char *ptr_out;
  for( byte i=0; i<3; i++){
    volatile char *ptr_out = reinterpret_cast<volatile char*>(xram);
    _labels[i] = ptr_out;
    xram += LABEL_SIZE;
  }
  resetLabels();
  _terminal = terminal;
  for( byte i=0; i<STACK_SIZE; i++) stack[i] = 0.0;
  input[0] = 0;
  return xram;
}


//
// Changes the calculator stack labels:
// n=0,1,2
// lbl - null terminated; UTF8 is allowed
//
void RRPN_Calculator::changeLabel( byte n, char *lbl){
  char *dest = _labels[n];
  strncpy( dest, lbl, LABEL_SIZE-1);
  dest[LABEL_SIZE-1] = 0;
}


//
// Changes the calculator stack labels:
// n=0,1,2
// lbl - in PROGMEM, null terminated; UTF8 is allowed
//
void RRPN_Calculator::changeLabel_P( byte n, const char *lbl){
  char *dest = _labels[n];
  strncpy_P( dest, lbl, LABEL_SIZE-1);
  dest[LABEL_SIZE-1] = 0;
}


//
// Resets the calculator stack labels to X:, Y:, Z:
//
void RRPN_Calculator::resetLabels(){
  setLabels( RRPN_STACK_LABELS);
}


//
// Changes all stack labels
// lbl must be a concatenation of 3 null-terminated strings
//
void RRPN_Calculator::setLabels( const char *lbl){
  for( byte i=0; i<3; i++){
    changeLabel_P( i, lbl);
    while( pgm_read_byte( lbl++));
    //lbl++;
  }
}


//
// Processes user input; resets flags
// Returns true if input is processed
//
bool RRPN_Calculator::parseInput(){  
  if( inputEmpty()) return false;
#ifdef RRPN_DEBUG_
  Serial.print(input);
  Serial.print( " converts to ");
#endif
  stack[0] = Parser.StringToDouble(input);
#ifdef RRPN_DEBUG_
  Serial.println(stack[0]);
#endif
  _clearInput();
  return true;
}


//
// Converts stack X into input; resets flags
// Returns true if processed
//
bool RRPN_Calculator::unparseInput(){
  if( *input) return false;
  Parser.DoubleToString(stack[0], input);
  cursorPosition = strlen(input);
  return true;
}


//
// clears input
// Returns true if processed
//
void RRPN_Calculator::_clearInput(){
  *input = '\0';
  cursorPosition = 0;
}


//
// Converts sign in either exponent or mantissa
//
void RRPN_Calculator::_changeInputSign(){
  char *ptr = strchr(input, 'e');
  if( ptr == 0) ptr = input;
  else ptr++;
  if( *ptr == '-'){
    *ptr = '+';
    return;
  }
  if( *ptr == '+'){
    *ptr = '-';
    return;
  }
  memmove( ptr+1, ptr, strlen(ptr)+1);
  *ptr = '-';
  cursorPosition++;
}


//
// Adds a character into the input string
//
void RRPN_Calculator::_insertCharacter( byte c){
  size_t len = strlen(input);
  if( len >= INPUT_SIZE) return;
  if( cursorPosition == len){
     input[cursorPosition++] = (char)c;
     input[cursorPosition] = '\0';
     return;
  }
  memmove( input+cursorPosition+1, input+cursorPosition, len-cursorPosition);
  input[cursorPosition++] = (char)c;  
}


//
// Removes one character left of cursor
//
void RRPN_Calculator::_backspaceCharacter(){
  size_t len = strlen(input);
  if( !cursorPosition) return;
  if( cursorPosition == len){
    input[--cursorPosition] = '\0';
    return;
    }
  memmove( input+cursorPosition-1, input+cursorPosition, len-cursorPosition+1);
  cursorPosition--;
}


//
// Removes one character at cursor
//
void RRPN_Calculator::_deleteCharacter(){
  size_t len = strlen(input);
  if( cursorPosition == len) return;
  memmove( input+cursorPosition, input+cursorPosition+1, len-cursorPosition+1);
}


//
// checks if the user can type a decimal
//
bool RRPN_Calculator::_decimal_allowed(){
  if( strchr( '.', input) != 0) return false;
  if( strchr( 'e', input) != 0) return false;
  return true;
}


//
// checks if the user can type an exponent
//
bool RRPN_Calculator::_exponent_allowed(){
  size_t len = strlen(input);
  if( len<1) return false;
  if( cursorPosition == 0) return false;
  if( *input == '.'){
    if( len==1) return false;
    if( cursorPosition == 1) return false;
  }
  if( strchr( 'e', input) != 0) return false;
  return true;
}


//
// checks if the string has one of "multiplier" letters
//
bool RRPN_Calculator::_greek_allowed(){
  if( strchr( 'e', input) != 0) return false;
  size_t len = strlen(input);
  if( len<1) return false;
  if( len==1 && *input == '.') return false;
  if( cursorPosition != len) return false;
  if( strchr( input[--len], "fpnumcdkMGT") != 0) return false;
  return true;
}


//
// Processes a single keyboard press
//
void RRPN_Calculator::send( char c){
  if( _processCommand( (byte)c)) return;
  if( _processCharacterInput( (byte) c)) return;
  if( _processOperator( (byte) c)) return;
}


//
// Processes a command code
//
bool RRPN_Calculator::_processCommand( byte c){
  if( !_command_expected) return false;
  size_t len = strlen(input);
  //double result = 0.0;
#ifdef RRPN_DEBUG_
  Serial.println(c);
#endif
  switch( c){
    case 1: // left
      unparseInput();
      cursorPosition--;
      if( cursorPosition < 0) cursorPosition = 0;
      _terminal->displayStackX( input, cursorPosition);
      break;
    case 2: // down
      _clearInput();
      pop();
      display();
      break;
    case 3: // up
      _clearInput();
      prev();
      display();
      break;
    case 4: // right
      unparseInput();
      cursorPosition++;
      if( cursorPosition > len) cursorPosition = len;
      _terminal->displayStackX( input, cursorPosition);
      break;
    case 5: // enter
      parseInput();
      push();
      display();
      break;
    case 6: // exponent
      if( !_exponent_allowed()) return true;
      _insertCharacter('e');
      _terminal->displayStackX( input, cursorPosition);
      break;
    case 8: // backspace
      if( inputEmpty() || !len){
        stack[0] = 0.0;
        display();
        break;
      }
      _backspaceCharacter();
      _terminal->displayStackX( input, cursorPosition);
      break;
    case 177: // sign change
      if( inputEmpty()){
        stack[0] = -stack[0];
        display();
        break;
      }
      _changeInputSign();
      _terminal->displayStackX( input, cursorPosition);
      break;
    default:
      break; 
  }
  _command_expected = false;
  return true;
}

//
// Processes a single keyboard press
//
bool RRPN_Calculator::_processCharacterInput( byte c){
  size_t len = strlen(input);
  switch( c){
    case 1:
#ifdef RRPN_DEBUG_
  Serial.print("Command expected: ");
#endif
      _command_expected = true;
      return true;
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
      if(inputEmpty()){
        push();
        display();
      }
      _insertCharacter(c);
      _terminal->displayStackX( input, cursorPosition);
      return true;
    case '.':
      if(inputEmpty()){
        push();
        display();
      }
      if( !_decimal_allowed()) return true;
      _insertCharacter(c);
      _terminal->displayStackX( input, cursorPosition);
      return true;
    case 'f':
    case 'p':
    case 'n':
    case 'u':
    case 'm':
    case 'c':
    case 'd':
    case 'k':
    case 'M':
    case 'G':
    case 'T':
      if( !_greek_allowed()) return true;
      _insertCharacter(c);
      _terminal->displayStackX( input, cursorPosition);
      return true;
    default:
      break;
  }
  return false; 
}


//
// Processes simple operators
//
bool RRPN_Calculator::_processOperator( byte c){
  size_t len = strlen(input);
#ifdef RRPN_DEBUG_
  Serial.println( c);
#endif
  switch( c){
    case '+':
      parseInput();
      previousX = stack[0];
      stack[0] += stack[1];
      pop(1);
      display();
      return true;
    case '-':
      parseInput();
      previousX = stack[0];
      stack[0] = stack[1] - stack[0];
      pop(1);
      display();
      return true;
    case '*':
      parseInput();
      previousX = stack[0];
      stack[0] *= stack[1];
      pop(1);
      display();
      return true;
    case '/':
      parseInput();
      previousX = stack[0];
      if( -5e-38 < stack[0] && stack[0] < 5e-38){
        stack[0] = 2e38;
        if( stack[0] > 0 && stack[1] < 0) stack[0] = -2e38;
        if( stack[0] < 0 && stack[1] > 0) stack[0] = -2e38;
      }
      else{
        stack[0] = stack[1] / stack[0];
      }
      pop(1);
      display();
      return true;
    default:
      return false; 
  }
}


void RRPN_Calculator::pop(size_t head=0){
#ifdef RRPN_DEBUG_
  Serial.println("POP:");
  Serial.println(stack[2]);
  Serial.println(stack[1]);
  Serial.println(stack[0]);
#endif
    for( size_t i=head; i<STACK_SIZE-1; i++)
    stack[i] = stack[i+1];
}

void RRPN_Calculator::push(){
#ifdef RRPN_DEBUG_
  Serial.println("PUSH:");
  Serial.println(stack[2]);
  Serial.println(stack[1]);
  Serial.println(stack[0]);
#endif
  for( size_t i=STACK_SIZE-1; i>0; i--)
    stack[i] = stack[i-1];
}

void RRPN_Calculator::prev(){
#ifdef RRPN_DEBUG_
  Serial.println("PREV:");
  Serial.println(stack[2]);
  Serial.println(stack[1]);
  Serial.println(stack[0]);
#endif
  for( size_t i=STACK_SIZE-1; i>0; i--)
    stack[i] = stack[i-1];
  stack[0] = previousX;  
}
