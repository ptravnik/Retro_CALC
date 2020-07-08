/*
  Cyrillic_Phonetic_CP1251_Mockup.h - codes and macros for Hardware keyboard.
  This is applicable to the pre-production mockup version;
  For the production layout, use Cyrillic_Phonetic_CP1251.h

  Copyright (c) 2019 Pavel Travnik.  All rights reserved.

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
const static uint16_t HWKBD_Codepage_Cyrillic_Phonetic_CP1251[] PROGMEM = {
  0x001b, 0x001b, 0x2009, 0x3009, 0x401b, 0x401b, 0x6009, 0x7009, // Button 01
  0x8000, 0x002b, 0x8003, 0x8009, 0x40fd, 0x40dd, 0xc00f, 0x402b, // Button 02
  0x8015, 0x002d, 0x8018, 0x002d, 0x40fe, 0x40de, 0xc01e, 0x402d, // Button 03
  0x8024, 0x002a, 0x8027, 0x002a, 0x40fa, 0x40da, 0xc02e, 0x402a, // Button 04
  0x8035, 0x002f, 0x8038, 0x803f, 0x40fc, 0x40dc, 0xc045, 0x402f, // Button 05
  0x804c, 0x005e, 0x005e, 0x804f, 0x40b8, 0x40a8, 0x405e, 0x405e, // Button 06
  0x8056, 0x007e, 0x005c, 0x8059, 0x407e, 0x405c, 0x40b1, 0x40b1, // Button 07
  0x0065, 0x0065, 0x0065, 0x0065, 0x40f8, 0x40d8, 0x4065, 0x4065, // Button 08
  0x002e, 0x002c, 0x003a, 0x003b, 0x40f9, 0x40d9, 0x40b0, 0x403b, // Button 09
  0x805f, 0x8065, 0x806c, 0xb074, 0xc07b, 0xc081, 0xc088, 0xf090, // Button 10
  0x0031, 0x0021, 0x0000, 0x0000, 0x4031, 0x4021, 0x4000, 0x4000, // Button 11
  0x0032, 0x0040, 0x0000, 0x0000, 0x4032, 0x4040, 0x4000, 0x4000, // Button 12
  0x0033, 0x0023, 0x0000, 0x0000, 0x4033, 0x4023, 0x4000, 0x4000, // Button 13
  0x0034, 0x0024, 0x0000, 0x0000, 0x4034, 0x4024, 0x4000, 0x4000, // Button 14
  0x0035, 0x0025, 0x0000, 0x0000, 0x4035, 0x4025, 0x4000, 0x4000, // Button 15
  0x0036, 0x007c, 0x8097, 0x809b, 0x4036, 0x407c, 0xc09f, 0xc0a3, // Button 16
  0x0037, 0x0026, 0x80a7, 0x80ab, 0x4037, 0x4026, 0xc0b1, 0xc0b5, // Button 17
  0x0038, 0x80bb, 0x80bf, 0x0027, 0x4038, 0xc0c3, 0xc0c7, 0x4027, // Button 18
  0x0039, 0x80cb, 0x80cf, 0x80d3, 0x4039, 0xc0d7, 0xc0db, 0xc0df, // Button 19
  0x0030, 0x003d, 0x80e3, 0x80e6, 0x4030, 0x402b, 0xc0e9, 0xc0ec, // Button 20
  0x0071, 0x0051, 0x80ef, 0x0000, 0x40ff, 0x40df, 0xc0f6, 0x4000, // Button 21
  0x0077, 0x0057, 0x80fd, 0x8105, 0x40e6, 0x40c6, 0xc10c, 0xc114, // Button 22
  0x0065, 0x0045, 0x0000, 0x0000, 0x40e5, 0x40c5, 0x4000, 0x4000, // Button 23
  0x0072, 0x0052, 0x811b, 0x8124, 0x40f0, 0x40d0, 0xc12b, 0xc134, // Button 24
  0x0074, 0x0054, 0x0000, 0x0000, 0x40f2, 0x40d2, 0x4000, 0x4000, // Button 25
  0x0079, 0x0059, 0x0000, 0x0000, 0x40fb, 0x40db, 0x4000, 0x4000, // Button 26
  0x0075, 0x0055, 0x0000, 0x0000, 0x40f3, 0x40d3, 0x4000, 0x4000, // Button 27
  0x0069, 0x0049, 0x0000, 0x0000, 0x40e8, 0x40c8, 0x4000, 0x4000, // Button 28
  0x006f, 0x004f, 0x0000, 0x0000, 0x40ee, 0x40ce, 0x4000, 0x4000, // Button 29
  0x0070, 0x0050, 0x0000, 0x0000, 0x40ef, 0x40cf, 0x4000, 0x4000, // Button 30
  0x0061, 0x0041, 0x813b, 0x8143, 0x40e0, 0x40c0, 0xc151, 0xc158, // Button 31
  0x0073, 0x0053, 0x8166, 0x816c, 0x40f1, 0x40d1, 0xc174, 0xc17a, // Button 32
  0x0064, 0x0044, 0x0000, 0x0000, 0x40e4, 0x40c4, 0x4000, 0x4000, // Button 33
  0x0066, 0x0046, 0x0000, 0x0000, 0x40f4, 0x40d4, 0x4000, 0x4000, // Button 34
  0x0067, 0x0047, 0x0000, 0x0000, 0x40e3, 0x40c3, 0x4000, 0x4000, // Button 35
  0x0068, 0x0048, 0x0000, 0x0000, 0x40f7, 0x40d7, 0x4000, 0x4000, // Button 36
  0x006a, 0x004a, 0x0000, 0x0000, 0x40e9, 0x40e9, 0x4000, 0x4000, // Button 37
  0x006b, 0x004b, 0x0000, 0x0000, 0x40ea, 0x40ca, 0x4000, 0x4000, // Button 38
  0x006c, 0x004c, 0x0000, 0x0000, 0x40eb, 0x40cb, 0x4000, 0x4000, // Button 39
  0x0008, 0x007f, 0x0008, 0x007f, 0x4008, 0x407f, 0x4008, 0x407f, // Button 40
  0x007a, 0x005a, 0x8182, 0x8188, 0x40e7, 0x40c7, 0xc18f, 0xc195, // Button 41
  0x0078, 0x0058, 0x819c, 0x81a2, 0x40f5, 0x40d5, 0xc1a9, 0xc1af, // Button 42
  0x0063, 0x0043, 0x81b6, 0x81bc, 0x40f6, 0x40d6, 0xc1c3, 0xc1c9, // Button 43
  0x0076, 0x0056, 0x0000, 0x0000, 0x40e2, 0x40c2, 0x4000, 0x4000, // Button 44
  0x0062, 0x0042, 0x0000, 0x0000, 0x40e1, 0x40c1, 0x4000, 0x4000, // Button 45
  0x006e, 0x004e, 0x0000, 0x0000, 0x40ed, 0x40cd, 0x4000, 0x4000, // Button 46
  0x006d, 0x004d, 0x0000, 0x0000, 0x40ec, 0x40cc, 0x4000, 0x4000, // Button 47
  0x81d0, 0x91d3, 0x003c, 0x81d6, 0xc1da, 0xd1dd, 0x403c, 0xc1e0, // Button 48
  0x91e4, 0x91e7, 0x003e, 0x81ea, 0xd1ed, 0xd1f0, 0x403e, 0xd1f3, // Button 49
  0x0020, 0x005f, 0x0009, 0x0000, 0x4020, 0x405f, 0x4009, 0x4000, // Button 50
  0x4000, 0x5000, 0x6000, 0x7000, 0x0000, 0x1000, 0x2000, 0x3000, // Button 51
  0x2000, 0x2000, 0x0000, 0x2000, 0x6000, 0x6000, 0x4000, 0x6000, // Button 52
  0x3000, 0x3000, 0x3000, 0x0000, 0x7000, 0x7000, 0x7000, 0x0000, // Button 53
  0x0011, 0x91f6, 0x0015, 0x91fd, 0x4011, 0xd204, 0x4015, 0xd20b, // Button 54
  0x0014, 0x9212, 0x0018, 0xb219, 0x4014, 0xd220, 0x4018, 0xf227, // Button 55
  0x0013, 0x922e, 0x0017, 0xb233, 0x4013, 0xd23a, 0x4017, 0xf23f, // Button 56
  0x0012, 0x9246, 0x0016, 0x824e, 0x4012, 0xd254, 0x4016, 0xc25c, // Button 57
  0x000d, 0x000d, 0x8262, 0x8269, 0x400d, 0x400d, 0xc271, 0xc278, // Button 58
  0x1000, 0x0000, 0x2000, 0x3000, 0x5000, 0x4000, 0x6000, 0x7000  // Button 59
};

const static uint8_t HWKBD_Macropage_Cyrillic_Phonetic_CP1251[] PROGMEM = {
  0x1f, 0x2b, 0x00, 0x0a, 0x72, 0x70, 0x6e, 0x0d,
  0x00, 0x0a, 0x61, 0x69, 0x6e, 0x0d, 0x00, 0x0a,
  0x72, 0x70, 0x6e, 0x0d, 0x00, 0x1f, 0x2d, 0x00,
  0x0a, 0x63, 0x6c, 0x69, 0x0d, 0x00, 0x0a, 0x63,
  0x6c, 0x69, 0x0d, 0x00, 0x1f, 0x2a, 0x00, 0x0a,
  0x65, 0x64, 0x69, 0x74, 0x0d, 0x00, 0x0a, 0x65,
  0x64, 0x69, 0x74, 0x0d, 0x00, 0x1f, 0x2f, 0x00,
  0x0a, 0x66, 0x6d, 0x61, 0x6e, 0x0d, 0x00, 0x0a,
  0x6f, 0x6f, 0x78, 0x0d, 0x00, 0x0a, 0x66, 0x6d,
  0x61, 0x6e, 0x0d, 0x00, 0x1f, 0x5e, 0x00, 0x0a,
  0x72, 0x6f, 0x6f, 0x74, 0x0d, 0x00, 0x1f, 0xb1,
  0x00, 0x0a, 0x6f, 0x6f, 0x78, 0x0d, 0x00, 0x1f,
  0x72, 0x75, 0x6e, 0x1f, 0x00, 0x1f, 0x68, 0x61,
  0x6c, 0x74, 0x1f, 0x00, 0x1f, 0x61, 0x62, 0x6f,
  0x72, 0x74, 0x1f, 0x00, 0x1f, 0x73, 0x74, 0x65,
  0x70, 0x1f, 0x00, 0x1f, 0x72, 0x75, 0x6e, 0x1f,
  0x00, 0x1f, 0x68, 0x61, 0x6c, 0x74, 0x1f, 0x00,
  0x1f, 0x61, 0x62, 0x6f, 0x72, 0x74, 0x1f, 0x00,
  0x1f, 0x73, 0x74, 0x65, 0x70, 0x1f, 0x00, 0x5b,
  0x5d, 0x11, 0x00, 0x7b, 0x7d, 0x11, 0x00, 0x5b,
  0x5d, 0x11, 0x00, 0x7b, 0x7d, 0x11, 0x00, 0x3c,
  0x3e, 0x11, 0x00, 0x3c, 0x2f, 0x3e, 0x11, 0x11,
  0x00, 0x3c, 0x3e, 0x11, 0x00, 0x3c, 0x2f, 0x3e,
  0x11, 0x11, 0x00, 0x22, 0x22, 0x11, 0x00, 0x27,
  0x27, 0x11, 0x00, 0x22, 0x22, 0x11, 0x00, 0x27,
  0x27, 0x11, 0x00, 0x28, 0x29, 0x11, 0x00, 0x5b,
  0x5d, 0x11, 0x00, 0x7b, 0x7d, 0x11, 0x00, 0x28,
  0x29, 0x11, 0x00, 0x5b, 0x5d, 0x11, 0x00, 0x7b,
  0x7d, 0x11, 0x00, 0x3d, 0x3d, 0x00, 0x21, 0x3d,
  0x00, 0x3d, 0x3d, 0x00, 0x21, 0x3d, 0x00, 0x0a,
  0x71, 0x75, 0x61, 0x64, 0x0d, 0x00, 0x0a, 0x71,
  0x75, 0x61, 0x64, 0x0d, 0x00, 0x0a, 0x67, 0x6f,
  0x66, 0x66, 0x32, 0x0d, 0x00, 0x0a, 0x6c, 0x69,
  0x6e, 0x32, 0x0d, 0x00, 0x0a, 0x67, 0x6f, 0x66,
  0x66, 0x32, 0x0d, 0x00, 0x0a, 0x6c, 0x69, 0x6e,
  0x32, 0x0d, 0x00, 0x0a, 0x72, 0x61, 0x64, 0x69,
  0x75, 0x73, 0x0d, 0x00, 0x0a, 0x63, 0x61, 0x74,
  0x68, 0x0d, 0x00, 0x0a, 0x72, 0x61, 0x64, 0x69,
  0x75, 0x73, 0x0d, 0x00, 0x0a, 0x63, 0x61, 0x74,
  0x68, 0x0d, 0x00, 0x0a, 0x63, 0x6c, 0x65, 0x61,
  0x72, 0x20, 0x00, 0x0a, 0x63, 0x6c, 0x65, 0x61,
  0x72, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x20,
  0x00, 0x0a, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x00,
  0x0a, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x20, 0x63,
  0x6f, 0x6e, 0x73, 0x74, 0x20, 0x00, 0x0a, 0x73,
  0x75, 0x6d, 0x0d, 0x00, 0x0a, 0x73, 0x75, 0x6d,
  0x78, 0x79, 0x0d, 0x00, 0x0a, 0x73, 0x75, 0x6d,
  0x0d, 0x00, 0x0a, 0x73, 0x75, 0x6d, 0x78, 0x79,
  0x0d, 0x00, 0x0a, 0x73, 0x69, 0x6e, 0x0d, 0x00,
  0x0a, 0x61, 0x73, 0x69, 0x6e, 0x0d, 0x00, 0x0a,
  0x73, 0x69, 0x6e, 0x0d, 0x00, 0x0a, 0x61, 0x73,
  0x69, 0x6e, 0x0d, 0x00, 0x0a, 0x63, 0x6f, 0x73,
  0x0d, 0x00, 0x0a, 0x61, 0x63, 0x6f, 0x73, 0x0d,
  0x00, 0x0a, 0x63, 0x6f, 0x73, 0x0d, 0x00, 0x0a,
  0x61, 0x63, 0x6f, 0x73, 0x0d, 0x00, 0x0a, 0x74,
  0x61, 0x6e, 0x0d, 0x00, 0x0a, 0x61, 0x74, 0x61,
  0x6e, 0x0d, 0x00, 0x0a, 0x74, 0x61, 0x6e, 0x0d,
  0x00, 0x0a, 0x61, 0x74, 0x61, 0x6e, 0x0d, 0x00,
  0x2c, 0x20, 0x00, 0x21, 0x20, 0x00, 0x3a, 0x0a,
  0x09, 0x00, 0x2c, 0x20, 0x00, 0x21, 0x20, 0x00,
  0x3a, 0x0a, 0x09, 0x00, 0x2e, 0x20, 0x00, 0x3f,
  0x20, 0x00, 0x3b, 0x0a, 0x00, 0x2e, 0x20, 0x00,
  0x3f, 0x20, 0x00, 0x2e, 0x0a, 0x00, 0x53, 0x2d,
  0x6c, 0x65, 0x66, 0x74, 0x00, 0x53, 0x2d, 0x68,
  0x6f, 0x6d, 0x65, 0x00, 0x53, 0x2d, 0x6c, 0x65,
  0x66, 0x74, 0x00, 0x53, 0x2d, 0x68, 0x6f, 0x6d,
  0x65, 0x00, 0x53, 0x2d, 0x64, 0x6f, 0x77, 0x6e,
  0x00, 0x53, 0x2d, 0x70, 0x67, 0x64, 0x6e, 0x00,
  0x53, 0x2d, 0x64, 0x6f, 0x77, 0x6e, 0x00, 0x53,
  0x2d, 0x70, 0x67, 0x64, 0x6e, 0x00, 0x53, 0x2d,
  0x75, 0x70, 0x00, 0x53, 0x2d, 0x70, 0x67, 0x75,
  0x70, 0x00, 0x53, 0x2d, 0x75, 0x70, 0x00, 0x53,
  0x2d, 0x70, 0x67, 0x75, 0x70, 0x00, 0x53, 0x2d,
  0x72, 0x69, 0x67, 0x68, 0x74, 0x00, 0x0a, 0x69,
  0x6e, 0x6a, 0x0d, 0x00, 0x53, 0x2d, 0x72, 0x69,
  0x67, 0x68, 0x74, 0x00, 0x0a, 0x69, 0x6e, 0x6a,
  0x0d, 0x00, 0x53, 0x43, 0x2d, 0x65, 0x6e, 0x64,
  0x00, 0x53, 0x43, 0x2d, 0x68, 0x6f, 0x6d, 0x65,
  0x00, 0x53, 0x43, 0x2d, 0x65, 0x6e, 0x64, 0x00,
  0x53, 0x43, 0x2d, 0x68, 0x6f, 0x6d, 0x65, 0x00
};
