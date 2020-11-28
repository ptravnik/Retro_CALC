//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#include "CP1251_mod.h"
#include "CircularBuffer.hpp"

//
// Pushes data into a buffer
//
void CircularBuffer::push( char c){
  _buffer[_in++] = c;
  if( _in >= CIRCULAR_BUFFER_LENGTH) _in = 0;
  if( _in == _out){ // cought up with the out pointer
    _out++;
    if( _out >= CIRCULAR_BUFFER_LENGTH) _out = 0;
  }
}

//
// Retrieves one byte
//
char CircularBuffer::pop(){
  if(_in == _out) return _NUL_;
  char c = _buffer[_out++];
  if( _out >= CIRCULAR_BUFFER_LENGTH) _out = 0;
  return c;
}

//
// Gives buffer content without moving the pointer
//
char CircularBuffer::peek(){
  if(_in == _out) return _NUL_;
  return _buffer[_out];
}
