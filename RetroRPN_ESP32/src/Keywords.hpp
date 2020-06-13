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

#define _NO_ERROR_     0
#define _WARNING_      1
#define _ERROR_        2

#define _REQUEST_REDRAW_MSG    0x80
#define _REQUEST_REDRAW_X      0x81
#define _REQUEST_REDRAW_XY     0x82
#define _REQUEST_REDRAW_ALL    0x83
#define _REQUEST_REDRAW_LABELS 0x40
#define _REQUEST_RESET_LABELS  0x20
#define _REQUEST_DO_IOM        0x10

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

#define _FUN_Variable_    -1
#define _FUNCTION_COUNT    74

// Keywords
#define _OPR_ABS_KW             0
#define _OPR_ACOS_KW            1
#define _OPR_ACOSH_KW           2
#define _OPR_ADD_KW             3
#define _OPR_AMODE_KW           4
#define _OPR_API2GCC_KW         5
#define _OPR_APPEND_KW          6
#define _OPR_AREAD_KW           7
#define _OPR_AREAD_INT_KW       8
#define _OPR_ASC_KW             9
#define _OPR_ASIN_KW            10
#define _OPR_ASINH_KW           11
#define _OPR_AT_KW              12
#define _OPR_ATAN_KW            13
#define _OPR_ATANH_KW           14
#define _OPR_AWRITE_KW          15
#define _OPR_AZIMUTH_KW         16
#define _OPR_AZIMUTH3_KW        17
#define _OPR_BAR2PSI_KW         18
#define _OPR_BBL2CM_KW          19
#define _OPR_BREAK_KW           20
#define _OPR_C2F_KW             21
#define _OPR_C2K_KW             22
#define _OPR_CATH_KW            23
#define _OPR_CF2CM_KW           24
#define _OPR_CHAIN_KW           25
#define _OPR_CHR_STR_KW         26
#define _OPR_CIRCLE_KW          27
#define _OPR_CLEAR_KW           28
#define _OPR_CLI_KW             29
#define _OPR_CLOSE_KW           30
#define _OPR_CLS_KW             31
#define _OPR_CM2BBL_KW          32
#define _OPR_CM2CF_KW           33
#define _OPR_COLOR_KW           34
#define _OPR_CON_KW             35
#define _OPR_CONST_KW           36
#define _OPR_CONT_KW            37
#define _OPR_CONTINUE_KW        38
#define _OPR_COORDS_KW          39
#define _OPR_COORDS3_KW         40
#define _OPR_COS_KW             41
#define _OPR_COSH_KW            42
#define _OPR_COT_KW             43
#define _OPR_CURSOR_KW          44
#define _OPR_D2DMS_KW           45
#define _OPR_DATA_KW            46
#define _OPR_DEF_KW             47
#define _OPR_DELAY_KW           48
#define _OPR_DET_KW             49
#define _OPR_DIM_KW             50
#define _OPR_DIR_KW             51
#define _OPR_DIRECTORY_KW       52
#define _OPR_DIV_KW             53
#define _OPR_DMODE_KW           54
#define _OPR_DMS2D_KW           55
#define _OPR_DNOTONE_KW         56
#define _OPR_DOT_KW             57
#define _OPR_DREAD_KW           58
#define _OPR_DTONE_KW           59
#define _OPR_DUMP_KW            60
#define _OPR_DWRITE_KW          61
#define _OPR_EDIT_KW            62
#define _OPR_ELLIPSE_KW         63
#define _OPR_ELSE_KW            64
#define _OPR_END_KW             65
#define _OPR_ENDIF_KW           66
#define _OPR_ENDLOOP_KW         67
#define _OPR_EXP_KW             68
#define _OPR_F2C_KW             69
#define _OPR_FLOAT_KW           70
#define _OPR_FMAN_KW            71
#define _OPR_FN_KW              72
#define _OPR_FOR_KW             73
#define _OPR_FREAD_KW           74
#define _OPR_FREAD_STR_KW       75
#define _OPR_FSIZE_KW           76
#define _OPR_FT2M_KW            77
#define _OPR_GAL2L_KW           78
#define _OPR_GCC2API_KW         79
#define _OPR_GOFF2_KW           80
#define _OPR_GOSUB_KW           81
#define _OPR_GOTO_KW            82
#define _OPR_HEX_STR_KW         83
#define _OPR_IDN_KW             84
#define _OPR_IF_KW              85
#define _OPR_INCH16_STR_KW      86
#define _OPR_INCH2_STR_KW       87
#define _OPR_INCH32_STR_KW      88
#define _OPR_INCH32_STR_KW      89
#define _OPR_INCH4_STR_KW       90
#define _OPR_INCH8_STR_KW       91
#define _OPR_INJ_KW             92
#define _OPR_INKEY_STR_KW       93
#define _OPR_INPUT_KW           94
#define _OPR_INSCAN_STR_KW      95
#define _OPR_INT_INT_KW         96
#define _OPR_INV_KW             97
#define _OPR_KG2LBS_KW          98
#define _OPR_KM2M_KW            99
#define _OPR_KM2NM_KW           100
#define _OPR_KMH2MS_KW          101
#define _OPR_L2GAL_KW           102
#define _OPR_LBS2KG_KW          103
#define _OPR_LCIRC_KW           104
#define _OPR_LEFT_STR_KW        105
#define _OPR_LET_KW             106
#define _OPR_LG_KW              107
#define _OPR_LIN2_KW            108
#define _OPR_LINE_KW            109
#define _OPR_LINETO_KW          110
#define _OPR_LINEW_KW           111
#define _OPR_LIST_KW            112
#define _OPR_LN_KW              113
#define _OPR_LOAD_KW            114
#define _OPR_LOG_KW             115
#define _OPR_LOOP_KW            116
#define _OPR_LT2MT_KW           117
#define _OPR_M2FT_KW            118
#define _OPR_M2KM_KW            119
#define _OPR_MAT_KW             120
#define _OPR_MATRIX_KW          121
#define _OPR_MEM_KW             122
#define _OPR_MID_STR_KW         123
#define _OPR_MM2IN_KW           124
#define _OPR_MOD_KW             125
#define _OPR_MPA2PSI_KW         126
#define _OPR_MS2KMH_KW          127
#define _OPR_MT2LT_KW           128
#define _OPR_MT2ST_KW           129
#define _OPR_MUL_KW             130
#define _OPR_NDIS_KW            131
#define _OPR_NEG_KW             132
#define _OPR_NEW_KW             133
#define _OPR_NEXT_KW            134
#define _OPR_NM2KM_KW           135
#define _OPR_OPEN_KW            136
#define _OPR_PA2PSI_KW          137
#define _OPR_PAUSE_KW           138
#define _OPR_POINT_KW           139
#define _OPR_POKE_KW            140
#define _OPR_POP_KW             141
#define _OPR_POW_KW             142
#define _OPR_PRINT_KW           143
#define _OPR_PRINTALT_KW        144
#define _OPR_PROB_KW            145
#define _OPR_PROBIT_KW          146
#define _OPR_PROBR_KW           147
#define _OPR_PROGRAM_KW         148
#define _OPR_PSI2BAR_KW         149
#define _OPR_PSI2MPA_KW         150
#define _OPR_PSI2PA_KW          151
#define _OPR_PSIA2PSIG_KW       152
#define _OPR_PSIG2PSIA_KW       153
#define _OPR_PUSH_KW            154
#define _OPR_QUAD_KW            155
#define _OPR_RADIUS_KW          156
#define _OPR_RANDOM_KW          157
#define _OPR_RANDOM_INT_KW      158
#define _OPR_READ_KW            159
#define _OPR_RECT_KW            160
#define _OPR_REGR_KW            161
#define _OPR_REM_KW             162
#define _OPR_REMALT_KW          163
#define _OPR_RESET_KW           164
#define _OPR_RESTORE_KW         165
#define _OPR_RIGHT_STR_KW       166
#define _OPR_RMD_KW             167
#define _OPR_RND_KW             168
#define _OPR_ROOT_KW            169
#define _OPR_ROT_KW             170
#define _OPR_ROT3c_KW           171
#define _OPR_ROT3s_KW           172
#define _OPR_ROUND_KW           173
#define _OPR_RPN_KW             174
#define _OPR_RUN_KW             175
#define _OPR_SAVE_KW            176
#define _OPR_SCIRC_KW           177
#define _OPR_SCREEN_KW          178
#define _OPR_SEED_KW            179
#define _OPR_SIGN_KW            180
#define _OPR_SIN_KW             181
#define _OPR_SINH_KW            182
#define _OPR_SM2SFT_KW          183
#define _OPR_SQ_KW              184
#define _OPR_SQRT_KW            185
#define _OPR_SSPHERE_KW         186
#define _OPR_ST2MT_KW           187
#define _OPR_STACK_KW           188
#define _OPR_STAT_KW            189
#define _OPR_STATXY_KW          190
#define _OPR_STEP_KW            191
#define _OPR_STOP_KW            192
#define _OPR_STORE_KW           193
#define _OPR_STR_STR_KW         194
#define _OPR_SUB_KW             195
#define _OPR_SUM_KW             196
#define _OPR_SUMS_KW            197
#define _OPR_SUMXY_KW           198
#define _OPR_SWAP_KW            199
#define _OPR_TAN_KW             200
#define _OPR_TANH_KW            201
#define _OPR_TERM_KW            202
#define _OPR_TIME_KW            203
#define _OPR_TO_KW              204
#define _OPR_TRN_KW             205
#define _OPR_VARS_KW            206
#define _OPR_VECTOR_KW          207
#define _OPR_VSPHERE_KW         208
#define _OPR_WEEKDAY_KW         209
#define _OPR_WRITE_KW           210
#define _OPR_ZER_KW             211
#define _OPR_ROLL_KW            212
#define _OPR_OOX_KW             213
#define _OPR_FACT_KW            214
#define _OPR_CNK_KW             215
#define _OPR_K2C_KW             216
#define _OPR_AIN_KW             217
#define _OPR_KM2MI_KW           218
#define _OPR_MI2KM_KW           219
#define _OPR_IN2MM_KW           220
#define _OPR_SFT2SM_KW          221
#define _OPERATOR_COUNT         222

#define RPN_STACK               20

#define _KEYWORD_ONLY_         -1
#define _RPN_FUNCTION_ONLY_     0
#define _ONE_ARGUMENT_          1
#define _TWO_ARGUMENTS_         2
#define _THREE_ARGUMENTS_       3

struct Keyword{
  int16_t id = 0;
  const char *name0 = NULL;
  const char *name1 = NULL;
  void *operator_ptr = NULL;
  int8_t argumentType = _KEYWORD_ONLY_;
};

class Keywords{
  public:
    Keyword *lastKeywordFound = NULL;
    void init();
    Keyword *getKeyword( byte *str);
    inline Keyword *getKeywordById( int16_t id){
      return _keywords + id; 
    };
    bool isKeyword(byte *str);
    byte *parse( byte *);
  private:
    size_t _id;
    Keyword _keywords[_OPERATOR_COUNT];
    void _addKeyword( const char *name0, const char *name1, void *method = NULL);
};

#endif // _KEYWORDS_HPP
