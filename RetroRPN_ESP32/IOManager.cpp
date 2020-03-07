//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include <Arduino.h>
#include "HWKbd.h"
#include "Cyrillic_Phonetic_CP1251.h"
#include "Utilities.hpp"
#include "IOManager.hpp"

//#define __DEBUG

static HWKbd hwKeyboard;
static HWKbd_Encoder hwEncoder(
  HWKBD_Codepage_Cyrillic_Phonetic_CP1251,
  HWKBD_Macropage_Cyrillic_Phonetic_CP1251);

unsigned long IOManager::init() {
  #ifdef HWKBD_FAST_PORTS
  Serial.begin(SERIAL_HARD_BAUD_RATE);
  Serial.println("Achtung! HWKBD fast ports don't work on ESP32!");
  hwKeyboard.connect(HWKBD_RST, HWKBD_CLK, HWKBD_DATA,
                     HWKBDLED_RST, HWKBDLED_CLK, &hwEncoder);
  #else // prevents extra LED activity
  hwKeyboard.connect(HWKBD_RST, HWKBD_CLK, HWKBD_DATA,
                     HWKBDLED_RST, HWKBDLED_CLK, &hwEncoder);
  Serial.begin(SERIAL_HARD_BAUD_RATE);
  #endif
  Serial2.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, IO_RXD2, IO_TXD2);
  while( Serial2.available()) Serial2.read();
  HWKeyboardConnected = hwKeyboard.isConnected;
  pinMode(IO_PM_ACTIVE, INPUT); 
  keepAwake();
  return lastInput;
}

unsigned long IOManager::tick() {
  hwKeyboard.input();
  return lastInput;
}

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
// The serial is converted from Unicode to CP1251
//
char IOManager::input(){
  char c;
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
  if( Serial.available()){
    c = Serial.read();
    #ifdef __DEBUG
    Serial.print( "VT100 KBD: code=");
    Serial.println( c, HEX);
    #endif
    keepAwake();
    return c;
  }
  if( digitalRead(IO_PM_ACTIVE) && Serial2.available()){
    c = Serial2.read();
    #ifdef __DEBUG
    Serial.print( "VT100 Serial 2: code=");
    Serial.println( c, HEX);
    #endif
    keepAwake();
    return c;
  }
  return 0;
}

void IOManager::sendToSerials( char *unicodeBuff, byte *message, bool cr){
  if( message != NULL)
    convertToUTF8( unicodeBuff, message);
  if( cr) Serial.println(unicodeBuff);
  else Serial.print(unicodeBuff);
  if( digitalRead(IO_PM_ACTIVE)){
    if( cr) Serial2.println(unicodeBuff);
    else Serial2.print(unicodeBuff);    
  }
  keepAwake();
  return;
}

//
// Brings io devices to low power mode
//
void IOManager::sleepOn(){
  if( isAsleep) return;
  Serial.end();
  Serial2.end();
  isAsleep = true;
}

//
// Wakes io devices up
//
void IOManager::sleepOff(){
  if( !isAsleep) return;
  Serial.begin(SERIAL_HARD_BAUD_RATE);
  while( Serial.available()) Serial.read();
  Serial2.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, IO_RXD2, IO_TXD2);
  while( Serial2.available()) Serial2.read();
  isAsleep = false;
}
