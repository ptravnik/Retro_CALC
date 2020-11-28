////////////////////////////////////////////////////////

/* RetroRPN - "Электроника МК-90" reborn
  Copyright (c) 2020 Mike Yakimov.  All rights reserved.

  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

////////////////////////////////////////////////////////
//
// This is a hardware companion to ESP32 Retro Calculator (modern reincarnation of the Soviet Электроника-90) 
// Implementation is for Sparkfun Micro Pro module (3.3 V) or valid clones
//
// Functionality:
// (1) Receive inputs from PC via USB Serial, perform conversion from UTF8 to CP1251, send to Software Serial
// (2) Receive inputs from Software Serial, perform conversion from CP1251 to UTF8, send to USB Serial
// (3) Receive inputs from Software Serial, send to PC as keystrokes (Via Arduino Keyboard library)
// (4) Receive inputs from Software Serial, send to PC as mouse movements/clicks (Via Arduino Mouse library)
// (5) Receive commands from Software Serial, play tones via piezo speaker
// (6) Receive commands from Software Serial, access time module (set/retrive time)
// (7) Constantly monitor hardware keyboard, convert inputs to CP1251, bufferize with USB serial
// (8) Receive commands from Software Serial, perform immediate hardware keyboard scan, return scancodes
// (9) Receive commands from Software Serial, set and reytrieve hardware keyboard LEDs
// (10) Receive commands from Software Serial, manipulate LCD PWM
// (11) Receive commands from Software Serial, perform power shutdown 
//
// Pin assignment:
//
// 00 - TX0 (Not connected)
// 01 - RX1 (Not connected)
// 02 - SDA to DS3231 RT Clock module
// 03 - SCL to DS3231
// 04 - Serial activity LED
// 05 - Piezo PWM Out
// 06 - LCD PWM Out
// 07 - Software Serial / operation Block
// 08 - Software Serial TX
// 09 - Software Serial RX
// 10 - KBD LED Clock
// 14 - KBD LED Reset
// 15 - KBD Sense
// 16 - KBD Clock
// A0 - KBD Reset
// A1 - Power Hold
// A2 - Power Button Sense (HIGH - shutdown wanted)
// A3 - Battery voltage sensor
//
////////////////////////////////////////////////////////

//#define __DEBUG

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "./src/CP1251_mod.h" 
#include "./src/SelfShutdown.hpp"
#include "./src/ActivityLED.hpp"
#include "./src/VoltageSensor.hpp"
#include "./src/CircularBuffer.hpp"
#include "./src/Commander.hpp"

// Uncomment for debugging
//#define __DEBUG
//#define __DEBUG_CODES

#define PC_BAUD 115200
#define SERIAL_BAUD 38400

#define RTC_SDA             2
#define RTC_SCL             3
#define ACTIVITY_LED        4
#define PIEZO_PWM_OUT       5
#define LCD_PWM_OUT         6
#define ACTIVITY_TO_ESP32   7
#define SERIAL_TX           8
#define SERIAL_RX           9
#define LED_CLK             10
#define LED_RESET           14
#define KBD_SIG             15
#define KBD_CLK             16
#define KBD_RESET           A0
#define SELF_POWER_HOLD     A1
#define POWER_OFF           A2
#define BATTERY_VOLTAGE     A3

//#define _SS_MAX_RX_BUFF 256
//SoftwareSerial mySerial(SERIAL_RX, SERIAL_TX);

//#define UTF8_BUFFER_LENGTH 64
//static byte UTF8_buffer[UTF8_BUFFER_LENGTH];
//static byte CP1251_buffer[UTF8_BUFFER_LENGTH];

//#define N_SUBS 3
//const byte _PLSMNS_SUB[] PROGMEM = { '+', '/', '-', _NUL_};
//const byte _YO_SUB[] PROGMEM = { 0xD1, 0x91, _NUL_};
//const byte _YO_CAP_SUB[] PROGMEM = { 0xD0, 0x81, _NUL_};

//const byte _CP1251_bytes[] PROGMEM = {
//  _PLSMNS_, 
//  0xB8, // small "YO"
//  0xA8  // capital "YO"
//  };
//const byte *const _CP1251_substitutes[] PROGMEM = {
//  _PLSMNS_SUB, 
//  _YO_SUB, 
//  _YO_CAP_SUB, 
//  };

static SelfShutdown myShutdown;
static ActivityLED myActivityLED;
static VoltageSensor myVoltageSensor;
static RtcDS3231<TwoWire> Rtc(Wire); // Wire is declared inside the Wire.h
static CircularBuffer cb;
static Commander comm;

//
// Runs once, upon the power-up
//
void setup() {
  myShutdown.init( POWER_OFF, SELF_POWER_HOLD);

// PIEZO_PWM_OUT        5
// LCD_PWM_OUT          6

  pinMode( ACTIVITY_TO_ESP32, OUTPUT);
  digitalWrite( ACTIVITY_TO_ESP32, LOW);

  myActivityLED.init( ACTIVITY_LED);
  myVoltageSensor.init( BATTERY_VOLTAGE, 6600, 0); // We use 20M:20M dividor in 3.3 V processor

  Serial.begin(PC_BAUD);
  while(Serial.available()) Serial.read();

  //mySerial.begin(SERIAL_BAUD);
  //while(mySerial.available()) mySerial.read();
  //Mouse.begin();
  //Keyboard.begin();

  comm.init( &cb);
  comm.addCommand( (byte)COMMAND_GET_COMPILE_TIME, printCompileTime);
  comm.addCommand( (byte)COMMAND_GET_VOLTAGE, printVoltageSensor);
  comm.addCommand( (byte)COMMAND_GET_DATETIME, printDateTime);
  comm.addCommand( (byte)COMMAND_GET_TEMPERATURE, printTemperature);
  comm.addCommand( (byte)COMMAND_SET_DATETIME, setDateTime);
  delay(50);
  for( byte i=0; i<3; i++) myActivityLED.blink( 30, 50);

  initRTC();

  //lastActive = millis();
  // permission to communicate granted to ESP32
  digitalWrite( ACTIVITY_TO_ESP32, HIGH);
}

//
// Runs continuously
//
void loop() {
  checkShutdown();

  //Serial.print( "Battery Voltage = ");
  //Serial.println( myVoltageSensor.read());
  //readRTC();
  comm.read();
  delay(500);

  // Eliminate unwanted Tx, Rx lights
  RXLED0;
  TXLED0;
  myActivityLED.blink( 100, 100);

  // char c = 0;
  // long t = millis();
  // blink_Activity();
  // if( Serial.available()){
  //   lastActive = t;
  //   c = Serial.read();
  //   if( c != 0)
  //     sendToESP32Serial(c);
  // }
  // while( mySerial.available()){
  //   c = mySerial.read();
  //   if( !sendToPCKeyboard(c) && c!=0)
  //     cb.push(c);
  // }
  // while( cb.available()){
  //   lastActive = t;
  //   Serial.write(cb.pop());
  // }
}

void checkShutdown(){
  if( !myShutdown.isPowerPressed()) return;
  Serial.println( "Shutdown requested (but I have USB power!)");
  for( int i=10; i>=1; i--)
    myActivityLED.blink( i*10, i*10);
  myShutdown.shutdown();
}

void printCompileTime( char *buff, byte n){
  snprintf_P(buff, n, PSTR("%s %s"),  __DATE__,  __TIME__);
}

void printVoltageSensor( char *buff, byte n){
  snprintf_P(buff, n, PSTR("%04u"), myVoltageSensor.read());
}

void printDateTime( char *buff, byte n){
  if (RTC_Check(buff, n)) return; 
  RtcDateTime dt = Rtc.GetDateTime();
  snprintf_P(buff, n,
    PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
    dt.Day(), dt.Month(), dt.Year(),
    dt.Hour(), dt.Minute(), dt.Second() );
}

void printTemperature( char *buff, byte n){
  if (RTC_Check(buff, n)) return; 
  RtcTemperature temp = Rtc.GetTemperature();
  Serial.printf( temp.AsFloatDegC());
  snprintf_P(buff, n,
    PSTR("%5.2fC"), temp.AsFloatDegC());
}

void setDateTime( char *buff, byte n){
    char *ptr = comm._inputBuff;
    for( byte i=0; i<INPUT_BUFFER_LENGTH-1; i++){
        if( !Serial.available()) break;
        char c = Serial.read();
        if( c == _CR_) break;
        *ptr++ = c;
    }
    *ptr = _NUL_;
}

void initRTC(){
    // 
    // With no modification and no battery the module takes 1.90 mA; running at 1.56 mA
    // installing battery makes no difference
    Rtc.Begin();
    if( RTC_Check( comm._inputBuff, INPUT_BUFFER_LENGTH)){
        if (Rtc.LastError() == 0) Rtc.SetDateTime(RtcDateTime( __DATE__, __TIME__));
        return;
    }

    if (!Rtc.GetIsRunning()) Rtc.SetIsRunning(true);

    // As a security precaution, never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
}

bool RTC_Check( char *buff, byte n){
    if (Rtc.IsDateTimeValid()) return false; 
    if (Rtc.LastError())
        snprintf_P(buff, n, PSTR("I2C ERR %02u"), Rtc.LastError());
    else
        snprintf_P(buff, n, PSTR("TIME LOST"));
    return true;
}

// //
// // Converts the keyboard injection command
// // Returns true if injected
// //
// bool sendToPCKeyboard( char c){
//   if( c != _KBD_INJ_ && c != _MOUSE_INJ_) return false;
//   if( c == _KBD_INJ_) return processKbdInject();
//   return processMouseInject();
// }
// bool processKbdInject(){
//   for( byte i=0; i<100; i++){
//     if( !mySerial.available()){
//       delayMicroseconds(200);
//       continue;
//     }
//     byte c = (byte)mySerial.read();
//     if( c < _BEL_) return false;
//     Keyboard.write(c);
//     return true;
//   }
//   // no command in 100 ms: misplaced inject
//   return false;
// }
// bool processMouseInject(){
//   for( byte i=0; i<100; i++){
//     if( !mySerial.available()){
//       delayMicroseconds(200);
//       continue;
//     }
//     byte c = (byte)mySerial.read();
//     byte command = c & 0x03; // two lower bytes are the command
//     c >>= 2;
//     int delta = (int)c - 31; 
//     switch(command){
//       case 0: // buttons click
//         if( c & 0x1) Mouse.click(MOUSE_LEFT);
//         if( c & 0x2) Mouse.click(MOUSE_RIGHT);
//         if( c & 0x4) Mouse.click(MOUSE_MIDDLE);
//         return true;
//       case 1: // movement x
//         Mouse.move( delta, 0, 0);
//         return true;
//       case 2: // movement y
//         Mouse.move( 0, delta, 0);
//         return true;
//       case 3: // movement z
//         Mouse.move( 0, 0, delta);
//         return true;
//       default:
//         break;
//     }
//     return true;
//   }
//   // no command in 100 ms: misplaced inject
//   return false;
// }

// //
// // Converts message from Unicode to CP1251
// //
// #ifdef __DEBUG
// void _reportUnknownUnicode( byte *ptr){
//   ptr--;
//   Serial.print( "Unknown unicode: ");
//   Serial.print( *ptr++, HEX);        
//   Serial.print( " ");        
//   Serial.print( *ptr++, HEX);        
//   Serial.print( " ");        
//   Serial.print( *ptr++, HEX);        
//   Serial.print( " ");        
//   Serial.println( *ptr++, HEX);        
// };
// #endif
// byte *_addCh( byte* ptr, byte* ptr_limit, byte c){
//   if( ptr >= ptr_limit) return ptr_limit;
//   *ptr++ = c;
//   return ptr;
// }
// char *convertToCP1251( byte *buff, char *message, size_t limit){
//   byte c;
//   byte Unicode_Prefix = 0;
//   byte *ptr1 = buff; 
//   byte *ptr2 = (byte *)message; 
//   byte *ptr_limit = ptr1 + limit - 1; 
//   for (byte i=0; i<strlen(message); i++) {
//     c = *ptr2++;
//     switch(Unicode_Prefix){
//       case 0: // No prefix
//         if( !c) // safety termination
//           return _addCh(ptr1, ++ptr_limit, (byte)0);
//         if( c < 128){ // lower chars
//           ptr1 = _addCh(ptr1, ptr_limit, c);
//           break;
//         }
//         Unicode_Prefix = c;
//         break;
//       case 0xD0:
//         Unicode_Prefix = 0;
//         if( 0x90 <= c && c < 0xD0){
//           ptr1 = _addCh(ptr1, ptr_limit, c + 0x30);
//           break;
//         }
//         if( c == 0x81){
//           ptr1 = _addCh(ptr1, ptr_limit, 0xA8);
//           break;
//         }
//         ptr1 = _addCh(ptr1, ptr_limit, c);
//         break;
//       case 0xD1:
//         Unicode_Prefix = 0;
//         if( 0x80 <= c && c < 0x90){
//           ptr1 = _addCh(ptr1, ptr_limit, c + 0x70);
//           break;
//         }
//         if( c == 0x91){
//           ptr1 = _addCh(ptr1, ptr_limit, 0xB8);
//           break;
//         }
//         ptr1 = _addCh(ptr1, ptr_limit, c);
//         break;
//       case 0xC2:
//         Unicode_Prefix = 0;
//         if( c == 0xAB || c == 0xBB){ // quote
//           ptr1 = _addCh(ptr1, ptr_limit, '\"');
//           break;
//         }
//         #ifdef __DEBUG
//         _reportUnknownUnicode( ptr2);
//         #endif
//         ptr1 = _addCh(ptr1, ptr_limit, UNKNOWN_UTF8);
//         break;
//       case 0xE2:
//         Unicode_Prefix = 0;
//         if( c == 0x80 && *ptr2 == 0x94){ // EM-dash
//           ptr1 = _addCh(ptr1, ptr_limit, '-');
//           ptr1 = _addCh(ptr1, ptr_limit, '-');
//           ptr2++;
//           break;
//         }
//         if( c == 0x80 && *ptr2 == 0xA6){ // triple dot
//           ptr1 = _addCh(ptr1, ptr_limit, '.');
//           ptr1 = _addCh(ptr1, ptr_limit, '.');
//           ptr1 = _addCh(ptr1, ptr_limit, '.');
//           ptr2++;
//           break;
//         }
//         #ifdef __DEBUG
//         _reportUnknownUnicode( ptr2);
//         #endif
//         ptr1 = _addCh(ptr1, ptr_limit, UNKNOWN_UTF8);
//         ptr2++;
//         break;
//       default: // Unknown Unicode is replaced with *
//         Unicode_Prefix = 0;
//         #ifdef __DEBUG
//         _reportUnknownUnicode( ptr2);
//         #endif
//         ptr1 = _addCh(ptr1, ptr_limit, UNKNOWN_UTF8);
//         break;
//     }
//   } // for
//   // safe string termination
//   return _addCh(ptr1, ++ptr_limit, (byte)0);
// }

// //
// // Sends characters to ESP32 serial port
// // Conversion from UTF8 to CP1251 performed
// // Returns the number of chars sent
// //
// byte sendToESP32Serial(char cc){
//   byte c = (byte)cc;
//   if( 0 < c && c < 128){
//     #ifdef __DEBUG
//     // loopback
//     Serial.write(cc);
//     #endif
//     mySerial.write(cc);
//     return 1;
//   }
//   char *ptr = (char *)UTF8_buffer;
//   *ptr++ = cc;
//   *ptr = _NUL_;
//   switch( c){
//     case _NUL_:
//       return 0;
//     case 0xD0: // double character (Russian letters)
//     case 0xD1: // double character (Russian letters)
//     case 0xC2: // double character (quotations)
//       if( !readNonlocking(ptr)) return 0;
//       return _sendToESP32();
//     case 0xE2: // triple character (Ellipsis and EM-dash)
//       if( !readNonlocking(ptr++)) return 0;
//       if( !readNonlocking(ptr)) return 0;
//       return _sendToESP32();
//     default: // unknown or misplaced Unicode is a star!
//       #ifdef __DEBUG
//       // loopback
//       Serial.write(UNKNOWN_UTF8);
//       #endif
//       mySerial.write(UNKNOWN_UTF8);
//       break;
//   }
//   return 0;
// }
// bool readNonlocking(byte *ptr){
//   for( byte i=0; i<100; i++){
//     if( !Serial.available()){
//       delayMicroseconds(100);
//       continue;
//     }
//     *ptr++ = (byte)Serial.read();
//     *ptr = _NUL_;
//     return true;
//   }
//   return false;
// }
// byte _sendToESP32(){
//   #ifdef __DEBUG
//   // loopback
//   Serial.println( (char *)UTF8_buffer);
//   #endif
//   #ifdef __DEBUG_CODES
//   Serial.print( "From [");
//   for( byte i=0; i<UTF8_BUFFER_LENGTH; i++){
//     byte c = UTF8_buffer[i];
//     if( c == _NUL_) break;
//     Serial.print( c, HEX);    
//   }
//   Serial.println( "]");
//   #endif
//   convertToCP1251( CP1251_buffer, (char *)UTF8_buffer, UTF8_BUFFER_LENGTH-1);
//   #ifdef __DEBUG_CODES
//   Serial.print( "To [");
//   for( byte i=0; i<UTF8_BUFFER_LENGTH; i++){
//     byte c = CP1251_buffer[i];
//     if( c == _NUL_) break;
//     Serial.print( c, HEX);    
//   }
//   Serial.println( "]");
//   #endif
//   char *ptr = (char *)CP1251_buffer;
//   for( byte i=0; i<UTF8_BUFFER_LENGTH; i++){
//     if(*ptr == 0) return i;
//     mySerial.write( *ptr++);        
//   }
//   return UTF8_BUFFER_LENGTH-1;
// }
