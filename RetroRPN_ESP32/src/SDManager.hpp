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
#include "ProgramCode.hpp"
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
    const char *LastError = NULL;

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

    bool checkExists( const char *name);
    void listDir( char *name=NULL);
    bool deleteEntity( const char *name);
    bool openProgramFileRead( const char *name);
    bool openProgramFileWrite( const char *name);
    bool openDataFileRead( const char *name);
    bool openDataFileWrite( const char *name);
    uint16_t readln( byte *buff);
    uint16_t print( char *buff, bool cr=true);
    inline void closeFile(){
      _currentFile.close();
    };

  private:
    byte *_io_buffer;
    IOManager *_iom;
    Keywords *_kwds;
    Variables *_vars;
    ProgramCode *_code;
    MessageBox *_mbox;
    ExpressionParser *_epar;
    File _currentFile;

    void _checkSDPin();
    void _checkRoot();
    File _getCurrentDir();
    bool _detectSDCard();

    void _readBuffer( void *f);
    bool _readDouble( void *f, double *v);
    bool _writeDouble( void *f, double v);
    bool _readString( void *f, byte *buff, size_t limit);
    bool _writeString( void *f, byte *v);

    bool _locateBASICFile( const char *name);
    bool _locateExistingFile( const char *name);
    bool _cardCheckMantra();
    char *_stripFolders( const char *name);
    bool _nameLengthCheckMantra( size_t len);
    bool _lookForFileMantra1( char *tmpName);
    bool _lookForFileMantra2( char *tmpName);
    bool _formFileMantra( const char *name, char *dest);
    bool _checkDirectoryStructure( char *name);
};

#endif // SDMANAGER_HPP
