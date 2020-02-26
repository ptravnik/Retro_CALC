#include "RPNCalculator.hpp"
#include "Utilities.hpp"

//#define __DEBUG

static NumberParser np;

const char RPN_StatusMessage[] PROGMEM = "    ~ RPN Mode ~";
const char RPN_RegName1[] PROGMEM = "x:";
const char RPN_RegName2[] PROGMEM = "y:";
const char RPN_RegName3[] PROGMEM = "z:";
const char RPN_Prompt[] PROGMEM = "> ";
const char *const RPN_Message_Table[] PROGMEM = {
  RPN_StatusMessage,
  RPN_RegName1,
  RPN_RegName2,
  RPN_RegName3,
  RPN_Prompt,
  };

//
// Inits calculator
//
void RPNCalculator::init( IOManager iom, LCDManager lcd) {
  _iom = iom;
  _lcd = lcd;
  _messages[0] = _messageBuffer;
  _messages[1] = _messages[0] + SCR_COLS;
  _messages[2] = _messages[1] + SCR_COLS;
  _messages[3] = _messages[2] + SCR_COLS;
  resetRPNLabels();
  memset(_input, (byte)0, INPUT_COLS);
  memset(_inputPrevious, (byte)0, INPUT_COLS);
  setStackRedraw();
  for( byte i=0; i<RPN_STACK; i++) rpnStack[i] = 3.1415 + i;
}

void RPNCalculator::show(){
  _lcd.wordWrap = false;
  _lcd.scrollLock = true;
  _lcd.clearScreen( _SP_, false);
  _lcd.invertRow(0, true);
  resetRPNLabels();
  updateIOM();
}

//
// Redraws the number area
//
void RPNCalculator::redraw() {
  byte lineNums[] = {0, 5, 3, 1}; 
  for(byte i=0; i<4; i++){
    if( !_messageRedrawRequired[i]) continue;
    _messageRedrawRequired[i] = false;
    _lcd.cursorTo( 1, lineNums[i]);
    _lcd.sendString( _messages[i]);
    _lcd.clearToEOL();
  }
  for(byte i=0, j=6; i<3; i++, j-=2){
    if( !_stackRedrawRequired[i]) continue;
    _stackRedrawRequired[i] = false;
    _lcd.clearLine( j);
    byte *ptr = (byte *)_lcd.getUnicodeBuffer();
    size_t len = convertDouble( rpnStack[i], ptr, _precision, _force_scientific) - ptr;
    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
    _lcd.cursorTo( SCR_RIGHT-len, j);
    _lcd.sendString( ptr);
  }
  byte *ptr = _input + display_starts;
  _lcd.cursorToBottom();
  _lcd.clearLine( SCR_BOTTOM);
  _lcd.sendStringUTF8( RPN_Message_Table[4]);
  _lcd.sendString( ptr, HSCROLL_LIMIT);
  _lcd.cursorTo( cursor_column - display_starts + 2, SCR_BOTTOM);
  _lcd.redraw();
}

//
// Draws to the serial ports
//
void RPNCalculator::updateIOM() {
  char *buff = _lcd.getUnicodeBuffer();
  byte *ptr = (byte *)buff;
  _iom.sendToSerials( buff, _messages[0]);
  for( byte i=3; i>0; i--){
    _iom.sendToSerials( buff, _messages[i]);
    size_t len = convertDouble( rpnStack[i-1], ptr, _precision, _force_scientific) - ptr;
    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
    for( byte j=0; j<SCR_RIGHT-len; j++) _iom.sendToSerials( " ");
    _iom.sendToSerials( buff, NULL, true);
  }
  _iom.sendToSerials( (char *)RPN_Message_Table[4]);
}

//
// Resets all calculator labels
//
void RPNCalculator::resetRPNLabels() {
  for( byte i=0; i<4; i++){
    convertToCP1251( _messages[i], RPN_Message_Table[i], HSCROLL_LIMIT);
    _messageRedrawRequired[i] = true;
  }
}

//
// Sets one label: 0 - header, 1 - X, 2 - Y, 3 - Z
//
void RPNCalculator::setRPNLabel( byte label, byte *message) {
  if( label > 3) label = 0;
  if( *message)
    strncpy( (char *)_messages[label], (char *)message, HSCROLL_LIMIT);  
  else
    convertToCP1251( _messages[label], RPN_Message_Table[label], HSCROLL_LIMIT);
  _messageRedrawRequired[label] = true;
}

//
// Sends one byte
//
void RPNCalculator::sendChar( byte c) {
  switch(c){
    case _BS_:
      processBS();
      return;
    case _CR_:
      processInput();
      return;
    case _LEFT_:
      processLEFT();
      return;
    case _RIGHT_:
      processRIGHT();
      return;
    case _HOME_:
      processHOME();
      return;
    case _END_:
      processEND();
      return;
    case _UP_:
      copyFromPrevious();
      return;
    case _DOWN_:
      Serial.println("SWAP XY");
      swap();
      return;
    case _PGUP_:
      Serial.println("PREV");
      prev();
      return;
    case _PGDN_:
      Serial.println("ROLL");
      roll();
      return;
    case _ESC_:
      processESC();
      return;
    case _DEL_:
      processDEL();
      return;
    default: // other chars
      processEntry(c);
      break;
  }
}

