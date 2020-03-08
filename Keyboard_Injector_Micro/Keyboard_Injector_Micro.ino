#include <SoftwareSerial.h>
#include "Keyboard.h"
#include "Mouse.h"
#include "CP1251_mod.h" 

// Uncomment for debugging
//#define __DEBUG
//#define __DEBUG_CODES

#define PC_BAUD 115200
#define SERIAL_BAUD 19200
#define ACTIVITY_TO_ESP32 7
#define SERIAL_RX 9
#define SERIAL_TX 8
#define ACTIVITY_LED 10
#define INACTIVITY_INTERVAL 2000
#define BLINK_INTERVAL 100
#define BLINK_DELAY 30

SoftwareSerial mySerial(SERIAL_RX, SERIAL_TX);

static volatile bool KeyboardWrite = false;
static volatile bool KeyboardPress = false;
static volatile bool KeyboardRelease = false;
static volatile bool MouseInject = false;
static volatile char MouseCommands[4]; // buttons, x, y, scroll
static volatile byte MouseCommandPosition = 0;

static volatile long lastActive = 0;
static volatile long lastBlinked = 0;

#define UTF8_BUFFER_LENGTH 64
static byte UTF8_buffer[UTF8_BUFFER_LENGTH];
static byte CP1251_buffer[UTF8_BUFFER_LENGTH];

#define N_SUBS 3
const byte _PLSMNS_SUB[] PROGMEM = { '+', '/', '-', _NUL_};
const byte _YO_SUB[] PROGMEM = { 0xD1, 0x91, _NUL_};
const byte _YO_CAP_SUB[] PROGMEM = { 0xD0, 0x81, _NUL_};

const byte _CP1251_bytes[] PROGMEM = {
  _PLSMNS_, 
  0xB8, // small "YO"
  0xA8  // capital "YO"
  };
const byte *const _CP1251_substitutes[] PROGMEM = {
  _PLSMNS_SUB, 
  _YO_SUB, 
  _YO_CAP_SUB, 
  };

//
// Runs once, upon the power-up
//
void setup() {
  memset( UTF8_buffer, 0, UTF8_BUFFER_LENGTH);
  Serial.begin(PC_BAUD);
  while(Serial.available()) Serial.read();
  mySerial.begin(SERIAL_BAUD);
  while(mySerial.available()) mySerial.read();
  pinMode( ACTIVITY_TO_ESP32, OUTPUT);
  digitalWrite( ACTIVITY_TO_ESP32, LOW);
  pinMode( ACTIVITY_LED, OUTPUT);
  digitalWrite( ACTIVITY_LED, LOW);
  Mouse.begin();
  Keyboard.begin();
  delay(100);
  blink_Activity();
  delay(300);
  blink_Activity();
  delay(300);
  blink_Activity();
  lastActive = millis();
  // permission to communicate granted to ESP32
  digitalWrite( ACTIVITY_TO_ESP32, HIGH);
  return;
}

//
// Runs continuously
//
void loop() {
  char c = 0;
  blink_Activity();
  if( Serial.available()){
    lastActive = millis();
    c = Serial.read();
    if( c != 0)
      sendToESP32Serial(c);
  }
  if( mySerial.available()){
    lastActive = millis();
    c = mySerial.read();
    if( !sendToPCKeyboard(c))
      sendToPCSerial(c);
  }

  // Eliminate unwanted Tx, Rx lights
  //RXLED1;
  //TXLED1;
  return;
}

//
// blinks external LED briefly
// only if the serial ports show any activity
//
void blink_Activity(){
  long t = millis();
  if( t - lastActive > INACTIVITY_INTERVAL){
    lastBlinked = t;
    return;
  }
  if( t - lastBlinked < BLINK_INTERVAL) return; 
  digitalWrite( ACTIVITY_LED, HIGH);  
  delay( BLINK_DELAY);
  digitalWrite( ACTIVITY_LED, LOW);
  lastBlinked = millis();
}

//
// Converts a character from CP1251 into Unicode
// (e.g. for printing via UTF8 console)
//
char *_setDuplet( byte *buff, byte c){
  *buff++ = c;
  *buff = _NUL_;
  return (char *)buff;  
}
char *_setTriplet( byte *buff, byte prefix, byte c){
  *buff++ = prefix;
  *buff++ = c;
  *buff = _NUL_;
  return buff;  
}
char *_setMacro( byte *buff, const char *macro){
  for( byte i=0; i<UTF8_BUFFER_LENGTH-1; i++){
    char c = pgm_read_byte_near(macro + i);
    if( c == 0) break;
    *buff++ = c;
  }
  *buff = _NUL_;
  return (char *)buff; 
}
char *convertToUTF8( byte *buff, byte c){
  if( c < 128) return _setDuplet(buff, c);
  for( byte i=0; i<N_SUBS; i++){
    if( c == pgm_read_byte_near(_CP1251_bytes + i))
      return _setMacro( buff, _CP1251_substitutes[i]);
    }
  if( c >= 0xF0) return _setTriplet(buff, 0xD1, c - 0x70); // small alpha
  if( c >= 0xC0) return _setTriplet(buff, 0xD0, c - 0x30); // large alpha
  return _setTriplet(buff, 0xD1, c);
}

