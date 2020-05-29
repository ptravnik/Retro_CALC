//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

//
// Pin assignment:
//
// 04 - SD insert sensor (DET_A)
// 05 - VSPI CSO - SD Select (DAT3)
// 18 - VSPI CLK - SD Clock (CLK)
// 19 - VSPI MISO - SD out (DAT0)
// 23 - VSPI MOSI - SD command (CMD)
//

#ifndef SDMANAGER_HPP
#define SDMANAGER_HPP

#include <FS.h>
#include "Parsers.hpp"
#include "MessageBox.hpp"

// or use LED_BUILTIN=2 constant for built-in LED
#define SD_DETECT_PIN      4
#define SD_CSO             13
#define SD_CLOCK           18
#define SD_MISO            19
#define SD_MOSI            23
#define BOARD_LED_PIN      2

#define SD_CARD_OUT          255
#define SD_CARD_NOT_MOUNTED  254
#define PROGLINE_INCREMENT    10

class SDManager{
  public:
    volatile bool SDInserted = false;
    volatile bool SDMounted = false;
    volatile bool SDPrevMounted = false;
    volatile uint64_t cardSize = 0;

    unsigned long init(void *components[]);
    unsigned long tick();
    uint8_t cardType();
    inline unsigned long keepAwake(){
      return _iom->keepAwake();
    };
    void sleepOn();
    void sleepOff();
    void loadState();
    void saveState();
    void storeVariables();
    void storeConstants();
    void listDir();
    void readFile( const char * path);
    void writeFile( const char * path, const char * message);
    void readRPNStatus( byte *inp, byte *last_inp, uint16_t *pos);
    void writeRPNStatus( byte *inp, byte *last_inp, uint16_t pos);
    const char *SD_Message();
    void checkSDStatus();
    bool setPrevMounted(){
      if( SDPrevMounted == SDMounted) return true;
      SDPrevMounted = SDMounted;
      return false;
    }
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Variables *_vars;
    MessageBox *_mbox;
    ExpressionParser *_epar;
    void _checkSDPin();
    void _checkRoot();
    File _getCurrentDir();
    bool _detectSDCard();
    void _readBuffer( void *f);
    bool _readDouble( void *f, double *v);
    bool _writeDouble( void *f, double v);
    bool _readString( void *f, byte *buff, size_t limit);
    bool _writeString( void *f, byte *v);
    size_t _writeVariable(void *f, const char *fmt, size_t lineNumber, VariableToken vt);
};

#endif // SDMANAGER_HPP
