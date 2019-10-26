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

#define RRPN_VERSION "2019-10"
// in milliseconds
#define SPLASH_SHOW 2000

// Valid: 9600, 19200, 38400, 115200, etc
#define CONSOLE_BAUD 115200

#include "RRPN_Hardware.h"

static const byte RRPN_INITIAL_MSG[]       PROGMEM = "ЭЛЕКТРОНИКА МК-2090";
static const byte RRPN_VERSION_MSG[]       PROGMEM = "version " RRPN_VERSION;

static RRPN_Parser Parser;
static RRPN_Calculator RPN;

//
// Hardware reset upon power-up
// The current hardware does not allow SD card swapping
//
void setup() {
  Serial.begin(CONSOLE_BAUD);
  Hard_Reset();
  Terminal.println_P((char *)RRPN_INITIAL_MSG, true);
  Terminal.println_P((char *)RRPN_VERSION_MSG, true);
  RPN.display();
}

//
// Arduino sketch runs in a loop, periodically yielding to the system
//
void loop() {
  char c;
  if( Terminal.wantsToBlink()){
    Terminal.flopCursor();
    Terminal.displayStackX( RPN.input, RPN.cursorPosition);
  }
  hwKeyboard.input();
  if( !hwKeyboard.available() && !Serial.available()){
    delay(10);
    return;
  }
  if( hwKeyboard.available()){
    c = hwKeyboard.read();
  }
  else if( Serial.available()){
    c = Serial.read();
  }
  RPN.send(c);
}
