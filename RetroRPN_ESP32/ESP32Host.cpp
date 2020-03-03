//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "ESP32Host.hpp"

//#define __DEBUG

static volatile bool PowerOffRequested = false;
static SDManager mySD;
static LCDManager myLCD;
static IOManager myIO;
static RPNCalculator myRPN;

const char IO_WelcomeMessage1[] PROGMEM = "*** ЭЛЕКТРОНИКА МК-2090 ***";
const char IO_WelcomeMessage2[] PROGMEM = "***  Retro  Calculator  ***";
const char IO_WelcomeMessage3[] PROGMEM = "(CO in) Serial to PC connected (CO out)";
const char IO_WelcomeMessage4[] PROGMEM = "Hardware keyboard connected";
const char IO_SleepMessage1[] PROGMEM = "Power switch - off...";
const char IO_SleepMessage2[] PROGMEM = "Inactive. Going to sleep...";
const char IO_SleepMessage3[] PROGMEM = "Programmatic shutdown. Going to sleep...";
const char *const IO_Message_Table[] PROGMEM = {
  IO_WelcomeMessage1, 
  IO_WelcomeMessage2,
  IO_WelcomeMessage3, 
  IO_WelcomeMessage4,
  IO_SleepMessage1,
  IO_SleepMessage2,
  IO_SleepMessage3
  };

void IRAM_ATTR isrPower() {
  PowerOffRequested = true;
}

unsigned long ESP32Host::init() {
  unsigned long initStarted = myIO.init();
  myLCD.init();
  // Captain Obvious reports the serial ports into the serial ports!
  for(byte i=0; i<3; i++)
    Serial.println(IO_Message_Table[i]);
  if(myIO.HWKeyboardConnected)
    Serial.println(IO_Message_Table[3]);
  mySD.init();
  myIO.flashKBDLEDs();
  pinMode(POWER_DETECT_PIN, INPUT);    // sets the digital pin for power check
  waitUntilSleepPinHigh();
  PowerOffRequested = false;
  attachInterrupt( POWER_DETECT_PIN, isrPower, RISING);
  myRPN.init(&myIO, &myLCD, &mySD);
  myLCD.waitForEndSplash( initStarted);
  myRPN.show();
  myRPN.redraw();
  myIO.keepAwake(); 
  return myIO.lastInput;
}

unsigned long ESP32Host::tick() {
  checkSleepPin();
  mySD.tick( myIO.lastInput);
  myIO.tick();
  myRPN.tick();
  unsigned long dt = millis();
  dt -= myIO.lastInput;
  if(dt > POWER_OFF_PERIOD){
    deepSleep( IO_MSG_INACTIVE);
    return millis();
  }
  if(dt > SCREEN_OFF_PERIOD){
    myLCD.sleepOn();
    return millis();
  }
  if(dt > SCREEN_LED_PERIOD){
    myLCD.LEDOff();
    return millis();
  }
  if( myLCD.isAsleep){
    delay(50);
    myLCD.sleepOff();
    myRPN.show();
    myRPN.redraw();
    return millis();
  }
  myLCD.LEDOn();
  myRPN.redraw();
  return millis();
}

void ESP32Host::checkSleepPin(){
  if( !PowerOffRequested) return;
  ESP32Host::deepSleep( IO_MSG_POWER_BUTTON);  
}

void ESP32Host::deepSleep( byte msg){
  if( msg > 0) Serial.println( IO_Message_Table[msg]);
  // TODO add calls for saving the system state
  myRPN.saveState();
  waitUntilSleepPinHigh();
  esp_sleep_enable_ext0_wakeup( (gpio_num_t)(POWER_DETECT_PIN), 0); //1 = High, 0 = Low
  esp_deep_sleep_start();
  // it never exits from here
}