//
// Sends characters to PC serial port
// Returns the number of chars sent
//
byte sendToPCSerial(char c){
  *UTF8_buffer = _NUL_; 
  convertToUTF8(UTF8_buffer, (byte)c);
  char *ptr = (char *)UTF8_buffer;
  for( byte i=0; i<UTF8_BUFFER_LENGTH; i++){
    if( *ptr == _NUL_) return i;
    Serial.write( *ptr++);
  }
  return UTF8_BUFFER_LENGTH-1;
}

//
// Converts the keyboard injection command
// Returns true if injected
//
bool sendToPCKeyboard( char c){
  if( c != _KBD_INJ_ && c != _MOUSE_INJ_) return false;
  if( c == _KBD_INJ_) return processKbdInject();
  return processMouseInject();
}
bool processKbdInject(){
  for( byte i=0; i<10; i++){
    if( !mySerial.available()){
      delay(10);
      continue;
    }
    byte c = (byte)mySerial.read();
    if( c < _BEL_) return false;
    Keyboard.write(c);
    return true;
  }
  // no command in 100 ms: misplaced inject
  return false;
}
bool processMouseInject(){
  for( byte i=0; i<10; i++){
    if( !mySerial.available()){
      delay(10);
      continue;
    }
    byte c = (byte)mySerial.read();
    byte command = c & 0x03; // two lower bytes are the command
    c >>= 2;
    int delta = (int)c - 31; 
    switch(command){
      case 0: // buttons click
        if( c & 0x1) Mouse.click(MOUSE_LEFT);
        if( c & 0x2) Mouse.click(MOUSE_RIGHT);
        if( c & 0x3) Mouse.click(MOUSE_MIDDLE);
        return true;
      case 1: // movement x
        Mouse.move( delta, 0, 0);
        return true;
      case 2: // movement y
        Mouse.move( 0, delta, 0);
        return true;
      case 3: // movement z
        Mouse.move( 0, 0, delta);
        return true;
      default:
        break;
    }
    return true;
  }
  // no command in 100 ms: misplaced inject
  return false;
}

//
// Converts message from Unicode to CP1251
//
#ifdef __DEBUG
void _reportUnknownUnicode( byte *ptr){
  ptr--;
  Serial.print( "Unknown unicode: ");
  Serial.print( *ptr++, HEX);        
  Serial.print( " ");        
  Serial.print( *ptr++, HEX);        
  Serial.print( " ");        
  Serial.print( *ptr++, HEX);        
  Serial.print( " ");        
  Serial.println( *ptr++, HEX);        
};
#endif
byte *_addCh( byte* ptr, byte* ptr_limit, byte c){
  if( ptr >= ptr_limit) return ptr_limit;
  *ptr++ = c;
  return ptr;
}
char *convertToCP1251( byte *buff, char *message, size_t limit){
  byte c;
  byte Unicode_Prefix = 0;
  byte *ptr1 = buff; 
  byte *ptr2 = (byte *)message; 
  byte *ptr_limit = ptr1 + limit - 1; 
  for (byte i=0; i<strlen(message); i++) {
    c = *ptr2++;
    switch(Unicode_Prefix){
      case 0: // No prefix
        if( !c) // safety termination
          return _addCh(ptr1, ++ptr_limit, (byte)0);
        if( c < 128){ // lower chars
          ptr1 = _addCh(ptr1, ptr_limit, c);
          break;
        }
        Unicode_Prefix = c;
        break;
      case 0xD0:
        Unicode_Prefix = 0;
        if( 0x90 <= c && c < 0xD0){
          ptr1 = _addCh(ptr1, ptr_limit, c + 0x30);
          break;
        }
        if( c == 0x81){
          ptr1 = _addCh(ptr1, ptr_limit, 0xA8);
          break;
        }
        ptr1 = _addCh(ptr1, ptr_limit, c);
        break;
      case 0xD1:
        Unicode_Prefix = 0;
        if( 0x80 <= c && c < 0x90){
          ptr1 = _addCh(ptr1, ptr_limit, c + 0x70);
          break;
        }
        if( c == 0x91){
          ptr1 = _addCh(ptr1, ptr_limit, 0xB8);
          break;
        }
        ptr1 = _addCh(ptr1, ptr_limit, c);
        break;
      case 0xC2:
        Unicode_Prefix = 0;
        if( c == 0xAB || c == 0xBB){ // quote
          ptr1 = _addCh(ptr1, ptr_limit, '\"');
          break;
        }
        #ifdef __DEBUG
        _reportUnknownUnicode( ptr2);
        #endif
        ptr1 = _addCh(ptr1, ptr_limit, UNKNOWN_UTF8);
        break;
      case 0xE2:
        Unicode_Prefix = 0;
        if( c == 0x80 && *ptr2 == 0x94){ // EM-dash
          ptr1 = _addCh(ptr1, ptr_limit, '-');
          ptr1 = _addCh(ptr1, ptr_limit, '-');
          ptr2++;
          break;
        }
        if( c == 0x80 && *ptr2 == 0xA6){ // triple dot
          ptr1 = _addCh(ptr1, ptr_limit, '.');
          ptr1 = _addCh(ptr1, ptr_limit, '.');
          ptr1 = _addCh(ptr1, ptr_limit, '.');
          ptr2++;
          break;
        }
        #ifdef __DEBUG
        _reportUnknownUnicode( ptr2);
        #endif
        ptr1 = _addCh(ptr1, ptr_limit, UNKNOWN_UTF8);
        ptr2++;
        break;
      default: // Unknown Unicode is replaced with *
        Unicode_Prefix = 0;
        #ifdef __DEBUG
        _reportUnknownUnicode( ptr2);
        #endif
        ptr1 = _addCh(ptr1, ptr_limit, UNKNOWN_UTF8);
        break;
    }
  } // for
  // safe string termination
  return _addCh(ptr1, ++ptr_limit, (byte)0);
}

