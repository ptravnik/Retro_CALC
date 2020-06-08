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

const char RPN_Error_NAN[] PROGMEM = "Err: NaN";
const char RPN_Error_Unknown[] PROGMEM = "Unknown:";
//const char RPN_Message_Complex[] PROGMEM = "Complex: \xb1i";
 
//
// Inits calculator
//
unsigned long RPNCalculator::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _vars = (Variables *)components[UI_COMP_Variables];
  _funs = (Functions *)components[UI_COMP_Functions];
  _epar = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _lex = (Lexer *)components[UI_COMP_Lexer];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _sdm = (SDManager *)components[UI_COMP_SDManager];
  _rsb = (RPNStackBox *)components[UI_COMP_RPNBox];
  _mbox = (MessageBox *)components[UI_COMP_MessageBox];
  _clb = (CommandLine *)components[UI_COMP_CommandLine];
  _io_buffer = _iom->getIOBuffer();
  _rsb->resetRPNLabels();
  loadState();
  _rsb->setStackRedrawAll();
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
  _mbox->setLabel(RPN_StatusMessage, false);
  _mbox->show();
  _clb->show();
}

//
// Redraws the number area on LCD
//
void RPNCalculator::redraw() {
  _sdm->checkSDStatus();
  _rsb->redraw();
  _mbox->redraw();
  _clb->redraw();
  _lcd->redraw();
}

//
// Draws the stack to the serial ports
//
void RPNCalculator::updateIOM( bool refresh) {
  if( !refresh) return;
  _rsb->updateIOM(true);
  _mbox->updateIOM(true);
  _clb->showPrompt();
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
      #ifdef __DEBUG
      Serial.println("_RPN_ received");
      #endif
      expectCommand = true;
      return;
    case _LF_:
      #ifdef __DEBUG
      Serial.println("_LF_ pressed");
      #endif
      processInput(true);
      return;
    case _CR_:
      #ifdef __DEBUG
      Serial.println("_CR_ pressed");
      #endif
      processInput(false);
      return;
    case _UP_:
      // TODO: lexer prev      
      _clb->copyFromPrevious();
      _clb->updateIOM();
      return;
    case _DOWN_:
      _lex->processRPNKeywordByID( _OPR_SWAP_KW);
      return;
    case _PGUP_:
      //_lex->processRPNKeywordByID( _OPR_PUSH_KW); //PUSH is an operator, not function
      _vars->saveRPNPrev();
      _vars->pushRPNStack( _vars->getRPNPrev());
      _rsb->setStackRedrawAll();
      return;
    case _PGDN_:
      _lex->processRPNKeywordByID( _OPR_ROLL_KW);
      return;
    case _ESC_:
      _clb->processESC();
      _mbox->setLabel(RPN_StatusMessage, false);
      _rsb->resetRPNLabels();
      updateIOM(true);
      return;
    default: // other chars go to command line
      _clb->sendChar(c);
      break;
  }
}

//
// Silent command execution
//
void RPNCalculator::_processCommand(byte c){
  if(_clb->isInputEmpty()){
    switch(c){
      case '+':
        _lex->processRPNKeywordByID( _OPR_ADD_KW);
        return;
      case '-':
        _lex->processRPNKeywordByID( _OPR_SUB_KW);
        return;
      case '*':
        _lex->processRPNKeywordByID( _OPR_MUL_KW);
        return;
      case '/':
        _lex->processRPNKeywordByID( _OPR_DIV_KW);
        return;
      case '^':
        _lex->processRPNKeywordByID( _OPR_POW_KW);
        return;
      case _PLSMNS_:
        _lex->processRPNKeywordByID( _OPR_NEG_KW);
        return;
    default:
      _clb->processEntry(c);
      break;
    }
    return;
  }
  switch(c){
    case '+':
    case '-':
      if( _clb->isMagnitudeEntry()){
        _clb->processEntry(c);
        return;
      }
      break;
    default:
      break;
  }
  _lex->parse(_clb->getInput());
  switch(_lex->result){
    case _RESULT_INTEGER_  :
    case _RESULT_REAL_     :
      _vars->pushRPNStack(_epar->numberParser.realValue());
    default:
      _clb->clearInput();
      _processCommand( c);
      break;
  } 
}

