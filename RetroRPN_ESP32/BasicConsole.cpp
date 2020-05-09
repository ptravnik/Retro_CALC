//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "BasicConsole.hpp"

//#define __DEBUG

const char BAS_StatusMessage[] PROGMEM = "BASIC Ready";
const char BAS_RegName1[] PROGMEM = "x:";
const char BAS_RegName2[] PROGMEM = "y:";
const char BAS_RegName3[] PROGMEM = "z:";
const char BAS_Prompt[] PROGMEM = "> ";
const char BAS_Error_DivZero[] PROGMEM = "Err: div 0";
const char BAS_Error_NAN[] PROGMEM = "Err: NaN";
const char BAS_Error_Trig[] PROGMEM = "Err: |X|>1";
const char BAS_Warning_ZeroPowerZero[] PROGMEM = "Warn: 0^0";
const char BAS_Warning_Accuracy[] PROGMEM = "Warn: Inaccurate!";
const char BAS_Mode_Degrees[] PROGMEM = "Mode: Degrees";
const char BAS_Mode_Radians[] PROGMEM = "Mode: Radians";
const char BAS_Mode_Gradians[] PROGMEM = "Mode: Gradians";
const char BAS_Message_Complex[] PROGMEM = "Complex: \xb1i";
const char BAS_Message_Trivial[] PROGMEM = "Trivial";
const char BAS_Message_NoRoots[] PROGMEM = "No roots";
const char BAS_Message_OneRoot[] PROGMEM = "Single root";
const char BAS_Message_TwoRoots[] PROGMEM = "Two roots";
const char BAS_Message_ComplexRoots[] PROGMEM = "Complex: X\xb1iY";
const char BAS_Message_Discriminant[] PROGMEM = "Discriminant";
const char BAS_Message_Root1[] PROGMEM = "Root 1";
const char BAS_Message_Root2[] PROGMEM = "Root 2";
const char BAS_Message_ComplexPart[] PROGMEM = "Complex part";
const char BAS_Message_RealPart[] PROGMEM = "Real part";
const char BAS_Message_Gain[] PROGMEM = "Gain";
const char BAS_Message_Offset[] PROGMEM = "Offset";
const char BAS_Message_Goff_Solution[] PROGMEM = "Y=Gain*X+Offset";
const char BAS_Message_SD_Mounted[] PROGMEM = "SD mounted";
const char BAS_Message_SD_Removed[] PROGMEM = "SD removed";
const char *const BAS_Message_Table[] PROGMEM = {
  BAS_StatusMessage,
  BAS_RegName1,
  BAS_RegName2,
  BAS_RegName3,
  BAS_Prompt
  };
const char *const BAS_AMOD_Table[] PROGMEM = {
  BAS_Mode_Degrees,
  BAS_Mode_Radians,
  BAS_Mode_Gradians
  };

//
// Inits calculator
//
unsigned long BasicConsole::init(IOManager *iom, LCDManager *lcd, SDManager *sd, ExpressionParser *ep, CommandLine *cl, RPNCalculator *rpn){
  _io_buffer = iom->getIOBuffer();
  _iom = iom;
  _lcd = lcd;
  _sd = sd;
  _ep = ep;
  _cl = cl;
  _rpn = rpn;
  _sdPrevMounted = _sd->SDMounted;
  loadState();
  setStackRedraw();
  return _iom->keepAwake();
}

unsigned long BasicConsole::tick(){
  char c = _iom->input();
  if(c != _NUL_)
    sendChar((byte)c);
  return _iom->lastInput;
}

void BasicConsole::show(){
  _lcd->wordWrap = false;
  _lcd->scrollLock = true;
  _lcd->clearScreen( _SP_, false);
  _lcd->invertRow(6, true);
  setStackRedraw();
  _cl->show();
}

//
// Redraws the number area on LCD
//
void BasicConsole::redraw() {
//  byte lineNums[] = {6, 4, 2, 0};
  _checkSDStatus();
//  for(byte i=0; i<4; i++){
//    if( !_messageRedrawRequired[i]) continue;
//    _messageRedrawRequired[i] = false;
//    _lcd->cursorTo( 1, lineNums[i]);
//    _lcd->sendString( _messages[i]);
//    _lcd->clearToEOL();
//  }
//  for(byte i=0, j=5; i<3; i++, j-=2){
//    if( !_stackRedrawRequired[i]) continue;
//    _stackRedrawRequired[i] = false;
//    _lcd->clearLine( j);
//    size_t len = _ep->numberParser.stringValue(_getSt(i), _io_buffer)-_io_buffer;
//    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
//    _lcd->cursorTo( SCR_RIGHT-len, j);
//    _lcd->sendString( _io_buffer);
//  }
  _cl->redraw();
}

