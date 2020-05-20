//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "RPNCalculator.hpp"

//#define __DEBUG

const char RPN_StatusMessage[] PROGMEM = "RPN Ready";
const char RPN_Error_DivZero[] PROGMEM = "Err: div 0";
const char RPN_Error_NAN[] PROGMEM = "Err: NaN";
const char RPN_Error_Trig[] PROGMEM = "Err: |X|>1";
const char RPN_Warning_ZeroPowerZero[] PROGMEM = "Warn: 0^0";
const char RPN_Warning_Accuracy[] PROGMEM = "Warn: Inaccurate!";
const char RPN_Mode_Degrees[] PROGMEM = "Mode: Degrees";
const char RPN_Mode_Radians[] PROGMEM = "Mode: Radians";
const char RPN_Mode_Gradians[] PROGMEM = "Mode: Gradians";
const char RPN_Message_Complex[] PROGMEM = "Complex: \xb1i";
const char RPN_Message_Trivial[] PROGMEM = "Trivial";
const char RPN_Message_NoRoots[] PROGMEM = "No roots";
const char RPN_Message_OneRoot[] PROGMEM = "Single root";
const char RPN_Message_TwoRoots[] PROGMEM = "Two roots";
const char RPN_Message_ComplexRoots[] PROGMEM = "Complex: X\xb1iY";
const char RPN_Message_Discriminant[] PROGMEM = "Discriminant";
const char RPN_Message_Root1[] PROGMEM = "Root 1";
const char RPN_Message_Root2[] PROGMEM = "Root 2";
const char RPN_Message_ComplexPart[] PROGMEM = "Complex part";
const char RPN_Message_RealPart[] PROGMEM = "Real part";
const char RPN_Message_Gain[] PROGMEM = "Gain";
const char RPN_Message_Offset[] PROGMEM = "Offset";
const char RPN_Message_Goff_Solution[] PROGMEM = "Y=Gain*X+Offset";
const char *const RPN_AMOD_Table[] PROGMEM = {
  RPN_Mode_Degrees,
  RPN_Mode_Radians,
  RPN_Mode_Gradians
  };

//
// Inits calculator
//
unsigned long RPNCalculator::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _sd = (SDManager *)components[UI_COMP_SDManager];
  _ep = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _rsb = (RPNStackBox *)components[UI_COMP_RPNBox];
  _mb = (MessageBox *)components[UI_COMP_MessageBox];
  _cl = (CommandLine *)components[UI_COMP_CommandLine];
  _io_buffer = _iom->getIOBuffer();
  resetRPNLabels(false);
  loadState();
  setStackRedraw();
  return _iom->keepAwake();
}

unsigned long RPNCalculator::tick(){
  char c = _iom->input();
  if(c != _NUL_)
    sendChar((byte)c);
  return _iom->lastInput;
}

void RPNCalculator::show(){
  _rsb->show();
  _mb->setLabel(RPN_StatusMessage, false);
  _mb->show();
  _cl->show();
}

//
// Redraws the number area on LCD
//
void RPNCalculator::redraw() {
  _sd->checkSDStatus();
  _rsb->redraw();
  _mb->redraw();
  _cl->redraw();
  _lcd->redraw();
}

//
// Draws the stack to the serial ports
//
void RPNCalculator::updateIOM( bool refresh) {
  if( !refresh) return;
  _rsb->updateIOM(true);
  _mb->updateIOM(true);
  _cl->showPrompt();
}

//
// Resets all calculator labels
//
void RPNCalculator::resetRPNLabels( bool refresh) {
  _rsb->resetRPNLabels();
  updateIOM(refresh);
}

//
// Sets one label: 0 - X, 1 - Y, 2 - Z
//
void RPNCalculator::setRPNLabel( byte label, byte *message, bool refresh) {
  _rsb->setRPNLabel( label, message);
  updateIOM(refresh);
}

//
// Sends one byte
//
void RPNCalculator::sendChar( byte c) {
  if( expectCommand){
    _processCommand(c);
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
      _cl->processESC();
      resetRPNLabels(true);
      return;
    default: // other chars go to command line
      _cl->sendChar(c);
      break;
  }
}

//
// Silent command execution
//
void RPNCalculator::_processCommand(byte c){
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
  _processCommand( c);
}

//
// Parses line and performs operation
//
void RPNCalculator::processInput( bool silent) {
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
      Serial.println("RPN: Result is NAN");
      break;
    default:
      _pushQuick(_ep->numberParser.realValue());
      if( !silent){
        setStackRedraw();
        updateIOM();
      }
      break;
  }
  _cl->clearInput();
}

