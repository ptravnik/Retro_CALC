//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _KEYWORDS_HPP
#define _KEYWORDS_HPP

#include <Arduino.h>
#include <math.h>
#include "CP1251_mod.h"
#include "Utilities.hpp"

#define _HUGE_POS_INTEGER_  8000000000000000000L
#define _HUGE_NEG_INTEGER_  -8000000000000000000L

#define _RESULT_UNDEFINED_  0 
#define _RESULT_INTEGER_    1
#define _RESULT_REAL_       2
#define _RESULT_STRING_     3
#define _RESULT_EXECUTED_   4

#define _MODE_DEGREES_  0
#define _MODE_RADIAN_   1 
#define _MODE_GRADIAN_  2

#define INPUT_COLS       256
#define INPUT_LIMIT      255
#define HSCROLL_LIMIT     18
#define RPNBOX_NLEVELS     3
#define _MAX_IDENTIFIER_  22
#define _NUMBER_LENGTH_   22

// screen geometry
#define SCR_MARGIN_X  127
#define SCR_MARGIN_Y  63
#define SCR_COLS      22
#define SCR_ROWS      8
#define SCR_TOP       0
#define SCR_BOTTOM    7
#define SCR_RIGHT     21
#define SCR_TABSTOP   2
#define SYMBOL_WIDTH  6
#define SYMBOL_HEIGHT 8
#define ROW_STEP      8
#define ROW_OFFSET    0
#define COL_OFFSET    0
#define SCR_SIZE (SCR_COLS * SCR_ROWS)

// UI components (in order of initialization)
#define UI_COMP_IOManager         0
#define UI_COMP_LCDManager        1
#define UI_COMP_Keywords          2
#define UI_COMP_Variables         3
#define UI_COMP_Functions         4
#define UI_COMP_ExpressionParser  5
#define UI_COMP_ProgramCode       6
#define UI_COMP_Lexer             7
#define UI_COMP_SDManager         8
#define UI_COMP_MessageBox        9
#define UI_COMP_CommandLine      10
#define UI_COMP_RPNBox           11
#define UI_COMP_TerminalBox      12
#define UI_COMP_FileManager      13
#define UI_COMP_RPNCalculator    14
#define UI_COMP_BasicConsole     15
#define UI_COMPONENTS_COUNT      16

// Current interface
#define UI_UNDEFINED        0
#define UI_RPNCALC          1
#define UI_FILEMAN          2
#define UI_EDITOR           3
#define UI_CONSOLE          4

#define _NOT_AN_ID_    0
#define _UNKNOWN_ID_   1
#define _CONSTANT_     2
#define _VARIABLE_     3
#define _KEYWORD_      4

#define _RPN_COMMON_      0
#define _RPN_CHECK_TRIG_  1
#define _RPN_SWAP_XY_     2
#define _RPN_POWER_       3
#define _RPN_AMODE_       4
#define _RPN_ROOTYX_      5
#define _RPN_QUAD_SOLVER  6
#define _RPN_SQRT_CHECK_  7
#define _RPN_DIV0_CHECK_  8
#define _RPN_QUICK_PUSH_  9
#define _RPN_SWAP_ONLY_   10
#define _RPN_INVTRIG_     11
#define _RPN_GOFF2_SOLVER 12
#define _RPN_STACK_GET    13

#define _FUN_AMODE_KW_     0
#define _FUN_SIN_KW_       1
#define _FUN_COS_KW_       2
#define _FUN_TAN_KW_       3
#define _FUN_POW_KW_       4
#define _FUN_NEG_KW_       5
#define _FUN_LN_KW_        6
#define _FUN_LG_KW_        7
#define _FUN_LOG_KW_       8
#define _FUN_EXP_KW_       9
#define _FUN_SQRT_KW_      10
#define _FUN_SQ_KW_        11
#define _FUN_ROOT_KW_      12
#define _FUN_SIGN_KW_      13
#define _FUN_ABS_KW_       14
#define _FUN_INV_KW_       15
#define _FUN_RADIUS_KW_    16
#define _FUN_QUAD_KW_      17
#define _FUN_SWAP_KW_      18
#define _FUN_LCIRC_KW_     19
#define _FUN_SCIRC_KW_     20
#define _FUN_VSPHERE_KW_   21
#define _FUN_SSPHERE_KW_   22
#define _FUN_ASIN_KW_      23
#define _FUN_ACOS_KW_      24
#define _FUN_ATAN_KW_      25
#define _FUN_GOFF2_KW_     26
#define _FUN_CATH_KW_      27
#define _FUN_STACK_KW_     28
#define _FUN_LIN2_KW_      29
#define _FUNCTION_COUNT    30

#define _OPR_REM_KW        0
#define _OPR_REMALT_KW     1
#define _OPR_CONST_KW      2
#define _OPR_LET_KW        3
#define _OPR_CLEAR_KW      4
#define _OPR_VARS_KW       5
#define _OPR_STORE_KW      6
#define _OPR_LIST_KW       7
#define _OPR_RESTORE_KW    8
#define _OPR_APPEND_KW     9
#define _OPR_DIM_KW        10
#define _OPR_VECTOR_KW     11
#define _OPR_MATRIX_KW     12
#define _OPR_PROGRAM_KW    13
#define _OPR_NEW_KW        14
#define _OPR_LOAD_KW       15
#define _OPR_SAVE_KW       16
#define _OPR_CHAIN_KW      17
#define _OPERATOR_COUNT    18

#define RPN_STACK          20

struct Keyword{
  int16_t id = 0;
  const char *name0 = NULL;
  const char *name1 = NULL;
  void *method = NULL;
};

class Keywords{
  public:
    Keyword *lastKeywordFound = NULL;
    void init();
    Keyword *getKeyword( byte *str);
    inline Keyword *getKeywordById( int16_t id){
      return _keywords + id; 
    };
    byte *parse( byte *);
  private:
    size_t _id;
    Keyword _keywords[_OPERATOR_COUNT];
    void _addKeyword( const char *name0, const char *name1, void *method = NULL);
};

#endif // _KEYWORDS_HPP
