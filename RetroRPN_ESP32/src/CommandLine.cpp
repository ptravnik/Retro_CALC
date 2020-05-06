//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "CommandLine.hpp"

//#define __DEBUG
const char CLI_Prompt[] PROGMEM = "> ";

//
// Inits command line interface
//
unsigned long CommandLine::init(IOManager *iom, LCDManager *lcd){
  _io_buffer = iom->getIOBuffer();
  _iom = iom;
  _lcd = lcd;
  memset(_input, (byte)0, INPUT_COLS);
  memset(_inputPrevious, (byte)0, INPUT_COLS);
  return _iom->keepAwake();
}

unsigned long CommandLine::tick(){
  char c = _iom->input();
  if(c != _NUL_)
    sendChar((byte)c);
  return _iom->lastInput;
}

void CommandLine::show(){
  _iom->sendLn();
}

//
// Redraws the number area on LCD
//
void CommandLine::redraw() {
  _lcd->cursorToBottom();
  _lcd->clearLine( SCR_BOTTOM);
  _lcd->sendStringUTF8( CLI_Prompt);
  _lcd->sendString( getInputDisplay(), HSCROLL_LIMIT);
  _lcd->cursorTo( cursor_column - display_starts + 2, SCR_BOTTOM);
  _lcd->redraw();
}

//
// Draws the command line to the serial ports
//
void CommandLine::updateIOM( bool refresh) {
  if( !refresh) return;
  _iom->sendLn();
  _iom->sendStringUTF8( CLI_Prompt);
  _iom->sendStringUTF8( _input);
}

//
// Sends one byte
//
void CommandLine::sendChar( byte c) {
  switch(c){
    case _BS_:
      processBS();
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
// process character entry
//
void CommandLine::processEntry(byte c) {
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
void CommandLine::processDEL() {
  byte *ptr = _input+cursor_column;
  if( !(*ptr)) return; // end of string - nothing to delete
  memmove( ptr, ptr+1, INPUT_LIMIT-cursor_column);
}

//
// process backspace - the same as pressing LEFT and DEL 
//
void CommandLine::processBS() {
  if( !cursor_column) return; // nothing to delete
  processLEFT();
  processDEL();
}

//
// process other buttons 
//
void CommandLine::processESC() {
  clearInput();
}

void CommandLine::processLEFT() {
  if( !cursor_column) return; // no place like home
  cursor_column--;
  if( display_starts>cursor_column)
    display_starts=cursor_column;
}

void CommandLine::processRIGHT() {
  byte *ptr = _input+cursor_column;
  if( !(*ptr)) return; // end of string - no place to move
  cursor_column++;
  if( cursor_column-display_starts >= HSCROLL_LIMIT)
    display_starts++;
}

void CommandLine::processHOME() {
  cursor_column = 0;
  display_starts = 0;
}

void CommandLine::processEND() {
  cursor_column = strlen( _input);
  if( cursor_column > HSCROLL_LIMIT)
    display_starts = cursor_column - HSCROLL_LIMIT;
}

//
// Copies command from previous storage 
//
void CommandLine::copyFromPrevious(){
  strcpy( (char *)_input, (char *)_inputPrevious);
  processEND();
}

//
// Removes input contents 
//
void CommandLine::clearInput() {
  *_input = 0;
  cursor_column = 0;
  display_starts = 0;
}

//
// Special check to return true if the RPN enters e power
//
bool CommandLine::isMagnitudeEntry() {
  if( cursor_column < 2) return false;
  if( getInputChar(cursor_column-1) != 'e') return false;
  return IsDigitOrDecimal( getInputChar(cursor_column-2));
}