//
// Stack operations
// 
void RPNCalculator::push(bool refresh) {
  _savePrev();
  _pushQuick();
  _setRedrawAndUpdateIOM( refresh);
}
void RPNCalculator::pop(bool refresh) {
  _savePrev();
  _popQuick();
  _setRedrawAndUpdateIOM( refresh);
}
void RPNCalculator::swap(bool refresh) {
  _savePrev();
  _swapQuick();
  _rsb->setStackRedrawRequired(0);
  _rsb->setStackRedrawRequired(1);
  updateIOM(refresh);
}
void RPNCalculator::roll(bool refresh) {
  _pushQuick(_getSt(RPN_STACK-1));
  _setRedrawAndUpdateIOM( refresh);
}
void RPNCalculator::prev(bool refresh) {
  _pushQuick(_ep->mathFunctions.previous_X);
  _setRedrawAndUpdateIOM( refresh);
}
void RPNCalculator::add(bool refresh) {
  _savePopAndUpdate( _getSt(1) + _getSt(0), refresh);
}
void RPNCalculator::subtract(bool refresh) {
  _savePopAndUpdate( _getSt(1) - _getSt(0), refresh);
}
void RPNCalculator::multiply(bool refresh) {
  _savePopAndUpdate( _getSt(1) * _getSt(0), refresh);
}
void RPNCalculator::divide(bool refresh) {
  if( abs(_getSt(0)) < 1e-300){
    _mb->setLabel( RPN_Error_DivZero);
    _setRedrawAndUpdateIOM( refresh);
    return;    
  }
  _savePopAndUpdate( _getSt(1) / _getSt(0), refresh);
}
void RPNCalculator::signchange(bool refresh) {
  _savePrev();
  _setSt(0, -_getSt(0));
  _setRedrawAndUpdateIOM( refresh);
}
void RPNCalculator::power(bool refresh) {
  _savePrev(1);
  //_messages
  // power zero: using "1 convention"  
  if( _getSt(0) == 0.0){
    _mb->setLabel( RPN_Warning_ZeroPowerZero);
    _popPartial( 1.0);
    updateIOM( refresh);
    return;
  }
  // positive power of zero: zero
  if( _getSt(0) > 0.0 && _getSt(1) == 0.0){
    _popPartial( 0.0);
    updateIOM( refresh);
    return;
  }
  // negative power of zero: div by zero
  if( _getSt(0) < 0.0 && _isStZero(1)){
    _mb->setLabel( RPN_Error_DivZero);
    _setRedrawAndUpdateIOM( refresh);
    return;
  }
  double tmp = pow( _getSt(1), _getSt(0));
  if( isnan(tmp)){
    _mb->setLabel( RPN_Error_NAN);
    _setRedrawAndUpdateIOM( refresh);
    return;    
  }
  _popPartial( tmp);
  updateIOM(refresh);
}

//
// If the number is large, periodic functions are useless
//
void RPNCalculator::_checkTrigAccuracy(){
  double tmp = abs( _ep->mathFunctions.getConvertedAngle(_getSt(0)));
  if( tmp > 1e+16)
      _mb->setLabel( RPN_Warning_Accuracy);
}

//
// Decorator for quad equation solver
//
void RPNCalculator::quad(bool refresh) {
  // Trivial solution
  if( _isStZero(2) && _isStZero(1) && _isStZero(0)){
    _mb->setLabel( RPN_Message_Trivial, false);
    return;    
  }
  // No roots
  if( _isStZero(2) && _isStZero(1)){
    _mb->setLabel( RPN_Message_NoRoots, false);
    return;    
  }
  // Solve
  double *tmp = _ep->mathFunctions.quad(_ep->mathFunctions.rpnStack);
  for(byte i=0; i<3; i++) _setSt(i, tmp[i]);
  setRPNLabel( 2, RPN_Message_Discriminant, false);
  // Complex roots
  if(tmp[2] < 0.0){
    _mb->setLabel(RPN_Message_ComplexRoots, false);
    setRPNLabel( 0, RPN_Message_RealPart, false);
    setRPNLabel( 1, RPN_Message_ComplexPart, false);
    return;
  }
  // Real roots
  setRPNLabel( 0, RPN_Message_Root1, false);
  setRPNLabel( 1, RPN_Message_Root2, false);
  if(tmp[2] == 0.0)
    _mb->setLabel(RPN_Message_OneRoot, false);
  else
    _mb->setLabel(RPN_Message_TwoRoots, false);
  return;
}

//
// Decorator for goff2 equation solver
//
void RPNCalculator::goff2(bool refresh) {
  // No solution
  if( _getSt(3) == _getSt(1)){
    _mb->setLabel(RPN_Message_Trivial, false);
    return;    
  }
  // Solution
  double *tmp = _ep->mathFunctions.goff2(_ep->mathFunctions.rpnStack);
  _mb->setLabel( RPN_Message_Goff_Solution, false);
  setRPNLabel( 0, RPN_Message_Offset, false);
  setRPNLabel( 1, RPN_Message_Gain, false);
  _popQuick(3);
  _setSt(0, tmp[0]);
  _setSt(1, tmp[1]);
  setStackRedraw();
  return;
}

