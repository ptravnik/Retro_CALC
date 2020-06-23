//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include <SD.h>
//#include <SPI.h> (included with Arduino.h)
#include "SDManager.hpp"

//#define __DEBUG


const char RPN_SaveStatusFile[] PROGMEM = "/_RPN_SaveStatus.txt";
const char RPN_SaveStatusFile2[] PROGMEM = "/_RPN_SaveStatus.bin";
const char RPN_SaveConstantsFile[] PROGMEM = "/_RPN_Constants.txt";
const char RPN_AutoexecFile[] PROGMEM = "/autotest.bas";

const char SD_Error_NotInserted[] PROGMEM = "No SD card";
const char SD_Error_NotMounted[] PROGMEM = "SD not mounted";
const char SD_Error_FileNameTooLong[] PROGMEM = "Name too long";
const char SD_Error_FileNotFound[] PROGMEM = "File not found";
const char SD_Error_FileAccessError[] PROGMEM = "Cannot access file";
const char SD_Error_OutOfMemory[] PROGMEM = "Out of memory";
const char SD_Error_DeleteFailed[] PROGMEM = "Delete failed";

const char SD_Message0[] PROGMEM = "+ SD inserted";
const char SD_Message1[] PROGMEM = "+ SD removed";
const char SD_Message2[] PROGMEM = "+ SD mounted";
const char SD_Message3[] PROGMEM = "+ SD mount failed";
const char SD_Message4[] PROGMEM = "+ SD size: %llu MB";
const char SD_Message_Mounted[] PROGMEM = "SD mounted";
const char SD_Message_Removed[] PROGMEM = "SD removed";
const char SD_Error_WriteFailed[] PROGMEM = "Err: SD failure";
const char *const SD_Message_Table[] PROGMEM = {
  SD_Message0,
  SD_Message1,
  SD_Message2,
  SD_Message3,
  SD_Message4
  };
const char SD_root[] PROGMEM = "/";
const char ConstantFileFormat[] PROGMEM = "%05d CONST ";
const char SD_DefaultExt1[] PROGMEM = ".bas";
const char SD_DefaultExt2[] PROGMEM = ".BAS";

//
// Timer to check the SD status
//
static volatile bool SDCheckRequested = false;
static void IRAM_ATTR isrSD() {
  SDCheckRequested = true;
}

void SDManager::_checkSDPin(){
  if( !SDCheckRequested) return;
  SDCheckRequested = false;
  _detectSDCard();
}

bool SDManager::_detectSDCard(){
  bool inserted = !digitalRead(SD_DETECT_PIN);
  delay(30);
  inserted = inserted && (!digitalRead(SD_DETECT_PIN));
  if( SDInserted == inserted) return inserted;
  SDInserted = inserted;
  _iom->sendStringUTF8Ln( SD_Message_Table[ SDInserted? 0: 1]);
  cardSize = 0;
  if( !inserted){
    if( SDMounted){
      SD.end();
      SDMounted = false;
    }
    return false;
  }
  SDMounted = SD.begin();
  _iom->sendStringUTF8Ln( SD_Message_Table[ SDMounted? 2: 3]);
  cardSize = 0;
  if( !SDMounted) return inserted;
  cardSize = SD.cardSize() >> 20;
  sprintf( (char *)_io_buffer, SD_Message_Table[4], cardSize);
  _iom->sendStringUTF8Ln( (char *)_io_buffer);
  #ifdef __DEBUG  
  Serial.printf("SD Card Size: %llu MB\n", cardSize);
  #endif
  _checkRoot();
  keepAwake();
  return inserted;  
}

//
// Init and status update
//
unsigned long SDManager::init(void *components[]){
  _iom = (IOManager *)components[UI_COMP_IOManager];
  _kwds = (Keywords *)components[UI_COMP_Keywords];
  _vars = (Variables *)components[UI_COMP_Variables];
  _code = (ProgramCode *)components[UI_COMP_ProgramCode];
  _mbox = (MessageBox *)components[UI_COMP_MessageBox];
  _epar = (ExpressionParser *)components[UI_COMP_ExpressionParser];
  _io_buffer = _iom->getIOBuffer();
  pinMode(SD_DETECT_PIN, INPUT_PULLUP);   // sets the digital pin for SD check
  _detectSDCard();
  SDPrevMounted = SDMounted;
  SDCheckRequested = false;
  attachInterrupt( SD_DETECT_PIN, isrSD, CHANGE);
  return keepAwake(); 
}

