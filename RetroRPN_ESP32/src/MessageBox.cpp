//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "MessageBox.hpp"

//#define __DEBUG

const char MB_StatusMessage[] PROGMEM = "MK-90 Ready";
const char MB_LCDOn_Message[] PROGMEM = "LCD On";
const char MB_LCDOff_Message[] PROGMEM = "LCD Off";
const char MB_LCDBrightness_Message[] PROGMEM = "LCD = ";

const char *const MB_Message_Table[] PROGMEM = {
  MB_LCDOn_Message,
  MB_LCDOff_Message,
  };

//
// Inits message box
//
unsigned long MessageBox::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _vars = (Variables *)components[UI_COMP_Variables];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _messageBuffer = _vars->scrMessage;
  setLabel( (byte *)NULL, false);
  return _iom->keepAwake();
}

void MessageBox::show(){
  _lcd->invertRow(6, true);
  _messageRedrawRequired = true;
}

//
// Redraws the message area on LCD
//
void MessageBox::redraw() {
  if( !_messageRedrawRequired) return;
  _messageRedrawRequired = false;
  _lcd->cursorTo( 1, 6);
  _lcd->sendString( _messageBuffer);
  _lcd->clearToEOL();
}

//
// Draws the message to the serial ports
//
void MessageBox::updateIOM( bool refresh) {
  if( !refresh) return;
  _iom->sendStringLn( _messageBuffer);
}

//
// Sets message label
//
void MessageBox::setLabel( byte *message, bool refresh) {
  if( message)
    strncpy( (char *)_messageBuffer, (char *)message, HSCROLL_LIMIT);  
  else
    convertToCP1251( _messageBuffer, MB_StatusMessage, HSCROLL_LIMIT);
  _messageRedrawRequired = true;
  updateIOM(refresh);
}
void MessageBox::appendLabel( byte *message, bool refresh) {
  if( !message) return;
  size_t i = strlen( _messageBuffer);
  if( i>= HSCROLL_LIMIT) return; 
  strncpy( (char *)(_messageBuffer+i), (char *)message, HSCROLL_LIMIT-i);  
  _messageRedrawRequired = true;
  updateIOM(refresh);
}

//
// Sends one of the pre-coded messages
//
void MessageBox::report( size_t message, bool refresh){
  setLabel( MB_Message_Table[message], refresh);
}

//
// Reports LCD LED PWM
//
void MessageBox::report_LCDBrightness( byte val, bool refresh){
  char *ptr = (char *)convertToCP1251( _messageBuffer, MB_LCDBrightness_Message, HSCROLL_LIMIT);
  sprintf (ptr-1, "%d", val);
  _messageRedrawRequired = true;
  updateIOM(refresh);
}
