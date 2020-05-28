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
const char *const BAS_AMOD_Table[] PROGMEM = {
  BAS_Mode_Degrees,
  BAS_Mode_Radians,
  BAS_Mode_Gradians
  };

//
// Inits console
//
unsigned long BasicConsole::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _vars = (Variables *)components[UI_COMP_Variables];
  _funs = (Functions *)components[UI_COMP_Functions];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _sdm = (SDManager *)components[UI_COMP_SDManager];
  _epar = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _rsb = (RPNStackBox *)components[UI_COMP_RPNBox];
  _clb = (CommandLine *)components[UI_COMP_CommandLine];
  _mbox = (MessageBox *)components[UI_COMP_MessageBox];
  _rpn = (RPNCalculator *)components[UI_COMP_RPNCalculator];
  _trm = (TerminalBox *)components[UI_COMP_TerminalBox];
  _io_buffer = _iom->getIOBuffer();
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
  _mbox->setLabel(BAS_StatusMessage, false);
  _mbox->show();
  _clb->show();
  redraw();
}

//
// Redraws the text area on LCD
//
void BasicConsole::redraw() {
//  byte lineNums[] = {6, 4, 2, 0};
  _sdm->checkSDStatus();
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
//    size_t len = _epar->numberParser.stringValue(_vars->rpnStack(i), _io_buffer)-_io_buffer;
//    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
//    _lcd->cursorTo( SCR_RIGHT-len, j);
//    _lcd->sendString( _io_buffer);
//  }
  _mbox->redraw();
  _clb->redraw();
  _lcd->redraw();
}

//
// Draws the line to the serial ports
//
void BasicConsole::updateIOM( bool refresh) {
//  if( !refresh) return;
//  _iom->sendLn();
//  for( byte i=3; i>0; i--){
//    _iom->sendStringLn( _messages[i]);
//    size_t len = _epar->numberParser.stringValue(_vars->rpnStack(i-1), _io_buffer)-_io_buffer;
//    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
//    for( byte j=0; j<SCR_RIGHT-len; j++) _iom->sendChar( ' ');
//    _iom->sendStringUTF8Ln();
//  }
//  _iom->sendStringLn( _messages[0]);
  _clb->updateIOM();
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
      return;
    case _LF_:
    case _CR_:
      processInput(false);
      return;
    case _UP_:
      _clb->copyFromPrevious();
      _clb->updateIOM();
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
      _clb->processESC();
      return;
    default: // other chars go to command line
      _clb->sendChar(c);
      break;
  }
}

//
// Silent command execution
//
void BasicConsole::processCommand(byte c){
  if(_clb->isInputEmpty()){
    switch(c){
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
      _clb->processEntry(c);
      break;
    }
    return;
  }
  _epar->parse(_clb->getInput());  
  if( _epar->result != _RESULT_UNDEFINED_)
    _vars->rpnPUSH(_epar->numberParser.realValue());
  _clb->clearInput();
  expectCommand = false;
  processCommand( c);
}

//
// Parses line and performs operation
//
void BasicConsole::processInput( bool silent) {
  #ifdef __DEBUG
  Serial.print("Processing Input: [");
  Serial.print( (char *)_clb->getInput());
  Serial.println("]");
  #else
  _iom->sendLn();
  #endif
  _clb->copyToPrevious();
  _epar->parse(_clb->getInput()); 
  switch(_epar->result){
    case _RESULT_STRING_:
      if( _epar->lastMathFunction == NULL){
        _evaluateString();
        return; 
      }
      _evaluateCommand();
      break;
    case _RESULT_UNDEFINED_:
      // TODO: Message
      Serial.println("Console: Result is NAN");
      break;
    default:
      _vars->rpnPUSH(_epar->numberParser.realValue());
      if( !silent){
        _rsb->setStackRedrawAll();
        updateIOM();
      }
      break;
  }
  _clb->clearInput();
}

