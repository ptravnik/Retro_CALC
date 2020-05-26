//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "TerminalBox.hpp"

//#define __DEBUG

//
// Inits calculator
//
unsigned long TerminalBox::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _epar = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _io_buffer = _iom->getIOBuffer();
  _vs_input_column = 0;
  _virtualScreen[0] = _NUL_;
  return _iom->keepAwake();
}

void TerminalBox::show(){
  _lcd->wordWrap = false;
  _lcd->scrollLock = true;
  _lcd->clearScreen( _SP_, false);
}

//
// Redraws the number area on LCD
//
void TerminalBox::redraw() {
  for( int8_t i=0; i<6; i++){
    _lcd->cursorTo( 1, 6);
//      _lcd->sendString( _messages[i]);
//      _lcd->clearToEOL();
//    }
//    if( _stackRedrawRequired[i]){
//      _stackRedrawRequired[i] = false;
//      _lcd->clearLine( rpnNums[i]);
//      size_t len = _epar->numberParser.stringValue(_getSt(i), _io_buffer)-_io_buffer;
//      if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
//      _lcd->cursorTo( SCR_RIGHT-len, rpnNums[i]);
//      _lcd->sendString( _io_buffer);
//    }
  }
}

//
// Draws the stack to the serial ports
//
void TerminalBox::updateIOM( bool refresh) {
  if( !refresh) return;
  _iom->sendLn();
//  for( int8_t i=2; i>=0; i--){
//    _iom->sendStringLn( _messages[i]);
//    size_t len = _epar->numberParser.stringValue( _getSt(i), _io_buffer)-_io_buffer;
//    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
//    for( byte j=0; j<SCR_RIGHT-len; j++) _iom->sendChar( ' ');
//    _iom->sendStringUTF8Ln();
//  }
}

////
//// Resets all calculator labels
////
//void TerminalBox::resetRPNLabels() {
//  for( byte i=0; i<3; i++){
//    convertToCP1251( _messages[i], RPN_Message_Table[i], HSCROLL_LIMIT);
//    _messageRedrawRequired[i] = true;
//  }
//}
//
////
//// Sets one label: 0 - X, 1 - Y, 2 - Z
////
//void TerminalBox::setRPNLabel( byte label, byte *message) {
//  if( label > 2) label = 0;
//  if( message)
//    strncpy( (char *)_messages[label], (char *)message, HSCROLL_LIMIT);  
//  else
//    convertToCP1251( _messages[label], RPN_Message_Table[label], HSCROLL_LIMIT);
//  _messageRedrawRequired[label] = true;
//}

void TerminalBox::sendChar( byte c, byte dest, bool wait_for_host){
  if(c == _CR_){
    byte *ptr1 = _virtualScreen + VIRTUAL_SCREEN_END1;
    byte *ptr2 = _virtualScreen + VIRTUAL_SCREEN_END2;
    for( size_t i=0; i<VIRTUAL_SCREEN_END2; i++, ptr1--, ptr2--)
      *ptr1 = *ptr2;
    _vs_input_column = 0;
    _virtualScreen[_vs_input_column] = _NUL_;
    return;
  }
  if( _vs_input_column < VIRTUAL_SCREEN_COLS){
    _virtualScreen[_vs_input_column++] = c;
    _virtualScreen[_vs_input_column] = _NUL_;
  }
  _iom->sendChar( c, SERIALS_BOTH, wait_for_host);
}
