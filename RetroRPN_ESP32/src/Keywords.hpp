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

#define INPUT_COLS    256
#define INPUT_LIMIT   255
#define HSCROLL_LIMIT  18

#define UI_COMP_IOManager         0
#define UI_COMP_LCDManager        1
#define UI_COMP_SDManager         2
#define UI_COMP_ExpressionParser  3
#define UI_COMP_MessageBox        4
#define UI_COMP_CommandLine       5
#define UI_COMP_RPNBox            6
#define UI_COMP_RPNCalculator     7
#define UI_COMP_TerminalBox       8
#define UI_COMP_BasicConsole      9
#define UI_COMP_FileManager      10
#define UI_COMPONENTS_COUNT      11

#define UI_UNDEFINED        0
#define UI_RPNCALC          1
#define UI_FILEMAN          2
#define UI_EDITOR           3
#define UI_CONSOLE          4

#define _NOT_AN_ID_ 0
#define _UNKNOWN_ID_ 1
#define _CONSTANT_ 2
#define _VARIABLE_ 3
#define _KEYWORD_ 4

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

#define _MF_AMODE_KW_     0
#define _MF_DEG_KW_       1
#define _MF_RAD_KW_       2
#define _MF_GRAD_KW_      3
#define _MF_PI_KW_        4
#define _MF_SIN_KW_       5
#define _MF_COS_KW_       6
#define _MF_TAN_KW_       7
#define _MF_POW_KW_       8
#define _MF_NEG_KW_       9
#define _MF_LN_KW_        10
#define _MF_LG_KW_        11
#define _MF_LOG_KW_       12
#define _MF_EXP_KW_       13
#define _MF_SQRT_KW_      14
#define _MF_SQ_KW_        15
#define _MF_ROOT_KW_      16
#define _MF_SIGN_KW_      17
#define _MF_ABS_KW_       18
#define _MF_INV_KW_       19
#define _MF_RADIUS_KW_    20
#define _MF_QUAD_KW_      21
#define _MF_PREV_KW_      22
#define _MF_SWAP_KW_      23
#define _MF_LCIRC_KW_     24
#define _MF_SCIRC_KW_     25
#define _MF_VSPHERE_KW_   26
#define _MF_SSPHERE_KW_   27
#define _MF_ASIN_KW_      28
#define _MF_ACOS_KW_      29
#define _MF_ATAN_KW_      30
#define _MF_TRUE_KW_      31
#define _MF_FALSE_KW_     32
#define _MF_GOFF2_KW_     33
#define _MF_CATH_KW_      34
#define _MF_STACK_KW_     35
#define _MF_LIN2_KW_      36

#define NMATH_FUNCTIONS   37
#define RPN_STACK         20

class Keyword{
  public:
    bool is;
    void show();
    void redraw();
  private:
    bool _lcd;
};

#endif // _KEYWORDS_HPP
