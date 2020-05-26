//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include <U8g2lib.h>
#include "LCDManager.hpp"
#include "CP1251_font.h"
static U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, LCD_CLOCK, LCD_DATA, LCD_CS, LCD_RESET);

//#define __DEBUG

const char LCD_WelcomeMessage1[] PROGMEM = "ЭЛЕКТРОНИКА МК-2090";
const char LCD_WelcomeMessage2[] PROGMEM = "Retro Calculator";
const char *const LCD_Message_Table[] PROGMEM = {
  LCD_WelcomeMessage1, 
  LCD_WelcomeMessage2
  };


//
// Inits LCD display
//
unsigned long LCDManager::init( void *components[]) {
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _io_buffer = _iom->getIOBuffer();

  // Set timer and attach to the led
  pinMode( LCD_LED_PIN, OUTPUT);
  ledcSetup(LCD_LEDC_CHANNEL_0, LCD_LEDC_BASE_FREQ, LCD_LEDC_TIMER_13_BIT);
  ledcAttachPin(LCD_LED_PIN, LCD_LEDC_CHANNEL_0);
  _ledcAnalogWrite(LCD_LEDC_CHANNEL_0, 0); // keep backlit off for now
  pinMode( LCD_POWER_PIN, OUTPUT);
  digitalWrite( LCD_POWER_PIN, HIGH); // display power on

  // perform init sequence and show splash
  u8g2.beginSimple();
  _graphics = u8g2.getBufferPtr();
  _tileHeight = u8g2.getBufferTileHeight();
  _tileWidth = u8g2.getBufferTileWidth();
  memset(_graphics, 0, SCR_BUFFER_SIZE);
  plotStringUTF8(  7, 14, LCD_Message_Table[0]);
  plotStringUTF8( 17, 27, LCD_Message_Table[1]);
  u8g2.updateDisplay();
  _dimLED( 0, ledBrightness, 60); // lit slowly

  #ifdef __DEBUG
  delay(SCR_SPLASH);  
  memset(_graphics, 0, SCR_BUFFER_SIZE);
  plotString(  0, 0, "123456789012345678901234567890");
  plotString(  0, 8, "123456789012345678901234567890");
  plotString(  0, 16, "123456789012345678901234567890");
  plotString(  0, 24, "123456789012345678901234567890");
  plotString(  0, 32, "123456789012345678901234567890");
  plotString(  0, 40, "123456789012345678901234567890");
  plotString(  0, 48, "123456789012345678901234567890");
  plotString(  0, 56, "123456789012345678901234567890");
  u8g2.updateDisplay();
  #endif

  return  keepAwake();
}

//
// Other inits are hidden behind the splash screen
//
void LCDManager::waitForEndSplash( unsigned long start, bool cls) {
  while( millis()-start < SCR_SPLASH) delay(50);
  memset( _buffer, _SP_, SCR_SIZE);
  memset( lineInversed, false, SCR_ROWS);
  if( cls) {
    _dimLED( ledBrightness, 0, 5); // dim before cleaning
    clearScreen( _SP_, true);
    redraw();
    _dimLED( 0, ledBrightness, 5);
  }
  keepAwake();
}

//
// Makes routine checks
//
unsigned long LCDManager::tick() {
  return _iom->lastInput;
}

//
// Writes a CP1251 character to screen
// Coordinates are in pixels, (0,0) - left top corner
//
uint8_t LCDManager::plotCharacter( uint8_t left, uint8_t top, uint8_t ch){
  if( left >= 128 || top >= 64) return left; // outside screen area
  uint8_t shift0 = left & 0x07;
  uint8_t shift1 = 8 - shift0;
  uint8_t hPosition = left>>3;  // 8 pixels per byte
  bool skipByte1 = shift1>6 || hPosition>=15; // nothing to pass to the right byte 
  size_t bPosition = top;
  bPosition <<= 4; 
  bPosition += hPosition;
  uint8_t *charPtr = (uint8_t *)CP1251_font + (ch<<3); // 8 bytes per character
  for( byte i=0; i<8; i++, bPosition+=16){
    byte t = *charPtr++;
    if( !t) continue; // no need to apply empty line
    if( bPosition >= SCR_BUFFER_SIZE) break;
    _graphics[bPosition] |= t>>shift0;
    if(skipByte1) continue;
    size_t j = bPosition+1;
    if( j >= SCR_BUFFER_SIZE) break;
    _graphics[j] |= t<<shift1;
  }
  return left+6;
}

