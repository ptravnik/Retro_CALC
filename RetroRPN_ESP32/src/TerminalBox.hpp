//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  TerminalBox.hpp 
  Implements the display portion of the BASIC console
*/

#ifndef TERMINALBOX_HPP
#define TERMINALBOX_HPP

#include "RPNStackBox.hpp"

#define VIRTUAL_SCREEN_COLS  81
#define VIRTUAL_SCREEN_ROWS  40
#define VIRTUAL_SCREEN_END1  3239
#define VIRTUAL_SCREEN_END2  3158

class TerminalBox{
  public:
    unsigned long init(void *components[]);
    void show();
    void redraw();
    void updateIOM( bool refresh=true);
    
    void sendChar( byte c, byte dest = SERIAL_DUMMY, bool wait_for_host=false);
//    void sendString( const char *str, size_t limit=0, byte dest = SERIALS_BOTH);
//    void sendStringLn( const char *str, size_t limit=0, byte dest = SERIALS_BOTH);
//    void sendStringUTF8( const char *str, byte dest = SERIALS_BOTH, bool wait_for_host=true);
//    void sendStringUTF8Ln( const char *str, byte dest = SERIALS_BOTH);
//    void sendLn( byte dest = SERIALS_BOTH);
//    inline void sendStringUTF8(){
//      sendStringUTF8( (const char *)_io_buffer);
//      };    
//    inline void sendStringUTF8Ln(){
//      sendStringUTF8Ln( (const char *)_io_buffer);
//      };    
//    inline void sendString( const byte *str, size_t limit=0, byte dest = SERIALS_BOTH){
//      sendString( (const char *)str, limit, dest);
//    };
//    inline void sendStringLn( const byte *str, size_t limit=0, byte dest = SERIALS_BOTH){
//      sendStringLn( (const char *)str, limit, dest);
//    };
//    inline void sendStringUTF8( const byte *str, byte dest = SERIALS_BOTH){
//      sendStringUTF8( (const char *)str, dest);
//    };
//    inline void sendStringUTF8Ln( const byte *str, byte dest = SERIALS_BOTH){
//      sendStringUTF8Ln( (const char *)str, dest);
//      };

  private:
    byte *_io_buffer;
    IOManager *_iom;
    LCDManager *_lcd;
    ExpressionParser *_epar;
    byte _virtualScreen[VIRTUAL_SCREEN_COLS * VIRTUAL_SCREEN_ROWS];
    size_t _vs_input_column = 0;
    size_t _vs_bottom_row = 0;
};

#endif //TERMINALBOX_HPP
