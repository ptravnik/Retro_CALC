#include "HWKbd.h"

#define CONSOLE_BAUD 115200
#define COMM_LEN 1024
#define BEEPER_PIN 2
#define NOTE_C3  131
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_B6  1976
#define MORSE_DASH  200
#define MORSE_DOT   50
#define MORSE_PAUSE 80


char Command[COMM_LEN];
size_t Command_position = 0;

HWKbd hwKeyboard;

void setup()
{
  Serial.begin(CONSOLE_BAUD);
  Serial.println();
  hwKeyboard.connect(3,4,5,6,7);
}


void loop()
{
  char c = 0;
  hwKeyboard.input();
  if( !hwKeyboard.available()) return;
  c = hwKeyboard.read();
  if( c==10){
    PlayMorse();
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

void PlayMorse(){
  for( size_t i=0; i<Command_position; i++)
  {
    PlayDashOrDot( Command[i] );
  }
  Command_position = 0;
}


void PlayDashOrDot( char c){
  switch(c){
    case '-':
      hwKeyboard.setLEDs( 1);
      tone(BEEPER_PIN, NOTE_B6, MORSE_DASH);
      delay(MORSE_DASH);
      hwKeyboard.setLEDs( 0);
      break;
    case '.':
      hwKeyboard.setLEDs( 2);
      tone(BEEPER_PIN, NOTE_B6, MORSE_DOT);
      delay(MORSE_DOT);
      hwKeyboard.setLEDs( 0);
      break;    
    case ' ':
      hwKeyboard.setLEDs( 4);
      delay(MORSE_PAUSE * 3);
      hwKeyboard.setLEDs( 0);
      break;    
    default:
      break;    
    }
    delay(MORSE_PAUSE);
}
