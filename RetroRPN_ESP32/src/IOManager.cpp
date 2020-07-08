//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include <Arduino.h>
#include <HWKbd.h>
#include "CP1251_mod.h"

// Use the appropriate hardware definition
#include "Cyrillic_Phonetic_CP1251.h"
//#include "Cyrillic_Phonetic_CP1251_Mockup.h"
#include "IOManager.hpp"

//#define __DEBUG

static HWKbd hwKeyboard;
static HWKbd_Encoder hwEncoder(
  HWKBD_Codepage_Cyrillic_Phonetic_CP1251,
  HWKBD_Macropage_Cyrillic_Phonetic_CP1251);

//
// Helper object for handling multiple keyboards
//
void CircularBuffer::push( byte c){
  _buffer[_in++] = (char)c;
  if( _in >= CIRCULAR_BUFFER_LENGTH) _in = 0;
  if( _in == _out){ // cought up with the out pointer
    _out++;
    if( _out >= CIRCULAR_BUFFER_LENGTH) _out = 0;
  }
}
char CircularBuffer::pop(){
  if(_in == _out) return _NUL_;
  char c = _buffer[_out++];
  if( _out >= CIRCULAR_BUFFER_LENGTH) _out = 0;
  return c;
}

//
// Main IO object
//
unsigned long IOManager::init( byte *io_buffer) {
  _io_buffer = io_buffer;
  hwKeyboard.connect(HWKBD_RST, HWKBD_CLK, HWKBD_DATA,
                     HWKBDLED_RST, HWKBDLED_CLK, &hwEncoder);
  HWKeyboardConnected = hwKeyboard.isConnected;
  pinMode( IO_PM_ACTIVE, INPUT);
  isAsleep = true;
  sleepOff();
  return keepAwake();
}

//
// Makes routine checks
//
unsigned long IOManager::tick() {
  hwKeyboard.input();
  return lastInput;
}

//
// Operates keyboard LEDs
//
void IOManager::setKBDLEDs( bool L0, bool L1, bool L2, bool L3){
  byte tmp = L0 & (L1<<1) & (L2<<2) & (L3<<3);
  hwKeyboard.setLEDs( tmp);
  keepAwake();
};
void IOManager::setKBDLEDs( byte val){
  hwKeyboard.setLEDs( val);
  keepAwake();
};
void IOManager::flashKBDLEDs(){
  #ifdef __DEBUG
  Serial.println("Writing to LEDs");
  #endif
  for( byte t=1; t<9; t<<=1){
    setKBDLEDs( t);
    delay(50);
  }
  for( byte t=4; t>0; t>>=1){
    setKBDLEDs( t);
    delay(50);
  }
  setKBDLEDs();
  #ifdef __DEBUG
  Serial.println("End writing to LEDs");
  #endif
};

//
// Inputs CP1251 character from either hardware keyboard or serials
// NB:
// The ESP32 Serial is supposed to operate in UTF8 mode
// The ESP32 Serial2 is connected to Pro Micro and sends keys in CP1251 mode
// This is because the hardware keyboard may be moved in the future to Pro Micro
// Both Serial and Serial2 receive in UTF8 mode TODO: check user preferences
//
char IOManager::input(){
  char c;
  if( _input_buffer.available()){
    c = _input_buffer.pop();
    #ifdef __DEBUG
    Serial.print( "Popped deferred code=");
    Serial.println( c, HEX);
    #endif
    keepAwake();
    return c;
  }
  if( hwKeyboard.available()){
    c = hwKeyboard.read();
    #ifdef __DEBUG
    Serial.print( "Hardware KBD: scan=");
    Serial.print( hwKeyboard.lastScanResult, DEC);
    Serial.print( " code=");
    Serial.println( c, HEX);
    #endif
    keepAwake();
    return c;
  }
  if( PM_active() && Serial2.available()){
    c = Serial2.read();
    #ifdef __DEBUG
    Serial.print( "VT100 Serial 2: code=");
    Serial.println( c, HEX);
    #endif
    keepAwake();
    return c;
  }
  if( Serial.available()){
    c = _receiveFromSerial();
    #ifdef __DEBUG
    Serial.print( "VT100 KBD: code=");
    Serial.println( c, HEX);
    #endif
    keepAwake();
    return c;
  }
  return _NUL_;
}

