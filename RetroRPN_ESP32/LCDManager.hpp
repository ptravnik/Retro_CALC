//
// Pin assignment:
//
// 12 - LCD Data (R/W)
// 13 - LCD Reset (RESET)
// 14 - LCD Clock (E)
// 15 - LCD CS (RS)
// 16 - LCD POWER (this model does not have sleep)
// 17 - LCD LED PWM
//

#ifndef LCDMANAGER_HPP
#define LCDMANAGER_HPP

#include <Arduino.h>
#include <U8g2lib.h>
#include "CP1251_mod.h"

// fade LED PIN (replace with LED_BUILTIN=2 constant for built-in LED)
// LCD PIN controls power for LCD
#define LCD_DATA           12
#define LCD_RESET          13
#define LCD_CLOCK          14
#define LCD_CS             15
#define LCD_POWER_PIN      16
#define LCD_LED_PIN        17

// use first channel of 16 channels (started from zero)
// use 13 bit precission for LEDC timer
// use 5000 Hz as a LEDC base frequency
#define LCD_LEDC_CHANNEL_0     0
#define LCD_LEDC_TIMER_13_BIT  13
#define LCD_LEDC_MAX_DUTY      8191
#define LCD_LEDC_BASE_FREQ     5000

// screen geometry
#define SCR_MARGIN_X  127
#define SCR_MARGIN_Y  63
#define SCR_COLS      22
#define SCR_ROWS      8
#define SCR_TOP       0
#define SCR_BOTTOM    7
#define SCR_RIGHT     21
#define SCR_TABSTOP   2
#define SYMBOL_WIDTH  6
#define SYMBOL_HEIGHT 8
#define ROW_STEP      8
#define ROW_OFFSET    0
#define COL_OFFSET    0
#define SCR_SIZE (SCR_COLS * SCR_ROWS)
#define SCR_MAX_UTF (SCR_COLS + SCR_COLS + 1)
#define SCR_BUFFER_SIZE 1024

// cursor blinker rate in ms
#define BLINKER_RATE 500

// show splashscreen in ms
#define SCR_SPLASH  5000 

class LCDManager{
  public:
    bool isAsleep = false;
    bool cursorShown = true;
    bool wordWrap = false;
    bool scrollLock = false;
    bool lineRedrawRequired[ SCR_ROWS];
    bool lineInversed[ SCR_ROWS];
    bool forceRedraw = false;
    byte ledBrightness = 200;

    void init( bool cls = false);
    void sleepOn();
    void sleepOff();
    inline void setRedrawAll(bool r){
      memset( lineRedrawRequired, r, SCR_ROWS);
    }
    void redraw();
    void clearScreen( byte ch=_SP_, bool clear_inversed=false);
    void clearLine( byte row, byte ch=_SP_);
    void clearToEOL(byte ch=_SP_);
    void scrollUp( byte rows);
    void scrollDown( byte rows);
    void sendChar( byte c);
    void sendString( const byte *c, size_t limit=0);
    inline void sendString( const char *c, size_t limit=0){
      sendString( (const byte *)c, limit);
    };
    void sendStringUTF8( const char *c);
    inline void cursorToTop(){
      cursorTo( 0, 0);        
    };
    inline void cursorToBottom(){
      cursorTo( 0, SCR_BOTTOM);        
    };
    void cursorRight();
    void cursorLeft();
    void cursorUp();
    void cursorDown();
    void cursorTo( byte column, byte row);
    void sendCarriageReturn();   
    void sendTab();   
    void sendDelete();   
    void sendBackspace();
    void invertRow( byte row, bool invert);
    inline byte *getTextBuffer(){
      return _buffer;
      };  
    inline char *getUnicodeBuffer(){
      return _buffer_Unicode;
      };  
    void setLED( byte v);
    void changeLED( int16_t v);
    uint8_t writeCharacter( uint8_t left, uint8_t top, uint8_t ch);
    uint8_t writeString( uint8_t left, uint8_t top, byte *str, byte limit=SCR_COLS);
    inline uint8_t writeString( uint8_t left, uint8_t top, char *str, byte limit=SCR_COLS){
      writeString( left, top, (byte *)str, limit);
    };
    uint8_t writeStringUTF8( uint8_t left, uint8_t top, const char *str);
  private:
    bool blinked = false;
    uint32_t last_blinked = 0;
    byte _buffer[SCR_SIZE];
    char _buffer_Unicode[SCR_MAX_UTF];
    uint8_t _tileHeight = 0;
    uint8_t _tileWidth = 0;
    uint8_t *_graphics;
    uint16_t cursor_column = 0;
    uint16_t cursor_row = 0;
    inline size_t cursor_position(){
      return cursor_row * SCR_COLS + cursor_column;
      };
    void dimLED( byte start_duty, byte stop_duty, byte step=30);
    void ledcAnalogWrite(uint8_t channel, uint8_t value);
};

#endif // LCDMANAGER_HPP
