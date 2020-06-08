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
  _lex = (Lexer *)components[UI_COMP_Lexer];
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
  switch(c){
    // _LF_ and _RPN are ignored
    case _LF_:
      Serial.println("Ignore _LF_");
      return;
    case _RPN_:
      Serial.println("Ignore _RPN_");
      return;
    case _CR_:
      Serial.println("Processing input!");
      processInput(false);
      return;
    case _UP_:
      // Here should be navigation through the terminal screen
      _clb->copyFromPrevious();
      _clb->updateIOM();
      return;
    case _DOWN_:
      // Here should be navigation through the terminal screen
      return;
    case _PGUP_:
      // Here should be navigation through the terminal screen
      return;
    case _PGDN_:
      // Here should be navigation through the terminal screen
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
  _lex->parse(_clb->getInput()); 
  switch(_lex->result){
    case _RESULT_STRING_:
      Serial.println("Console: String");
      if( _epar->lastMathFunction != NULL) break;
      _evaluateString();
      return; 
    case _RESULT_EXECUTED_:
      Serial.println("Console: Executed");
      break;
    case _RESULT_UNDEFINED_:
      // TODO: Message
      Serial.println("Console: Result is Undefined");
      break;
    default:
      Serial.println("Console: Push result to stack");
      _vars->pushRPNStack(_epar->numberParser.realValue());
      if( !silent){
        _rsb->setStackRedrawAll();
        updateIOM();
      }
      break;
  }
  Serial.println("Console: Clear input");
  _clb->clearInput();
}

//
// Process other string commands, such as "hex" 
// This is a kludge! TODO 
//
void BasicConsole::_evaluateString(){
  byte *ptr;
  if( IsToken( _epar->nameParser.Name(), "cls", false)){
    // TODO: add screen clear
    _clb->clearInput();
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
  Serial.println("Unknown command:");
  Serial.println((char *)_epar->_getCurrentPosition());
}