//
// Stack operations
// 
void BasicConsole::push(bool refresh) {
  _vars->rpnSavePreviousX();
  _vars->rpnPUSH();
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::pop(bool refresh) {
  _vars->rpnSavePreviousX();
  _vars->rpnPOP();
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::swap(bool refresh) {
  _rpn->swap(false);
  updateIOM(refresh);
}
void BasicConsole::roll(bool refresh) {
  _vars->rpnPUSH(_vars->rpnGetStack(RPN_STACK-1));
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::prev(bool refresh) {
  _vars->rpnPUSH(_vars->rpnGetPreviousX());
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::add(bool refresh) {
  _savePopAndUpdate( _vars->rpnGetStack(1) + _vars->rpnGetStack(), refresh);
}
void BasicConsole::subtract(bool refresh) {
  _savePopAndUpdate( _vars->rpnGetStack(1) - _vars->rpnGetStack(), refresh);
}
void BasicConsole::multiply(bool refresh) {
  _savePopAndUpdate( _vars->rpnGetStack(1) * _vars->rpnGetStack(), refresh);
}
void BasicConsole::divide(bool refresh) {
  if( abs(_vars->rpnGetStack()) < 1e-300){
    _mbox->setLabel(BAS_Error_DivZero);
    _setRedrawAndUpdateIOM( refresh);
    return;    
  }
  _savePopAndUpdate( _vars->rpnGetStack(1) / _vars->rpnGetStack(), refresh);
}
void BasicConsole::signchange(bool refresh) {
  _vars->rpnSavePreviousX();
  _vars->rpnSetStack( -_vars->rpnGetStack());
  _setRedrawAndUpdateIOM( refresh);
}
void BasicConsole::power(bool refresh) {
  _vars->rpnSavePreviousX(1);
  //_messages
  // power zero: using "1 convention"  
  if( _vars->rpnGetStack() == 0.0){
    _mbox->setLabel( BAS_Warning_ZeroPowerZero);
    _popPartial( 1.0);
    updateIOM(refresh);
    return;
  }
  // positive power of zero: zero
  if( _vars->rpnGetStack() > 0.0 && _vars->rpnGetStack(1) == 0.0){
    _popPartial( 0.0);
    updateIOM(refresh);
    return;
  }
  // negative power of zero: div by zero
  if( _vars->rpnGetStack() < 0.0 && _vars->rpnIsZero(1)){
    _mbox->setLabel(BAS_Error_DivZero);
    _setRedrawAndUpdateIOM( refresh);
    return;
  }
  double tmp = pow( _vars->rpnGetStack(1), _vars->rpnGetStack());
  if( isnan(tmp)){
    _mbox->setLabel( BAS_Error_NAN);
    _setRedrawAndUpdateIOM( refresh);
    return;    
  }
  _popPartial( tmp);
  updateIOM(refresh);
}

//
// If the number is large, periodic functions are useless
//
void BasicConsole::_checkTrigAccuracy(){
  double tmp = abs( _vars->getConvertedAngle());
  if( tmp <= 1e+16) return;
  _mbox->setLabel( BAS_Warning_Accuracy);
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
// Process a command, such as "sin" without parameters 
//
void BasicConsole::_evaluateCommand(){
//  #ifdef __DEBUG
//  Serial.println((char *)_epar->lastMathFunction->name0);
//  #endif
//  bool doPopPartial = true;
//  bool doUpdateIOM = true;
//  double *return_ptr;
//  _vars->savePrevRPN();
//  switch(_epar->lastMathFunction->RPNtag){
//    case _RPN_AMODE_:
//      _funs->Compute( _epar->lastMathFunction, _funs->rpnStack);
//      _rpn->setRPNLabel( 0, BAS_AMOD_Table[_funs->angleMode]);
//      pop(true);
//      return;
//    case _RPN_CHECK_TRIG_:
//      _checkTrigAccuracy();
//      break;
//    case _RPN_INVTRIG_:
//      if( abs(_vars->rpnStack()) > 1.0){
//        _rpn->setRPNLabel( 0, BAS_Error_Trig);
//        _stackRedrawRequired[ 0] = true;
//        _updateIOM(doUpdateIOM);
//        return;
//      }
//      break;
//    case _RPN_DIV0_CHECK_:
//      if( _isStZero(0)){
//        _rpn->setRPNLabel( 0, BAS_Error_DivZero);
//        doPopPartial = false;
//      }
//      break;
//    case _RPN_ROOTYX_:
//      if( _isStZero(0)){
//        _rpn->setRPNLabel( 0, BAS_Error_DivZero);
//        doPopPartial = false;
//        break;
//      }
//      _vars->setStack( 1.0 / _vars->rpnStack());
//      // fall-through!
//    case _RPN_POWER_:
//      power(true);
//      return;
//    case _RPN_SWAP_ONLY_:
//      swap(true);
//      return;
//    case _RPN_SWAP_XY_:
//      _vars->savePrevRPN(1);
//      _vars->rpnSWAP();
//      break;
//    case _RPN_QUICK_PUSH_:
//      _vars->rpnPUSH();
//      break;
//    case _RPN_SQRT_CHECK_:
//      if( _vars->rpnStack() < 0.0){
//        _rpn->setRPNLabel( 0, BAS_Message_Complex);
//        _vars->setStack( -_vars->rpnStack());
//      }      
//      break;
//    case _RPN_QUAD_SOLVER:
//      quad(true);
//      _setRedrawAndUpdateIOM( doUpdateIOM);
//      return;
//    case _RPN_GOFF2_SOLVER:
//      goff2(true);
//      _setRedrawAndUpdateIOM( doUpdateIOM);
//      return;
//    default:
//      break;
//  }
//  return_ptr = _funs->Compute( _epar->lastMathFunction, _funs->rpnStack);
//  if( _epar->lastMathFunction->nArgs > 0) _vars->setStack(return_ptr[0]);
//  if( doPopPartial && _epar->lastMathFunction->nArgs > 1) _popPartial();
//  else _rsb->setStackRedraw();
//  _updateIOM(doUpdateIOM);
}

//
// Process other string commands, such as "roll" 
//
void BasicConsole::_evaluateString(){
  byte *ptr;
  if( IsToken( _epar->nameParser.Name(), "cls", false)){
    // TODO: add screen clear
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "push", false)){
    push();
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "pop", false)){
    pop();
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "roll", false)){
    roll();
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "hex", false)){
    ptr = _clb->getInput();
    _epar->numberParser.stringHex( _vars->rpnGetStack(), ptr);
    _clb->processEND();
    _iom->sendStringLn( ptr);
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "inj", false)){
    _epar->numberParser.stringValue( _vars->rpnGetStack(), _io_buffer);
    _iom->injectKeyboard();
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "rpn", false)){
    _clb->copyToPrevious();
    _clb->clearInput();
    nextUI = UI_RPNCALC;
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "fman", false)){
    _clb->copyToPrevious();
    _clb->clearInput();
    nextUI = UI_FILEMAN;
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr prompt ", false)){
    _clb->copyToPrevious();
    _mbox->setLabel(_clb->getInput(12));
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr labelx ", false)){
    _clb->copyToPrevious();
    _rpn->setRPNLabel( 0, _clb->getInput(12));
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr labely ", false)){
    _clb->copyToPrevious();
    _rpn->setRPNLabel( 1, _clb->getInput(12));
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr labelz ", false)){
    _clb->copyToPrevious();
    _rpn->setRPNLabel( 2, _clb->getInput(12));
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr off", false)){
    _mbox->report(MB_MESSAGE_LCD_Off);
    _lcd->sleepOn();
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr on", false)){
    _mbox->report(MB_MESSAGE_LCD_On);
    _lcd->sleepOff();
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr+", false)){
    _lcd->changeLED( 16);
    _mbox->report_LCDBrightness( _lcd->ledBrightness);
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr-", false)){
    _lcd->changeLED( -16);
    _mbox->report_LCDBrightness( _lcd->ledBrightness);
    _clb->clearInput();
    return;
  }
  Serial.println("Unknown command:");
  Serial.println((char *)_epar->_getCurrentPosition());
}

void BasicConsole::_popPartial() {
  _vars->rpnPOP(2);
  _rsb->setStackRedrawAll();
}
void BasicConsole::_popPartial( double v) {
  _popPartial();
  _vars->rpnSetStack(v);
}
