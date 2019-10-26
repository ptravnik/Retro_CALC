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
// Processes a single keyboard press
//
void RRPN_Calculator::display(){
  _terminal->displayRPN( input, stack, _labels);
}


//
// Processes a single keyboard press
//
void RRPN_Calculator::send( char c){
  if( _command_expected){
    _processCommand( (byte)c);
    return;
  } 
  _processCharacter( (byte) c);
}

//
// Processes a command code
//
void RRPN_Calculator::_processCommand( byte c){
  size_t len = strlen(input);
  double result = 0.0;
#ifdef RRPN_DEBUG_
  Serial.println(c);
#endif
  switch( c){
    case '\1': // left
      cursorPosition--;
      if( cursorPosition < 0) cursorPosition = 0;
      _terminal->displayStackX( input, cursorPosition);
      break;
    case '\2': // down
      _clearInput();
      pop();
      display();
      break;
    case '\3': // up
      _clearInput();
      prev();
      display();
      break;
    case '\4': // right
      cursorPosition++;
      if( cursorPosition > len) cursorPosition = len;
      _terminal->displayStackX( input, cursorPosition);
      break;
    case '\5': // enter
      if( len){
#ifdef RRPN_DEBUG_
  Serial.print(input);
  Serial.print( " converts to ");
#endif
        stack[0] = Parser.StringToDouble(input);
#ifdef RRPN_DEBUG_
  Serial.println(stack[0]);
#endif
        _clearInput();
      }
      push();
      display();
      break;
    default:
      break; 
  }
  _command_expected = false;
}

//
// Processes a single keyboard press
//
void RRPN_Calculator::_processCharacter( byte c){
  size_t len = strlen(input);
  switch( c){
    case 1:
#ifdef RRPN_DEBUG_
  Serial.print("Command expected: ");
#endif
      _command_expected = true;
      break;
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
    case '.':
    case 'e':
      if( len >= INPUT_SIZE) break;
      if( cursorPosition == len){
        input[cursorPosition++] = (char)c;
        input[cursorPosition] = 0;
        _terminal->displayStackX( input, cursorPosition);
        break;
      }
      memmove( input+cursorPosition+1, input+cursorPosition, len-cursorPosition);
      input[cursorPosition++] = (char)c;
      _terminal->displayStackX( input, cursorPosition);
      break;
    default:
      break; 
  }
}


void RRPN_Calculator::pop(){
#ifdef RRPN_DEBUG_
  Serial.println("POP:");
  Serial.println(stack[2]);
  Serial.println(stack[1]);
  Serial.println(stack[0]);
#endif
    for( size_t i=0; i<STACK_SIZE-1; i++)
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
