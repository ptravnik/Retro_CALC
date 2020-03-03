//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "RPNCalculator.hpp"
#include "Utilities.hpp"
#include "IOManager.hpp"
#include "LCDManager.hpp"
#include "SDManager.hpp"
#include "Keywords.hpp"

//#define __DEBUG

static NumberParser np;

const char RPN_StatusMessage[] PROGMEM = "    ~ RPN Mode ~";
const char RPN_RegName1[] PROGMEM = "x:";
const char RPN_RegName2[] PROGMEM = "y:";
const char RPN_RegName3[] PROGMEM = "z:";
const char RPN_Prompt[] PROGMEM = "> ";
const char RPN_Error_DivZero[] PROGMEM = "Error: div 0";
const char *const RPN_Message_Table[] PROGMEM = {
  RPN_StatusMessage,
  RPN_RegName1,
  RPN_RegName2,
  RPN_RegName3,
  RPN_Prompt,
  RPN_Error_DivZero
  };

//
// Inits calculator
//
unsigned long RPNCalculator::init(IOManager *iom, LCDManager *lcd, SDManager *sd){
  _iom = iom;
  _lcd = lcd;
  _sd = sd;
  _messages[0] = _messageBuffer;
  _messages[1] = _messages[0] + SCR_COLS;
  _messages[2] = _messages[1] + SCR_COLS;
  _messages[3] = _messages[2] + SCR_COLS;
  resetRPNLabels();
  memset(_input, (byte)0, INPUT_COLS);
  memset(_inputPrevious, (byte)0, INPUT_COLS);
  setStackRedraw();
  for( byte i=0; i<RPN_STACK; i++) rpnStack[i] = 0.0;
  _iom->keepAwake();
  return _iom->lastInput;
}

unsigned long RPNCalculator::tick(){
  char c = _iom->input();
  if(c == _NUL_){
    delay(50);
    return millis();
  }
  sendChar((byte)c);
  _iom->keepAwake();
  return _iom->lastInput;
}

void RPNCalculator::show(){
  _lcd->wordWrap = false;
  _lcd->scrollLock = true;
  _lcd->clearScreen( _SP_, false);
  _lcd->invertRow(0, true);
  resetRPNLabels();
  setStackRedraw();
}

//
// Redraws the number area
//
void RPNCalculator::redraw() {
  byte lineNums[] = {0, 5, 3, 1}; 
  for(byte i=0; i<4; i++){
    if( !_messageRedrawRequired[i]) continue;
    _messageRedrawRequired[i] = false;
    _lcd->cursorTo( 1, lineNums[i]);
    _lcd->sendString( _messages[i]);
    _lcd->clearToEOL();
  }
  for(byte i=0, j=6; i<3; i++, j-=2){
    if( !_stackRedrawRequired[i]) continue;
    _stackRedrawRequired[i] = false;
    _lcd->clearLine( j);
    byte *ptr = (byte *)_lcd->getUnicodeBuffer();
    size_t len = convertDouble( rpnStack[i], ptr, _precision, _force_scientific) - ptr;
    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
    _lcd->cursorTo( SCR_RIGHT-len, j);
    _lcd->sendString( ptr);
  }
  byte *ptr = _input + display_starts;
  _lcd->cursorToBottom();
  _lcd->clearLine( SCR_BOTTOM);
  _lcd->sendStringUTF8( RPN_Message_Table[4]);
  _lcd->sendString( ptr, HSCROLL_LIMIT);
  _lcd->cursorTo( cursor_column - display_starts + 2, SCR_BOTTOM);
  _lcd->redraw();
}

//
// Draws to the serial ports
//
void RPNCalculator::updateIOM( bool refresh) {
  if( !refresh) return;
  char *buff = _lcd->getUnicodeBuffer();
  byte *ptr = (byte *)buff;
  _iom->sendToSerials( buff, _messages[0]);
  for( byte i=3; i>0; i--){
    _iom->sendToSerials( buff, _messages[i]);
    size_t len = convertDouble( rpnStack[i-1], ptr, _precision, _force_scientific) - ptr;
    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
    for( byte j=0; j<SCR_RIGHT-len; j++) _iom->sendToSerials( " ");
    _iom->sendToSerials( buff, NULL, true);
  }
  _iom->sendToSerials( (char *)RPN_Message_Table[4]);
}

//
// Resets all calculator labels
//
void RPNCalculator::resetRPNLabels() {
  for( byte i=0; i<4; i++){
    convertToCP1251( _messages[i], RPN_Message_Table[i], HSCROLL_LIMIT);
    _messageRedrawRequired[i] = true;
  }
  updateIOM();
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
  updateIOM();
}