//
// Parses line and performs operation
//
void RPNCalculator::processInput( bool silent) {
  #ifdef __DEBUG
  if( silent) Serial.print( "Silently p");
  else Serial.print( "P");
  Serial.print("rocessing input: [");
  Serial.print( (char *)_clb->getInput());
  Serial.println("]");
  #else
  _iom->sendLn();
  #endif
  if( !silent && _clb->isInputEmpty()){
    #ifdef __DEBUG
    Serial.println("_CR_ pressed for _PUSH_");
    #endif
    _vars->pushRPNStack();
    _rsb->setStackRedrawAll();
    _rsb->updateIOM();
    return;
  }
  _clb->copyToPrevious();
  _lex->parse(_clb->getInput()); 
  switch(_lex->result){
    case _RESULT_INTEGER_:
    case _RESULT_REAL_:
      #ifdef __DEBUG
      Serial.println("Numeric result");
      #endif
      _vars->pushRPNStack(_epar->numberParser.realValue());
      break;
    case _RESULT_STRING_:
      #ifdef __DEBUG
      Serial.println("String result");
      #endif
      if( _epar->lastMathFunction != NULL) break;
      _evaluateString();
      return; 
    case _RESULT_EXECUTED_:
      #ifdef __DEBUG
      Serial.println("Executed result");
      #endif
      break;
    case _RESULT_UNDEFINED_:
      #ifdef __DEBUG
      Serial.println("Undefined result");
      #endif
      //_mbox->setLabel( RPN_StatusMessage, true);
      //_mbox->setLabel( RPN_Error_NAN, true);
      return;
    default:
      break;
  }
  _clb->clearInput();
  if( !silent){
    _rsb->setStackRedrawAll();
    updateIOM();
  }
}

//
// TODO: load and save functionality here
//
void RPNCalculator::loadState(){
  if( !_sdm->SDMounted) return;
  _sdm->readRPNStatus(_clb->_input, _clb->_inputPrevious, &(_clb->cursor_column));
  #ifdef __DEBUG
  Serial.print("loaded: [");
  Serial.print( (char*)_clb->_input);
  Serial.println("]");
  #endif
}

void RPNCalculator::saveState(){
  if( !_sdm->SDMounted) return;
  _sdm->writeRPNStatus(_clb->_input, _clb->_inputPrevious, _clb->cursor_column);
  //_clb->
  //  uint16_t _clb->cursor_column = 0;
  //  uint16_t _clb->display_starts = 0;
  //  byte _clb->_input[INPUT_COLS];
  //  byte _clb->_inputPrevious[INPUT_COLS];
}

//
// Process sume unimplemented string commands, such as "hex"
// This is a kludge! TODO 
//
void RPNCalculator::_evaluateString(){
  byte *ptr;
  if( IsToken( _epar->nameParser.Name(), "cls", false)){
    _rsb->resetRPNLabels();
    _clb->clearInput();
    updateIOM(true);
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "hex", false)){
    ptr = _clb->getInput();
    _epar->numberParser.stringHex( _vars->getRPNRegister(), ptr);
    _clb->processEND();
    _iom->sendStringLn( ptr);
    return;
  }
  if( IsToken( _epar->nameParser.Name(), "inj", false)){
    _epar->numberParser.stringValue( _vars->getRPNRegister(), _io_buffer);
    _iom->injectKeyboard();
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr prompt ", false)){
    _clb->copyToPrevious();
    _mbox->setLabel( _clb->getInput(12));
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr labelx ", false)){
    _clb->copyToPrevious();
    _rsb->setRPNLabel( 0, _clb->getInput(12));
    updateIOM(true);
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr labely ", false)){
    _clb->copyToPrevious();
    _rsb->setRPNLabel( 1, _clb->getInput(12));
    updateIOM(true);
    _clb->clearInput();
    return;
  }
  if( IsToken( _epar->_getCurrentPosition(), "#scr labelz ", false)){
    _clb->copyToPrevious();
    _rsb->setRPNLabel( 2, _clb->getInput(12));
    updateIOM(true);
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
  _mbox->setLabel( RPN_Error_Unknown, true);  
}