//
// Returns a message about the SD status
//
const char *SDManager::SD_Message(){
  if( SDMounted) return SD_Message_Mounted;
  return SD_Message_Removed;
}

unsigned long SDManager::tick(){
  _checkSDPin();
  return _iom->lastInput;
}

void SDManager::checkSDStatus(){
  if(setPrevMounted()) return;
  _mbox->setLabel(SD_Message());
}

uint8_t SDManager::cardType(){
  if(!SDInserted) return SD_CARD_OUT;
  if(!SDMounted) return SD_CARD_NOT_MOUNTED;
  return SD.cardType();
}

void SDManager::_checkRoot(){
  if(!SDInserted) return;
  if(!SDMounted) return;
  File root = SD.open(_vars->currentDir);
  if(!root){
    #ifdef __DEBUG
    Serial.println("Failed to open directory, reset to root");
    #endif
    strncat2( _vars->currentDir, SD_root, CURRENT_DIR_LEN);
    return;
  }
  if(!root.isDirectory()){
    #ifdef __DEBUG
    Serial.println("Not a directory, reset to root");
    #endif
    strncat2( _vars->currentDir, SD_root, CURRENT_DIR_LEN);
  }
  root.close();
}

File SDManager::_getCurrentDir(){
  _checkRoot();
  return SD.open(_vars->currentDir);
}

void SDManager::sleepOn(){
  if( !SDMounted) return;
  SD.end();
  SDMounted = false;
}

void SDManager::sleepOff(){
  SDInserted = false;
  SDMounted = false;
  _detectSDCard();
}

bool SDManager::checkExists( const char *name){
  if( _cardCheckMantra()) return false;
  return SD.exists( name);
}

bool SDManager::deleteEntity( const char *name){
  if( _cardCheckMantra()) return true;
  Serial.print( "Deleting: ");
  Serial.println( name);
  if( strlen(name) == 1 && name[0] == '/') return false;
  if( !SD.exists( name)) return false;
  File root = SD.open( name);
  bool dir = root.isDirectory();
  if( dir){
    File file = root.openNextFile();
    while(file){
      if( deleteEntity( file.name())) break;;
      file = root.openNextFile();
    } // while
  }//if
  root.close();
  bool res = dir? SD.rmdir(name): SD.remove( name);
  if( !res){
    LastError = SD_Error_DeleteFailed;
    return true;
  }
  return false;
}

//
// TODO: load and save functionality here
//

static uint16_t _ReadLn_( File file, byte *ptr){
  uint16_t nBytes = 0;
  while(file.available()){
    byte b = file.read();
    if( b==_CR_) continue;
    if( b==_LF_) break;
    ptr[nBytes++] = b;
    if( nBytes >= INPUT_LIMIT) break;
  }
  ptr[nBytes] = _NUL_;
  return nBytes;
}

void SDManager::loadState(){
  if(!SDInserted) return;
  if(!SDMounted) return;
  File file = SD.open( RPN_SaveStatusFile2);
  if(!file){
    #ifdef __DEBUG
    Serial.println("Failed to open status file for reading");
    #endif
    return;
  }
  size_t tmpSize = file.size();
  size_t maxSize = _vars->_const_top - 2;
  if( tmpSize >= maxSize) tmpSize = maxSize;
  file.read( (byte *)_vars->_buffer, tmpSize);
  file.close();
  _vars->_buffer[tmpSize] = _NUL_; // prevents name overflow
  _vars->_var_bottom = tmpSize;
  #ifdef __DEBUG
  Serial.print("Loaded status: ");
  Serial.print(_vars->_var_bottom);
  Serial.println(" bytes");
  #endif

  file = SD.open( RPN_AutoexecFile);
  if(!file){
    #ifdef __DEBUG
    Serial.println("Failed to open autotest.bat for reading");
    #endif
    return;
  }
  _code->clearProgram();
  while( true){
    if( !_ReadLn_( file, _io_buffer)) break;
    if( _code->addLine( _io_buffer)) break;
  }
  _vars->setPrgCounter( _code->getFirstLine().lineNumber);
  file.close();
}

