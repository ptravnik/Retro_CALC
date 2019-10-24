#include "Arduino.h"
#include "LCD_Console.h"

extern U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, LCD_CS_PIN, LCD_RESET);

//
// Console connection:
// allocates memory for PROGMEM reading buffer
// returns the next available slot in xram
//
void *LCD_Console::connect( void *xram){
  _buffer = reinterpret_cast<volatile byte*>(xram);
  u8g2.begin();
  u8g2.enableUTF8Print();
  isConnected = true;
  return xram + LCD_TEXT_BUFFER_LENGTH;
}


//
// Shows splash screen
//
void LCD_Console::splash( const byte* line1, const byte* line2){
  if( !isConnected) return;
  u8g2.firstPage();  
  do {
    u8g2.setFont( u8g2_font_6x12_t_cyrillic);
    displayPROGMEM(  7, 24, line1);
    u8g2.setFont( u8g2_font_5x8_t_cyrillic);
    displayPROGMEM( 25, 33, line2);
  }while ( u8g2.nextPage() );
  lastUpdateMillis = millis();
}


//
// Prints a message from PROGMEM in any screen location
//
void LCD_Console::displayPROGMEM(uint8_t x, uint8_t y, const byte *msg){
  if( !isConnected) return;
  append_from_PROGMEM( msg, true, false);
  u8g2.drawUTF8(x,y,_buffer);
}


//
// Prints a static message from PROGMEM
//
void LCD_Console::printPROGMEM( const byte *msg, bool serial_only=false){
  if(!isConnected) return;
  append_from_PROGMEM( msg, true, false);
  if( duplicateToSerial || serial_only) Serial.print( (char*)_buffer);
  if(serial_only) return;
  
}

void LCD_Console::printlnPROGMEM( const byte *msg, bool serial_only=false){
  printPROGMEM( msg);
  if( duplicateToSerial || serial_only) Serial.println();
}


//
// Appends a PROGMEM massage into a memory location;
// Trims the string to prevent screen overflow
// Returns the total number of bytes in the string
//
void LCD_Console::append_from_PROGMEM( const byte *msg, bool reset, bool unicode_clip){
  if(!isConnected) return;
  size_t i = 0;
  byte c;
  if( reset) screen_position = 0;
  else i=strlen( _buffer);
  while( i<LCD_TEXT_BUFFER_LENGTH-1){
    c = pgm_read_byte( msg++ );
    _buffer[i++] = (char)c;
    if( !c) break;
    if( c != 208 && c != 209) screen_position++;
    if( unicode_clip && screen_position >= LCD_SCREEN_COLUMNS){
       _buffer[i++] = '>';
       _buffer[i++] = 0;
       break;
    }
  }
  _buffer[LCD_TEXT_BUFFER_LENGTH-1] = 0; // prevents buffer overflow
}
