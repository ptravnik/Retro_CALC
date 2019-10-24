#include "Arduino.h"
#include "HWKbd.h"

//
// Encoder constructor
//
HWKbd_Encoder::HWKbd_Encoder( const uint16_t *c, const byte *m){
  codepage = c;
  macropage = m;
}


//
// returns the number of bytes available in the buffer
//
byte HWKbd_Encoder::available( void){
  return _buflen;
}


//
// returns the next character from the buffer
//
char HWKbd_Encoder::read( void){
  if( !_buflen) return 0;
  char c = (char)_buffer[_tail++];
  _tail = _tail & (HWKBD_BUFFER_SIZE-1);
  _buflen--;
  return c;
}


//
// allows to look into the buffer without changing the counters
//
char HWKbd_Encoder::peek( void){
  if( !_buflen) return 0;
  return (char)_buffer[_tail];
}


//
// Converts a scan sharacter into a buffer entry
// returns the new mode
//
byte HWKbd_Encoder::encode( byte r, byte _mode){
  if( codepage==0 || macropage == 0) return _mode;
  size_t loc = ((r-1)<<3) + _mode;
  uint16_t code = pgm_read_word( &codepage[loc]);
#ifdef HWKBD_DEBUG_
  Serial.println("Encoder call");
  Serial.print("Location: ");
  Serial.println(loc);
  Serial.print("Code: ");
  Serial.println(code);
#endif
  _mode = (code & 0x7000) >> 12;
  if( code & 0x8000){
    char *ptr = (char *)(macropage + (code & 0x0FFF));
    while( _buflen <= HWKBD_BUFFER_SIZE){
      char c = pgm_read_byte(ptr++);
      if( !c) break;
      _buffer[_head++] = c;
      _head = _head & (HWKBD_BUFFER_SIZE-1);
      _buflen++; 
    }
    return _mode;
  }
  byte byte0 = (byte)((code & 0x0F00) >> 8);
  byte byte1 = (byte)((code & 0x00FF));
  if( byte0 && _buflen <= HWKBD_BUFFER_SIZE-2){
    _buffer[_head++] = byte0;
    _head = _head & (HWKBD_BUFFER_SIZE-1);
    _buffer[_head++] = byte1;
    _head = _head & (HWKBD_BUFFER_SIZE-1);
    _buflen += 2;
    return _mode;
  }
  if( byte1 && _buflen < HWKBD_BUFFER_SIZE){
    _buffer[_head++] = byte1;
    _head = _head & (HWKBD_BUFFER_SIZE-1);
    _buflen++;
  }
  return _mode;
}


void HWKbd::connect(int pKBDrst, int pKBDclk, int pKBDread, int pLEDrst, int pLEDclk, HWKbd_Encoder *enc){
  pinMode(pKBDrst, OUTPUT);
  pinMode(pKBDclk, OUTPUT);
  pinMode(pKBDread, INPUT);
  pinMode(pLEDrst, OUTPUT);
  pinMode(pLEDclk, OUTPUT);
  pinKBD_reset = pKBDrst;
  pinKBD_clock = pKBDclk;
  pinKBD_read  = pKBDread;
  pinLED_reset = pLEDrst;
  pinLED_clock = pLEDclk;
  _encoder = enc;
#ifdef HWKBD_FAST_PORTS
  _bitKBD_reset = digitalPinToBitMask(pinKBD_reset);
  _bitKBD_clock = digitalPinToBitMask(pinKBD_clock);
  _bitKBD_read  = digitalPinToBitMask(pinKBD_read);
  _bitLED_reset = digitalPinToBitMask(pinLED_reset);
  _bitLED_clock = digitalPinToBitMask(pinLED_clock);
  _prtKBD_reset = (volatile unsigned int *)portOutputRegister(digitalPinToPort(pinKBD_reset));
  _prtKBD_clock = (volatile unsigned int *)portOutputRegister(digitalPinToPort(pinKBD_clock));
  _prtKBD_read  = (volatile unsigned int *)portInputRegister(digitalPinToPort(pinKBD_read));
  _prtLED_reset = (volatile unsigned int *)portOutputRegister(digitalPinToPort(pinLED_reset));
  _prtLED_clock = (volatile unsigned int *)portOutputRegister(digitalPinToPort(pinLED_clock));
#endif
  isConnected = true;
  setLEDs( 0);
#ifdef HWKBD_DEBUG_
  Serial.println( "hwKeyboard connected, ports: ");
  Serial.print( pinKBD_reset);
  Serial.print( " ");
  Serial.print( pinKBD_clock);
  Serial.print( " ");
  Serial.print( pinKBD_read);
  Serial.print( " ");
  Serial.print( pinLED_reset);
  Serial.print( " ");
  Serial.println( pinLED_clock);
#endif
}

