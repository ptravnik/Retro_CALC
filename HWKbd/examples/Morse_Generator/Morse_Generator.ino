#include "HWKbd.h"
#include "Morse_Code.h"

#define CONSOLE_BAUD 115200
#define COMM_LEN 1024
#define BEEPER_PIN 9
#define NOTE_C3  131
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_B6  1976
#define MORSE_DASH  200
#define MORSE_DOT   50
#define MORSE_PAUSE 80

//
// Comment this for the direct loop use
// At this point of time the timer interrupt does not work very well
// because the PROGMEM reading consumes a lot of cycles.
// On Arduino MEGA the function hwKeyboard.input() should be called in the loop()
//
// #define HARDWARE_INTERRUPT
//

char Command[COMM_LEN];
size_t Command_position = 0;

static HWKbd hwKeyboard;
static HWKbd_Encoder hwEncoder(HWKBD_Codepage_IMC, HWKBD_Macropage_IMC);


void setup()
{
  Serial.begin(CONSOLE_BAUD);
  Serial.println();
  hwKeyboard.connect(3,4,5,6,7, &hwEncoder);
  Serial.println("Morse generator connected:");
  Serial.println("Type letters or numbers");
  Serial.println("or press button #10 for Alarm!");
#ifdef HARDWARE_INTERRUPT
  SetKeyboardTimer();
#endif
}

//
// Sets timer1 interrupt for 10Hz
//
#ifdef HARDWARE_INTERRUPT

static volatile byte prevByte = 0; 

void SetKeyboardTimer(){
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 10 Hz increments
  OCR1A = 1563; // = (16*10^6) / (10*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts  
}

ISR(TIMER1_COMPA_vect){ //timer1 interrupt reads the keyboard at 10Hz frequency
  // Critical section - disable interrupts
  uint8_t oldSREG = SREG;
  cli();
  byte r = hwKeyboard.scan();
  if( !r || (r && r != prevByte)){
    if( prevByte) hwEncoder.encode( prevByte, 0);
    prevByte = r;
    SREG = oldSREG;
    return;     
  }
  if( !prevByte) prevByte = r;
  SREG = oldSREG;
}
#endif

void loop()
{
  char c = 0;
#ifndef HARDWARE_INTERRUPT
  if( hwKeyboard.input()){
    return;
  }
  delayMicroseconds(100);
#endif
  
  if( !hwKeyboard.available()){
    delayMicroseconds(100);
    return;
  }
  c = hwKeyboard.read();
  PlayDashOrDot( c);
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
      Serial.print("-");
      hwKeyboard.setLEDs( 1);
      tone(BEEPER_PIN, NOTE_B6, MORSE_DASH);
      delay(MORSE_DASH+MORSE_PAUSE);
      hwKeyboard.setLEDs( 0);
      break;
    case '.':
      Serial.print(".");
      hwKeyboard.setLEDs( 2);
      tone(BEEPER_PIN, NOTE_B6, MORSE_DOT);
      delay(MORSE_DOT+MORSE_PAUSE);
      hwKeyboard.setLEDs( 0);
      break;    
    case ' ':
      Serial.print(" ");
      hwKeyboard.setLEDs( 4);
      delay(MORSE_PAUSE * 3);
      hwKeyboard.setLEDs( 0);
      break;    
    case '_':
      Serial.println();
      break;
    default:
      Serial.write( c);
      break;    
    }
}