//
// Writes a CP1251 string character to screen
// Coordinates are in pixels, (0,0) - left top corner
//
uint8_t LCDManager::plotString( uint8_t left, uint8_t top, byte *str, byte limit){
  if( top >= 64) return left;
  for( byte i=0; i<limit; i++){
    if( left >= 127) break;
    if( !(*str)) break;
    left = plotCharacter( left, top, *str++);
  }
  return left;
}

//
// Performs on-the-fly conversion to CP1251
// Coordinates are in pixels, (0,0) - left top corner
//
uint8_t LCDManager::plotStringUTF8( uint8_t left, uint8_t top, const char *str){
  convertToCP1251( _io_buffer, str, SCR_MAX_UTF);
  plotString(left, top, _io_buffer);  
}

//
// Brings the screen to low power mode
//
void LCDManager::sleepOn(){
  if( isAsleep) return;
  LEDOff();
  digitalWrite( LCD_POWER_PIN, LOW);
  isAsleep = true;
  #ifdef __DEBUG
  Serial.print("Gone to sleep: [");
  Serial.print( _cursor_column);
  Serial.print(", ");
  Serial.print( _cursor_row);
  Serial.println( "]");
  #endif  
}

//
// Wakes the screen up
//
void LCDManager::sleepOff(){
  if( !isAsleep) return;
  #ifdef __DEBUG
  Serial.print("Waking up: [");
  Serial.print( _cursor_column);
  Serial.print(", ");
  Serial.print( _cursor_row);
  Serial.println( "]");  
  #endif  
  digitalWrite( LCD_POWER_PIN, HIGH);
  u8g2.begin();
  //u8g2.enableUTF8Print();
  isAsleep = false;
  setRedrawAll(true);
  redraw();
  LEDOn();
}

//
// Brings the screen to low power mode
//
void LCDManager::LEDOff(){
  if( isAsleep) return;
  if( isLEDoff) return;
  #ifdef __DEBUG
  Serial.println("LED off");
  #endif
  _dimLED( ledBrightness, 0, 10);
  ledcDetachPin(LCD_LED_PIN);
  digitalWrite(LCD_LED_PIN, LOW);
  isLEDoff = true;
}

//
// Wakes the screen up
//
void LCDManager::LEDOn(){
  if( isAsleep) return;
  if( !isLEDoff) return;
  #ifdef __DEBUG
  Serial.println("LED on");
  #endif
  ledcAttachPin(LCD_LED_PIN, LCD_LEDC_CHANNEL_0);
  _dimLED( 0, ledBrightness, 10);
  isLEDoff = false;
}

//
// Set the LED brightness.
//
void LCDManager::setLED( byte v){
  if( v == ledBrightness) return;
  if( !isAsleep) _dimLED( ledBrightness, v);
  #ifdef __DEBUG
  Serial.print("Brightness changed to: ");
  Serial.println(v, HEX);
  #endif
  ledBrightness = v;
}

//
// Changes the LED brightness by v points (1/256 levels).
//
void LCDManager::changeLED( int16_t v){
  v += ((int16_t)ledBrightness & 0xFF);
  if( v < 0) v = 0;
  if( v > 255) v = 255;
  setLED( (byte)v);
}