byte IOManager::scan(){
  return hwKeyboard.scan();
}

//
// Converts characters from the serial port
// from UTF8 to CP1251.
//
char IOManager::_receiveFromSerial(){  
  byte c = (byte)Serial.read();
  if( 0 < c && c < 128){
    #ifdef __DEBUG
    // loopback
    Serial.write((char)c);
    #endif
    return c;
  }
  char *ptr = (char *)_io_buffer;
  *ptr++ = (char)c;
  *ptr = _NUL_;
  switch( c){
    case _NUL_:
      return _NUL_;
    case 0xD0: // double character (Russian letters)
    case 0xD1: // double character (Russian letters)
    case 0xC2: // double character (quotations)
      if( !_readNonlocking(ptr)) return _NUL_;
      return _sendToHost();
    case 0xE2: // triple character (Ellipsis and EM-dash)
      if( !_readNonlocking(ptr++)) return _NUL_;
      if( !_readNonlocking(ptr)) return _NUL_;
      return _sendToHost();
    default: // unknown or misplaced Unicode is a star!
      #ifdef __DEBUG
      // loopback
      Serial.write(UNKNOWN_UTF8);
      #endif
      return UNKNOWN_UTF8;
  }
  return _NUL_;
}
bool IOManager::_readNonlocking(char *ptr){
  for( byte i=0; i<50; i++){
    if( !Serial.available()){
      delayMicroseconds(200);
      continue;
    }
    *ptr++ = Serial.read();
    *ptr = _NUL_;
    return true;
  }
  return false;
}
char IOManager::_sendToHost(){
  #ifdef __DEBUG
  // loopback
  Serial.println( (char *)_io_buffer);
  #endif
  #ifdef __DEBUG_CODES
  Serial.print( "From [");
  for( byte i=0; i<5; i++){
    byte c = _UTF8_buffer[i];
    if( c == _NUL_) break;
    Serial.print( c, HEX);    
  }
  Serial.println( "]");
  #endif
  convertToCP1251( _CP1251_buffer, (char *)_io_buffer, 7);
  #ifdef __DEBUG_CODES
  Serial.print( "To [");
  for( byte i=0; i<7; i++){
    byte c = _CP1251_buffer[i];
    if( c == _NUL_) break;
    Serial.print( c, HEX);    
  }
  Serial.println( "]");
  #endif
  // push all the characters, except the first
  char *ptr = (char *)_CP1251_buffer;
  if(*ptr == _NUL_) return _NUL_;
  ptr++;
  for( byte i=0; i<7; i++){
    if(*ptr == _NUL_) break;
    _input_buffer.push( *ptr++);        
  }
  return (char)(*_CP1251_buffer);
}

