/////////////////////////////////////////////////////

/* RetroRPN - "Электроника МК-90" reborn
  Copyright (c) 2019 Pavel Travnik.  All right reserved.

  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 Inspired by work of: 
    Gordon Brandly (Tiny Basic for 68000)
    Mike Field (Arduino Basic)
    Scott Lawrence (TinyBasic Plus)
    Brian O'Dell (INPUT)
    David A. Mellis (SD card utils)
    Oli Kraus (LCD library)
    Mike Yakimov (TinyBasic with real numbers)
    Andrey Skvortsov (Terminal-BASIC and Arduino SRAM)
    David Boucher (Tiny Keyboard)
    User "Dita Schop" (Arduino RPN calc)
*/

/////////////////////////////////////////////////////

//
// This implementation is for ESP32 Dev Module
//
// Pin assignment:
//
// 04 - SD insert sensor (DET_A)
// 05 - VSPI CSO - SD Select (DAT3)
// 12 - HSPI MISO LCD Data (R/W)
// 13 - HSPI MOSI LCD Reset (RESET)
// 14 - HSPI CLK LCD Clock (E)
// 15 - HSPI CSO LCD Select (RS)
// 16 - LCD POWER (this model does not have sleep)
// 17 - LCD LED PWM
// 18 - VSPI CLK - SD Clock (CLK)
// 19 - VSPI MISO - SD out (DAT0)
// 23 - VSPI MOSI - SD command (CMD)
// 25 - KBD CLK
// 26 - KBD RST
// 27 - KBD_LED CLK
// 32 - KBD_LED RST
// 33 - KBD DATA
// 34 - POWER ON/OFF
//

//#define __DEBUG

#include "Utilities.hpp"
#include "IOManager.hpp"
#include "LCDManager.hpp"
#include "SDManager.hpp"
#include "RPNCalculator.hpp"
#include "HWKbd.h"

#define MAX_COMMAND 256
static char Command[MAX_COMMAND];
static char Command2[MAX_COMMAND];
static size_t Command_Position = 0;
static IOManager iom;
static LCDManager lcd;
static RPNCalculator rpn;

static bool b1 = false;
static bool b2 = false;
static bool b3 = false;
static bool b4 = false;

void setup(void) {
  Serial.begin(115200);
  delay(500);
  iom.init();
  lcd.init();
  lcd.redraw();
  rpn.init( iom, lcd);
  rpn.show();
}

void loop(void) {
  char c;
  rpn.redraw();
  c = iom.input();
  if( !c){
    delay(50);
    return;
  }
  if(c>=32) Serial.write(c);
  if(c==_CR_ || c==_LF_) Serial.println();
  rpn.sendChar((byte)c);

//  if( Command_Position < MAX_COMMAND-1){
//    Command[ Command_Position++] = c;    
//    Command[ Command_Position] = 0;    
//  }
//  if( c == 0x0D || c == 0x0A){
//    Serial.println(Command);
//    //lcd.sendStringUTF8( Command);
//    convertToCP1251( (byte *)Command2, Command, MAX_COMMAND);
//    ExecuteCommand( Command2);
//    Command_Position = 0;
//    Command[0] = 0;
//  }
}

void ExecuteCommand( const char *comm){
//  if( IsToken( (char *)comm, "$b", false)){
//    Serial.println("To bottom line");
//    lcd.cursorBottom();
//    return;
//  }

//  if( IsToken( (char *)comm, "$su", false)){
//    Serial.println("Scroll up");
//    lcd.scrollUp(1);
//    return;
//  }

//  if( IsToken( (char *)comm, "$sd", false)){
//    Serial.println("Scroll down");
//    lcd.scrollDown(1);
//    return;
//  }

  if( IsToken( (char *)comm, "help")){
//    lcd.sendString( comm);
//    lcd.sendString("\rcls - clear screen\r");
//    lcd.sendString("wrap=on/off\r");
//    lcd.sendString("scroll=on/off\r");
//    lcd.sendString("$h or $b\r");
//    lcd.sendString("$u/d/l/r - move cursor\r");
//    lcd.sendString("$su/d - scroll up/down\r");
    Serial.println("cls - clear screen");
    Serial.println("wrap=on/off");
    Serial.println("scroll=on/off");
    Serial.println("$h - home");
    Serial.println("$b - bottom line");
    Serial.println("$u/d - cursor up/down");
    Serial.println("$l/r - cursor left/right");
    Serial.println("$su/d - scroll up/down");
    return;
  }

  for( size_t i=0; i<strlen(comm); i++){
    rpn.sendChar( (byte)comm[i]);
  }
}
