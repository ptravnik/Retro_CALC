//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "SDManager.hpp"
#include "Utilities.hpp"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

//#define __DEBUG

const char SD_Message0[] PROGMEM = "+ SD inserted";
const char SD_Message1[] PROGMEM = "+ SD removed";
const char SD_Message2[] PROGMEM = "+ SD mounted";
const char SD_Message3[] PROGMEM = "+ SD mount failed";
const char SD_Message4[] PROGMEM = "+ SD size: %llu MB";
const char *const SD_Message_Table[] PROGMEM = {
  SD_Message0,
  SD_Message1,
  SD_Message2,
  SD_Message3,
  SD_Message4
  };

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
  //Serial.printf("SD Card Size: %llu MB\n", cardSize);
  keepAwake();
  return inserted;  
}

//
// Init and status update
//
unsigned long SDManager::init( IOManager *iom){
  _io_buffer = iom->getIOBuffer();
  _iom = iom;
  pinMode(SD_DETECT_PIN, INPUT_PULLUP);   // sets the digital pin for SD check
  _detectSDCard();
  SDCheckRequested = false;
  attachInterrupt( SD_DETECT_PIN, isrSD, CHANGE);
  return keepAwake(); 
}

unsigned long SDManager::tick(){
  _checkSDPin();
  return _iom->lastInput;
}

uint8_t SDManager::cardType(){
  if(!SDInserted) return SD_CARD_OUT;
  if(!SDMounted) return SD_CARD_NOT_MOUNTED;
  return SD.cardType();
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

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
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
//
//void readFile(fs::FS &fs, const char * path){
//    Serial.printf("Reading file: %s\n", path);
//
//    File file = fs.open(path);
//    if(!file){
//        Serial.println("Failed to open file for reading");
//        return;
//    }
//
//    Serial.print("Read from file: ");
//    while(file.available()){
//        Serial.write(file.read());
//    }
//    file.close();
//}
//
//void writeFile(fs::FS &fs, const char * path, const char * message){
//  Serial.println("Writing file: %s\n", path);
//
//    File file = fs.open(path, FILE_WRITE);
//    if(!file){
//        Serial.println("Failed to open file for writing");
//        return;
//    }
//    if(file.print(message)){
//        Serial.println("File written");
//    } else {
//        Serial.println("Write failed");
//    }
//    file.close();
//}
//
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
