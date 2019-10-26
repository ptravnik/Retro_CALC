#ifndef LCD_Console_h
#define LCD_Console_h

/*
  LCD_Console.h - User interface wrapper for LCD screens
  for use in the RPN calculator
  Copyright (c) 2019 Pavel Travnik.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Hardware Notes.
  
  The current version of LCD is a Black-on-Blue QC12864BW type
  with ST7920 driver, e.g.
  https://www.exploreembedded.com/wiki/images/7/77/QC12864B.pdf

  The 12864B Graphic LCD module is a 128 x 64 pixel LCD display with a blue backlight and white foreground.
  The display is fully programmable and can display a combination of both graphics and text.
  It can operate in both parallel and serial (SPI) modes which can be configured by the external pin PSB.
  In SPI mode only 3 data pins are required to drive this display.
  No potentiometer is required to set the contrast as this is pre set by the factory to optimum level.

  It communicates with Arduino MEGA via hardware SPI interface:
  1. Vss (BLK) - ground (will combine with LED- later)  
  2. Vdd (RED) - connect to +5 V on Mega or to Vin on ESP-32
  3. RS/SS (YEL) - Mega SS / chip select pin 53
  4. RW (BRN) - Mega MOSI pin 51 (Mega MISO not used)
  5. E (GRN) - Mega CLK pin 52
  6.-13. DB0 to DB8 - not used
  14. PSB (ORA) - must be ground to force SPI mode
  15. RESET (WHI) - Mega pin 50
  16. VOUT - not used
  17. LED+ (MAG) - connects to +5 V with a 330R resistor; potentially will use PWM
  18. LED- (BLU) - ground
 
  Later we evaluate parallel interfacing - it must be faster
*/

// uncomment for debugging
//#define LCD_DEBUG_

// Controller delay 72 us
#define LCD_CONTROLLER_DELAY 72
#define LCD_RESET 50
#define LCD_CS_PIN 53
#define LCD_SCREEN_ROWS 8
#define LCD_SCREEN_COLUMNS 25
#define LCD_BLINK_RATE 400

// Buffer length must acommodate for Unicode in each position plus the trailing zero
// (2*LCD_SCREEN_COLUMNS + 1)
#define LCD_TEXT_BUFFER_LENGTH 256
#define LCD_LINE_LENGTH 51
#define LCD_SCROLL_BUFFER 2040

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

static char *ConvertDouble( double n, char *buff, byte decimal_places=6);

class LCD_Console{    
  public:
    bool isConnected = false;
    bool duplicateToSerial = true;
    long lastUpdateMillis = 0L;
    long lastBlinkMillis = 0L;
    bool cursorOn = false;
    size_t screen_position = 0;
    void *connect(  void *xram);
    void splash( const byte* line1, const byte* line2);
    void display_P(uint8_t x, uint8_t y, const byte *msg);
    void print_P( const byte *msg, bool serial_only=false);
    void println_P( const byte *msg, bool serial_only=false);
    void displayRPN( char* stackX, double *stack, char* labels[]);
    void displayStackX( char* stackX, size_t cursor_Position);
    void append( byte *msg, bool reset=true, bool unicode_clip=false);
    void append_P( const byte *msg, bool reset=true, bool unicode_clip=false);
    inline bool wantsToBlink(){ return (millis() - lastBlinkMillis) > LCD_BLINK_RATE;};
    inline bool flopCursor(){ cursorOn = !cursorOn;};
  private:
    volatile char *_buffer;
};

#endif