//
// Draws the stack to the serial ports
//
void BasicConsole::_updateIOM( bool refresh) {
//  if( !refresh) return;
//  _iom->sendLn();
//  for( byte i=3; i>0; i--){
//    _iom->sendStringLn( _messages[i]);
//    size_t len = _ep->numberParser.stringValue(_getSt(i-1), _io_buffer)-_io_buffer;
//    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
//    for( byte j=0; j<SCR_RIGHT-len; j++) _iom->sendChar( ' ');
//    _iom->sendStringUTF8Ln();
//  }
//  _iom->sendStringLn( _messages[0]);
  _iom->sendStringUTF8( BAS_Message_Table[4]);
}

//
// Sends one byte
//
void BasicConsole::sendChar( byte c) {
  if( expectCommand){
    processCommand(c);
    expectCommand = false;
    return;
  }
  switch(c){
    case _RPN_:
      expectCommand = true;
      return;
    case _LF_:
      processInput(true);
      return;
    case _CR_:
      processInput(false);
      return;
    case _UP_:
      _cl->copyFromPrevious();
      _cl->updateIOM();
      return;
    case _DOWN_:
      swap();
      return;
    case _PGUP_:
      prev();
      return;
    case _PGDN_:
      roll();
      return;
    case _ESC_:
      _rpn->resetRPNLabels(false);
      _cl->processESC();
      return;
    default: // other chars go to command line
      _cl->sendChar(c);
      break;
  }
}

//
// Silent command execution
//
void BasicConsole::processCommand(byte c){
  if(_cl->isInputEmpty()){
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
      _cl->processEntry(c);
      break;
    }
    return;
  }
  switch(c){
    case '+':
    case '-':
      if( _cl->isMagnitudeEntry()){
        _cl->processEntry(c);
        return;
      }
      break;
    default:
      break;
  }
  _ep->parse(_cl->getInput());  
  if( _ep->result != _NOT_A_NUMBER_)
    _pushQuick(_ep->numberParser.realValue());
  _cl->clearInput();
  expectCommand = false;
  processCommand( c);
}

//
// Parses line and performs operation
//
void BasicConsole::processInput( bool silent) {
  #ifdef __DEBUG
  Serial.print("Processing Input: [");
  Serial.print( (char *)_cl->getInput());
  Serial.println("]");
  #else
  _iom->sendLn();
  #endif
  _cl->copyToPrevious();
  _ep->parse(_cl->getInput()); 
  switch(_ep->result){
    case _STRING_:
      if( _ep->lastMathFunction == NULL){
        _evaluateString();
        return; 
      }
      _evaluateCommand();
      break;
    case _NOT_A_NUMBER_:
      // TODO: Message
      Serial.println("Result is NAN");
      break;
    default:
      _pushQuick(_ep->numberParser.realValue());
      if( !silent){
        setStackRedraw();
        _updateIOM();
      }
      break;
  }
  _cl->clearInput();
}

