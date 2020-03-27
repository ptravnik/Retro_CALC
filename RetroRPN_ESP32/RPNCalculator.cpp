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
#include "MathFunctions.hpp"

//#define __DEBUG

//static ExpressionParser ep;

const char RPN_StatusMessage[] PROGMEM = "RPN Ready";
const char RPN_RegName1[] PROGMEM = "x:";
const char RPN_RegName2[] PROGMEM = "y:";
const char RPN_RegName3[] PROGMEM = "z:";
const char RPN_Prompt[] PROGMEM = "> ";
const char RPN_Error_DivZero[] PROGMEM = "Error: div 0";
const char RPN_Error_Complex[] PROGMEM = "Complex: \xb1i";
const char RPN_Error_Trivial[] PROGMEM = "Trivial";
const char RPN_Error_NoRoots[] PROGMEM = "No roots";
const char RPN_Error_OneRoot[] PROGMEM = "Single root";
const char RPN_Error_TwoRoots[] PROGMEM = "Two roots";
const char RPN_Error_ComplexRoots[] PROGMEM = "Complex: X\xb1iY";
const char RPN_Error_NAN[] PROGMEM = "Error: NaN";
const char RPN_Warning_ZeroPowerZero[] PROGMEM = "Warning: 0^0";
const char RPN_Warning_Accuracy[] PROGMEM = "Warning: Inaccurate";
const char *const RPN_Message_Table[] PROGMEM = {
  RPN_StatusMessage,
  RPN_RegName1,
  RPN_RegName2,
  RPN_RegName3,
  RPN_Prompt,
  RPN_Error_DivZero,
  RPN_Error_Complex,
  RPN_Error_Trivial,
  RPN_Error_NoRoots,
  RPN_Error_OneRoot,
  RPN_Error_TwoRoots,
  RPN_Error_ComplexRoots,
  RPN_Error_NAN,
  RPN_Warning_ZeroPowerZero,
  RPN_Warning_Accuracy
  };

//
// Inits calculator
//
unsigned long RPNCalculator::init(IOManager *iom, LCDManager *lcd, SDManager *sd, ExpressionParser *ep){
  _io_buffer = iom->getIOBuffer();
  _iom = iom;
  _lcd = lcd;
  _sd = sd;
  _ep = ep;
  _messages[0] = _messageBuffer;
  _messages[1] = _messages[0] + SCR_COLS;
  _messages[2] = _messages[1] + SCR_COLS;
  _messages[3] = _messages[2] + SCR_COLS;
  resetRPNLabels();
  memset(_input, (byte)0, INPUT_COLS);
  memset(_inputPrevious, (byte)0, INPUT_COLS);
  setStackRedraw();
  for( byte i=0; i<RPN_STACK; i++) rpnStack[i] = 0.0;
  return _iom->keepAwake();
}

unsigned long RPNCalculator::tick(){
  char c = _iom->input();
  if(c != _NUL_)
    sendChar((byte)c);
  return _iom->lastInput;
}

void RPNCalculator::show(){
  _lcd->wordWrap = false;
  _lcd->scrollLock = true;
  _lcd->clearScreen( _SP_, false);
  _lcd->invertRow(6, true);
  setStackRedraw();
  resetRPNLabels( false);
  _iom->sendLn();
}