void SDManager::saveState(){
  if(!SDInserted) return;
  if(!SDMounted) return;
  File file = SD.open( RPN_SaveStatusFile2, FILE_WRITE);
  if(!file){
    #ifdef __DEBUG
    Serial.println("Failed to open status file for writing");
    #endif
    return;
  }
  #ifdef __DEBUG
    Serial.print("Wrote ");
    Serial.print(file.write( _vars->_buffer, _vars->_var_bottom));
    Serial.println(" bytes of status");
    Serial.print("Variable bottom at ");
    Serial.print(_vars->_var_bottom);
    Serial.println(" bytes");
  #else
  file.write( (byte *)_vars->_buffer, _vars->_var_bottom);
  #endif
  file.close();
}

void SDManager::listDir( char *name){
  if( _cardCheckMantra()) return;
  if( name == NULL) name = _vars->currentDir;

  // TODO: change to IOM
  Serial.print("Listing directory: ");
  Serial.println( name);
  File root = SD.open( name);
  if(!root) return;
  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  ");
      Serial.print( _stripFolders( file.name()));
      Serial.println("  [DIR]");
    }
    else {
      Serial.print("  ");
      Serial.print( _stripFolders( file.name()));
      Serial.print(" ");
      Serial.print(file.size());
      Serial.println("b");
    }
  file = root.openNextFile();
  } // while
}

//void createDir(fs::FS &fs, const char * path){
//    Serial.printf("Creating Dir: %s\n", path);
//    if(fs.mkdir(path)){
//        Serial.println("Dir created");
//    } else {
//        Serial.println("mkdir failed");
//    }
//}
//

void SDManager::readFile(const char * path){
  if(!SDInserted) return;
  if(!SDMounted) return;
  Serial.printf("Reading file: %s\n", path);
  File file = SD.open(path);
  if(!file){
      Serial.println("Failed to open file for reading");
      return;
  }
  Serial.print("Read from file: ");
  while(file.available()){
      Serial.write(file.read());
  }
  file.close();
}