//
// Stack operations
// 
void BasicConsole::push(bool refresh) {
  _savePrev();
  _pushQuick();
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::pop(bool refresh) {
  _savePrev();
  _popQuick();
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::swap(bool refresh) {
  _savePrev();
  _swapQuick();
  _stackRedrawRequired[ 0] = true;
  _stackRedrawRequired[ 1] = true;
  _updateIOM(refresh);
}
void BasicConsole::roll(bool refresh) {
  _pushQuick(_getSt(RPN_STACK-1));
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::prev(bool refresh) {
  _pushQuick(_ep->mathFunctions.previous_X);
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::add(bool refresh) {
  _savePopAndUpdate( _getSt(1) + _getSt(0), refresh);
}
void BasicConsole::subtract(bool refresh) {
  _savePopAndUpdate( _getSt(1) - _getSt(0), refresh);
}
void BasicConsole::multiply(bool refresh) {
  _savePopAndUpdate( _getSt(1) * _getSt(0), refresh);
}
void BasicConsole::divide(bool refresh) {
  if( abs(_getSt(0)) < 1e-300){
    _rpn->setRPNLabel( 0, BAS_Error_DivZero);
    _setRedrawAndUpdateIOM( refresh);
    return;    
  }
  _savePopAndUpdate( _getSt(1) / _getSt(0), refresh);
}
void BasicConsole::signchange(bool refresh) {
  _savePrev();
  _setSt(0, -_getSt(0));
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::power(bool refresh) {
  _savePrev(1);
  //_messages
  // power zero: using "1 convention"  
  if( _getSt(0) == 0.0){
    _rpn->setRPNLabel( 0, BAS_Warning_ZeroPowerZero);
    _popPartial( 1.0);
    _updateIOM(refresh);
    return;
  }
  // positive power of zero: zero
  if( _getSt(0) > 0.0 && _getSt(1) == 0.0){
    _popPartial( 0.0);
    _updateIOM(refresh);
    return;
  }
  // negative power of zero: div by zero
  if( _getSt(0) < 0.0 && _isStZero(1)){
    _rpn->setRPNLabel( 0, BAS_Error_DivZero);
    _setRedrawAndUpdateIOM( refresh);
    return;
  }
  double tmp = pow( _getSt(1), _getSt(0));
  if( isnan(tmp)){
    _rpn->setRPNLabel( 0, BAS_Error_NAN);
    _setRedrawAndUpdateIOM( refresh);
    return;    
  }
  _popPartial( tmp);
  _updateIOM(refresh);
}

//
// If the number is large, periodic functions are useless
//
void BasicConsole::_checkTrigAccuracy(){
  double tmp = abs( _ep->mathFunctions.getConvertedAngle(_getSt(0)));
  if( tmp <= 1e+16) return;
  _rpn->setRPNLabel( 0, BAS_Warning_Accuracy);
  Serial.println(BAS_Warning_Accuracy);
}

//
// Decorator for quad equation solver
//
void BasicConsole::quad(bool refresh) {
  _rpn->quad(false);
}

//
// Decorator for goff2 equation solver
//
void BasicConsole::goff2(bool refresh) {
  _rpn->goff2(false);
}

//
// TODO: load and save functionality here
//
void BasicConsole::loadState(){
  if( !_sd->SDMounted) return;
  #ifdef __DEBUG
  Serial.println("loadState called");
  #endif
}

void BasicConsole::saveState(){
  if( !_sd->SDMounted) return;
  #ifdef __DEBUG
  Serial.println("saveState called");
  #endif
}

//
// Process a command, such as "sin" without parameters 
//
void BasicConsole::_evaluateCommand(){
  #ifdef __DEBUG
  Serial.println((char *)_ep->lastMathFunction->name0);
  #endif
  bool doPopPartial = true;
  bool doUpdateIOM = true;
  double *return_ptr;
  _savePrev();
  switch(_ep->lastMathFunction->RPNtag){
    case _RPN_AMODE_:
      _ep->mathFunctions.Compute( _ep->lastMathFunction, _ep->mathFunctions.rpnStack);
      _rpn->setRPNLabel( 0, BAS_AMOD_Table[_ep->mathFunctions.angleMode]);
      pop(true);
      return;
    case _RPN_CHECK_TRIG_:
      _checkTrigAccuracy();
      break;
    case _RPN_INVTRIG_:
      if( abs(_getSt(0)) > 1.0){
        _rpn->setRPNLabel( 0, BAS_Error_Trig);
        _stackRedrawRequired[ 0] = true;
        _updateIOM(doUpdateIOM);
        return;
      }
      break;
    case _RPN_DIV0_CHECK_:
      if( _isStZero(0)){
        _rpn->setRPNLabel( 0, BAS_Error_DivZero);
        doPopPartial = false;
      }
      break;
    case _RPN_ROOTYX_:
      if( _isStZero(0)){
        _rpn->setRPNLabel( 0, BAS_Error_DivZero);
        doPopPartial = false;
        break;
      }
      _setSt(0, 1.0 / _getSt(0));
      // fall-through!
    case _RPN_POWER_:
      power(true);
      return;
    case _RPN_SWAP_ONLY_:
      swap(true);
      return;
    case _RPN_SWAP_XY_:
      _savePrev(1);
      _swapQuick();
      break;
    case _RPN_QUICK_PUSH_:
      _pushQuick();
      break;
    case _RPN_SQRT_CHECK_:
      if( _getSt(0) < 0.0){
        _rpn->setRPNLabel( 0, BAS_Message_Complex);
        _setSt(0, -_getSt(0));
      }      
      break;
    case _RPN_QUAD_SOLVER:
      quad(true);
      _setRedrawAndUpdateIOM( doUpdateIOM);
      return;
    case _RPN_GOFF2_SOLVER:
      goff2(true);
      _setRedrawAndUpdateIOM( doUpdateIOM);
      return;
    default:
      break;
  }
  return_ptr = _ep->mathFunctions.Compute( _ep->lastMathFunction, _ep->mathFunctions.rpnStack);
  if( _ep->lastMathFunction->nArgs > 0) _setSt(0, return_ptr[0]);
  if( doPopPartial && _ep->lastMathFunction->nArgs > 1) _popPartial();
  else setStackRedraw();
  _updateIOM(doUpdateIOM);
}

//
// Process other string commands, such as "roll" 
//
void BasicConsole::_evaluateString(){
  byte *ptr;
  if( IsToken( _ep->nameParser.Name(), "cls", false)){
    // TODO: add screen clear
    _rpn->resetRPNLabels( false);
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->nameParser.Name(), "push", false)){
    push();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->nameParser.Name(), "pop", false)){
    pop();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->nameParser.Name(), "roll", false)){
    roll();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->nameParser.Name(), "hex", false)){
    ptr = _cl->getInput();
    _ep->numberParser.stringHex( _getSt(0), ptr);
    _cl->processEND();
    _iom->sendStringLn( ptr);
    return;
  }
  if( IsToken( _ep->nameParser.Name(), "inj", false)){
    _ep->numberParser.stringValue( _getSt(0), _io_buffer);
    _iom->injectKeyboard();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->nameParser.Name(), "rpn", false)){
    _cl->copyToPrevious();
    _cl->clearInput();
    nextUI = UI_RPNCALC;
    return;
  }
  if( IsToken( _ep->nameParser.Name(), "fman", false)){
    _cl->copyToPrevious();
    _cl->clearInput();
    nextUI = UI_FILEMAN;
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr prompt ", false)){
    _cl->copyToPrevious();
    _rpn->setRPNLabel( 0, _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr labelx ", false)){
    _cl->copyToPrevious();
    _rpn->setRPNLabel( 1, _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr labely ", false)){
    _cl->copyToPrevious();
    _rpn->setRPNLabel( 2, _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr labelz ", false)){
    _cl->copyToPrevious();
    _rpn->setRPNLabel( 3, _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr off", false)){
    Serial.println("LCD off");
    _lcd->sleepOn();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr on", false)){
    Serial.println("LCD on");
    _lcd->sleepOff();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr+", false)){
    Serial.println("LCD up");
    _lcd->changeLED( 16);
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr-", false)){
    Serial.println("LCD down");
    _lcd->changeLED( -16);
    _cl->clearInput();
    return;
  }
  Serial.println("Unknown command:");
  Serial.println((char *)_ep->_getCurrentPosition());
}

void BasicConsole::_pushQuick(){
  for(byte i=RPN_STACK-1; i>0; i--)
    _ep->mathFunctions.rpnStack[i] = _ep->mathFunctions.rpnStack[i-1];
}
void BasicConsole::_pushQuick(double v){
  _pushQuick();
  _ep->mathFunctions.rpnStack[0] = v;
}
void BasicConsole::_popQuick(byte start){
  for(byte i=start; i<RPN_STACK; i++)
    _ep->mathFunctions.rpnStack[i-1] = _ep->mathFunctions.rpnStack[i];
}
void BasicConsole::_popPartial() {
  _popQuick(2);
  setStackRedraw();
}
void BasicConsole::_popPartial( double v) {
  _popPartial();
  _ep->mathFunctions.rpnStack[0] = v;
}
void BasicConsole::_swapQuick(){
  double tmp = _ep->mathFunctions.rpnStack[0];
  _ep->mathFunctions.rpnStack[0] = _ep->mathFunctions.rpnStack[1];
  _ep->mathFunctions.rpnStack[1] = tmp;
}
void BasicConsole::_checkSDStatus(){
  if(_sdPrevMounted == _sd->SDMounted) return;
  _sdPrevMounted = _sd->SDMounted;
  Serial.println(_sdPrevMounted? BAS_Message_SD_Mounted: BAS_Message_SD_Removed);
  //setRPNLabel( 0, _sdPrevMounted? BAS_Message_SD_Mounted: BAS_Message_SD_Removed);
}
