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

const char SD_Message0[] PROGMEM = "+ SD inserted";
const char SD_Message1[] PROGMEM = "+ SD removed";
const char SD_Message2[] PROGMEM = "+ SD mounted";
const char SD_Message3[] PROGMEM = "+ SD mount failed";
const char SD_Message4[] PROGMEM = "+ SD size: %llu MB";
const char SD_Message_Mounted[] PROGMEM = "SD mounted";
const char SD_Message_Removed[] PROGMEM = "SD removed";
const char *const SD_Message_Table[] PROGMEM = {
  SD_Message0,
  SD_Message1,
  SD_Message2,
  SD_Message3,
  SD_Message4
  };
const char SD_root[] PROGMEM = "/";

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
  _vars = (Variables *)components[UI_COMP_Variables];
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
    strncpy( _vars->currentDir, SD_root, CURRENT_DIR_LEN);
    return;
  }
  if(!root.isDirectory()){
    #ifdef __DEBUG
    Serial.println("Not a directory, reset to root");
    #endif
    strncpy( _vars->currentDir, SD_root, CURRENT_DIR_LEN);
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

//
// TODO: load and save functionality here
//
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
  size_t tmpsize = file.size();
  if( tmpsize > VARIABLE_SPACE) tmpsize = VARIABLE_SPACE;
  file.read( _vars->_buffer, tmpsize);
  file.close();
  _vars->_var_bottom = tmpsize;
  #ifdef __DEBUG
  Serial.print("Loaded status: ");
  Serial.print(_vars->_var_bottom);
  Serial.println(" bytes");
  #endif
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
  file.write( _vars->_buffer, _vars->_var_bottom);
  #endif
  file.close();
}

void SDManager::listDir(){
    if(!SDInserted) return;
    if(!SDMounted) return;
    Serial.print("Listing directory: ");
    Serial.println(_vars->currentDir);

    File root = _getCurrentDir();
    if(!root) return;

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
        }
        else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
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
//void removeDir(fs::FS &fs, const char * path){
//    Serial.printf("Removing Dir: %s\n", path);
//    if(fs.rmdir(path)){
//        Serial.println("Dir removed");
//    } else {
//        Serial.println("rmdir failed");
//    }
//}

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
//void setup(){
//    Serial.begin(115200);
//    if(!SD.begin()){
//        Serial.println("Card Mount Failed");
//        return;
//    }
//    uint8_t cardType = SD.cardType();
//
//    if(cardType == CARD_NONE){
//        Serial.println("No SD card attached");
//        return;
//    }
//
//    Serial.print("SD Card Type: ");
////    if(cardType == CARD_MMC){
//        Serial.println("MMC");
//    } else if(cardType == CARD_SD){
//        Serial.println("SDSC");
//    } else if(cardType == CARD_SDHC){
//        Serial.println("SDHC");
//    } else {
//        Serial.println("UNKNOWN");
//    }
//
//    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//    Serial.printf("SD Card Size: %lluMB\n", cardSize);
//
//    listDir(SD, "/", 0);
//    createDir(SD, "/mydir");
//    listDir(SD, "/", 0);
//    removeDir(SD, "/mydir");
//    listDir(SD, "/", 2);
//    writeFile(SD, "/hello.txt", "Hello ");
//    appendFile(SD, "/hello.txt", "World!\n");
//    readFile(SD, "/hello.txt");
//    deleteFile(SD, "/foo.txt");
//    renameFile(SD, "/hello.txt", "/foo.txt");
//    readFile(SD, "/foo.txt");
//    testFileIO(SD, "/test.txt");
//    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
//    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
//}
