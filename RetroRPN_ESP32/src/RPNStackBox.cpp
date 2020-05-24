//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "RPNStackBox.hpp"

//#define __DEBUG

const char RPN_RegName1[] PROGMEM = "x:";
const char RPN_RegName2[] PROGMEM = "y:";
const char RPN_RegName3[] PROGMEM = "z:";
const char *const RPN_Message_Table[] PROGMEM = {
  RPN_RegName1,
  RPN_RegName2,
  RPN_RegName3,
  };

//
// Inits calculator
//
unsigned long RPNStackBox::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _vars = (Variables *)components[UI_COMP_Variables];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _ep = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _io_buffer = _iom->getIOBuffer();
  _messages[0] = _vars->rpnLabelX;
  _messages[1] = _vars->rpnLabelY;
  _messages[2] = _vars->rpnLabelZ;
  resetRPNLabels();
  setStackRedraw();
  return _iom->keepAwake();
}

void RPNStackBox::show(){
  _lcd->wordWrap = false;
  _lcd->scrollLock = true;
  _lcd->clearScreen( _SP_, false);
  setStackRedraw();
  setMessageRedraw();
}

//
// Redraws the number area on LCD
//
void RPNStackBox::redraw() {
  byte messageNums[] = {4, 2, 0};
  byte rpnNums[] = {5, 3, 1};
  for(byte i=0; i<3; i++){
    if( _messageRedrawRequired[i]){
      _messageRedrawRequired[i] = false;
      _lcd->cursorTo( 1, messageNums[i]);
      _lcd->sendString( _messages[i]);
      _lcd->clearToEOL();
    }
    if( _stackRedrawRequired[i]){
      _stackRedrawRequired[i] = false;
      _lcd->clearLine( rpnNums[i]);
      size_t len = _ep->numberParser.stringValue(_vars->rpnGetStack(i), _io_buffer)-_io_buffer;
      if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
      _lcd->cursorTo( SCR_RIGHT-len, rpnNums[i]);
      _lcd->sendString( _io_buffer);
    }
  }
}

//
// Draws the stack to the serial ports
//
void RPNStackBox::updateIOM( bool refresh) {
  if( !refresh) return;
  _iom->sendLn();
  for( int8_t i=2; i>=0; i--){
    _iom->sendStringLn( _messages[i]);
    size_t len = _ep->numberParser.stringValue( _vars->rpnGetStack(i), _io_buffer)-_io_buffer;
    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
    for( byte j=0; j<SCR_RIGHT-len; j++) _iom->sendChar( ' ');
    _iom->sendStringUTF8Ln();
  }
}

//
// Resets all calculator labels
//
void RPNStackBox::resetRPNLabels() {
  for( byte i=0; i<3; i++){
    convertToCP1251( _messages[i], RPN_Message_Table[i], HSCROLL_LIMIT);
    _messageRedrawRequired[i] = true;
  }
}

//
// Sets one label: 0 - X, 1 - Y, 2 - Z
//
void RPNStackBox::setRPNLabel( byte label, byte *message) {
  if( label > 2) label = 0;
  if( message)
    strncpy( (char *)_messages[label], (char *)message, HSCROLL_LIMIT);  
  else
    convertToCP1251( _messages[label], RPN_Message_Table[label], HSCROLL_LIMIT);
  _messageRedrawRequired[label] = true;
}