//
// Lights or dims LED slowly in steps of 16/256 to the preset level.
// The larger the step in ms, the slower the dim.
//
void LCDManager::_dimLED( byte start_duty, byte stop_duty, byte step){
  int16_t duty = start_duty;
  while( true){
    if( duty == stop_duty) return;
    if(duty < stop_duty){
      duty += 16;
      if( duty > stop_duty) duty = stop_duty; 
    }
    else{
      duty -= 16;
      if( duty < stop_duty) duty = stop_duty;       
    }
    _ledcAnalogWrite( LCD_LEDC_CHANNEL_0, (byte)duty);
    if( duty != stop_duty) delay(step);      
    }
}

//
// Normalizes LED PWM to 256 levels.
//
void LCDManager::_ledcAnalogWrite(uint8_t channel, uint8_t value) {
  uint32_t duty = (LCD_LEDC_MAX_DUTY * value) >> 8;
  ledcWrite(channel, duty);
}

//
// Redraws the entire screen
//
void LCDManager::redraw() {
  if( isAsleep) return;
  int16_t x0, cp;
  byte c, tmp;
  uint8_t *ptr;
  cp = _cursor_position();
  tmp = _buffer[ cp];
  if( cursorShown){
    if( _blinked) _buffer[ cp] = '_';
    lineRedrawRequired[_cursor_row] = true;
  }
  #ifdef __DEBUG
  Serial.print( "Row redraws: [");  
  for( byte ln=0; ln<SCR_ROWS; ln++)
    Serial.write( lineRedrawRequired[ln]? 'x': ' ');
  Serial.println( "]");      
  #endif
  for( int16_t ln=0, y=ROW_OFFSET; ln<SCR_ROWS; ln++, y+=ROW_STEP){
    if( !lineRedrawRequired[ln]) continue;
    lineRedrawRequired[ln] = false;
    ptr = _graphics+(ln<<7);
    memset( ptr, 0, 128);
    plotString( COL_OFFSET, y, _buffer + ln*SCR_COLS, SCR_COLS);
    if( lineInversed[ ln]){
      for( size_t i=0; i<128; i++)
      ptr[i] = ~ptr[i];
    }
    u8g2.updateDisplayArea(0, ln, _tileWidth, 1);
  }
  _buffer[ cp] = tmp;
  uint32_t t = millis();
  if( t-_last_blinked < BLINKER_RATE) return;
  _last_blinked = t;
  _blinked = !_blinked;
}

//
// Clears the screen
//
void LCDManager::clearScreen(byte ch, bool clear_inversed) {
  cursorToTop();
  memset( _buffer, ch, SCR_SIZE);
  if( clear_inversed)
    memset( lineInversed, false, SCR_ROWS);
  setRedrawAll( true);
  if( forceRedraw) redraw();
}

//
// Clears one line
//
void LCDManager::clearLine(byte row, byte ch) {
  if( row >= SCR_ROWS) return;
  memset( _buffer + row*SCR_COLS, ch, SCR_COLS);
  lineRedrawRequired[ row] = true;
  if( forceRedraw) redraw();
}

//
// Clears to the end of line
//
void LCDManager::clearToEOL(byte ch) {
  memset( _buffer + _cursor_position(), ch, SCR_COLS-_cursor_column);
  lineRedrawRequired[ _cursor_row] = true;
  if( forceRedraw) redraw();
}

//
// Scrolls the screen up
//
void LCDManager::scrollUp(byte rows) {
  if( scrollLock) return; 
  if( !rows) return;
  if( rows >= SCR_ROWS){
    clearScreen();
    return;
  }
  int16_t offset = rows * SCR_COLS;
  int16_t amount = SCR_SIZE-offset;
  memmove(_buffer, _buffer+offset, amount);
  memset( _buffer+amount, _SP_, offset);
  setRedrawAll( true);
  if( forceRedraw) redraw();
}

//
// Scrolls the screen down
//
void LCDManager::scrollDown(byte rows) {
  if( scrollLock) return; 
  if( !rows) return;
  if( rows >= SCR_ROWS){
    clearScreen();
    return;
  }
  int16_t offset = rows * SCR_COLS;
  int16_t amount = SCR_SIZE-offset;
  memmove(_buffer+offset, _buffer, amount);
  memset( _buffer, _SP_, offset);
  setRedrawAll( true);
  if( forceRedraw) redraw();
}