//
// Sends characters to ESP32 serial port
// Conversion from UTF8 to CP1251 performed
// Returns the number of chars sent
//
byte sendToESP32Serial(char cc){
  byte c = (byte)cc;
  if( 0 < c && c < 128){
    #ifdef __DEBUG
    // loopback
    Serial.write(cc);
    #endif
    mySerial.write(cc);
    return 1;
  }
  char *ptr = (char *)UTF8_buffer;
  *ptr++ = cc;
  *ptr = _NUL_;
  switch( c){
    case _NUL_:
      return 0;
    case 0xD0: // double character (Russian letters)
    case 0xD1: // double character (Russian letters)
    case 0xC2: // double character (quotations)
      if( !readNonlocking(ptr)) return 0;
      return _sendToESP32();
    case 0xE2: // triple character (Ellipsis and EM-dash)
      if( !readNonlocking(ptr++)) return 0;
      if( !readNonlocking(ptr)) return 0;
      return _sendToESP32();
    default: // unknown or misplaced Unicode is a star!
      #ifdef __DEBUG
      // loopback
      Serial.write(UNKNOWN_UTF8);
      #endif
      mySerial.write(UNKNOWN_UTF8);
      break;
  }
  return 0;
}
bool readNonlocking(byte *ptr){
  for( byte i=0; i<10; i++){
    if( !Serial.available()){
      delay(10);
      continue;
    }
    *ptr++ = (byte)Serial.read();
    *ptr = _NUL_;
    return true;
  }
  return false;
}
byte _sendToESP32(){
  #ifdef __DEBUG
  // loopback
  Serial.println( (char *)UTF8_buffer);
  #endif
  #ifdef __DEBUG_CODES
  Serial.print( "From [");
  for( byte i=0; i<UTF8_BUFFER_LENGTH; i++){
    byte c = UTF8_buffer[i];
    if( c == _NUL_) break;
    Serial.print( c, HEX);    
  }
  Serial.println( "]");
  #endif
  convertToCP1251( CP1251_buffer, (char *)UTF8_buffer, UTF8_BUFFER_LENGTH-1);
  #ifdef __DEBUG_CODES
  Serial.print( "To [");
  for( byte i=0; i<UTF8_BUFFER_LENGTH; i++){
    byte c = CP1251_buffer[i];
    if( c == _NUL_) break;
    Serial.print( c, HEX);    
  }
  Serial.println( "]");
  #endif
  char *ptr = (char *)CP1251_buffer;
  for( byte i=0; i<UTF8_BUFFER_LENGTH; i++){
    if(*ptr == 0) return i;
    mySerial.write( *ptr++);        
  }
  return UTF8_BUFFER_LENGTH-1;
}
