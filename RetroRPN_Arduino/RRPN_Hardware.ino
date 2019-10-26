/////////////////////////////////////////////////////
//
// RetroRPN - "Электроника МК-90" reborn
// Copyright (c) 2019 Pavel Travnik.  All right reserved. 
//
/////////////////////////////////////////////////////

//
// Performs environment reset
//
static void Hard_Reset(){  
  // Always start with memory init
  init_XRAM();

  // Init hardware keyboard
  hwKeyboard.connect(A0,A1,A2,A3,A4, &hwEncoder);

  // Init LCD terminal
  void *nextXRAMptr = Terminal.connect(XRAM_START);
  Terminal.splash( RRPN_INITIAL_MSG, RRPN_VERSION_MSG);

  // Init RPN calculator
  nextXRAMptr = RPN.init(nextXRAMptr, &Terminal);
  
  while(millis() - Terminal.lastUpdateMillis < SPLASH_SHOW) delay(50);

//  environment_Reset();
//
//  // LCD inits first to show the splash screen
//  init_LCD();
//  init_Console();
//  
//  // Other hardware inits in no particular order
//  #ifdef EEPROM_ENABLE
//  init_EEPROM();
//  #endif /* EEPROM_ENABLE */
//  init_SD();
//  #ifdef BEEPER_ENABLE
//  noTone( BEEPER_PIN);
//  #endif
//  hwKeyboard.connect(A0,A1,A2,A3,A4, &hwEncoder);
//
//  // Dog-and-pony show for testing functionality under development
//  delay(1000);
//  display_StackScreen();
//  delay(1000);
//  //  display_EditScreen();
//  //  delay(2000);
//  //  display_TerminalScreen();
//
//  // Report memory available
//  process_KW_MEM( false);
//  start_New_Entry(false, '>');
}

//
// Inits external memory
//
static void init_XRAM(){
  pinMode( XRAM_ENABLE_PIN, OUTPUT);    // chip selection
  XRAM_ENABLE();
  bitSet(XMCRA, SRE);                   // enable external memory
  XMCRB=0;                              // need all 64K. no pins released
  bitClear(XMCRA, SRW11);               // no waits  
  bitClear(XMCRA, SRW10);
}
