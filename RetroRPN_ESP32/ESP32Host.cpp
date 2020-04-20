//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "ESP32Host.hpp"

//#define __DEBUG

static volatile bool PowerOffRequested = false;
static ExpressionParser myEP;
static IOManager myIO;
static SDManager mySD;
static LCDManager myLCD;
static RPNCalculator myRPN;

const char IO_WelcomeMessage0[] PROGMEM = "***********************";
const char IO_WelcomeMessage1[] PROGMEM = "* ЭЛЕКТРОНИКА МК-2090 *";
const char IO_WelcomeMessage2[] PROGMEM = "*  Retro  Calculator  *";
const char IO_WelcomeMessage3[] PROGMEM = "+ ESP32 connected via Serial";
const char IO_WelcomeMessage4[] PROGMEM = "+ Pro Micro appears to be off";
const char IO_WelcomeMessage5[] PROGMEM = "+ ESP32 connected via Pro Micro";
const char IO_WelcomeMessage6[] PROGMEM = "+ Hardware keyboard activated";
const char *const IO_Message_Table[] PROGMEM = {
  IO_WelcomeMessage0, 
  IO_WelcomeMessage1, 
  IO_WelcomeMessage2,
  IO_WelcomeMessage3, 
  IO_WelcomeMessage4,
  IO_WelcomeMessage5,
  IO_WelcomeMessage6
  };

const char IO_SleepMessage0[] PROGMEM = "+ Power switch - off...";
const char IO_SleepMessage1[] PROGMEM = "+ Inactive. Going to sleep...";
const char IO_SleepMessage2[] PROGMEM = "+ Programmatic shutdown. Going to sleep...";
const char *const IO_Sleep_Table[] PROGMEM = {
  IO_SleepMessage0,
  IO_SleepMessage1,
  IO_SleepMessage2
  };

void IRAM_ATTR isrPower() {
  PowerOffRequested = true;
}

unsigned long ESP32Host::init() {
  unsigned long initStarted = myIO.init( _io_buffer);
  myLCD.init( &myIO);
  
  // show ASCII graphics
  for(byte i=0; i<3; i++) myIO.sendLn();
  for(byte i=0; i<3; i++)
    myIO.sendStringUTF8Ln(IO_Message_Table[i]);
  myIO.sendStringUTF8Ln(IO_Message_Table[0]);
  for(byte i=0; i<3; i++) myIO.sendLn();

  // Captain Obvious reports the serial ports into the serial ports!
  myIO.sendStringUTF8Ln(IO_Message_Table[3], SERIAL_ONLY);
  if( myIO.PM_active())
    myIO.sendStringUTF8Ln(IO_Message_Table[5], SERIAL2_ONLY);
  else
    myIO.sendStringUTF8Ln(IO_Message_Table[4], SERIAL_ONLY);

  // Report HW keyboard
  if(myIO.HWKeyboardConnected){
    myIO.sendStringUTF8Ln( IO_Message_Table[6]);
  }

  mySD.init( &myIO);
  myIO.flashKBDLEDs();

  pinMode(POWER_DETECT_PIN, INPUT);    // sets the digital pin for power check
  _waitUntilSleepPinHigh();
  PowerOffRequested = false;
  attachInterrupt( POWER_DETECT_PIN, isrPower, RISING);
  myEP.init();
  myRPN.init(&myIO, &myLCD, &mySD, &myEP);
  myLCD.waitForEndSplash( initStarted);

  // TODO: interface selector here
  myRPN.show();
  myRPN.redraw();
  return myIO.keepAwake(); 
}

unsigned long ESP32Host::tick() {
  _checkSleepPin();
  mySD.tick();
  myIO.tick();
  myLCD.tick();
  myRPN.tick();  
  unsigned long dt = millis() - myIO.lastInput;
  if(dt > POWER_OFF_PERIOD){
    deepSleep( IO_MSG_INACTIVE);
    return myIO.lastInput;
  }
  if(dt > SCREEN_OFF_PERIOD){
    myLCD.sleepOn();
    return myIO.lastInput;
  }
  if(dt > SCREEN_LED_PERIOD){
    myLCD.LEDOff();
    return myIO.lastInput;
  }
  if( myLCD.isAsleep){
    myLCD.sleepOff();
    myRPN.show();
    myRPN.redraw();
    return myIO.lastInput;
  }
  myLCD.LEDOn();
  myRPN.redraw();
  return myIO.lastInput;
}

void ESP32Host::_checkSleepPin(){
  if( !PowerOffRequested) return;
  ESP32Host::deepSleep( IO_MSG_POWER_BUTTON);  
}

void ESP32Host::deepSleep( byte msg){
  if( msg > 0) myIO.sendStringUTF8Ln( IO_Sleep_Table[msg]);
  delay(1500);
  myLCD.sleepOn();
  // TODO add calls for saving the system state
  myRPN.saveState();
  delay(1500);
  mySD.sleepOn();
  myIO.sleepOn(); // consoles quit the last!
  _waitUntilSleepPinHigh(); // prevent jitter
  esp_sleep_enable_ext0_wakeup( (gpio_num_t)(POWER_DETECT_PIN), 0); //1 = High, 0 = Low
  esp_deep_sleep_start();
  
  // it never exits from here
}
