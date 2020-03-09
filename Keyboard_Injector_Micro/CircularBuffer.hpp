#include <Arduino.h>
#define CIRCULAR_BUFFER_LENGTH 512

class CircularBuffer{
  public:
    inline bool available(){
      return _in != _out;
      };
    void push( char c);
    char pop();
  private:
    size_t _in = 0;
    size_t _out = 0;
    char _buffer[CIRCULAR_BUFFER_LENGTH];
};
