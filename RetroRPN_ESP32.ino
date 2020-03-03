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
// 01 - TX0
// 02 - In-built LED (SD activity)
// 03 - RX0
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

#define __DEBUG

#include "ESP32Host.hpp"

static ESP32Host host;

void setup(void) {
  #ifdef __DEBUG
  unsigned long dt = millis();  
  dt = host.init() - dt;
  Serial.print("Init completed in ");
  Serial.print(dt);
  Serial.println(" ms");
  #else
  host.init();
  #endif  
}

void loop(void) {
  host.tick();  
}
