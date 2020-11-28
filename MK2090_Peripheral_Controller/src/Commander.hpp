//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2020 Mike Yakimov.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef COMMANDER_HPP
#define COMMANDER_HPP

#include <Arduino.h>
#include <RtcDS3231.h>
#include "CircularBuffer.hpp"

#define NCOMMANDS                 10
#define INPUT_BUFFER_LENGTH       40
#define COMMAND_SET_DATETIME      'A'
#define COMMAND_GET_DATETIME      'B'
#define COMMAND_GET_COMPILE_TIME  'C'
#define COMMAND_GET_VOLTAGE       'D'
#define COMMAND_GET_TEMPERATURE   'E'

struct Command{
    uint8_t code;
    void (*action)( char *, uint8_t);
};

class Commander{
  public:
    void init( CircularBuffer *buffer);
    void addCommand( uint8_t code, void (*action)( char *, uint8_t));
    void read();
    void _printDateTime(const RtcDateTime& dt);
    char _inputBuff[INPUT_BUFFER_LENGTH];
  private:
    uint8_t _ncomm = 0;
    Command _commands[NCOMMANDS];
    CircularBuffer *_buffer;
};

#endif
