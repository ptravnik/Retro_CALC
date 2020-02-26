#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Arduino.h>

#define UNKNOWN_UTF8 (byte)'*'
#define DECIMAL_PLACES  9
#define MAXIMUM_NUMBER_LENGTH 22

bool IsSpacer(byte b);
inline bool IsSpacer(char b){
  return IsSpacer((byte)b);
};

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
