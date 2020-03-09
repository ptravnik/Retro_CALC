#ifndef HWKbd_h
#define HWKbd_h

/*
  HWKbd.h - Hardware keyboard for Arduino with an SPI-like interface
  Copyright (c) 2019 Pavel Travnik.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Hardware Notes.
  
  The keyboard communicates with Arduino via J1:
  1. POWER - connect to +5 V on Mega or +3.3V on Duo
  2. KBD_RESET - output, any digital pin
  3. KBD_CLOCK - output, any digital pin
  4. KBD_READ - input (no pullup!), any digital pin
  5. LED_RESET - output, any digital pin
  6. LED_CLOCK - output, any digital pin
  7. RESET - connect to Arduino RESET (optional)
  8. GROUND
  To read the keyboard, send a pulse to KBD_RESET and read
  the first button state from KBD_READ (HIGH means "pressed").
  Then, continue sending pulses on KBD_CLOCK to read buttons
  2, 3, 4, etc.
  To set the LEDs, send a pulse to LED_RESET and cycle
  LED_CLOCK from 0 to 15 times, which corresponds to 16 binary states.
  Note that the software presimes that no PWM is sent to the keyboard ports!

  Software Notes.
  The keyboard communicates in UTF-8. The current implementation supports
  8 reading modes:
  0. English character set, small letters, numbers
  1. SHIFT. English character set, capital letters, punctuation marks.
  2. F1 - additional letters and macros in English
  3. F2 - additional letters and macros in English
  4. LANG - national language mode (currently only Cyrillic is implemented)
  5. SHIFT+LANG - capital letters for Cyrillic
  6. F1+LANG - additional letters and macros in national language
  7. F2+LANG - additional letters and macros in national language
  
  The key codes are composed of 16-bit integers, 8 numbers per button
  (by the number of modes). The bit pattern is as following:
  0MMM0000 cccccccc
  1MMMaaaa aaaaaaaa
  MMM - the next mode (from 0 to 7)
  cccccccc - a single-byte Unicode character (if >128, it is a control character, such as an arrow)
  aaaaa aaaaaaaaa - a 12-bit shift in the macros table;
  the latter is composed of null-terminated strings, up to 4k in length.
*/

// uncomment for debugging
//#define HWKBD_DEBUG_

// delay before the repeat starts and the repeat rate in ms
#define HWKBD_TYPEMATIC_DELAY 200
#define HWKBD_BUTTON_REPEAT 50

// pulse width in microseconds
#define HWKBD_PULSE_LEN 1

// keyboard circular buffer;
// must be a power of 2 and less than 256
#define HWKBD_BUFFER_SIZE 32

// number of buttons (maximum is 80)
#define HWKBD_LAST_BUTTON 59
#define HWKBD_LAST_CODEPAGE 59


#include <Arduino.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
// comment out for standard Arduino library writing
// (including timer disable on PWM)
// will slow down everything by a factor of 20
#define HWKBD_FAST_PORTS
#else
#include <pgmspace.h>
// ESP32 does not support fast ports
#undef HWKBD_FAST_PORTS
#endif


class HWKbd_Encoder
{  
  public:
    const uint16_t *codepage = 0;
    const byte *macropage = 0;
    HWKbd_Encoder( const uint16_t *c, const byte *m);
    byte encode( byte r, byte _mode);
    byte available(void);
    char read(void);
    char peek(void);
  private:
    volatile char _buffer[HWKBD_BUFFER_SIZE];
    volatile uint8_t _head = 0;
    volatile uint8_t _tail = 0;
    volatile uint8_t _buflen = 0;
};


class HWKbd
{    
  public:
    bool isConnected = false;
    int pinKBD_reset = -1;
    int pinKBD_clock = -1;
    int pinKBD_read  = -1;
    int pinLED_reset = -1;
    int pinLED_clock = -1;
    volatile bool modeLEDsEnabled = true;
    volatile byte lastScanResult = 0;
    volatile long lastReleaseTime = 0L;
    volatile long lastPressTime = 0L;
    volatile bool buttonPressed = false;
    volatile char lastButtonRegistered = 0;
    void connect(int pKBDrst, int pKBDclk, int pKBDread, int pLEDrst, int pLEDclk, HWKbd_Encoder *enc);
    void setLEDs( byte n);
    void setMode( byte newmode);
    byte scan( void);
    bool input( void);
    inline byte available(void){ return _encoder->available();};
    inline char read(void){ return _encoder->read();};
    inline char peek(void){ return _encoder->peek();};

  private:
    volatile uint8_t _mode = 0;
#ifdef HWKBD_FAST_PORTS
    uint8_t _bitKBD_reset;
    uint8_t _bitKBD_clock;
    uint8_t _bitKBD_read;
    uint8_t _bitLED_reset;
    uint8_t _bitLED_clock;
    volatile unsigned int *_prtKBD_reset;
    volatile unsigned int *_prtKBD_clock;
    volatile unsigned int *_prtKBD_read;
    volatile unsigned int *_prtLED_reset;
    volatile unsigned int *_prtLED_clock;
#else
    void _sendPulse(int pin);
#endif
    HWKbd_Encoder *_encoder; 
    byte _debounce( void);
};

#endif
