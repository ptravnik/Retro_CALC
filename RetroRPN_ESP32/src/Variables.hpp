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
#define NEAR_ZERO         1.0e-100
#define CURRENT_DIR_LEN   255
#define CURRENT_FILE_LEN  255
#define LIST_LEN          64 

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
    byte mathError = 0;
    size_t _var_bottom = 0;
    size_t _read_only_bottom = 0;
    size_t _standard_bottom = 0;
    size_t _const_top = VARIABLE_SPACE;
    size_t _standard_top = VARIABLE_SPACE;
    byte *_buffer = NULL;
    char *currentDir;
    char *currentFile;
    byte *scrMessage;
    byte *rpnLabelX;
    byte *rpnLabelY;
    byte *rpnLabelZ;

    // Rudimentary data list for backward compatibility
    double dataList[LIST_LEN];
    byte listWritePosition = 0;
    byte listReadPosition = 0;

    // 1-D statistical
    double *nmean;
    double *mean;
    double *stdev;
    double sumsq = 0.0;
    double variance = 0.0;

    // 2-D statistical
    double *nmeanXY;
    double *meanX;
    double *stdevX;
    double sumsqX = 0.0;
    double varianceX = 0.0;
    double *meanY;
    double *stdevY;
    double sumsqY = 0.0;
    double varianceY = 0.0;
    double sumsqXY = 0.0;
    double varianceXY = 0.0;
    double sumsqYX = 0.0;
    double varianceYX = 0.0;
    double *gain;
    double *offset;
    double rXY = 0.0;
    double stErrXY = 0.0;

    void init( void *components[]);

    //
    // List available variables or constants
    //
    inline VariableToken getFirstVar(){ return 2;};
    inline VariableToken getFirstConst(){ return _const_top+2;};
    VariableToken getNextVar( VariableToken vt);
    inline byte getVarType( VariableToken vt){ return _buffer[vt-2];};
    inline byte getVarNameLength( VariableToken vt){ return _buffer[vt-1];};
    inline byte *getVarName( VariableToken vt){ return (byte *)_buffer + vt;};
    uint16_t getTotalSize( VariableToken vt);
    uint16_t getRowSize( VariableToken vt);
    uint16_t getColumnSize( VariableToken vt);
    inline bool isConstant( VariableToken vt){ return vt>_const_top;};
    inline bool isReadOnly( VariableToken vt){
        if( vt-2 <=_read_only_bottom) return true;
        return vt>_standard_top;};
    inline bool isUnremovable( VariableToken vt){
        if( vt < 2) return true; 
        if( vt-2 <=_standard_bottom) return true;
        return vt > _standard_top;};
    byte nameEndsWith( VariableToken vt);
    bool isNameBASICReal( VariableToken vt);
    bool isNameBASICString( VariableToken vt);
    bool isNameBASICInteger( VariableToken vt);

    //
    // Set and return values
    //
    void setValueReal( VariableToken vt, double v, uint16_t i=0, uint16_t j=0);
    void setValueInteger( VariableToken vt, int64_t v, uint16_t i=0, uint16_t j=0);
    inline void setValueInteger( VariableToken vt, int v, uint16_t i=0, uint16_t j=0){
        setValueInteger( vt, (int64_t)v, i, j);};
    inline void setValueInteger( VariableToken vt, long v, uint16_t i=0, uint16_t j=0){
        setValueInteger( vt, (int64_t)v, i, j);};
    void setValueString( VariableToken vt, const char *v);
    void setValueRealArray( VariableToken vt, double v);
    void setValueIntegerArray( VariableToken vt, int64_t v);
    inline void setValueIntegerArray( VariableToken vt, long v){
        setValueIntegerArray( vt, (int64_t)v);};
    inline void setValueIntegerArray( VariableToken vt, int v){
        setValueIntegerArray( vt, (int64_t)v);};
    double realValue( VariableToken vt, uint16_t i=0, uint16_t j=0);
    int64_t integerValue( VariableToken vt, uint16_t i=0, uint16_t j=0);
    byte *stringValue( VariableToken vt);
    inline uint16_t getPrgCounter(){ return (uint16_t)(_prgCounter[0]);};
    inline void setPrgCounter(uint16_t ln){ _prgCounter[0] = (int64_t)ln;};
    void appendToData( double value);
    inline void clearData(){listWritePosition = 0;};
    double readData();
    inline void restoreReadPosition(){ listReadPosition = 0;};

    //
    // Looks for constants and variables
    //
    VariableToken findByName( const char *name);
    inline VariableToken findByName( byte *name){
        return findByName( (const char *)name);};
    VariableToken _findConstantByName( const char *name);
    inline VariableToken _findConstantByName( byte *name){
        return _findConstantByName( (const char *)name);};
    VariableToken _findVariableByName( const char *name);
    inline VariableToken _findVariableByName( byte *name){
        return _findVariableByName( (const char *)name);};
    VariableToken getOrCreateNumber( bool asConstant, byte *name);
    byte *findDataPtr( const char * name, uint16_t i=0, uint16_t j=0);

    //
    // Variables and Constants removal
    //
    void removeByToken( VariableToken vt);
    void removeByName( const char *name);
    inline void removeByName( byte *name){
        removeByName( (const char *)name);};
    inline void removeAllVariables(){ _var_bottom = _standard_bottom;};
    inline void removeAllConstants(){ _const_top = _standard_top;};
    
    //
    // RPN banging methods
    //
    void swapRPNXY();
    void popRPNStack(byte start=1);
    void pushRPNStack();
    void pushRPNStack( double v);
    void rollRPNStack();
    inline double getRPNRegister( byte i=0){ return _rpnStack[i];};
    inline void setRPNRegister( double v, byte i=0){ _rpnStack[i] = v;};
    inline void negateRPNX(){ _rpnStack[0] = -_rpnStack[0];};    
    inline void inverseRPNX(){ _rpnStack[0] = 1.0/_rpnStack[0];};    
    inline void clearRPNStack(){
        *_prev = 0.0;
        for( byte i=0; i<RPN_STACK; i++) _rpnStack[i] = 0.0;};
    void clearRPNSum();
    void clearRPNSumXY();
    void addSample2RPNSum( double v);
    void addSample2RPNSumXY( double x, double y);
    inline double *getRPNStackPtr(){ return _rpnStack;};
    inline double getRPNPrev(){ return *_prev;};
    inline void setRPNPrev( double v){ *_prev = v;};
    inline void saveRPNPrev( byte i=0){ *_prev = _rpnStack[i];};
    inline void restoreRPNPrev(){ _rpnStack[0] = *_prev;};
    inline bool isRPNRegisterZero( byte i=0){ return _rpnStack[i] == 0.0;};

    //
    // angle mode is used in trig computations
    //
    inline byte getAngleMode(){
        if( *_amode > _MODE_GRADIAN_) *_amode = _MODE_GRADIAN_;
        if( *_amode < _MODE_DEGREES_) *_amode = _MODE_DEGREES_;
        return (byte)(*_amode);
        };
    inline void setAngleMode( byte m){
        if(m != _MODE_RADIAN_ && m != _MODE_GRADIAN_) m = _MODE_DEGREES_;
        *_amode = (int64_t)m;};
    double getConvertedAngle( double a); // converts argument to radians
    double getUnconvertedAngle( double a); // converts argument to current angle representation
    inline double getConvertedAngleRPNX(){return getConvertedAngle( _rpnStack[0]);};
    const char *getAMODEString();

    //
    // safe access to screen messages 
    //
    inline void setScrMessage( const char *mess){
        strncat2( (char *)scrMessage, mess, HSCROLL_LIMIT);};
    inline void setRPNLabelX( const char *mess){
        strncat2( (char *)rpnLabelX, mess, HSCROLL_LIMIT);};
    inline void setRPNLabelY( const char *mess){
        strncat2( (char *)rpnLabelY, mess, HSCROLL_LIMIT);};
    inline void setRPNLabelZ( const char *mess){
        strncat2( (char *)rpnLabelZ, mess, HSCROLL_LIMIT);};

    //
    // returns available variable memory
    //
    inline size_t getMemoryAvailable(){
        return _const_top - _var_bottom;};
    inline void setMemoryAvailable( size_t prg = 0){
        *_varAvailble = (int64_t)getMemoryAvailable();
        *_prgAvailble = (int64_t)prg;};

    //
    // simple data converters and mantras
    //
    byte _convert1_( double *args, double *rets, bool isRPN,
        double Gain, double Offset=0.0);
    byte _convert2_( double *args, double *rets, bool isRPN,
        double Gain, double Offset1=0.0, double Offset2=0.0);
    byte _RPN_Mantra_( double value, byte pops=0);
    byte _nonRPN_Mantra_( double value, double *rets);
    byte _Universal_Mantra_( bool isRPN,  double value, double *rets, byte pops=0){
        return isRPN?
            _RPN_Mantra_( value, pops):
            _nonRPN_Mantra_( value, rets);};

  private:
    Keywords *_kwds;
    double *_rpnStack = NULL;
    double *_prev = NULL;
    int64_t *_amode = NULL;
    int64_t *_varAvailble = NULL;
    int64_t *_prgAvailble = NULL;
    int64_t *_prgCounter = NULL;

    int64_t _limitHuge( double v);
    uint16_t _getVarLength( uint16_t nameLen, byte vtype=VARTYPE_NONE,
                          uint16_t total_size=0);
    byte *_getLengthBlockPtr( VariableToken vt);
    byte *_getVarBlockPtr( VariableToken vt);
    byte *_getDataPtr( VariableToken vt, uint16_t i=0, uint16_t j=0);
    VariableToken _allocateVarSpace( bool isConst,
        uint16_t nameLen, byte varType, uint16_t totalSize);
    VariableToken _placeVarName( bool isConst, const char *name,
        byte varType, uint16_t totalSize);
    VariableToken _placeNumber( bool isConst,
        const char *name, double value=0.0);
    VariableToken _placeVector( bool isConst,
        const char *name, uint16_t length, double value=0.0);
    VariableToken _placeMatrix( bool isConst,
        const char *name, uint16_t rows, uint16_t cols, double value=0.0);
    VariableToken _placeString( bool isConst,
        const char *name, uint16_t length, const char* value=NULL);
    VariableToken _getNextVar( VariableToken vt);
    void _removeVariable( VariableToken vt);
    void _removeConstant( VariableToken vt);
};

#endif // _VARIABLES_HPP
