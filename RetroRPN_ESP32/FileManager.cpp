//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "FileManager.hpp"
#include "./src/Keywords.hpp"
#include "./src/IOManager.hpp"
#include "./src/LCDManager.hpp"
#include "SDManager.hpp"

//#define __DEBUG

//const char RPN_StatusMessage[] PROGMEM = "RPN Ready";
//const char RPN_RegName1[] PROGMEM = "x:";
//const char RPN_RegName2[] PROGMEM = "y:";
//const char RPN_RegName3[] PROGMEM = "z:";
//const char RPN_Prompt[] PROGMEM = "> ";
//const char RPN_Error_DivZero[] PROGMEM = "Err: div 0";
//const char RPN_Error_NAN[] PROGMEM = "Err: NaN";
//const char RPN_Error_Trig[] PROGMEM = "Err: |X|>1";
//const char RPN_Warning_ZeroPowerZero[] PROGMEM = "Warn: 0^0";
//const char RPN_Warning_Accuracy[] PROGMEM = "Warn: Inaccurate!";
//const char RPN_Mode_Degrees[] PROGMEM = "Mode: Degrees";
//const char RPN_Mode_Radians[] PROGMEM = "Mode: Radians";
//const char RPN_Mode_Gradians[] PROGMEM = "Mode: Gradians";
//const char RPN_Message_Complex[] PROGMEM = "Complex: \xb1i";
//const char RPN_Message_Trivial[] PROGMEM = "Trivial";
//const char RPN_Message_NoRoots[] PROGMEM = "No roots";
//const char RPN_Message_OneRoot[] PROGMEM = "Single root";
//const char RPN_Message_TwoRoots[] PROGMEM = "Two roots";
//const char RPN_Message_ComplexRoots[] PROGMEM = "Complex: X\xb1iY";
//const char RPN_Message_Discriminant[] PROGMEM = "Discriminant";
//const char RPN_Message_Root1[] PROGMEM = "Root 1";
//const char RPN_Message_Root2[] PROGMEM = "Root 2";
//const char RPN_Message_ComplexPart[] PROGMEM = "Complex part";
//const char RPN_Message_RealPart[] PROGMEM = "Real part";
//const char RPN_Message_Gain[] PROGMEM = "Gain";
//const char RPN_Message_Offset[] PROGMEM = "Offset";
//const char RPN_Message_Goff_Solution[] PROGMEM = "Y=Gain*X+Offset";
//const char *const RPN_Message_Table[] PROGMEM = {
//  RPN_StatusMessage,
//  RPN_RegName1,
//  RPN_RegName2,
//  RPN_RegName3,
//  RPN_Prompt
//  };
//const char *const RPN_AMOD_Table[] PROGMEM = {
//  RPN_Mode_Degrees,
//  RPN_Mode_Radians,
//  RPN_Mode_Gradians
//  };

//
// Inits File Manager 
//
unsigned long FileManager::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _lcd = (LCDManager *)components[UI_COMP_LCDManager];
  _sd = (SDManager *)components[UI_COMP_SDManager];
  _ep = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _io_buffer = _iom->getIOBuffer();

//  _messages[0] = _messageBuffer;
//  _messages[1] = _messages[0] + SCR_COLS;
//  _messages[2] = _messages[1] + SCR_COLS;
//  _messages[3] = _messages[2] + SCR_COLS;
//  resetRPNLabels(false);
//  memset(_input, (byte)0, INPUT_COLS);
//  memset(_inputPrevious, (byte)0, INPUT_COLS);
//  loadState();
//  setStackRedraw();
  return _iom->keepAwake();
}

unsigned long FileManager::tick(){
  char c = _iom->input();
  if(c != _NUL_)
    sendChar((byte)c);
  return _iom->lastInput;
}

void FileManager::show(){
  _lcd->wordWrap = false;
  _lcd->scrollLock = true;
  _lcd->clearScreen( _SP_, true);
  for(byte r=0; r<8; r++){
    _lcd->cursorTo( 0, r);
    _lcd->sendString( "File Manager!");
  }  
  //_lcd->invertRow(6, true);
  //setStackRedraw();
  //resetRPNLabels( false);
  _iom->sendLn();
  _iom->sendStringLn( "File Manager here");
  _lcd->redraw();
  _sd->listDir();
  delay(2000);
  nextUI = UI_RPNCALC;
}

//
// Redraws the File area on LCD
//
void FileManager::redraw() {
//  byte lineNums[] = {6, 4, 2, 0}; 
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
//    size_t len = _ep->numberParser.stringValue(rpnStack[i], _io_buffer)-_io_buffer;
//    if( len >= SCR_RIGHT) len = SCR_RIGHT-1;    
//    _lcd->cursorTo( SCR_RIGHT-len, j);
//    _lcd->sendString( _io_buffer);
//  }
//  byte *ptr = _input + display_starts;
//  _lcd->cursorToBottom();
//  _lcd->clearLine( SCR_BOTTOM);
//  _lcd->sendStringUTF8( RPN_Message_Table[4]);
//  _lcd->sendString( ptr, HSCROLL_LIMIT);
//  _lcd->cursorTo( cursor_column - display_starts + 2, SCR_BOTTOM);
//  _lcd->redraw();
}

//
// Sends one byte
//
void FileManager::sendChar( byte c) {
//  if( expectCommand){
//    processCommand(c);
//    expectCommand = false;
//    return;
//  }
//  switch(c){
//    case _BS_:
//      processBS();
//      return;
//    case _RPN_:
//      expectCommand = true;
//      return;
//    case _LF_:
//      processInput(true);
//      return;
//    case _CR_:
//      processInput(false);
//      return;
//    case _LEFT_:
//      processLEFT();
//      return;
//    case _RIGHT_:
//      processRIGHT();
//      return;
//    case _HOME_:
//      processHOME();
//      return;
//    case _END_:
//      processEND();
//      return;
//    case _UP_:
//      copyFromPrevious();
//      return;
//    case _DOWN_:
//      swap();
//      return;
//    case _PGUP_:
//      prev();
//      return;
//    case _PGDN_:
//      roll();
//      return;
//    case _ESC_:
//      processESC();
//      return;
//    case _DEL_:
//      processDEL();
//      return;
//    default: // other chars
//      processEntry(c);
//      break;
//  }
}
