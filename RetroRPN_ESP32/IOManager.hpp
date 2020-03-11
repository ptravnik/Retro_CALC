//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

//
// Pin assignment:
//
// 01 - TX0
// 03 - RX0
// 16 - RX2 (serial to Pro Micro 8) Note Pro Micro 14, 15, 16 are SPI/burn
// 17 - TX2 (serial to Pro Micro 9)
// 25 - KBD CLK
// 26 - KBD RST
// 27 - KBD_LED CLK
// 32 - KBD_LED RST
// 33 - KBD DATA
// 35 - (input only) Pro Micro active
//

#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#define IO_RXD2 16
#define IO_TXD2 17
#define IO_PM_ACTIVE 35
#define HWKBD_CLK 25
#define HWKBD_RST 26
#define HWKBD_CLK 25
#define HWKBD_DATA 33
#define HWKBDLED_RST 32
#define HWKBDLED_CLK 27

#define SERIAL_HARD_BAUD_RATE 115200
#define SERIAL2_BAUD_RATE 38400
#define CIRCULAR_BUFFER_LENGTH 32
#define SERIALS_BOTH 3
#define SERIAL_ONLY 1
#define SERIAL2_ONLY 2
#define SERIAL_DUMMY 0

#define MOUSE_MOVE_HORIZONTAL 1
#define MOUSE_MOVE_VERTICAL   2
#define MOUSE_MOVE_WHEEL      3

class CircularBuffer{
  public:
    inline bool available(){
      return _in != _out;
      };
    void push( byte c);
    char pop();
  private:
    byte _in = 0;
    byte _out = 0;
    char _buffer[CIRCULAR_BUFFER_LENGTH];
};

class IOManager{
  public:
    bool isAsleep = true;
    bool serial_Active = true;
    bool serial2_Active = false;
    volatile bool HWKeyboardConnected = false;
    volatile unsigned long lastInput = 0;
    
    unsigned long init( byte *io_buffer);
    unsigned long tick();
    char input();
    
    void setKBDLEDs( bool L0=false, bool L1=false, bool L2=false, bool L3=false);
    void setKBDLEDs( byte val);
    void flashKBDLEDs();
    
    void sendChar( byte c, byte dest = SERIALS_BOTH, bool wait_for_host=true);
    void sendString( const char *str, size_t limit=0, byte dest = SERIALS_BOTH);
    void sendStringLn( const char *str, size_t limit=0, byte dest = SERIALS_BOTH);
    void sendStringUTF8( const char *str, byte dest = SERIALS_BOTH, bool wait_for_host=true);
    void sendStringUTF8Ln( const char *str, byte dest = SERIALS_BOTH);
    void sendLn( byte dest = SERIALS_BOTH);

    inline void sendStringUTF8(){
      sendStringUTF8( (const char *)_io_buffer);
      };    
    inline void sendStringUTF8Ln(){
      sendStringUTF8Ln( (const char *)_io_buffer);
      };    
    inline void sendString( const byte *str, size_t limit=0, byte dest = SERIALS_BOTH){
      sendString( (const char *)str, limit, dest);
    };
    inline void sendStringLn( const byte *str, size_t limit=0, byte dest = SERIALS_BOTH){
      sendStringLn( (const char *)str, limit, dest);
    };
    inline void sendStringUTF8( const byte *str, byte dest = SERIALS_BOTH){
      sendStringUTF8( (const char *)str, dest);
    };
    inline void sendStringUTF8Ln( const byte *str, byte dest = SERIALS_BOTH){
      sendStringUTF8Ln( (const char *)str, dest);
      };
    inline unsigned long keepAwake(){
      lastInput = millis();
      return lastInput;
    };

    void injectKeyboard( byte c);
    void injectKeyboard( byte *str);
    inline void injectKeyboard(){
      injectKeyboard(_io_buffer);
    };
    void injectMouseClick( bool left, bool right, bool middle);
    void injectMouseMove( byte direction, int8_t amount);
    
    void sleepOn();
    void sleepOff();
    inline bool PM_active(){
      return digitalRead(IO_PM_ACTIVE);
    };

    inline byte *getIOBuffer(){
      return _io_buffer;
    }
  private:
    CircularBuffer _input_buffer;  
    byte *_io_buffer;
    byte _CP1251_buffer[8]; // for on-the-fly keyboard conversions
    char _receiveFromSerial();
    bool _readNonlocking(char *ptr);
    char _sendToHost();
    bool _wait_PM_active( byte waits=500);
    void _print_io_buffer();
    inline bool _sendToSerial( byte dest){
      return dest & 1;
    };
    inline bool _sendToSerial2( byte dest){
      return (dest & 2) && digitalRead(IO_PM_ACTIVE);
    };
    inline void _wait_for_transmission( size_t sent){
      delayMicroseconds(sent*400);
    };
};

#endif // IOMANAGER_HPP