//
// Sends one byte
//
void RPNCalculator::sendChar( byte c) {
  if( expectCommand){
    processCommand(c);
    expectCommand = false;
    return;
  }
  switch(c){
    case _BS_:
      processBS();
      return;
    case _RPN_:
      expectCommand = true;
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
// Silent command execution
//
void RPNCalculator::processCommand(byte c){
  if(isInputEmpty()){
    switch(c){
      case '+':
        add();
        return;
      case '-':
        subtract();
        return;
      case '*':
        multiply();
        return;
      case '/':
        divide();
        return;
    default:
      processEntry(c);
      break;
    }
    return;
  }
  switch(c){
    case '+':
    case '-':
      if( cursor_column >= 2 && _input[cursor_column-1] == 'e' && IsDigitOrDecimal( _input[cursor_column-2])){
        processEntry(c);
        return;
      }
      break;
    default:
      break;
  }
  np.parse(_input);  
  if( np.result != _NOT_A_NUMBER_){
    for(byte i=RPN_STACK-1; i>0; i--)
      rpnStack[i] = rpnStack[i-1];
    rpnStack[0] = np.realValue();
  }
  _clearInput();
  expectCommand = false;
  processCommand( c);
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
    _clearInput();
    return;
  }
  if( IsToken( _input, "#scr labelx ", false)){
    copyToPrevious();
    setRPNLabel( 1, _input+12);
    _clearInput();
    return;
  }
  if( IsToken( _input, "#scr labely ", false)){
    copyToPrevious();
    setRPNLabel( 2, _input+12);
    _clearInput();
    return;
  }
  if( IsToken( _input, "#scr labelz ", false)){
    copyToPrevious();
    setRPNLabel( 3, _input+12);
    _clearInput();
    return;
  }
  if( IsToken( _input, "push", false)){
    push();
    _clearInput();
    return;
  }
  if( IsToken( _input, "pop", false)){
    pop();
    _clearInput();
    return;
  }
  if( IsToken( _input, "swap", false)){
    swap();
    _clearInput();
    return;
  }
  if( IsToken( _input, "roll", false)){
    roll();
    _clearInput();
    return;
  }
  if( IsToken( _input, "prev", false)){
    prev();
    _clearInput();
    return;
  }
  if( IsToken( _input, "#scr off", false)){
    Serial.println("LCD off");
    _lcd->sleepOn();
    _clearInput();
    return;
  }
  if( IsToken( _input, "#scr on", false)){
    Serial.println("LCD on");
    _lcd->sleepOff();
    _clearInput();
    return;
  }
  if( IsToken( _input, "#scr+", false)){
    Serial.println("LCD up");
    _lcd->changeLED( 16);
    _clearInput();
    return;
  }
  if( IsToken( _input, "#scr-", false)){
    Serial.println("LCD down");
    _lcd->changeLED( -16);
    _clearInput();
    return;
  }
  np.parse(_input);  
  if( np.result != _NOT_A_NUMBER_){
    for(byte i=RPN_STACK-1; i>0; i--)
      rpnStack[i] = rpnStack[i-1];
    rpnStack[0] = np.realValue();
    setStackRedraw();
    updateIOM();
  }
  _clearInput();
}

//
// Stack operations
// 
void RPNCalculator::push(bool refresh) {
  previous_X = rpnStack[0];
  for(byte i=RPN_STACK-1; i>0; i--)
    rpnStack[i] = rpnStack[i-1];
  setStackRedraw();
  updateIOM(refresh);
}
void RPNCalculator::pop(bool refresh) {
  previous_X = rpnStack[0];
  for(byte i=1; i<RPN_STACK; i++)
    rpnStack[i-1] = rpnStack[i];
  setStackRedraw();
  updateIOM(refresh);
}
void RPNCalculator::_popPartial() {
  for(byte i=2; i<RPN_STACK; i++)
    rpnStack[i-1] = rpnStack[i];
  setStackRedraw();
}
void RPNCalculator::swap(bool refresh) {
  previous_X = rpnStack[0];
  rpnStack[0] = rpnStack[1];
  rpnStack[1] = previous_X;
  _stackRedrawRequired[ 0] = true;
  _stackRedrawRequired[ 1] = true;
  updateIOM(refresh);
}
void RPNCalculator::roll(bool refresh) {
  double tmp = rpnStack[RPN_STACK-1];
  push();
  rpnStack[0] = tmp;
  updateIOM(refresh);
}
void RPNCalculator::prev(bool refresh) {
  for(byte i=RPN_STACK-1; i>0; i--)
    rpnStack[i] = rpnStack[i-1];
  rpnStack[0] = previous_X;
  updateIOM(refresh);
}
void RPNCalculator::add(bool refresh) {
  previous_X = rpnStack[0];
  rpnStack[0] += rpnStack[1];
  _popPartial();
  updateIOM(refresh);
}
void RPNCalculator::subtract(bool refresh) {
  previous_X = rpnStack[0];
  rpnStack[0] = rpnStack[1] - rpnStack[0];
  _popPartial();
  updateIOM(refresh);
}
void RPNCalculator::multiply(bool refresh) {
  previous_X = rpnStack[0];
  rpnStack[0] *= rpnStack[1];
  _popPartial();
  updateIOM(refresh);
}
void RPNCalculator::divide(bool refresh) {
  if( -1e-300 < rpnStack[0] && rpnStack[0] < 1e-300){
    setRPNLabel( 1, RPN_Message_Table[5]);
    updateIOM(refresh);
    return;    
  }
  previous_X = rpnStack[0];
  rpnStack[0] = rpnStack[1] / rpnStack[0];
  _popPartial();
  updateIOM(refresh);
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
  setRPNLabel( 1, "");
  _clearInput();
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

void RPNCalculator::loadState(){
}

void RPNCalculator::saveState(){
}

//
// process input after entry 
//
void RPNCalculator::_clearInput() {
  *_input = 0;
  cursor_column = 0;
  display_starts = 0;
}
