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
static IOManager myIO;
static Variables myVar;
static MathFunctions myMFun;
static SDManager mySD;
static LCDManager myLCD;
static ExpressionParser myEP;
static MessageBox myMB;
static CommandLine myCL;
static RPNStackBox myRPNBox;
static TerminalBox myTerminalBox;
static RPNCalculator myRPN;
static BasicConsole myBAS;
static FileManager myFM;

// Damn VSCode does not understand UTF8 !
const char IO_WelcomeMessage0[] PROGMEM = "***********************";
//const char IO_WelcomeMessage1[] PROGMEM = "* ЭЛЕКТРОНИКА МК-2090 *";
const char IO_WelcomeMessage1[] PROGMEM = "* ЭЛEKTPOHИKA MK-2090 *";
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
  // Take timestamp
  unsigned long initStarted = myIO.init( _io_buffer);
  
  // In order of initialization
  _host_Components[UI_COMP_IOManager] = &myIO;
  _host_Components[UI_COMP_Variables] = &myVar;
  _host_Components[UI_COMP_MathFunctions] = &myMFun;
  _host_Components[UI_COMP_LCDManager] = &myLCD;
  _host_Components[UI_COMP_SDManager] = &mySD;
  _host_Components[UI_COMP_ExpressionParser] = &myEP;
  _host_Components[UI_COMP_MessageBox] = &myMB;
  _host_Components[UI_COMP_CommandLine] = &myCL;
  _host_Components[UI_COMP_RPNBox] = &myRPNBox;
  _host_Components[UI_COMP_RPNCalculator] = &myRPN;
  _host_Components[UI_COMP_TerminalBox] = &myTerminalBox;
  _host_Components[UI_COMP_BasicConsole] = &myBAS;
  _host_Components[UI_COMP_FileManager] = &myFM;

  // Warm-up LCD, show splash, while showing splash, do the rest
  myLCD.init(_host_Components);
  
  // Show ASCII graphics
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

  // Init SD and check hardware
  myVar.init();
  myMFun.init(_host_Components);
  mySD.init(_host_Components);
  myIO.flashKBDLEDs();

  // Power management up
  pinMode(POWER_DETECT_PIN, INPUT);    // sets the digital pin for power check
  _waitUntilSleepPinHigh();
  PowerOffRequested = false;
  attachInterrupt( POWER_DETECT_PIN, isrPower, RISING);

  // Other UI components
  myEP.init(_host_Components);
  myMB.init(_host_Components);
  myCL.init(_host_Components);
  myRPNBox.init(_host_Components);
  myRPN.init(_host_Components);
  myTerminalBox.init(_host_Components);
  myBAS.init(_host_Components);
  myFM.init(_host_Components);
  mySD.loadState();
  myLCD.waitForEndSplash( initStarted);

  // TODO: remember selector here
  selectUI(UI_RPNCALC);
  redraw();
  return myIO.keepAwake(); 
}

unsigned long ESP32Host::tick() {
  _checkSleepPin();
  mySD.tick();
  myIO.tick();
  myLCD.tick();
  byte *uiRequest = NULL;
  switch(currentUI){
    case UI_RPNCALC:
      myRPN.tick();
      uiRequest = &(myRPN.nextUI); 
      break;
    case UI_FILEMAN:
      myFM.tick();
      uiRequest = &(myFM.nextUI); 
      break;
    case UI_EDITOR:
    case UI_CONSOLE:
      myBAS.tick();
      uiRequest = &(myBAS.nextUI); 
    default:
      break; 
  }   
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
    show();
    redraw();
    return myIO.lastInput;
  }
  myLCD.LEDOn();
  if( uiRequest != NULL && *uiRequest != UI_UNDEFINED)
    selectUI(uiRequest);
  else
    redraw();
  return myIO.lastInput;
}

void ESP32Host::selectUI(byte *ui){
  if( *ui == UI_UNDEFINED) return;
  byte tmp = *ui;
  *ui = UI_UNDEFINED;
  selectUI(tmp);
}

void ESP32Host::selectUI(byte ui){
  if( currentUI == ui) return;
  currentUI = ui;
  show();
}

void ESP32Host::show(){
  switch(currentUI){
    case UI_RPNCALC:
      myRPN.show();
      break;
    case UI_FILEMAN:
      myFM.show();
      break;
    case UI_EDITOR:
    case UI_CONSOLE:
      myBAS.show();
      break;
    default:
      Serial.println("UI undefined");
      break; 
  }   
}

void ESP32Host::redraw(){
  switch(currentUI){
    case UI_RPNCALC:
      myRPN.redraw();
      break;
    case UI_FILEMAN:
      myFM.redraw();
      break;
    case UI_EDITOR:
    case UI_CONSOLE:
      myBAS.redraw();
    default:
      break; 
  }   
}

void ESP32Host::_checkSleepPin(){
  if( !PowerOffRequested) return;
  ESP32Host::deepSleep( IO_MSG_POWER_BUTTON);  
}

void ESP32Host::deepSleep( byte msg){
  if( msg > 0) myIO.sendStringUTF8Ln( IO_Sleep_Table[msg]);
  delay(500);
  myLCD.sleepOn();
  // TODO add calls for saving the system state
  mySD.saveState();
  myRPN.saveState();
  delay(1000); // finish card activity
  mySD.sleepOn();
  myIO.sleepOn(); // consoles quit the last!
  _waitUntilSleepPinHigh(); // prevent jitter
  esp_sleep_enable_ext0_wakeup( (gpio_num_t)(POWER_DETECT_PIN), 0); //1 = High, 0 = Low
  esp_deep_sleep_start();
  
  // it never exits from here
}