//
// TODO: load and save functionality here
//
void RPNCalculator::loadState(){
  for( byte i=0; i<RPN_STACK; i++) _setSt(i, 0.0);
  if( !_sd->SDMounted) return;
  #ifdef __DEBUG
  Serial.println("loadState called");
  #endif
  _sd->readRPNStatus(_cl->_input, _cl->_inputPrevious, &(_cl->cursor_column),
    _ep->mathFunctions.rpnStack, &(_ep->mathFunctions.previous_X));
  Serial.print("loaded: [");
  Serial.print( (char*)_cl->_input);
  Serial.println("]");
}

void RPNCalculator::saveState(){
  if( !_sd->SDMounted) return;
  #ifdef __DEBUG
  Serial.println("saveState called");
  #endif
  _sd->writeRPNStatus(_cl->_input, _cl->_inputPrevious, _cl->cursor_column,
    _ep->mathFunctions.rpnStack, _ep->mathFunctions.previous_X);
  //writeFile( RPN_SaveStatusFile, "Hey, Bubba!");
  //_ep->mathFunctions.previous_X;
  //_ep->mathFunctions.rpnStack[i];  
  //_cl->
  //    uint16_t _cl->cursor_column = 0;
  //  uint16_t _cl->display_starts = 0;
  //  byte _cl->_input[INPUT_COLS];
  //  byte _cl->_inputPrevious[INPUT_COLS];
}

//
// Process a command, such as "sin" without parameters 
//
void RPNCalculator::_evaluateCommand(){
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
      _mb->setLabel( RPN_AMOD_Table[_ep->mathFunctions.angleMode]);
      pop(true);
      return;
    case _RPN_CHECK_TRIG_:
      _checkTrigAccuracy();
      break;
    case _RPN_INVTRIG_:
      if( abs(_getSt(0)) > 1.0){
        _mb->setLabel(RPN_Error_Trig);
        _rsb->setStackRedrawRequired();
        updateIOM(doUpdateIOM);
        return;
      }
      break;
    case _RPN_DIV0_CHECK_:
      if( _isStZero(0)){
        _mb->setLabel(RPN_Error_DivZero);
        doPopPartial = false;
      }
      break;
    case _RPN_ROOTYX_:
      if( _isStZero(0)){
        _mb->setLabel(RPN_Error_DivZero);
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
        _mb->setLabel(RPN_Message_Complex);
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
  updateIOM(doUpdateIOM);
}

//
// Process other string commands, such as "roll" 
//
void RPNCalculator::_evaluateString(){
  byte *ptr;
  if( IsToken( _ep->nameParser.Name(), "cls", false)){
    resetRPNLabels();
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
  if( IsToken( _ep->nameParser.Name(), "cli", false)){
    _cl->copyToPrevious();
    _cl->clearInput();
    nextUI = UI_CONSOLE;
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
    _mb->setLabel( _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr labelx ", false)){
    _cl->copyToPrevious();
    setRPNLabel( 0, _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr labely ", false)){
    _cl->copyToPrevious();
    setRPNLabel( 1, _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr labelz ", false)){
    _cl->copyToPrevious();
    setRPNLabel( 2, _cl->getInput(12));
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr off", false)){
    _mb->report(MB_MESSAGE_LCD_Off);
    _lcd->sleepOn();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr on", false)){
    _mb->report(MB_MESSAGE_LCD_On);
    _lcd->sleepOff();
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr+", false)){
    _lcd->changeLED( 16);
    _mb->report_LCDBrightness( _lcd->ledBrightness);
    _cl->clearInput();
    return;
  }
  if( IsToken( _ep->_getCurrentPosition(), "#scr-", false)){
    _lcd->changeLED( -16);
    _mb->report_LCDBrightness( _lcd->ledBrightness);
    _cl->clearInput();
    return;
  }
  Serial.println("Unknown command:");
  Serial.println((char *)_ep->_getCurrentPosition());
}

void RPNCalculator::_pushQuick(){
  for(byte i=RPN_STACK-1; i>0; i--)
    _ep->mathFunctions.rpnStack[i] = _ep->mathFunctions.rpnStack[i-1];
}
void RPNCalculator::_pushQuick(double v){
  _pushQuick();
  _ep->mathFunctions.rpnStack[0] = v;
}
void RPNCalculator::_popQuick(byte start){
  for(byte i=start; i<RPN_STACK; i++)
    _ep->mathFunctions.rpnStack[i-1] = _ep->mathFunctions.rpnStack[i];
}
void RPNCalculator::_popPartial() {
  _popQuick(2);
  setStackRedraw();
}
void RPNCalculator::_popPartial( double v) {
  _popPartial();
  _ep->mathFunctions.rpnStack[0] = v;
}
void RPNCalculator::_swapQuick(){
  double tmp = _ep->mathFunctions.rpnStack[0];
  _ep->mathFunctions.rpnStack[0] = _ep->mathFunctions.rpnStack[1];
  _ep->mathFunctions.rpnStack[1] = tmp;
}
