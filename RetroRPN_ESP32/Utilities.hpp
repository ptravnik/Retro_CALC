//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All right reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Arduino.h>

#define UNKNOWN_UTF8 (byte)'*'
#define DECIMAL_PLACES  9
#define MAXIMUM_NUMBER_LENGTH 22

//
// Returns true if a spacer
//
bool IsSpacer(byte b);
inline bool IsSpacer(char b){
  return IsSpacer((byte)b);
};

//
// Returns true if a digit
//
inline bool IsDigit(byte b){
  return 48<=b && b<=57;
};
inline bool IsDigit(char b){
  return '0'<=b && b<='9';
};

//
// Returns true if a digit or decimal
//
inline bool IsDigitOrDecimal(byte b){
  return b==46 || (48<=b && b<=57);
};
inline bool IsDigitOrDecimal(char b){
  return b=='.' || ('0'<=b && b<='9');
};

//
// Returns true if a valid number terminator
//
bool IsNumberTerm(byte b);
inline bool IsNumberTerm(char b){
  return IsNumberTerm((byte)b);
};

//
// Compares a token
//
bool IsToken(char *line, const char *token, bool separator_needed=true);
inline bool IsToken(byte *line, const char *token, bool separator_needed=true){
  return IsToken( (char *)line, token, separator_needed);
};

//
// Converts message or byte from CP1251 into Unicode
// Returns a pointer to the end of the converted string
// buff must have enough space to acommodate data
// (conversion may double the size!)
//
char *convertToUTF8( char *buff, byte *message, size_t nchar);
char *convertToUTF8( char *buff, byte *message);
char *convertToUTF8( char *buff, byte c);

//
// Converts message from Unicode to CP1251
// Returns a pointer to the end of the converted string
// buff must have enough space to acommodate data
// Note that only CP1251 subset is supported
//
byte *convertToCP1251( byte *buff, const char *message, size_t limit);

//
// Byted version of strlen
//
inline size_t strlen( byte *t){
  return strlen((char *)t);
}

//
// Converts double properly
// Note that the buffer length must be sufficient to acommodate at least 
// the sign, 9 characters of the number, 6 characters of the exponent and 
// the final zero. That is 17 characters.
// Returns a ponter to the end of converted string.
//
byte *convertDouble( double n, byte *buff, byte precision=8, bool force_sci=true);

#endif // UTILITIES_HPP