#ifndef HWKBD_FAST_PORTS
void HWKbd::_sendPulse(int pin){
  digitalWrite( pin, HIGH);
#ifdef HWKBD_PULSE_LEN
  delayMicroseconds(HWKBD_PULSE_LEN);
#endif
  digitalWrite( pin, LOW);
#ifdef HWKBD_PULSE_LEN
  delayMicroseconds(HWKBD_PULSE_LEN);
#endif
}
#endif

#ifdef HWKBD_FAST_PORTS
void HWKbd::setLEDs( byte n){
  if( !isConnected) return;
  // Critical section - disable interrupts
  uint8_t oldSREG = SREG;
  cli();
  *_prtLED_reset |= _bitLED_reset; 
  *_prtLED_reset &= ~_bitLED_reset; 
  for( uint8_t i=0; i<n; i++){
    *_prtLED_clock |= _bitLED_clock; 
    *_prtLED_clock &= ~_bitLED_clock;     
  }
  SREG = oldSREG;
}
#else
void HWKbd::setLEDs( byte n){
  if( !isConnected) return;
  _sendPulse(pinLED_reset);
  for( byte i=0; i<n; i++) 
    _sendPulse(pinLED_clock); 
}
#endif

void HWKbd::setMode(byte newmode){
  uint8_t leds[] = {0,8,2,4,1,9,3,5};
  if(_mode == newmode) return;  
  _mode = newmode;
  if( modeLEDsEnabled)
    setLEDs(leds[_mode]);
}

#ifdef HWKBD_FAST_PORTS
byte HWKbd::scan( void){
  if( !isConnected) return 0;
  byte Output = 0;
  // Critical section - disable interrupts
  uint8_t oldSREG = SREG;
  cli();
  *_prtKBD_reset |= _bitKBD_reset; 
  *_prtKBD_reset &= ~_bitKBD_reset; 
  for( uint8_t i=0; i<HWKBD_LAST_BUTTON; i++){
    if( *_prtKBD_read & _bitKBD_read){
      Output = i+1;
      break;
    }
    *_prtKBD_clock |= _bitKBD_clock; 
    *_prtKBD_clock &= ~_bitKBD_clock;     
  }
  SREG = oldSREG;
#ifdef HWKBD_DEBUG_
  if( !Output) return 0;
  Serial.print("Scanned: ");
  Serial.println( Output);
  delay(50);  
#endif
  return Output;
}
#else

//
// scan checks the button immediately and returns the scan code;
// the total read cycle is better than 60 microseconds if
// the fast port access is used
// this method is useful for games
//
byte HWKbd::scan( void){
  if( !isConnected) return 0;
  byte Output = 0;
  _sendPulse(pinKBD_reset);
  for( byte i=0; i<HWKBD_LAST_BUTTON; i++){
    if( digitalRead( pinKBD_read)){
      Output = i+1;
      break;
    }
    _sendPulse(pinKBD_clock);
  }
#ifdef HWKBD_DEBUG_
  if( !Output) return 0;
  Serial.print("Scanned: ");
  Serial.println( Output);
  delay(50);  
#endif
  return Output;
}
#endif

byte HWKbd::_debounce( void){
  byte r = scan();
  long t = millis();
  
  // no button pressed
  if( !r && !buttonPressed){
    return 0;
  }

  // button apparently not pressed, but it was
  // means the button is bouncing
  if( !r && buttonPressed){
    lastReleaseTime = t;
    buttonPressed = false;
    return lastScanResult;
  }
  
  // new press
  if( r && !buttonPressed){
    if( lastScanResult != r){
      lastScanResult = r;
      lastPressTime = t;
      buttonPressed = true;
      return 0;
    }
    else{
      buttonPressed = (t-lastReleaseTime) > HWKBD_BUTTON_REPEAT;
      if(buttonPressed) lastPressTime = t;
      return 0;      
    }
  }
  
  // holding the button
  if( r && buttonPressed){
    if( lastScanResult != r){
      byte tmp = lastScanResult;
      lastReleaseTime = t;
      lastPressTime = t;
      lastScanResult = r;
      return tmp;
    }
    if( (t-lastPressTime) > HWKBD_TYPEMATIC_DELAY){
      delay(HWKBD_BUTTON_REPEAT);
      return lastScanResult;
    }
  }
  return 0;
}

//
// input registers the key with proper debouncing
// the input is committed if the button is released
// or if the button is held down for time longer than
// the TYPEMATIC_DELAY
//
bool HWKbd::input( void){
  byte r = _debounce();
  if( r == 0) return false;
  lastButtonRegistered = r;
  if( r > HWKBD_LAST_CODEPAGE) return true;
  byte newMode = _encoder->encode( r, _mode);
  setMode(newMode);
  return true;
}
