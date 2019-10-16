/*
  Cyrillic_Unicode.h - codes and macros for Hardware keyboard.
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

const static uint16_t HWKBD_Codepage_Cyrillic_UTF8[] PROGMEM = {
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 01
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 02
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 03
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 04
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 05
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 06
0x002b,0x0026,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 07
0x002d,0x007c,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 08
0x002a,0x005e,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 09
0x002f,0x0025,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 10
0x0031,0x0060,0x0000,0x0000,0xc000,0xc003,0x4000,0x4000, // Button 11
0x0032,0x0040,0x0000,0x0000,0xc006,0xc009,0x4000,0x4000, // Button 12
0x0033,0x007e,0x0000,0x0000,0xc00c,0xc00f,0x4000,0x4000, // Button 13
0x0034,0x8012,0x0000,0x0000,0xc015,0xc018,0x4000,0x4000, // Button 14
0x0035,0x801b,0x0000,0x0000,0xc01e,0xc021,0x4000,0x4000, // Button 15
0x0036,0x8024,0x8027,0x802a,0xc02d,0xc030,0x4000,0x4000, // Button 16
0x0037,0x003c,0x8033,0x0000,0xc036,0xc039,0x4000,0x4000, // Button 17
0x0038,0x003e,0x803c,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 18
0x0039,0x0021,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 19
0x0030,0x003d,0x8040,0x8043,0x4030,0x403d,0xc046,0xc049, // Button 20
0x0071,0x0051,0x0000,0x0000,0xc04c,0xc04f,0x4000,0x4000, // Button 21
0x0077,0x0057,0x0000,0x0000,0xc052,0xc055,0x4000,0x4000, // Button 22
0x0065,0x0045,0x0000,0x0000,0xc058,0xc05b,0x4000,0x4000, // Button 23
0x0072,0x0052,0x0000,0x0000,0xc05e,0xc061,0x4000,0x4000, // Button 24
0x0074,0x0054,0x0000,0x0000,0xc064,0xc067,0x4000,0x4000, // Button 25
0x0079,0x0059,0x0000,0x0000,0xc06a,0xc06d,0x4000,0x4000, // Button 26
0x0075,0x0055,0x0000,0x0000,0xc070,0xc073,0x4000,0x4000, // Button 27
0x0069,0x0069,0x0000,0x0000,0xc076,0xc079,0x4000,0x4000, // Button 28
0x006f,0x006f,0x0000,0x0000,0xc07c,0xc07f,0x4000,0x4000, // Button 29
0x0070,0x0050,0x0000,0x0000,0xc082,0xc085,0x4000,0x4000, // Button 30
0x0061,0x0041,0x0000,0x0000,0xc088,0xc08b,0x4000,0x4000, // Button 31
0x0073,0x0053,0x0000,0x0000,0xc08e,0xc091,0x4000,0x4000, // Button 32
0x0064,0x0044,0x0000,0x0000,0xc094,0xc097,0x4000,0x4000, // Button 33
0x0066,0x0046,0x0000,0x0000,0xc09a,0xc09d,0x4000,0x4000, // Button 34
0x0067,0x0047,0x0000,0x0000,0xc0a0,0xc0a3,0x4000,0x4000, // Button 35
0x0068,0x0048,0x0000,0x0000,0xc0a6,0xc0a9,0x4000,0x4000, // Button 36
0x006a,0x004a,0x0000,0x0000,0xc0ac,0xc0af,0x4000,0x4000, // Button 37
0x006b,0x004b,0x0000,0x0000,0xc0b2,0xc0b5,0x4000,0x4000, // Button 38
0x006c,0x004c,0x0000,0x0000,0xc0b8,0xc0bb,0x4000,0x4000, // Button 39
0x80be,0x80c1,0x80c4,0x0000,0xc0c7,0xc0ca,0xc0cd,0x4000, // Button 40
0x007a,0x005a,0x0000,0x0000,0xc0d0,0xc0d3,0x4000,0x4000, // Button 41
0x0078,0x0058,0x0000,0x0000,0xc0d6,0xc0d9,0x4000,0x4000, // Button 42
0x0063,0x0043,0x0000,0x0000,0xc0dc,0xc0df,0x4000,0x4000, // Button 43
0x0076,0x0056,0x0000,0x0000,0xc0e2,0xc0e5,0x4000,0x4000, // Button 44
0x0020,0x005f,0x0000,0x0000,0x4020,0x405f,0x4000,0x4000, // Button 45
0x0062,0x0042,0x0000,0x0000,0xc0e8,0xc0eb,0x4000,0x4000, // Button 46
0x006e,0x004e,0x0000,0x0000,0xc0ee,0xc0f1,0x4000,0x4000, // Button 47
0x006d,0x004d,0x0000,0x0000,0xc0f4,0xc0f7,0x4000,0x4000, // Button 48
0x002e,0x003f,0x90fa,0x90fd,0xd100,0xd103,0x402e,0x403f, // Button 49
0x0000,0x0000,0x0000,0x0000,0x4000,0x4000,0x4000,0x4000, // Button 50
0x4000,0x5000,0x6000,0x7000,0x0000,0x1000,0x2000,0x3000, // Button 51
0x2000,0x2000,0x0000,0x2000,0x6000,0x6000,0x4000,0x6000, // Button 52
0x3000,0x3000,0x3000,0x0000,0x7000,0x7000,0x7000,0x0000, // Button 53
0x8106,0x910b,0x8112,0x9117,0xc11e,0xd123,0xc12a,0xd12f, // Button 54
0x8136,0x913b,0xa142,0xb149,0xc152,0xd157,0xe15e,0xf165, // Button 55
0x816e,0x9171,0xa176,0xb17b,0xc182,0xd185,0xe18a,0xf18f, // Button 56
0x8196,0x919c,0xa1a4,0xb1a8,0xc1ae,0xd1b4,0xe1bc,0xf1c0, // Button 57
0x000a,0x000a,0x81c6,0x81cd,0x400a,0x400a,0xc1d5,0xc1dc, // Button 58
0x1000,0x1000,0x1000,0x1000,0x5000,0x5000,0x5000,0x5000  // Button 59
};

const static byte HWKBD_Macropage_Cyrillic_UTF8[] PROGMEM = {
0xd1,0x91,0x00,0xd0,0x81,0x00,0xd1,0x8a,0x00,0xd0,0xaa,0x00,0xd1,0x8c,0x00,0xd0,
0xac,0x00,0x27,0x27,0x00,0xd1,0x8d,0x00,0xd0,0xad,0x00,0x22,0x22,0x00,0xd1,0x8e,
0x00,0xd0,0xae,0x00,0x28,0x29,0x00,0x5b,0x5d,0x00,0x7b,0x7d,0x00,0xd1,0x85,0x00,
0xd0,0xa5,0x00,0x3c,0x3e,0x00,0xd0,0xb6,0x00,0xd0,0x96,0x00,0x3c,0x2f,0x3e,0x00,
0x3d,0x3d,0x00,0x21,0x3d,0x00,0x3d,0x3d,0x00,0x21,0x3d,0x00,0xd0,0xb9,0x00,0xd0,
0x99,0x00,0xd1,0x86,0x00,0xd0,0xa6,0x00,0xd1,0x83,0x00,0xd0,0xa3,0x00,0xd0,0xba,
0x00,0xd0,0x9a,0x00,0xd0,0xb5,0x00,0xd0,0x95,0x00,0xd0,0xbd,0x00,0xd0,0x9d,0x00,
0xd0,0xb3,0x00,0xd0,0x93,0x00,0xd1,0x88,0x00,0xd0,0xa8,0x00,0xd1,0x89,0x00,0xd0,
0xa9,0x00,0xd0,0xb7,0x00,0xd0,0x97,0x00,0xd1,0x84,0x00,0xd0,0xa4,0x00,0xd1,0x8b,
0x00,0xd0,0xab,0x00,0xd0,0xb2,0x00,0xd0,0x92,0x00,0xd0,0xb0,0x00,0xd0,0x90,0x00,
0xd0,0xbf,0x00,0xd0,0x9f,0x00,0xd1,0x80,0x00,0xd0,0xa0,0x00,0xd0,0xbe,0x00,0xd0,
0x9e,0x00,0xd0,0xbb,0x00,0xd0,0x9b,0x00,0xd0,0xb4,0x00,0xd0,0x94,0x00,0x2c,0x20,
0x00,0x3b,0x20,0x00,0x3b,0x0a,0x00,0x2c,0x20,0x00,0x3b,0x20,0x00,0x3b,0x0a,0x00,
0xd1,0x8f,0x00,0xd0,0xaf,0x00,0xd1,0x87,0x00,0xd0,0xa7,0x00,0xd1,0x81,0x00,0xd0,
0xa1,0x00,0xd0,0xbc,0x00,0xd0,0x9c,0x00,0xd0,0xb8,0x00,0xd0,0x98,0x00,0xd1,0x82,
0x00,0xd0,0xa2,0x00,0xd0,0xb1,0x00,0xd0,0x91,0x00,0x2e,0x20,0x00,0x3f,0x20,0x00,
0x2e,0x20,0x00,0x3f,0x20,0x00,0x6c,0x65,0x66,0x74,0x00,0x53,0x2d,0x6c,0x65,0x66,
0x74,0x00,0x68,0x6f,0x6d,0x65,0x00,0x53,0x2d,0x68,0x6f,0x6d,0x65,0x00,0x6c,0x65,
0x66,0x74,0x00,0x53,0x2d,0x6c,0x65,0x66,0x74,0x00,0x68,0x6f,0x6d,0x65,0x00,0x53,
0x2d,0x68,0x6f,0x6d,0x65,0x00,0x64,0x6f,0x77,0x6e,0x00,0x53,0x2d,0x64,0x6f,0x77,
0x6e,0x00,0x70,0x67,0x64,0x6f,0x77,0x6e,0x00,0x53,0x2d,0x70,0x67,0x64,0x6f,0x77,
0x6e,0x00,0x64,0x6f,0x77,0x6e,0x00,0x53,0x2d,0x64,0x6f,0x77,0x6e,0x00,0x70,0x67,
0x64,0x6f,0x77,0x6e,0x00,0x53,0x2d,0x70,0x67,0x64,0x6f,0x77,0x6e,0x00,0x75,0x70,
0x00,0x53,0x2d,0x75,0x70,0x00,0x70,0x67,0x75,0x70,0x00,0x53,0x2d,0x70,0x67,0x75,
0x70,0x00,0x75,0x70,0x00,0x53,0x2d,0x75,0x70,0x00,0x70,0x67,0x75,0x70,0x00,0x53,
0x2d,0x70,0x67,0x75,0x70,0x00,0x72,0x69,0x67,0x68,0x74,0x00,0x53,0x2d,0x72,0x69,
0x67,0x68,0x74,0x00,0x65,0x6e,0x64,0x00,0x53,0x2d,0x65,0x6e,0x64,0x00,0x72,0x69,
0x67,0x68,0x74,0x00,0x53,0x2d,0x72,0x69,0x67,0x68,0x74,0x00,0x65,0x6e,0x64,0x00,
0x53,0x2d,0x65,0x6e,0x64,0x00,0x53,0x43,0x2d,0x65,0x6e,0x64,0x00,0x53,0x43,0x2d,
0x68,0x6f,0x6d,0x65,0x00,0x53,0x43,0x2d,0x65,0x6e,0x64,0x00,0x53,0x43,0x2d,0x68,
0x6f,0x6d,0x65,0x00
};