//
// Sends one byte
//
void LCDManager::sendChar( byte c) {
  if( c == _TAB_){
    sendTab();
    return;
  }
  if( c == _CR_){
    sendCarriageReturn();
    return;
  }
  if( c < _SP_) return; // service characters;
  if( !wordWrap && _cursor_column >= SCR_COLS-1) return;
  _buffer[_cursor_position()] = c;
  lineRedrawRequired[_cursor_row] = true;
  cursorRight();
  if( forceRedraw) redraw();
}

//
// Sends a null-terminated string
//
void LCDManager::sendString( const byte *str, size_t limit) {
  bool tmp = forceRedraw;
  forceRedraw = false;
  for(size_t i=0; i<strlen((char *)str); i++){
    if( limit && i>=limit) break;
    sendChar( (byte)str[i]);
  }
  forceRedraw = tmp;
  if( forceRedraw) redraw();
}

//
// Sends a null-terminated string (buffer and cursor are not updated)
//
void LCDManager::sendStringUTF8( const char *str) {
  convertToCP1251( _io_buffer, str, SCR_MAX_UTF);
  sendString( _io_buffer);
}

void LCDManager::cursorRight(){
  _cursor_column++;
  if( _cursor_column < SCR_COLS) return;
  if( !wordWrap || (scrollLock && _cursor_row >= SCR_ROWS-1)){
    _cursor_column--;
    return;
  }
  lineRedrawRequired[_cursor_row] = true;
  _cursor_row++;
  _cursor_column = 0;
  if( _cursor_position() >= SCR_SIZE){
    scrollUp(1);
    _cursor_row = SCR_ROWS-1;
  }
}

void LCDManager::cursorLeft(){
  if( !_cursor_position()) return;
  if( !_cursor_column){
    if( !wordWrap) return;
    lineRedrawRequired[_cursor_row] = true;
    _cursor_row--;
    _cursor_column = SCR_COLS-1;
    return;
  }
  _cursor_column--;
}

void LCDManager::cursorUp(){
  lineRedrawRequired[_cursor_row] = true;
  if( !_cursor_row) return;
  _cursor_row--;
}

void LCDManager::cursorDown(){
  cursorTo( _cursor_column, _cursor_row++);
}

void LCDManager::cursorTo( byte column, byte row){
  if( column >= SCR_COLS) column = SCR_COLS-1;
  if( row >= SCR_ROWS) row = SCR_ROWS-1;
  lineRedrawRequired[_cursor_row] = (row != _cursor_row);
  _cursor_column = column;
  _cursor_row = row;
}

void LCDManager::sendDelete(){
  if( _cursor_column == SCR_COLS-1) return;
  byte *ptr = _buffer + _cursor_position();
  memmove( ptr, ptr+1, SCR_COLS-_cursor_column-1);
  _buffer[(_cursor_row+1)*SCR_COLS-1] = _SP_;
  if( forceRedraw) redraw();
}

void LCDManager::sendBackspace(){
  lineRedrawRequired[_cursor_row] = true;
  cursorLeft();
  sendDelete();
}

void LCDManager::sendCarriageReturn(){
  lineRedrawRequired[_cursor_row] = true;
  memset( _buffer + _cursor_position(), _SP_, SCR_COLS-_cursor_column);
  _cursor_column = 0;
  _cursor_row++;
  if( _cursor_row >= SCR_ROWS){
    _cursor_row--;
    scrollUp(1);
  }
  if( forceRedraw) redraw();
}

void LCDManager::sendTab(){
  lineRedrawRequired[_cursor_row] = true;
  do sendChar( _SP_);
  while( (_cursor_column % SCR_TABSTOP) > 0);
  if( forceRedraw) redraw();
}

void LCDManager::invertRow(byte row, bool invert){
  lineInversed[ row] = invert;
  lineRedrawRequired[row] = true;
  if( forceRedraw) redraw();
}
