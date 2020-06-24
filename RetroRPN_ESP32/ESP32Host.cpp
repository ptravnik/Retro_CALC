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

static IOManager myIOManager;
static LCDManager myLCDManager;
static Keywords myKeywords;
static Variables myVariables;
static Functions myFunctions;
static ExpressionParser myExpressionParser;
static ProgramCode myProgramCode;
static Lexer myLexer;
static SDManager mySDManager;
static MessageBox myMessageBox;
static CommandLine myCommandLine;
static RPNStackBox myRPNStackBox;
static TerminalBox myTerminalBox;
static FileManager myFileManager;
static RPNCalculator myRPNCalculator;
static BasicConsole myBasicConsole;
static Editor myEditor;


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
  unsigned long initStarted = myIOManager.init( _io_buffer);
  
  // In order of initialization
  _host_Components[UI_COMP_IOManager] = &myIOManager;
  _host_Components[UI_COMP_LCDManager] = &myLCDManager;
  _host_Components[UI_COMP_Keywords] = &myKeywords;
  _host_Components[UI_COMP_Variables] = &myVariables;
  _host_Components[UI_COMP_Functions] = &myFunctions;
  _host_Components[UI_COMP_ExpressionParser] = &myExpressionParser;
  _host_Components[UI_COMP_ProgramCode] = &myProgramCode;
  _host_Components[UI_COMP_Lexer] = &myLexer;
  _host_Components[UI_COMP_SDManager] = &mySDManager;
  _host_Components[UI_COMP_MessageBox] = &myMessageBox;
  _host_Components[UI_COMP_CommandLine] = &myCommandLine;
  _host_Components[UI_COMP_RPNBox] = &myRPNStackBox;
  _host_Components[UI_COMP_TerminalBox] = &myTerminalBox;
  _host_Components[UI_COMP_FileManager] = &myFileManager;
  _host_Components[UI_COMP_RPNCalculator] = &myRPNCalculator;
  _host_Components[UI_COMP_BasicConsole] = &myBasicConsole;
  _host_Components[UI_COMP_Editor] = &myEditor;

  // Warm-up LCD, show splash, while showing splash, do the rest
  myLCDManager.init(_host_Components);
  _showAsciiSplash();

  // init BASIC components
  myKeywords.init();
  myVariables.init(_host_Components);
  myFunctions.init(_host_Components);
  myExpressionParser.init(_host_Components);
  myProgramCode.init(_host_Components);
  myLexer.init(_host_Components);

  // Init SD and check hardware
  mySDManager.init(_host_Components);
  myIOManager.flashKBDLEDs();

  // Power management up
  pinMode(POWER_DETECT_PIN, INPUT);    // sets the digital pin for power check
  _waitUntilSleepPinHigh();
  PowerOffRequested = false;
  attachInterrupt( POWER_DETECT_PIN, isrPower, RISING);

  // Other UI components
  myMessageBox.init(_host_Components);
  myCommandLine.init(_host_Components);
  myRPNStackBox.init(_host_Components);
  myTerminalBox.init(_host_Components);
  myFileManager.init(_host_Components);
  myBasicConsole.init(_host_Components);
  myEditor.init(_host_Components);
  myRPNCalculator.init(_host_Components);
  myLexer.loadState();
  
  myLCDManager.waitForEndSplash( initStarted);

  if( myLexer.currentUI < UI_RPNCALC || myLexer.currentUI > UI_CONSOLE )
    myLexer.currentUI = UI_RPNCALC;
  show();
  redraw();
  return myIOManager.keepAwake();
}

