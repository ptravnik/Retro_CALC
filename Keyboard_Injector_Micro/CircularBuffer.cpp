#include "CP1251_mod.h"
#include "CircularBuffer.hpp"

void CircularBuffer::push( char c){
  _buffer[_in++] = c;
  if( _in >= CIRCULAR_BUFFER_LENGTH) _in = 0;
  if( _in == _out){ // cought up with the out pointer
    _out++;
    if( _out >= CIRCULAR_BUFFER_LENGTH) _out = 0;
  }
}

char CircularBuffer::pop(){
  if(_in == _out) return _NUL_;
  char c = _buffer[_out++];
  if( _out >= CIRCULAR_BUFFER_LENGTH) _out = 0;
  return c;
}
