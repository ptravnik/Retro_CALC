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

    size_t loadBinary( const char *name, byte *buff, size_t minSize, size_t maxSize);
    bool saveBinary( const char *name, byte *buff, size_t Size);

    void readFile( const char * path);
    void writeFile( const char * path, const char * message);
    const char *SD_Message();
    void checkSDStatus();
    bool setPrevMounted(){
      if( SDPrevMounted == SDMounted) return true;
      SDPrevMounted = SDMounted;
      return false;
    }

    void checkRootExists();
    bool checkEntityExists( const char *name);
    void listFolder( char *name=NULL);
    bool deleteEntity( const char *name);
    void createFolder( char *name);

    bool openProgramFileRead( const char *name);
    bool openProgramFileWrite( const char *name);
    bool openDataFileRead( const char *name);
    bool openDataFileWrite( const char *name);
    uint16_t readln( byte *buff);
    uint16_t print( char *buff, bool cr=true);
    inline void closeFile(){
      _currentFile.close();
    };
    bool readDouble( double *v);
    bool writeDouble( double v);
    size_t readString( byte *buff, size_t limit);
    bool writeString( byte *v);
    bool writeSettingNumber( const char *title, double value);
    bool writeSettingString( const char *title, byte *value);

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
    File _getCurrentDir();
    bool _detectSDCard();

    bool _locateBASICFile( const char *name);
    bool _locateExistingFile( const char *name);
    bool _cardCheckMantra();
    char *_stripFolders( const char *name);
    bool _nameLengthCheckMantra( size_t len);
    bool _lookForFileMantra1( char *tmpName);
    bool _lookForFileMantra2( char *tmpName);
    bool _formFileMantra( const char *name, char *dest);
    bool _checkFolderStructure( char *name);
};

#endif // SDMANAGER_HPP
