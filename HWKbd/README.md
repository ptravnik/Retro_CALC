  # Hardware keyboard for Arduino with an SPI-like interface
  
  Copyright (c) 2019 Pavel Travnik.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  ## Hardware Notes.
  
  The keyboard communicates with Arduino via J1:
  1. **POWER** - connect to +5 V on Mega or +3.3V on Duo
  1. **KBD_RESET** - output, any digital pin
  1. **KBD_CLOCK** - output, any digital pin
  1. **KBD_READ** - input (no pullup!), any digital pin
  1. **LED_RESET** - output, any digital pin
  1. **LED_CLOCK** - output, any digital pin
  1. **RESET** (optional) - connect to Arduino/ESP32 RESET or to ESP32 wake-up GPIO. In the latter case, pull the GPIO up to 3.3V using an external 11k resistor - the GPIO is not powered up in the sleep mode. 
  1. **GROUND**
  
  To read the keyboard, send a pulse to **KBD_RESET** and read
  the first button state from **KBD_READ** (HIGH means "pressed").
  Then, continue sending pulses on **KBD_CLOCK** to read buttons
  2, 3, 4, etc.
  
  To set the LEDs, send a pulse to **LED_RESET** and cycle
  **LED_CLOCK** from 0 to 15 times, which corresponds to 16 binary states.
  Note that the software presumes that no PWM is sent to the keyboard ports!

  ## Software Notes.
  
  The keyboard communicates in UTF-8. The current implementation supports
  8 reading modes:
  **000** English character set, small letters, numbers
  **001** SHIFT. English character set, capital letters, punctuation marks.
  **010** F1 - additional letters and macros in English
  **011** F2 - additional letters and macros in English
  **100** LANG - national language mode (currently only Cyrillic is implemented)
  **101** SHIFT+LANG - capital letters for Cyrillic
  **110** F1+LANG - additional letters and macros in national language
  **111** F2+LANG - additional letters and macros in national language
  
  The key codes are composed of 16-bit integers, 8 numbers per button
  (by the number of modes). The bit pattern is as following:
  
  **0MMM0000 cccccccc** or
  
  **1MMMaaaa aaaaaaaa**
  
  **MMM** - the next mode (from 000 to 111)
  
  **cccccccc** - a single-byte Unicode character (if >128, it is a control character, such as an arrow)
  
  **aaaaa aaaaaaaaa** - a 12-bit shift in the macros table; the latter is composed of null-terminated strings, up to 4k in length.