//
// Redraws the number area on LCD
//
void RPNCalculator::redraw() {
  byte lineNums[] = {6, 4, 2, 0}; 
  for(byte i=0; i<4; i++){
    if( !_messageRedrawRequired[i]) continue;
    _messageRedrawRequired[i] = false;
    _lcd->cursorTo( 1, lineNums[i]);
    _lcd->sendString( _messages[i]);
    _lcd->clearToEOL();
  }
  for(byte i=0, j=5; i<3; i++, j-=2){
    if( !_stackRedrawRequired[i]) continue;
    _stackRedrawRequired[i] = false;
    _lcd->clearLine( j);
    size_t len = _ep->numberParser.stringValue(rpnStack[i], _io_buffer)-_io_buffer;
    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
    _lcd->cursorTo( SCR_RIGHT-len, j);
    _lcd->sendString( _io_buffer);
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
// Draws the stack to the serial ports
//
void RPNCalculator::updateIOM( bool refresh) {
  if( !refresh) return;
  _iom->sendLn();
  for( byte i=3; i>0; i--){
    _iom->sendStringLn( _messages[i]);
    size_t len = _ep->numberParser.stringValue(rpnStack[i-1], _io_buffer)-_io_buffer;
    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
    for( byte j=0; j<SCR_RIGHT-len; j++) _iom->sendChar( ' ');
    _iom->sendStringUTF8Ln();
  }
  _iom->sendStringLn( _messages[0]);
  _iom->sendStringUTF8( RPN_Message_Table[4]);
}

//
// Resets all calculator labels
//
void RPNCalculator::resetRPNLabels( bool refresh) {
  for( byte i=0; i<4; i++){
    convertToCP1251( _messages[i], RPN_Message_Table[i], HSCROLL_LIMIT);
    _messageRedrawRequired[i] = true;
  }
  updateIOM(refresh);
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
      case '^':
        power();
        return;
      case _PLSMNS_:
        signchange();
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
  _ep->parse(_input);  
  if( _ep->result != _NOT_A_NUMBER_){
    for(byte i=RPN_STACK-1; i>0; i--)
      rpnStack[i] = rpnStack[i-1];
    rpnStack[0] = _ep->numberParser.realValue();
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
  #else
  _iom->sendLn();
  #endif
  copyToPrevious();
  _ep->parse(_input); 
  switch(_ep->result){
    case _STRING_:
      Serial.println("Result is string");
      if( _ep->lastMathFunction == NULL) break; 
      _evaluateCommand();
      break;
    case _NOT_A_NUMBER_:
      Serial.println("Result is NAN");
      break;
    default:
      for(byte i=RPN_STACK-1; i>0; i--)
        rpnStack[i] = rpnStack[i-1];
      rpnStack[0] = _ep->numberParser.realValue();
      setStackRedraw();
      updateIOM();
      break;
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
  push(false);
  rpnStack[0] = tmp;
  updateIOM(refresh);
}
void RPNCalculator::prev(bool refresh) {
  for(byte i=RPN_STACK-1; i>0; i--)
    rpnStack[i] = rpnStack[i-1];
  rpnStack[0] = previous_X;
  setStackRedraw();
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
    setRPNLabel( 0, RPN_Error_DivZero);
    setStackRedraw();
    updateIOM(refresh);
    return;    
  }
  previous_X = rpnStack[0];
  rpnStack[0] = rpnStack[1] / rpnStack[0];
  _popPartial();
  updateIOM(refresh);
}
void RPNCalculator::signchange(bool refresh) {
  previous_X = rpnStack[0];
  rpnStack[0] = -rpnStack[0];
  _stackRedrawRequired[ 0] = true;
  updateIOM(refresh);
}
void RPNCalculator::power(bool refresh) {
  previous_X = rpnStack[1];
  //_messages
  // power zero: using "1 convention"  
  if( rpnStack[0] == 0.0){
    setRPNLabel( 0, RPN_Warning_ZeroPowerZero);
    rpnStack[0] = 1.0;
    _popPartial();
    updateIOM(refresh);
    return;
  }
  // positive power of zero: zero
  if( rpnStack[0] > 0.0 && rpnStack[1] == 0.0){
    rpnStack[0] = 0.0;
    _popPartial();
    updateIOM(refresh);
    return;
  }
  // negative power of zero: div by zero
  if( rpnStack[0] < 0.0 && rpnStack[1] == 0.0){
    setRPNLabel( 0, RPN_Error_DivZero);
    setStackRedraw();
    updateIOM(refresh);
    return;
  }
  double tmp = pow( rpnStack[1], rpnStack[0]);
  if( isnan(tmp)){
    setRPNLabel( 0, RPN_Error_NAN);
    setStackRedraw();
    updateIOM(refresh);
    return;    
  }
  rpnStack[0] = tmp;
  _popPartial();
  updateIOM(refresh);
}

void RPNCalculator::_checkTrigAccuracy(){
  double tmp = _ep->mathFunctions.getConvertedAngle(rpnStack[0]);
  if( tmp < 0) tmp = -tmp;
  if( tmp > 1e+16)
    setRPNLabel( 0, RPN_Warning_Accuracy);
}

void RPNCalculator::quad(bool refresh) {
  // Trivial solution
  if( rpnStack[2] == 0.0 && rpnStack[1] == 0.0 && rpnStack[0] == 0.0){
    setRPNLabel( 0, RPN_Message_Table[7]);
    setStackRedraw();
    updateIOM(refresh);
    return;    
  }
  // No roots
  if( rpnStack[2] == 0.0 && rpnStack[1] == 0.0){
    setRPNLabel( 0, RPN_Message_Table[8]);
    setStackRedraw();
    updateIOM(refresh);
    return;    
  }
  double tmp = rpnStack[2] * 2.0;
  // Linear single root
  if( tmp == 0.0){
    setRPNLabel( 0, RPN_Message_Table[9]);
    tmp = -rpnStack[0] / rpnStack[1];
    _popPartial();
    _popPartial();
    rpnStack[0] = tmp;
    setStackRedraw();
    updateIOM(refresh);
    return;    
  }
  double discr = rpnStack[1] * rpnStack[1] - 4.0 * rpnStack[0] * rpnStack[2];
  // Quad single root
  if( -1e-300 < discr && discr < 1e-300){
    setRPNLabel( 0, RPN_Message_Table[9]);
    tmp = -rpnStack[1] / tmp;
    _popPartial();
    _popPartial();
    rpnStack[0] = tmp;
    setStackRedraw();
    updateIOM(refresh);
    return;    
  }
  // Quad two roots
  if( discr > 0.0){
    setRPNLabel( 0, RPN_Message_Table[10]);
    rpnStack[2] = discr;
    discr = sqrt( discr) / tmp;
    tmp = -rpnStack[1] / tmp;
    rpnStack[0] = tmp+discr;
    rpnStack[1] = tmp-discr;
    setStackRedraw();
    updateIOM(refresh);
    return;    
  }
  setRPNLabel( 0, RPN_Message_Table[11]);
  rpnStack[2] = discr;
  discr = sqrt( -discr) / tmp;
  rpnStack[0] = -rpnStack[1] / tmp;
  rpnStack[1] = discr;
  setStackRedraw();
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
  _iom->sendChar(c);
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
  resetRPNLabels(true);
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

//
// Process a command, such as "sin" without parameters 
//
void RPNCalculator::_evaluateCommand(){
  Serial.println((char *)_ep->lastMathFunction->name0);
  bool doPopPartial = true;
  bool doUpdateIOM = true;
  double *return_ptr;
  previous_X = rpnStack[0];
  switch(_ep->lastMathFunction->RPNtag){
    case _RPN_CHECK_TRIG_:
      _checkTrigAccuracy();
      break;
    case _RPN_SWAP_XY_:
      _swapQuick();
      break;
    default:
      break;
  }
  return_ptr = _ep->mathFunctions.Compute( _ep->lastMathFunction, rpnStack);
  rpnStack[0] = return_ptr[0];
  if( doPopPartial && _ep->lastMathFunction->nArgs > 1) _popPartial();
  else _stackRedrawRequired[ 0] = true;
  updateIOM(doUpdateIOM);

//  if( IsToken( _input, "#scr status ", false)){
//    copyToPrevious();
//    setRPNLabel( 0, _input+12);
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "#scr labelx ", false)){
//    copyToPrevious();
//    setRPNLabel( 1, _input+12);
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "#scr labely ", false)){
//    copyToPrevious();
//    setRPNLabel( 2, _input+12);
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "#scr labelz ", false)){
//    copyToPrevious();
//    setRPNLabel( 3, _input+12);
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "push", false)){
//    push();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "pop", false)){
//    pop();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "swap", false)){
//    swap();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "roll", false)){
//    roll();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "prev", false)){
//    prev();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "#scr off", false)){
//    Serial.println("LCD off");
//    _lcd->sleepOn();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "#scr on", false)){
//    Serial.println("LCD on");
//    _lcd->sleepOff();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "#scr+", false)){
//    Serial.println("LCD up");
//    _lcd->changeLED( 16);
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "#scr-", false)){
//    Serial.println("LCD down");
//    _lcd->changeLED( -16);
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "hex", false)){
//    _ep->numberParser.stringHex( rpnStack[0], _input);
//    cursor_column = strlen(_input);
//    display_starts = 0;
//    _iom->sendStringLn( _input);
//    return;
//  }
//  if( IsToken( _input, "sin", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] = sin( rpnStack[0]);
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "cos", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] = cos( rpnStack[0]);
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "tan", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] = tan( rpnStack[0]);
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "exp", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] = exp( rpnStack[0]);
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "sqrt", false)){
//    previous_X = rpnStack[0];
//    if(rpnStack[0] < 0.0){
//      setRPNLabel( 0, RPN_Message_Table[6]);
//      rpnStack[0] = -rpnStack[0]; 
//    }
//    rpnStack[0] = sqrt( rpnStack[0]);
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "sq", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] *= rpnStack[0];
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "sign", false)){
//    previous_X = rpnStack[0];
//    if( rpnStack[0]==0) rpnStack[0]=0;
//    else
//      rpnStack[0] = (rpnStack[0]<0)? -1: 1;
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "abs", false)){
//    previous_X = rpnStack[0];
//    if( rpnStack[0] < 0.0) rpnStack[0] = - rpnStack[0];
//    _stackRedrawRequired[ 0] = true;
//    _clearInput();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "radius", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] = sqrt( rpnStack[0]*rpnStack[0] + rpnStack[1]*rpnStack[1]);
//    _clearInput();
//    _popPartial();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "circa", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] = 2.0 * rpnStack[0] * RPN_PI;
//    _clearInput();
//    _popPartial();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "circ", false)){
//    previous_X = rpnStack[0];
//    rpnStack[0] = rpnStack[0] * rpnStack[0] * RPN_PI;
//    _clearInput();
//    _popPartial();
//    updateIOM(true);
//    return;
//  }
//  if( IsToken( _input, "quad", false)){
//    quad();
//    _clearInput();
//    return;
//  }
//  if( IsToken( _input, "inv", false)){
//    _clearInput();
//    if( -1e-300 < rpnStack[0] && rpnStack[0] < 1e-300){
//      setRPNLabel( 0, RPN_Message_Table[5]);
//      setStackRedraw();
//      updateIOM(true);
//      return;    
//    }
//    previous_X = rpnStack[0];
//    rpnStack[0] = 1.0 / rpnStack[0];
//    _popPartial();
//    updateIOM(true);
//  }
//  if( IsToken( _input, "inj", false)){
//    _ep->numberParser.stringValue( rpnStack[0], _io_buffer);
//    _iom->injectKeyboard();
//  }
//  //_ep->parse(_input);  
//  if( _ep->result != _NOT_A_NUMBER_){
//    for(byte i=RPN_STACK-1; i>0; i--)
//      rpnStack[i] = rpnStack[i-1];
//    rpnStack[0] = _ep->numberParser.realValue();
//    setStackRedraw();
//    updateIOM();
//  }
}
