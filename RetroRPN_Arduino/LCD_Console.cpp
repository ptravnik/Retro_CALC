#include "Arduino.h"
#include "LCD_Console.h"
#include "RRPN_Parser.h"

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
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont( u8g2_font_6x12_t_cyrillic);
  display_P(  7, 24, line1);
  u8g2.setFont( u8g2_font_5x8_t_cyrillic);
  display_P( 25, 33, line2);
  u8g2.sendBuffer();          // transfer internal memory to the display
  lastUpdateMillis = millis();
}


//
// Prints a message from PROGMEM in any screen location
//
void LCD_Console::display_P(uint8_t x, uint8_t y, const byte *msg){
  if( !isConnected) return;
  append_P( msg, true, false);
  u8g2.drawUTF8(x,y,_buffer);
}


//
// Prints a message from RAM to any screen location
//
//void LCD_Console::display(uint8_t x, uint8_t y, char *msg, byte font){
//  if( !isConnected) return;
//  
//  u8g2.drawUTF8(x,y,msg);
//}


//
// Shows the RPN screen
//
void LCD_Console::displayRPN( char* stackX, double *stack, char* labels[]){
  if( !isConnected) return;
  u8g2.clearBuffer();
  u8g2.setFont( u8g2_font_7x13_t_cyrillic);
  u8g2.drawUTF8( 8, 20, Parser.DoubleToString( stack[2], _buffer));
  u8g2.drawUTF8( 8, 41, Parser.DoubleToString( stack[1], _buffer));
  if( !stackX[0])
      u8g2.drawUTF8( 8, 62, Parser.DoubleToString( stack[0], _buffer));
  u8g2.setFont( u8g2_font_5x8_t_cyrillic);
  u8g2.drawUTF8( 0,  7, labels[2]);
  u8g2.drawUTF8( 0, 28, labels[1]);
  u8g2.drawUTF8( 0, 49, labels[0]);
  u8g2.sendBuffer();
  lastUpdateMillis = millis();     
}


//
// Shows the content of the X register (for editing)
//
void LCD_Console::displayStackX( char* stackX, size_t cursor_Position){
  if( !isConnected) return;
  if( !stackX[0]) return;
  u8g2.setFont( u8g2_font_7x13_t_cyrillic);
  uint8_t tw = u8g2.getBufferTileWidth();
  uint8_t th = u8g2.getBufferTileHeight();
  uint8_t *ptr = u8g2.getBufferPtr() + tw*50;
  uint16_t clearTiles = tw*14;
  memset( ptr, 0, clearTiles);
  if( cursorOn){
    if( cursor_Position == strlen( stackX)){
      append((byte *)stackX);
      append("_", false);
    }
    else{
      char tmp = stackX[cursor_Position];
      stackX[cursor_Position] = '_';
      append((byte *)stackX);
      stackX[cursor_Position] = tmp;
    } 
  }
  else{
      append((byte *)stackX);    
  }
  u8g2.drawUTF8( 8, 62, _buffer);
  u8g2.updateDisplayArea(0, th-2, tw, 2);
  lastBlinkMillis = millis();
  lastUpdateMillis = lastBlinkMillis;
}


//
// Prints a static message from PROGMEM
//
void LCD_Console::print_P( const byte *msg, bool serial_only=false){
  if(!isConnected) return;
  append_P( msg, true, false);
  if( duplicateToSerial || serial_only) Serial.print( (char*)_buffer);
  if(serial_only) return;
  
}
void LCD_Console::println_P( const byte *msg, bool serial_only=false){
  print_P( msg);
  if( duplicateToSerial || serial_only) Serial.println();
}


//
// Appends a string;
// Trims the string to prevent screen overflow
// Returns the total number of bytes in the string
//
void LCD_Console::append( byte *msg, bool reset=true, bool unicode_clip=false){
  if(!isConnected) return;
  size_t i = 0;
  byte c;
  if( reset) screen_position = 0;
  else i=strlen( _buffer);
  while( i<LCD_TEXT_BUFFER_LENGTH-1){
    c = *msg++;
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


//
// Appends a PROGMEM massage into a memory location;
// Trims the string to prevent screen overflow
// Returns the total number of bytes in the string
//
void LCD_Console::append_P( const byte *msg, bool reset=true, bool unicode_clip=false){
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