//
// Parses line and performs operation
//
void RPNCalculator::processInput() {
  #ifdef __DEBUG
  Serial.print("Processing Input: [");
  Serial.print( (char *)_input);
  Serial.println("]");
  #endif
  copyToPrevious();
  if( IsToken( _input, "#scr status ", false)){
    copyToPrevious();
    setRPNLabel( 0, _input+12);
    processESC();
    return;
  }
  if( IsToken( _input, "#scr labelx ", false)){
    copyToPrevious();
    setRPNLabel( 1, _input+12);
    processESC();
    return;
  }
  if( IsToken( _input, "#scr labely ", false)){
    copyToPrevious();
    setRPNLabel( 2, _input+12);
    processESC();
    return;
  }
  if( IsToken( _input, "#scr labelz ", false)){
    copyToPrevious();
    setRPNLabel( 3, _input+12);
    processESC();
    return;
  }
  if( IsToken( _input, "push", false)){
    push();
    processESC();
    return;
  }
  if( IsToken( _input, "pop", false)){
    pop();
    processESC();
    return;
  }
  if( IsToken( _input, "swap", false)){
    swap();
    processESC();
    return;
  }
  if( IsToken( _input, "roll", false)){
    roll();
    processESC();
    return;
  }
  if( IsToken( _input, "prev", false)){
    prev();
    processESC();
    return;
  }
  if( IsToken( _input, "#scr off", false)){
    Serial.println("LCD off");
    _lcd.sleepOn();
    processESC();
    return;
  }
  if( IsToken( _input, "#scr on", false)){
    Serial.println("LCD on");
    _lcd.sleepOff();
    processESC();
    return;
  }
  if( IsToken( _input, "#scr+", false)){
    Serial.println("LCD up");
    _lcd.changeLED( 16);
    processESC();
    return;
  }
  if( IsToken( _input, "#scr-", false)){
    Serial.println("LCD down");
    _lcd.changeLED( -16);
    processESC();
    return;
  }
  np.parse(_input);  
  if( np.result != _NOT_A_NUMBER_){
    //push();
    rpnStack[0] = np.realValue();
    _stackRedrawRequired[ 0] = true;
  }
  processESC();
}

//
// Stack operations
// 
void RPNCalculator::push() {
  previous_X = rpnStack[0];
  for(byte i=RPN_STACK-1; i>0; i--)
    rpnStack[i] = rpnStack[i-1];
  setStackRedraw();
  updateIOM();
}
void RPNCalculator::pop() {
  previous_X = rpnStack[0];
  for(byte i=1; i<RPN_STACK; i++)
    rpnStack[i-1] = rpnStack[i];
  setStackRedraw();
  updateIOM();
}
void RPNCalculator::swap() {
  previous_X = rpnStack[0];
  rpnStack[0] = rpnStack[1];
  rpnStack[1] = previous_X;
  _stackRedrawRequired[ 0] = true;
  _stackRedrawRequired[ 1] = true;
  updateIOM();
}
void RPNCalculator::roll() {
  double tmp = rpnStack[RPN_STACK-1];
  push();
  rpnStack[0] = tmp;
  updateIOM();
}
void RPNCalculator::prev() {
  for(byte i=RPN_STACK-1; i>0; i--)
    rpnStack[i] = rpnStack[i-1];
  rpnStack[0] = previous_X;  updateIOM();
}

//
// process character entry
//
void RPNCalculator::processEntry(byte c) {
  if( c < _SP_) return; // unsupported service characters
  if( cursor_column >= INPUT_LIMIT) return; // line full
  byte *ptr = _input+cursor_column;
  if( *ptr){ // insert to the middle of the line
    memmove( ptr+1, ptr, INPUT_LIMIT-cursor_column);
    *ptr = c;
    _input[INPUT_LIMIT] = 0;
    }
  else{ // add at the end of line
    *ptr++ = c;
    *ptr = 0;
    }
  processRIGHT(); // move the cursor
}

//
// process del
//
void RPNCalculator::processDEL() {
  byte *ptr = _input+cursor_column;
  if( !(*ptr)) return; // end of string - nothing to delete
  memmove( ptr, ptr+1, INPUT_LIMIT-cursor_column);
}

//
// process backspace - the same as pressing LEFT and DEL 
//
void RPNCalculator::processBS() {
  if( !cursor_column) return; // nothing to delete
  processLEFT();
  processDEL();
}

//
// process other buttons 
//
void RPNCalculator::processESC() {
  *_input = 0;
  cursor_column = 0;
  display_starts = 0;
  updateIOM();
}

void RPNCalculator::processLEFT() {
  if( !cursor_column) return; // no place like home
  cursor_column--;
  if( display_starts>cursor_column)
    display_starts=cursor_column;
}

void RPNCalculator::processRIGHT() {
  byte *ptr = _input+cursor_column;
  if( !(*ptr)) return; // end of string - no place to move
  cursor_column++;
  if( cursor_column-display_starts >= HSCROLL_LIMIT)
    display_starts++;
}

void RPNCalculator::processHOME() {
  cursor_column = 0;
  display_starts = 0;
}

void RPNCalculator::processEND() {
  cursor_column = strlen( _input);
  if( cursor_column > HSCROLL_LIMIT)
    display_starts = cursor_column - HSCROLL_LIMIT;
}

void RPNCalculator::copyFromPrevious(){
  strcpy( (char *)_input, (char *)_inputPrevious);
  processEND();
  updateIOM();
}
