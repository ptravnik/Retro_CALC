//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "IOManager.hpp"
#include "Utilities.hpp"

#define __DEBUG

const char IO_WelcomeMessage1[] PROGMEM = "*** ЭЛЕКТРОНИКА МК-2090 ***";
const char IO_WelcomeMessage2[] PROGMEM = "***  Retro  Calculator  ***";
const char IO_WelcomeMessage3[] PROGMEM = "(CO in) Serial to PC connected (CO out)";
const char IO_WelcomeMessage4[] PROGMEM = "Hardware keyboard connected";
const char *const IO_Message_Table[] PROGMEM = {
  IO_WelcomeMessage1, 
  IO_WelcomeMessage2,
  IO_WelcomeMessage3, 
  IO_WelcomeMessage4,
  };

static HWKbd hwKeyboard;
static HWKbd_Encoder hwEncoder(
  HWKBD_Codepage_Cyrillic_Phonetic_CP1251,
  HWKBD_Macropage_Cyrillic_Phonetic_CP1251);

void IOManager::init() {
  #ifdef HWKBD_FAST_PORTS
  Serial.println("Achtung! HWKBD fast ports don't work on ESP32!");
  #endif
  hwKeyboard.connect(HWKBD_RST, HWKBD_CLK, HWKBD_DATA,
                     HWKBDLED_RST, HWKBDLED_CLK, &hwEncoder);
  for(byte i=0; i<4; i++)
    Serial.println(IO_Message_Table[i]);
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
  return;
}

void IOManager::setKBDLEDs( bool L0, bool L1, bool L2, bool L3){
  byte tmp = L0 & (L1<<1) & (L2<<2) & (L3<<3);
  hwKeyboard.setLEDs( tmp);
};
void IOManager::setKBDLEDs( byte val){
  hwKeyboard.setLEDs( val);
};


//    void setLEDs( byte n);
//    void setMode( byte newmode);
//    byte scan( void);
//    bool input( void);
//    inline byte available(void){ return _encoder->available();};
//    inline char read(void){ return _encoder->read();};
//    inline char peek(void){ return _encoder->peek();};


//
// Inputs CP1251 character from either hardware keyboard or serials
// The serial is converted from Unicode to CP1251
//
char IOManager::input(){
  char c;
  hwKeyboard.input();
  if( hwKeyboard.available()){
    c = hwKeyboard.read();
    #ifdef __DEBUG
    Serial.print( "Hardware KBD: scan=");
    Serial.print( hwKeyboard.lastScanResult, DEC);
    Serial.print( " code=");
    Serial.println( c, HEX);
    #endif
    return c;
  }
  if( Serial.available()){
    c = Serial.read();
    #ifdef __DEBUG
    Serial.print( "VT100 KBD: code=");
    Serial.println( c, HEX);
    #endif
    return c;
  }
  return 0;
}

void IOManager::sendToSerials( char *unicodeBuff, byte *message, bool cr){
  if( message != NULL)
    convertToUTF8( unicodeBuff, message);
  if( cr) Serial.println(unicodeBuff);
  else Serial.print(unicodeBuff);
  return;
}

//  if( c==10){
//    Command[ Command_position] = 0;
//    Serial.println( Command);
//    for( int i=0; i<Command_position; i++){
//      Serial.print((byte)Command[i], HEX);      
//      Serial.print(" ");      
//    }
//    Serial.println();      
//    Command_position = 0;
//    return;
//  }
//  if( Command_position < COMM_LEN){
//    Command[ Command_position++] = c;
//  }
//  if( hwKeyboard.lastScanResult == 10) DogAndPony();
