#include "HWKbd.h"
#include "Cyrillic_Phonetic_CP1251.h"

#define CONSOLE_BAUD 115200
#define COMM_LEN 255
#define BEEPER_PIN 8
#define NOTE_C3  131
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_B6  1976

char Command[COMM_LEN+1];
char Command2[COMM_LEN*2+1];
size_t Command_position = 0;

static HWKbd hwKeyboard;
static HWKbd_Encoder hwEncoder(
  HWKBD_Codepage_Cyrillic_Phonetic_CP1251,
  HWKBD_Macropage_Cyrillic_Phonetic_CP1251);

void setup() {
  Serial.begin(CONSOLE_BAUD);
  Serial.println();
  #ifdef HWKBD_FAST_PORTS
  Serial.println("Achtung! Fast ports!");
  #endif
  hwKeyboard.connect(13,12,11,10,9, &hwEncoder);
  Serial.println("Keyboard connected:");
  Serial.println("Type a string, followed by <Enter>.");
  Serial.println("Or press button #10 for Dog-and-Pony show.");
  return;
}

static byte LED_Set = 0;

void loop() {
  char c = 0;
  hwKeyboard.input();
  if( !hwKeyboard.available() && !Serial.available())
    return;
  if( hwKeyboard.available()){
    Serial.print( "Last scan: ");
    Serial.println( hwKeyboard.lastScanResult, DEC);
    c = hwKeyboard.read();
  }
  if( Serial.available())
    c = Serial.read();
  Serial.print( "Input: ");
  Serial.print( c, DEC);
  Serial.print( "[0x");
  Serial.print( c, HEX);
  Serial.println( "]");
  if( c==10){
    Command[ Command_position] = 0;
    convertToUTF8( Command2, Command);
    Serial.print ( "Command entered: [");
    Serial.print( Command2);
    Serial.println( "]");
    for( int i=0; i<Command_position; i++){
      Serial.print((byte)Command[i], HEX);      
      Serial.print(" ");      
    }
    Serial.println();      
    Command_position = 0;
    return;
  }
  if( Command_position < COMM_LEN){
    Command[ Command_position++] = c;
  }
  //if( hwKeyboard.lastScanResult == 10) DogAndPony();
}

void DogAndPony(){
  Serial.println("Emergency!");
  for( byte i=0; i<16; i++){
    hwKeyboard.setLEDs( i);
    //tone(BEEPER_PIN, NOTE_C3, 50);
    delay(100);
  }
  hwKeyboard.setLEDs( 0);
  //tone(BEEPER_PIN, NOTE_A4, 100);
  delay(500);
  hwKeyboard.setLEDs( 15);
  //tone(BEEPER_PIN, NOTE_A4, 100);
  delay(500);
  hwKeyboard.setLEDs( 0);
  //tone(BEEPER_PIN, NOTE_A4, 100);
  delay(500);
  for( byte i=16; i>0; i--){
    hwKeyboard.setLEDs( i-1);
    //tone(BEEPER_PIN, NOTE_B4, 50);
    delay(100);
  }  
}

void sendPulse(byte port){
  digitalWrite(port, HIGH);
  delay(10);
  digitalWrite(port, LOW);
}

//
// Converts a string from CP1251 into Unicode (e.g. for printing via UTF8)
//
inline char *_setDuplet( char *buff, byte c){
  *buff++ = c;
  *buff = 0;
  return buff;  
}
inline char *_setTriplet( char *buff, byte prefix, byte c){
  *buff++ = prefix;
  *buff++ = c;
  *buff = 0;
  return buff;  
}
char *convertToUTF8( char *buff, char *message, size_t nchar){
  byte c;
  for (size_t i=0; i<nchar; i++) {
    c = message[i];
    if( !c) break;
    buff = convertToUTF8( buff, (byte)c);
  }
  return buff;
}
char *convertToUTF8( char *buff, char *message){
  return convertToUTF8( buff, message, strlen(message));
}
char *convertToUTF8( char *buff, byte c){
  if( c < 128) return _setDuplet(buff, c);
  if( c == 0xB8) return _setTriplet(buff, 0xD1, 0x91);
  if( c == 0xA8) return _setTriplet(buff, 0xD0, 0x81);
  if( c >= 0xF0) return _setTriplet(buff, 0xD1, c - 0x70);
  if( c >= 0xC0) return _setTriplet(buff, 0xD0, c - 0x30);
  return _setTriplet(buff, 0xD1, c);
}