//
// Serial communication: provides the console input-output
// It is presumed that consoles operate UTF8 sets
//
void IOManager::sendChar( byte c, byte dest, bool wait_for_host){
  // TODO: temporary backslash printing instead of backspace
  if( c==8 || c==127){
    if( _sendToSerial(dest)) Serial.write('\\'); 
    if( _sendToSerial2(dest)) Serial2.write('\\');
    if( wait_for_host) _wait_for_transmission(1);
    keepAwake();
    return;
  }  
  // speed up processing
  if( c<127){
    if( _sendToSerial(dest)) Serial.write(c); 
    if( _sendToSerial2(dest)) Serial2.write(c);
    if( wait_for_host) _wait_for_transmission(1);
    keepAwake();
    return;
  }
  // full conversion to UTF8
  _CP1251_buffer[0] = c;
  _CP1251_buffer[1] = _NUL_;
  convertToUTF8( (char *)_io_buffer, _CP1251_buffer, 15);
  sendStringUTF8( (const char *)_io_buffer, dest, wait_for_host);
}
void IOManager::sendString( const char *str, size_t limit, byte dest){
  if(limit == 0) limit = strlen(str);
  if(limit == 0) return;
  if(limit > INPUT_LIMIT) limit = INPUT_LIMIT;
  convertToUTF8( (char *)_io_buffer, (byte *)str, limit);
  sendStringUTF8( (const char *)_io_buffer, dest);
}
void IOManager::sendStringLn( const char *str, size_t limit, byte dest){
  if(limit == 0) limit = strlen(str);
  if(limit == 0) return;
  if(limit > INPUT_LIMIT) limit = INPUT_LIMIT;
  convertToUTF8( (char *)_io_buffer, (byte *)str, limit);
  sendStringUTF8Ln( (const char *)_io_buffer, dest);
}
void IOManager::sendStringUTF8( const char *str, byte dest, bool wait_for_host){
  if( _sendToSerial(dest)) Serial.print( str); 
  if( _sendToSerial2(dest)) Serial2.print( str);
  if( wait_for_host) _wait_for_transmission( strlen(str));
  keepAwake();
}
void IOManager::sendStringUTF8Ln( const char *str, byte dest){
  if( _sendToSerial(dest)) Serial.println( str); 
  if( _sendToSerial2(dest)) Serial2.println( str);
  _wait_for_transmission( strlen(str));
  keepAwake();
}
void IOManager::sendLn( byte dest){
  if( _sendToSerial(dest)) Serial.println(); 
  if( _sendToSerial2(dest)) Serial2.println();
  _wait_for_transmission( 2);
}

//
// Keyboard and mouse injection
//
void IOManager::injectKeyboard( byte c){
  byte Code[] = { _KBD_INJ_, _NUL_, _NUL_};
  Code[1] = c;
  sendStringUTF8( Code, SERIAL2_ONLY);
}
void IOManager::injectKeyboard( byte *str){
  for( byte i=0; i<255 && (*str)!=_NUL_; i++)
    injectKeyboard( *str++);
}
void IOManager::injectMouseClick( bool left, bool right, bool middle){
  byte Code[] = { _MOUSE_INJ_, _NUL_, _NUL_};
  if( left) Code[1] += 4;
  if( right) Code[1] += 8;
  if( middle) Code[1] += 16;
  sendStringUTF8( Code, SERIAL2_ONLY);
}
void IOManager::injectMouseMove( byte direction, int8_t amount){
  byte Code[] = { _MOUSE_INJ_, _NUL_, _NUL_};
  if( amount < -31) amount = -31;
  if( amount > +31) amount = +31;
  Code[1] = direction & 0x3;
  if(!Code[1]) return; // wrong code!
  Code[1] |= (amount+31)<<2;
  sendStringUTF8( Code, SERIAL2_ONLY);
}

//
// Brings io devices to low power mode;
// Don't forget to power the indicators off
//
void IOManager::sleepOn(){
  if( isAsleep) return;
  hwKeyboard.setLEDs( 0);
  if( serial_Active){
    Serial.end();
    serial_Active = false;
  }
  if( serial2_Active){
    Serial2.end();
    serial2_Active = false;
  }
  isAsleep = true;
}

//
// Wakes io devices up
//
void IOManager::sleepOff(){
  if( !isAsleep) return;
  if( !serial_Active){
    Serial.begin(SERIAL_HARD_BAUD_RATE);
    delay(300);
    while( Serial.available()) Serial.read();
    serial_Active = true;
  }
  if( !serial2_Active){
    if( _wait_PM_active()){
      Serial2.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, IO_RXD2, IO_TXD2);
      delay(300);
      while( Serial2.available()) Serial2.read();
      serial2_Active = true;
    }
  }
  isAsleep = false;
}

//
// Waits for Pro Micro to wake up
//
bool IOManager::_wait_PM_active( int16_t waits){
  for( int16_t i=0; i<waits; i++){
    if(digitalRead(IO_PM_ACTIVE)) return true;
    delay(20);
  }
  return false;
}
