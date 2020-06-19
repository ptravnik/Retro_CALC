//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Arduino.h>
#include "CP1251_mod.h"

#define UNKNOWN_UTF8 (byte)'*'
#define DECIMAL_PLACES  9
#define MAXIMUM_NUMBER_LENGTH 22
#define MAX_LINE_NUMBER    10000

//
// Returns true if a spacer
//
bool IsSpacer(byte b);
inline bool IsSpacer(char b){
  return IsSpacer((byte)b);
};

//
// Returns true if an operator terminator
//
bool IsOperatorTerm(byte b);
inline bool IsOperatorTerm(char b){
  return IsOperatorTerm((byte)b);
};

//
// Skips any blanks in the input
//
inline byte *ignore_Blanks( byte * ptr){
  while( IsSpacer(*ptr)) ptr++;
  return ptr;
};

//
// Returns true if a digit
//
inline bool IsDigit(byte b){
  return _ZERO_<=b && b<=_NINER_;
};
inline bool IsDigit(char b){
  return '0'<=b && b<='9';
};

//
// ComparesStrings, returns true if identical
//
inline bool IsKeyword(byte *str, const char *keyword){
  return strcmp((char *)str, keyword) == 0;
};
inline bool IsOneOfKeywords(byte *str, const char **keywords, byte nkw=1);

//
// Returns true if a hexadecimal digit
//
inline byte convertHex(byte b){
  if(_ZERO_<=b && b<=_NINER_) return b-_ZERO_;  // 0-9
  if(_ALPHA_<=b && b<=_FOXTROT_) return b-87; // a-e converts to 10-15
  if(_ALPHA_C_<=b && b<=_FOXTROT_C_) return b-55;  // A-E converts to 10-15
  return 16; // impossible
};
inline byte convertHex(char b){
  convertHex((byte)b);
};

//
// Returns true if a digit or a decimal point
//
inline bool IsDigitOrDecimal(byte b){
  return b==_DECIMAL_ || (_ZERO_<=b && b<=_NINER_);
};
inline bool IsDigitOrDecimal(char b){
  return b=='.' || ('0'<=b && b<='9');
};

//
// Returns true if a valid name starter
//
bool IsNameStarter(byte b);
inline bool IsNameStarter(char b){
  return IsNameStarter((byte)b);
};
bool IsFilenameStarter(byte b);
inline bool IsFilenameStarter(char b){
  return IsFilenameStarter((byte)b);
};

//
// A valid name can contain an underscore,
// Latin or Russian letter,
// a dollar sign $ (for strings)
// and a percent sign % (for integers)
//
inline bool IsNameCharacter(byte b){
  if(IsNameStarter( b)) return true;
  if(IsDigit(b)) return true;
  return (b == _PERCENT_) || (b ==_DOLLAR_);
};
inline bool IsNameCharacter(char b){
  if(IsNameStarter( (byte)b)) return true;
  if(IsDigit(b)) return true;
  return (b == '%') || (b=='$');
};
inline bool IsFilenameCharacter(byte b){
  if(IsFilenameStarter( b)) return true;
  return b == _SP_;
};
inline bool IsFilenameCharacter(char b){
  if(IsFilenameStarter( (byte)b)) return true;
  return b == _SP_;
};

//
// Returns true if a valid terminator
//
bool IsNumberTerm(byte b);
inline bool IsNumberTerm(char b){
  return IsNumberTerm((byte)b);
};
bool IsNameTerm(byte b);
inline bool IsNameTerm(char b){
  return IsNameTerm((byte)b);
};
bool IsListTerm(byte b);
inline bool IsListTerm(char b){
  return IsListTerm((byte)b);
};

//
// returns true if the character is an end of statement
//
bool IsEndStatement( byte b);
inline bool IsEndStatement( char b){
  return IsEndStatement((byte)b);
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
};
inline size_t strlen( const byte *t){
  return strlen((char *)t);
};
inline bool startsWithC(byte *ptr, const char *val){
  byte * tmp = (byte *)strstr((const char*)ptr, val);
  return tmp == ptr;
};

//
// Safe version of strncpy
//
inline char *strncat2(  char * destination, const char * source, size_t num){
  *destination = _NUL_;
  return strncat( destination, source, num);
};

//
// Converts double properly
// Note that the buffer length must be sufficient to acommodate at least 
// the sign, 9 characters of the number, 6 characters of the exponent and 
// the final zero. That is 17 characters.
// Returns a ponter to the end of converted string.
//
//byte *convertDouble( double n, byte *buff, byte precision=8, bool force_sci=true);

//
// Converts a hexadecimal
// Note that the buffer length must be sufficient to acommodate the digits
// Returns a ponter to the end of converted string
//
byte *convert0xH( double n, byte *buff);

//
// Converts large integers into a reasonable line number
//
inline size_t clipLineNumber( int64_t v, size_t bottom=0){
  if( v<bottom) return bottom;
  if( v>MAX_LINE_NUMBER) return MAX_LINE_NUMBER;
  return (size_t)v;
};

#endif // UTILITIES_HPP
