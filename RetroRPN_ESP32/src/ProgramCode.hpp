/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _PROGRAMCODE_HPP
#define _PROGRAMCODE_HPP

#include "Parsers.hpp"

#define BASIC_SPACE       64000
#define LineNumber        size_t
#define LineLenght        size_t

//
// Program is placed in the code space as following:
//
// bytes 0 and 1  - line number as size_t
// byte3 2 and 3  - line length as size_t
// bytes 4 and above - a null-terminated string with code
//
// The program stack is placed below and grows up
//

class ProgramCode{
  public:
    size_t _program_bottom = 0;
    byte _buffer[ BASIC_SPACE];
    LineNumber currentLine = 0;
    
    void init( void *components[]);
    inline byte *getBottom(){ return _buffer + _program_bottom;};
    inline size_t memoryAvailable(){ return  BASIC_SPACE - _program_bottom  - 2*sizeof(size_t);};
    void convertLine();

    // VariableToken placeNewVariable( const char *name, byte type=VARTYPE_NUMBER,
    //         size_t row_size = 1, size_t column_size = 1);
    // inline VariableToken placeNewVariable( byte *name, byte type=VARTYPE_NUMBER,
    //         size_t row_size=1, size_t column_size=1){
    //   return placeNewVariable( (const char *)name, type, row_size, column_size);
    // };
    // VariableToken placeNewConstant( const char *name, byte type=VARTYPE_NUMBER,
    //         size_t row_size = 1, size_t column_size = 1);
    // inline VariableToken placeNewConstant( byte *name, byte type=VARTYPE_NUMBER,
    //         size_t row_size=1, size_t column_size=1){
    //   return placeNewConstant( (const char *)name, type, row_size, column_size);
    // };
    // VariableToken placeNewConstantValue( const char *name, double v);
    
    // VariableToken removeVariable( const char *name);
    // VariableToken removeConstant( const char *name);
    // inline void removeVariables(){ _var_bottom = _standard_bottom;};
    // inline void removeConstants(){ _const_top = _standard_top;};

    // VariableToken getVariable( const char *name);
    // inline VariableToken getVariable( byte *name){
    //   return getVariable( (const char *)name);};
    // VariableToken getConstant( const char *name);
    // inline VariableToken getConstant( byte *name){
    //   return getConstant( (const char *)name);};
    // inline bool isStandardConstant( VariableToken vt){
    //   return vt>_standard_top;};

    // VariableToken getOrCreate( bool asConstant, byte *name, byte type=VARTYPE_NUMBER,
    //   size_t row_size=1, size_t column_size=1);
    // inline VariableToken getOrCreate( bool asConstant, const char *name, byte type=VARTYPE_NUMBER,
    //   size_t row_size=1, size_t column_size=1){
    //   return getOrCreate( asConstant, (byte *)name, type, row_size, column_size);
    // };

    // inline VariableToken getFirstVar(){ return 2;};
    // inline VariableToken getFirstConst(){ return _const_top+2;};
    // VariableToken getNextVar( VariableToken vt);

    // inline byte getVarType( VariableToken vt){ return _buffer[vt-2];};
    // inline byte getVarNameLength( VariableToken vt){ return _buffer[vt-1];};
    // inline const char *getVarName( VariableToken vt){ return (const char *)_buffer + vt;};
    // byte nameEndsWith( VariableToken vt);
    // bool isNameBASICReal( VariableToken vt);
    // bool isNameBASICString( VariableToken vt);
    // bool isNameBASICInteger( VariableToken vt);
    // double realValue( VariableToken vt, size_t i=0, size_t j=0);
    // int64_t integerValue( VariableToken vt, size_t i=0, size_t j=0);
    // byte *stringValue( VariableToken vt);
    // void setValue( VariableToken vt, double v, size_t i=0, size_t j=0);
    // void setValue( VariableToken vt, int64_t v, size_t i=0, size_t j=0);
    // inline void setValue( VariableToken vt, int v, size_t i=0, size_t j=0){
    //   setValue( vt, (int64_t)v, i, j);};
    // inline void setValue( VariableToken vt, long v, size_t i=0, size_t j=0){
    //   setValue( vt, (int64_t)v, i, j);};
    // void setValue( VariableToken vt, const char *v);
    // void setVector( VariableToken vt, double v);
    // void setVector( VariableToken vt, int64_t v);
    // inline void setVector( VariableToken vt, long v){ setVector( vt, (int64_t)v);};
    // inline void setVector( VariableToken vt, int v){ setVector( vt, (int64_t)v);};
    // void rpnSWAP();
    // double getConvertedAngle( double a); // converts argument to radians
    // double getUnconvertedAngle( double a); // converts argument to current angle representation
    // inline double getConvertedAngle(){return getConvertedAngle( _rpnStack[0]);};

    // // used for quick access from the program
    // inline double rpnGetStack( byte i=0){ return _rpnStack[i];};
    // inline void rpnSetStack( double v, byte i=0){ _rpnStack[i] = v;};
    // inline double *rpnGetStackPtr(){ return _rpnStack;};
    // inline double rpnGetPreviousX(){ return *_prev;};
    // inline void rpnSetPreviousX( double v){ *_prev = v;};
    // inline byte getAngleMode(){ return *_amode;};
    // inline void setAngleMode( byte m){
    //   if(m != _MODE_RADIAN_ && m != _MODE_GRADIAN_) m = _MODE_DEGREES_;
    //   *_amode = (int64_t)m;
    // };
    // inline void rpnSavePreviousX( byte i=0){ *_prev = _rpnStack[i];};
    // inline void rpnRestorePreviousX(){ _rpnStack[0] = *_prev;};
    // inline bool rpnIsZero( byte i=0){ return _rpnStack[i] == 0.0;};
    // inline void rpnPOP(byte start=1){
    //   for(byte i=start; i<RPN_STACK; i++) _rpnStack[i-1] = _rpnStack[i];
    // };
    // inline void rpnPUSH(){
    //   for(byte i=RPN_STACK-1; i>0; i--) _rpnStack[i] = _rpnStack[i-1];
    // };
    // inline void rpnPUSH( double v){
    //   rpnPUSH();
    //   *_rpnStack = v;
    // };

  private:
    Keywords *_kwds;
    ExpressionParser *_epar;
};

#endif // _PROGRAMCODE_HPP