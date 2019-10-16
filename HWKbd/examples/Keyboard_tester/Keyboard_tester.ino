#include "HWKbd.h"
//#include "Morse_Code.h"
#include "Cyrillic_Unicode.h"

#define CONSOLE_BAUD 115200
#define COMM_LEN 255
#define BEEPER_PIN 2
#define NOTE_C3  131
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_B6  1976

char Command[COMM_LEN+1];
size_t Command_position = 0;

static HWKbd hwKeyboard;
static HWKbd_Encoder hwEncoder(HWKBD_Codepage_UTF8, HWKBD_Macropage_UTF8);

void setup() {
  Serial.begin(CONSOLE_BAUD);
  Serial.println();
  hwKeyboard.connect(3,4,5,6,7, &hwEncoder);
  Serial.println("Keyboard connected:");
  Serial.println("Type a string, followed by <Enter>.");
  Serial.println("Or press button #10 for Dog-and-Pony show.");
  return;
}


void loop() {
  char c = 0;
  hwKeyboard.input();
  if( !hwKeyboard.available() && !Serial.available())
    return;
  if( hwKeyboard.available())
    c = hwKeyboard.read();
  if( Serial.available())
    c = Serial.read();
  if( c==10){
    Command[ Command_position] = 0;
    Serial.println( Command);
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
  if( hwKeyboard.lastScanResult == 10) DogAndPony();
}


void DogAndPony(){
  Serial.println("Emergency!");
  for( byte i=0; i<16; i++){
    hwKeyboard.setLEDs( i);
    tone(BEEPER_PIN, NOTE_C3, 50);
    delay(100);
  }
  hwKeyboard.setLEDs( 0);
  tone(BEEPER_PIN, NOTE_A4, 100);
  delay(500);
  hwKeyboard.setLEDs( 15);
  tone(BEEPER_PIN, NOTE_A4, 100);
  delay(500);
  hwKeyboard.setLEDs( 0);
  tone(BEEPER_PIN, NOTE_A4, 100);
  delay(500);
  for( byte i=16; i>0; i--){
    hwKeyboard.setLEDs( i-1);
    tone(BEEPER_PIN, NOTE_B4, 50);
    delay(100);
  }  
}