unsigned long ESP32Host::tick() {
  _checkSleepPin();
  myLexer.tick();
  mySDManager.tick();
  myIOManager.tick();
  myLCDManager.tick();
  switch(myLexer.currentUI){
    case UI_RPNCALC:
      myRPNCalculator.tick();
      break;
    case UI_FILEMAN:
      myFileManager.tick();
      break;
    case UI_EDITOR:
      myEditor.tick();
      break;
    case UI_CONSOLE:
      myBasicConsole.tick();
    default:
      break; 
  }   
  unsigned long dt = millis() - myIOManager.lastInput;
  if(dt > POWER_OFF_PERIOD){
    deepSleep( IO_MSG_INACTIVE);
    return myIOManager.lastInput;
  }
  if(dt > SCREEN_OFF_PERIOD){
    // TODO: check here if saving is needed
    // myLexer.saveState();
    myLCDManager.sleepOn();
    return myIOManager.lastInput;
  }
  if(dt > SCREEN_LED_PERIOD){
    myLCDManager.LEDOff();
    return myIOManager.lastInput;
  }
  if( myLCDManager.isAsleep){
    myLCDManager.sleepOff();
    show();
    redraw();
    return myIOManager.lastInput;
  }
  myLCDManager.LEDOn();
  selectNextUI();
  redraw();
  return myIOManager.lastInput;
}

void ESP32Host::selectNextUI(){
  if( myLexer.nextUI == UI_UNDEFINED) return;
  byte tmp = myLexer.nextUI;
  myLexer.nextUI = UI_UNDEFINED;
  selectUI(tmp);
}

void ESP32Host::selectUI(byte ui){
  if( myLexer.currentUI == ui) return;
  myLexer.currentUI = ui;
  myCommandLine.clearInput();
  show();
}

void ESP32Host::show(){
  switch(myLexer.currentUI){
    case UI_RPNCALC:
      myRPNCalculator.show();
      break;
    case UI_FILEMAN:
      myFileManager.show();
      break;
    case UI_EDITOR:
      myEditor.show();
      break;
    case UI_CONSOLE:
      myBasicConsole.show();
      break;
    default:
      Serial.println("UI undefined");
      break; 
  }   
}

void ESP32Host::redraw(){
  switch(myLexer.currentUI){
    case UI_RPNCALC:
      myRPNCalculator.redraw();
      break;
    case UI_FILEMAN:
      myFileManager.redraw();
      break;
    case UI_EDITOR:
      myEditor.redraw();
      break;
    case UI_CONSOLE:
      myBasicConsole.redraw();
    default:
      break; 
  }   
}

void ESP32Host::_checkSleepPin(){
  if( !PowerOffRequested) return;
  ESP32Host::deepSleep( IO_MSG_POWER_BUTTON);  
}

void ESP32Host::deepSleep( byte msg){
  if( msg > 0) myIOManager.sendStringUTF8Ln( IO_Sleep_Table[msg]);
  delay(500);
  myLCDManager.sleepOn();
  // TODO add calls for saving the system state
  myLexer.saveState();
  delay(1000); // finish card activity
  mySDManager.sleepOn();
  myIOManager.sleepOn(); // consoles quit the last!
  _waitUntilSleepPinHigh(); // prevent jitter
  esp_sleep_enable_ext0_wakeup( (gpio_num_t)(POWER_DETECT_PIN), 0); //1 = High, 0 = Low
  esp_deep_sleep_start();  
  // it never exits from here
}

//
// Show ASCII graphics at startup
//
void ESP32Host::_showAsciiSplash(){
  for(byte i=0; i<3; i++) myIOManager.sendLn();
  for(byte i=0; i<3; i++)
     myIOManager.sendStringUTF8Ln(IO_Message_Table[i]);
  myIOManager.sendStringUTF8Ln(IO_Message_Table[0]);
  for(byte i=0; i<3; i++) myIOManager.sendLn();

  // Captain Obvious reports the serial ports into the serial ports!
  myIOManager.sendStringUTF8Ln(IO_Message_Table[3], SERIAL_ONLY);
  if( myIOManager.PM_active())
    myIOManager.sendStringUTF8Ln(IO_Message_Table[5], SERIAL2_ONLY);
  else
    myIOManager.sendStringUTF8Ln(IO_Message_Table[4], SERIAL_ONLY);

  // Report HW keyboard
  if(myIOManager.HWKeyboardConnected){
    myIOManager.sendStringUTF8Ln( IO_Message_Table[6]);
  }
};
