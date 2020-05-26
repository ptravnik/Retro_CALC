//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  RPNStackBox.hpp 
  Implements the display portion of the RPN calculator
*/

#ifndef RPNSTACKBOX_HPP
#define RPNSTACKBOX_HPP

#include "Parsers.hpp"
#include "LCDManager.hpp"

class RPNStackBox{
  public:
    unsigned long init(void *components[]);
    void show();
    void redraw();
    void updateIOM( bool refresh=true);
    void resetRPNLabels();
    void setRPNLabel( byte label, byte *message);
    inline void setRPNLabel( byte label, char *message){
      setRPNLabel( label, (byte *)message);
    };
    inline void setRPNLabel( byte label, const char *message){
      setRPNLabel( label, (byte *)message);
    };
    inline void setStackRedrawAll(){
      memset( _stackRedrawRequired, true, RPNBOX_NLEVELS);
    };
    inline void setLabelRedrawAll(){
      memset( _labelRedrawRequired, true, RPNBOX_NLEVELS);
    };
    inline void setStackRedrawRequired( byte n=0){
      _stackRedrawRequired[ n] = true;
    };
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Variables *_vars;
    LCDManager *_lcd;
    ExpressionParser *_epar;
    bool _labelRedrawRequired[ RPNBOX_NLEVELS];
    bool _stackRedrawRequired[ RPNBOX_NLEVELS];
    byte *_labels[ 3];
};

#endif //RPNSTACKBOX_HPP
