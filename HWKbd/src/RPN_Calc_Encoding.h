/*
  RPN_Calculator.h - codes and macros for Hardware keyboard.
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
*/

const static uint16_t HWKBD_Codepage_RPN[] PROGMEM = {
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 01
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 02
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 03
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 04
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 05
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 06
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 07
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 08
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 09
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 10
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 11
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 12
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 13
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 14
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 15
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 16
0x0037,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 17
0x0038,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 18
0x0039,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 19
0x0030,0x8000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 20
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 21
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 22
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 23
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 24
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 25
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 26
0x0034,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 27
0x0035,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 28
0x0036,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 29
0x005e,0x0070,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 30
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 31
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 32
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 33
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 34
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 35
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 36
0x0031,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 37
0x0032,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 38
0x0033,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 39
0x8003,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 40
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 41
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 42
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 43
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 44
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 45
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 46
0x0030,0x006e,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 47
0x8006,0x006d,0x004d,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 48
0x002e,0x0000,0x1000,0x1000,0x5000,0x5000,0x4000,0x4000, // Button 49
0x8009,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 50
0x4000,0x5000,0x6000,0x7000,0x0000,0x1000,0x2000,0x3000, // Button 51
0x2000,0x2000,0x0000,0x2000,0x6000,0x6000,0x4000,0x6000, // Button 52
0x3000,0x3000,0x3000,0x0000,0x7000,0x7000,0x7000,0x0000, // Button 53
0x800c,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 54
0x800f,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 55
0x8012,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 56
0x8015,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 57
0x8018,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 58
0x1000,0x1000,0x1000,0x1000,0x5000,0x5000,0x5000,0x5000  // Button 59
};

const static byte HWKBD_Macropage_RPN[] PROGMEM = {
0x30,0x30,0x00,0x01,0xb1,0x00,0x01,0x06,0x00,0x01,0x08,0x00,0x01,0x01,0x00,0x01,
0x02,0x00,0x01,0x03,0x00,0x01,0x04,0x00,0x01,0x05,0x00
};