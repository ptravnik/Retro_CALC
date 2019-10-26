#ifndef RRPN_Parser_h
#define RRPN_Parser_h

/*
  Parser.h - parser for use in the RPN calculator
  Copyright (c) 2019 Pavel Travnik.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

// uncomment for debugging
//#define RRPN_PARSER_DEBUG_

#include <Arduino.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

class RRPN_Parser{    
  public:
    byte decimal_places = 6;
    bool expression_Error = false;
    char *parser_Position;
    char *DoubleToString( double n, char *buff);
    double StringToDouble( char *buff);
  private:
    double _process_Double( double result, int exponent_value, bool exponent_minus);
    double _process_Multiplier( double result, int exponent_value, bool exponent_minus, int mult);
    inline void _ignore_Blanks(){
      while(*parser_Position == ' ' || *parser_Position == '\t') parser_Position++;
      };
};

extern RRPN_Parser Parser;
#endif
