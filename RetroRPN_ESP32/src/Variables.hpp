/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _VARIABLES_HPP
#define _VARIABLES_HPP

#include "Keywords.hpp"

#define VARIABLE_SPACE    32000
#define RPN_PI            3.14159265359
#define CURRENT_DIR_LEN   255

#define VARTYPE_NONE      0
#define VARTYPE_NUMBER    1
#define VARTYPE_VECTOR    2
#define VARTYPE_MATRIX    3
#define VARTYPE_STRING    4

#define VariableToken size_t

//
// Variables are placed in the variable space as following:
//
// byte  0      - variable type
// byte  1      - variable name length (N)
// bytes 2 to N+2 - variable name (null-terminated)
// Then:
// byte  N+3   - either an 8-byte double or an 8-byte integer
// Or:
// byte  N+3   - total length as a 2-byte unsigned integer
// bytes N+5 and above - either a null-terminated string or a vector of 8-byte numbers
// Or:
// byte  N+3   - total length as a 2-byte unsigned integer
// byte  N+5   - row length as a 2-byte unsigned integer
// byte  N+7 and above - a matrix of numbers composed by row
//

class Variables{
  public:
    size_t _var_bottom = 0;
    size_t _standard_bottom = 0;
    size_t _const_top = VARIABLE_SPACE;
    size_t _standard_top = VARIABLE_SPACE;
    byte _buffer[ VARIABLE_SPACE];
    char *currentDir;
    byte *scrMessage;
    byte *rpnLabelX;
    byte *rpnLabelY;
    byte *rpnLabelZ;
    
    // used for quick access from the program
    inline double rpnGetStack( byte i=0){ return _rpnStack[i];};
    inline void rpnSetStack( double v, byte i=0){ _rpnStack[i]=v;};
    inline double *rpnGetStackPtr(){ return _rpnStack;};
    inline double rpnGetPreviousX(){ return *_prev;};
    inline void rpnSetPreviousX( double v){ *_prev = v;};
    inline byte getAngleMode(){ return *_amode;};
    inline void setAngleMode( byte v){
      if( v > 2) v = 0;
      *_amode = (int64_t)v;
    };
    inline void rpnSavePreviousX( byte i=0){ *_prev = _rpnStack[i];};
    inline void rpnRestorePreviousX(){ _rpnStack[0] = *_prev;};
    inline bool rpnIsZero( byte i=0){ return _rpnStack[i] == 0.0;};

    void init();
    VariableToken placeNewVariable( const char *name, byte type=VARTYPE_NUMBER,
            size_t row_size = 1, size_t column_size = 1);
    inline VariableToken placeNewVariable( byte *name, byte type=VARTYPE_NUMBER,
            size_t row_size=1, size_t column_size=1){
      return placeNewVariable( (const char *)name, type, row_size, column_size);
    };
    VariableToken placeNewConstant( const char *name, byte type=VARTYPE_NUMBER,
            size_t row_size = 1, size_t column_size = 1);
    inline VariableToken placeNewConstant( byte *name, byte type=VARTYPE_NUMBER,
            size_t row_size=1, size_t column_size=1){
      return placeNewConstant( (const char *)name, type, row_size, column_size);
    };
    VariableToken placeNewConstantValue( const char *name, double v);

    VariableToken removeVariable( const char *name);
    VariableToken removeConstant( const char *name);

    VariableToken getVariable( const char *name);
    inline VariableToken getVariable( byte *name){
      return getVariable( (const char *)name);};
    VariableToken getConstant( const char *name);
    inline VariableToken getConstant( byte *name){
      return getConstant( (const char *)name);};

    inline VariableToken getFirstVar(){ return 2;};
    inline VariableToken getFirstConst(){ return _const_top+2;};

    VariableToken getNextVar( VariableToken vt);
    inline byte getVarType( VariableToken vt){ return _buffer[vt-2];};
    inline byte getVarNameLength( VariableToken vt){ return _buffer[vt-1];};
    inline const char *getVarName( VariableToken vt){ return (const char *)_buffer + vt;};
    byte nameEndsWith( VariableToken vt);
    bool isNameBASICReal( VariableToken vt);
    bool isNameBASICString( VariableToken vt);
    bool isNameBASICInteger( VariableToken vt);
    double realValue( VariableToken vt, size_t i=0, size_t j=0);
    int64_t integerValue( VariableToken vt, size_t i=0, size_t j=0);
    byte *stringValue( VariableToken vt);
    void setValue( VariableToken vt, double v, size_t i=0, size_t j=0);
    void setValue( VariableToken vt, int64_t v, size_t i=0, size_t j=0);
    inline void setValue( VariableToken vt, int v, size_t i=0, size_t j=0){
      setValue( vt, (int64_t)v, i, j);};
    inline void setValue( VariableToken vt, long v, size_t i=0, size_t j=0){
      setValue( vt, (int64_t)v, i, j);};
    void setValue( VariableToken vt, const char *v);
    void setVector( VariableToken vt, double v);
    void setVector( VariableToken vt, int64_t v);
    inline void setVector( VariableToken vt, long v){ setVector( vt, (int64_t)v);};
    inline void setVector( VariableToken vt, int v){ setVector( vt, (int64_t)v);};
    void rpnSWAP();
    double getConvertedAngle( double a); // converts argument to radians
    double getUnconvertedAngle( double a); // converts argument to current angle representation
    inline double getConvertedAngle(){return getConvertedAngle( _rpnStack[0]);};

  private:
    double *_rpnStack = NULL;
    double *_prev = NULL;
    int64_t *_amode = NULL;    
    size_t _getVarLength( size_t nameLen, byte type=VARTYPE_NONE,
                          size_t total_size=0);
    byte *_getVarBlockPtr( VariableToken vt);
    byte *_getDataPtr( VariableToken vt, size_t i=0, size_t j=0);
    int64_t _limitHuge( double v);
    void _placeVar( 
      byte *ptr, const char *name, byte type, size_t nameLen, 
      size_t total_size, size_t row_size);
    size_t _limitName( const char *name);
};

#endif // _VARIABLES_HPP
