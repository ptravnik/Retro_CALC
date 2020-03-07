#include <SoftwareSerial.h>
//#include "Keyboard.h"
//#include "Mouse.h"

#define __DEBUG

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

void setup() {
  Serial.begin(PC_BAUD);
  while(Serial.available()) Serial.read();
  mySerial.begin(SERIAL_BAUD);
  while(mySerial.available()) mySerial.read();
  pinMode( ACTIVITY_TO_ESP32, OUTPUT);
  digitalWrite( ACTIVITY_TO_ESP32, LOW);
  pinMode( ACTIVITY_LED, OUTPUT);
  digitalWrite( ACTIVITY_LED, LOW);
  //Mouse.begin();
  //Keyboard.begin();
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

void loop() {
  char c = 0;
  blink_Activity();
  if( Serial.available()){
    lastActive = millis();
    c = Serial.read();
    if( c != 0){
      #ifdef __DEBUG
      // loopback
      Serial.write(c);
      #endif
      mySerial.write(c);
    }
  }
  if( mySerial.available()){
    lastActive = millis();
    c = mySerial.read();
    if( c != 0){
      Serial.write(c);
    }
  }

  //if( mySerial.available()){
  //  c = mySerial.read();
  //  Serial.write(c);
  //}
//    switch( c){
//      case 0:
//        return;
//      case 1:
//        KeyboardWrite = !KeyboardWrite;
//        return;
//      case 2:
//        KeyboardPress = !KeyboardPress;
//        return;
//      case 3:
//        KeyboardRelease = !KeyboardPress;
//        return;
//      case 4:
//        resetKeyboardPrefix();
//        Keyboard.releaseAll();
//        return;
//      default:
//        if( KeyboardWrite){
//          Keyboard.write(c);
//          resetKeyboardPrefix();
//          return;
//        }
//        if( KeyboardPress){
//          Keyboard.press(c);
//          resetKeyboardPrefix();
//          return;
//        }
//        if( KeyboardRelease){
//          Keyboard.release(c);
//          resetKeyboardPrefix();
//          return;
//        }
//        mySerial.write(c);
//        return;
//    }
//  }

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

//void resetKeyboardPrefix(){
//  KeyboardWrite = false;
//  KeyboardPress = false;
//  KeyboardRelease = false;
//}