void SDManager::writeFile( const char * path, const char * message){
  if(!SDInserted) return;
  if(!SDMounted) return;
  Serial.print("Writing file: ");
  Serial.println( path);
  File file = SD.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void SDManager::readRPNStatus( byte *inp, byte *last_inp, uint16_t *pos){
  if(!SDInserted) return;
  if(!SDMounted) return;
  File file = SD.open(RPN_SaveStatusFile);
  if(!file){
    #ifdef __DEBUG
    Serial.println("Failed to open status file for reading");
    #endif
    return;
  }
  if( _readString( &file, last_inp, INPUT_COLS)){
    file.close();
    return;
  }
  if( _readString( &file, inp, INPUT_COLS)){
    file.close();
    return;
  }
  size_t inplen = strlen(inp);
  if(inplen>0){ 
    double tmp = 0.0;
    _readDouble( &file, &tmp);
    *pos = (uint16_t)tmp;
    if( *pos > inplen) *pos = inplen; 
  }
  file.close();
}

void SDManager::writeRPNStatus(  byte *inp, byte *last_inp, uint16_t pos){
  if(!SDInserted) return;
  if(!SDMounted) return;
  File file = SD.open( RPN_SaveStatusFile, FILE_WRITE);
  if(!file){
    #ifdef __DEBUG
    Serial.println("Failed to open status file for writing");
    #endif
    return;
  }
  if( _writeString( &file, last_inp)){
    file.close();
    return;
  }
  if( _writeString( &file, inp)){
    file.close();
    return;
  }
  _writeDouble( &file, (double)pos);
  file.close();
}

void SDManager::_readBuffer( void *f){
  File *file = (File *)f;
  size_t pos = 0;
  while(file->available()){
    byte b = file->read();
    if( b == _CR_) continue;
    if( b == _LF_) break;
    if( pos >= INPUT_LIMIT) continue;
    _io_buffer[pos++] = b;
  }
  _io_buffer[pos] = _NUL_;
}

bool SDManager::_readDouble( void *f, double *v){
  *v = 0.0;
  _readBuffer( f);
  _epar->parse(_io_buffer);
  if(_epar->result == _RESULT_UNDEFINED_) return true;
  *v = _epar->numberParser.realValue();
  return false;
}

bool SDManager::_writeDouble( void *f, double v){
  File *file = (File *)f;
  char *message = (char *)_epar->numberParser.stringValue( v);
  if( !file->print(message)) return true;
  return file->print("\r\n") != 2;
}

//
// Note CP1251 conversion on the fly
//
bool SDManager::_readString( void *f, byte *buff, size_t limit){
  _readBuffer( f);
  convertToCP1251( buff, (const char *)_io_buffer, limit);
  return strlen(buff) == 0;
}

//
// Note UTF8 conversion on the fly
//
bool SDManager::_writeString( void *f, byte *v){
  char buff[8]; 
  File *file = (File *)f;
  while(*v){
    convertToUTF8( buff, *v++);
    if( !file->print(buff)) break;
  }
  return file->print("\r\n") != 2;
}

//void appendFile(fs::FS &fs, const char * path, const char * message){
//    Serial.printf("Appending to file: %s\n", path);
//
//    File file = fs.open(path, FILE_APPEND);
//    if(!file){
//        Serial.println("Failed to open file for appending");
//        return;
//    }
//    if(file.print(message)){
//        Serial.println("Message appended");
//    } else {
//        Serial.println("Append failed");
//    }
//    file.close();
//}
//
//void renameFile(fs::FS &fs, const char * path1, const char * path2){
//    Serial.printf("Renaming file %s to %s\n", path1, path2);
//    if (fs.rename(path1, path2)) {
//        Serial.println("File renamed");
//    } else {
//        Serial.println("Rename failed");
//    }
//}
//
//void deleteFile(fs::FS &fs, const char * path){
//    Serial.printf("Deleting file: %s\n", path);
//    if(fs.remove(path)){
//        Serial.println("File deleted");
//    } else {
//        Serial.println("Delete failed");
//    }
//}
//
//void testFileIO(fs::FS &fs, const char * path){
//    File file = fs.open(path);
//    static uint8_t buf[512];
//    size_t len = 0;
//    uint32_t start = millis();
//    uint32_t end = start;
//    if(file){
//        len = file.size();
//        size_t flen = len;
//        start = millis();
//        while(len){
//            size_t toRead = len;
//            if(toRead > 512){
//                toRead = 512;
//            }
//            file.read(buf, toRead);
//            len -= toRead;
//        }
//        end = millis() - start;
//        Serial.printf("%u bytes read for %u ms\n", flen, end);
//        file.close();
//    } else {
//        Serial.println("Failed to open file for reading");
//    }
//
//
//    file = fs.open(path, FILE_WRITE);
//    if(!file){
//        Serial.println("Failed to open file for writing");
//        return;
//    }
//
//    size_t i;
//    start = millis();
//    for(i=0; i<2048; i++){
//        file.write(buf, 512);
//    }
//    end = millis() - start;
//    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
//    file.close();
//}
//

//
// Opens a data file for reading
//
bool SDManager::openDataFileRead( const char *name){
  if( _cardCheckMantra()) return true;

  #ifdef __DEBUG
  Serial.print("Will be reading: ");
  Serial.println( name);
  #endif
  size_t len = strlen( name);

  // first, try the absolute name; no memory allocation needed
  if( len > 2 && name[0] == '/'){
    _currentFile = SD.open( name);
    return !_currentFile;
  }
  
  // next, try directory + name
  LastError = SD_Error_FileNotFound;
  len += strlen( _vars->currentDir) + 5;
  char *tmpName = (char *) malloc( len);
  if( tmpName == NULL){
    LastError = SD_Error_OutOfMemory;
    return true;
  }
  strcpy( tmpName, _vars->currentDir);
  while( *name == '/') name++;
  strcat( tmpName, name);
  if( _lookForFileMantra1( tmpName)) return LastError == NULL; // note it frees tmpName

  // next, try directory + name + ".bas"
  size_t eos = strlen( tmpName);
  strcat( tmpName, SD_DefaultExt1);
  if( _lookForFileMantra1( tmpName)) return LastError == NULL; // note it frees tmpName

  // finally, try directory + name + ".BAS"
  tmpName[eos] = _NUL_;
  strcat( tmpName, SD_DefaultExt2);
  if( _lookForFileMantra1( tmpName)) return LastError == NULL; // note it frees tmpName

  // if we are here, nobody freed the memory
  free( tmpName);
  return true;
}

//
// Opens a program file for reading; it must initialize the current program file
//
bool SDManager::openProgramFileRead( const char *name){
  if( _cardCheckMantra()) return true;

  // first, try the absolute name; no memory allocation needed
  if( name == NULL){
    _currentFile = SD.open( _vars->currentFile);
    if(!_currentFile){
      LastError = SD_Error_FileNotFound;
      return true;
    }
    return false;
  }

  if( _locateBASICFile( name)) return true;
  #ifdef __DEBUG
  Serial.print("Will be loading: ");
  Serial.println( _vars->currentFile);
  #endif

  if( !SD.exists(_vars->currentFile)){
    LastError = SD_Error_FileNotFound;
    return true;
  }
  _currentFile = SD.open( _vars->currentFile);
  if(!_currentFile){
    LastError = SD_Error_FileAccessError;
    return true;
  }
  return false;
}

//
// Opens a data file for writing
//
bool SDManager::openDataFileWrite( const char *name){
  Serial.println("file given: ");
  Serial.println(name);
  if( _cardCheckMantra()) return true;
  uint16_t len = strlen( name);

  // first, try the absolute name; no memory allocation needed
  if( len > 1 && name[0] == '/'){
    _currentFile = SD.open( name, "w");
    if(!_currentFile){
      LastError = SD_Error_FileAccessError;
      return true;
    }
    return false;
  }

  len += strlen( _vars->currentDir) + 1;
  char *tmpName = (char *) malloc( len);
  if( tmpName == NULL){
    LastError = SD_Error_OutOfMemory;
    return true;
  }
  strcpy( tmpName, _vars->currentDir);
  while( *name == '/') name++;
  strcat( tmpName, name);

  if( _checkDirectoryStructure( tmpName)){
    free( tmpName);
    return true;
  }
  _currentFile = SD.open( tmpName, "w");
  free( tmpName);
  if(!_currentFile){
    LastError = SD_Error_FileAccessError;
    return true;
  }
  return false;
}

//
// Opens a program file for writing
//
bool SDManager::openProgramFileWrite( const char *name){
  if( _cardCheckMantra()) return true;
  if( _formFileMantra( name, _vars->currentFile)) return true;
  Serial.println("Current file set: ");
  Serial.println(( const char*)_vars->currentFile);
  if( SD.exists(_vars->currentFile)){
    _currentFile = SD.open( _vars->currentFile, "w");
    if(!_currentFile){
      LastError = SD_Error_FileAccessError;
      return true;
    }
    return false;
  }
  if( _checkDirectoryStructure( _vars->currentFile)) return true;
  _currentFile = SD.open( _vars->currentFile, "w");
  if(!_currentFile){
    LastError = SD_Error_FileAccessError;
    return true;
  }
  return false;
}

//
// Locates an existing BASIC file on disk
//
bool SDManager::_locateBASICFile( const char *name){
  size_t len = strlen( name);
  if( _nameLengthCheckMantra( len)) return true;

  // first, try the absolute name; no memory allocation needed
  if( SD.exists(name)){
    strncat2( _vars->currentFile, name, CURRENT_FILE_LEN);
    return false;
  }
  
  // next, try directory + name
  len += strlen( _vars->currentDir) + 5;
  if( _nameLengthCheckMantra( len)) return true;
  char *tmpName = (char *) malloc( len);
  if( tmpName == NULL){
    LastError = SD_Error_OutOfMemory;
    return true;
  }
  strcpy( tmpName, _vars->currentDir);
  while( *name == '/') name++;
  strcat( tmpName, name);
  if( _lookForFileMantra2( tmpName)) return false; // note it frees tmpName

  // next, try directory + name + ".bas"
  size_t eos = strlen( tmpName);
  strcat( tmpName, SD_DefaultExt1);
  if( _lookForFileMantra2( tmpName)) return false; // note it frees tmpName

  // finally, try directory + name + ".BAS"
  tmpName[eos] = _NUL_;
  strcat( tmpName, SD_DefaultExt2);
  if( _lookForFileMantra2( tmpName)) return false; // note it frees tmpName

  // if we are here, nobody freed the memory
  free( tmpName);
  LastError = SD_Error_FileNotFound;
  return true;
}

//
// Short mantras for file handling
//
bool SDManager::_cardCheckMantra(){
  LastError = SD_Error_NotInserted;
  if(!SDInserted) return true;
  LastError = SD_Error_NotMounted;
  if(!SDMounted) return true;
  LastError = NULL;
  return false;
}
char *SDManager::_stripFolders( const char *name){
  int16_t lastHash = strlen( name) - 1;
  while( lastHash>0 && name[lastHash] != '/') lastHash--;
  return (char *)name+lastHash+1; 
}

bool SDManager::_nameLengthCheckMantra( size_t len){
  if( len <= CURRENT_FILE_LEN) return false;
  LastError = SD_Error_FileNameTooLong;
  return true;
}
bool SDManager::_lookForFileMantra1( char *tmpName){
  #ifdef __DEBUG
  Serial.print( "Looking for file: ");
  Serial.println( tmpName);
  #endif
  if( !SD.exists(tmpName)) return false;
  _currentFile = SD.open( tmpName);
  free( tmpName);
  if( !_currentFile) LastError = NULL;
  return true;
}
bool SDManager::_lookForFileMantra2( char *tmpName){
  #ifdef __DEBUG
  Serial.print( "Looking for file: ");
  Serial.println( tmpName);
  #endif
  if( !SD.exists(tmpName)) return false;
  strncat2( _vars->currentFile, tmpName, CURRENT_FILE_LEN);
  free( tmpName);
  return true;
}
bool SDManager::_formFileMantra( const char *name, char *dest){
  if( name == NULL) return false;
  int16_t len = strlen( name);
  if( len<2){
    LastError = SD_Error_FileAccessError;
    return true;
  }
  if( name[0] != '/') len += strlen( _vars->currentDir) + 1;
  if( _nameLengthCheckMantra( len)) return true;
  dest[0] = _NUL_;
  if( name[0] != '/') strcat( dest, _vars->currentDir);
  strcat( dest, name);
  return false;
}

//
// Checks if directory structure exists, if not - creates
//
bool SDManager::_checkDirectoryStructure( char *name){
  if( SD.exists( name)) return false;
  int16_t lastHash = strlen( name) - 1;
  while( lastHash>0 && name[lastHash] != '/') lastHash--;
  if( lastHash <= 0) return false; 
  for( int i=1; i<=lastHash; i++){
    if( name[i] != '/') continue;
    char tmp = name[i];
    name[i] = _NUL_;
    if( SD.exists( (const char*)name)){
      #ifdef __DEBUG
      Serial.print(name);
      Serial.println(" is OK");
      #endif
      name[i] = tmp;
      continue;
    }
    #ifdef __DEBUG
    Serial.print("Creating ");
    Serial.println(name);
    #endif
    bool ret = SD.mkdir( (const char*)name);
    name[i] = tmp;
    if( !ret){
      #ifdef __DEBUG
      Serial.println("Create failed, exiting");
      #endif
      LastError = SD_Error_FileAccessError;
      return true;
    }
  }
  // directory structure checked
  return false;
}

//
// Read/write functions
//
uint16_t SDManager::readln( byte *buff){
  uint16_t nBytes = 0;
  while(_currentFile.available()){
    byte b = _currentFile.read();
    if( b==_CR_) continue;
    if( b==_LF_) break;
    buff[nBytes++] = b;
    if( nBytes >= INPUT_LIMIT) break;
  }
  buff[nBytes] = _NUL_;
  return nBytes;
}
uint16_t SDManager::print( char *buff, bool cr){
  uint16_t nBytes = _currentFile.print( buff);
  if(cr) return nBytes + _currentFile.print( "\r\n");
  return nBytes;
}
