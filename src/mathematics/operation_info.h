#ifndef OPERATION_INFO_H
#define OPERATION_INFO_H

#include <stdlib.h>

#include "derivative.h"
#include "my_string.h"

struct operation_info_s 
{
    operations_e operation;
    string_s     operation_name;
    double       (*calc_function) (derivative_t, ssize_t);
    ssize_t      (*op_function) (derivative_t, ssize_t);
    size_t       argc;
    const char*  latex_dump_name;
    size_t       priority;
};

#ifdef _CALCULATION_
    #define TURN_OFF_CALCULATE(_CALCULATE__) _CALCULATE__
#else
    #define TURN_OFF_CALCULATE(_CALCULATE__) NULL
#endif
#ifdef _DERIVATOR_
    #define TURN_OFF_DERIVATOR(_DERIVATE_) _DERIVATE_
#else
    #define TURN_OFF_DERIVATOR(_DERIVATE_) NULL
#endif
#ifndef _READER_
    #define TURN_OFF_OP(_STRING_SRC_, _STRING_SIZE_) {_STRING_SRC_, _STRING_SIZE_}
#else
    #define TURN_OFF_OP(_STRING_SRC_, _STRING_SIZE_) {NULL,  0}
#endif

#pragma GCC diagnostic ignored "-Wwrite-strings"
const operation_info_s OPERATION_INFO[] =
{
   //OPERATION           OPERATION_NAME          CALCULATION                       DERIVATION
    {OPERATOR_UNDEFINED, TURN_OFF_OP( NULL, 0) , TURN_OFF_CALCULATE(NULL        ), TURN_OFF_DERIVATOR(NULL               ), 0, "undefined", 0},
    {OPERATOR_PLUS     , TURN_OFF_OP( "-" , 1) , TURN_OFF_CALCULATE(CalculateSum), TURN_OFF_DERIVATOR(TakePlusDerivative ), 2, "+"        , 1},
    {OPERATOR_MINUS    , TURN_OFF_OP( "*" , 1) , TURN_OFF_CALCULATE(CalculateSub), TURN_OFF_DERIVATOR(TakeMinusDerivative), 2, "-"        , 1},
    {OPERATOR_MUL      , TURN_OFF_OP( "*" , 1) , TURN_OFF_CALCULATE(CalculateMul), TURN_OFF_DERIVATOR(TakeMulDerivative  ), 2, "*"        , 2},
    {OPERATOR_DIV      , TURN_OFF_OP( "/" , 1) , TURN_OFF_CALCULATE(CalculateDiv), TURN_OFF_DERIVATOR(TakeDivDerivative  ), 2, "\\over"   , 2},
    {OPERATOR_SIN      ,            {"sin", 3} , TURN_OFF_CALCULATE(CalculateSin), TURN_OFF_DERIVATOR(TakeSinDerivative  ), 1, "sin"      , 3},
    {OPERATOR_COS      ,            {"cos", 3} , TURN_OFF_CALCULATE(CalculateCos), TURN_OFF_DERIVATOR(TakeCosDerivative  ), 1, "cos"      , 3},
    {OPERATOR_POWER    , TURN_OFF_OP( "^" , 1) , TURN_OFF_CALCULATE(CalculatePow), TURN_OFF_DERIVATOR(TakePowDerivative  ), 2, "^"        , 3},
    {OPERATOR_LN       ,            {"ln" , 2} , TURN_OFF_CALCULATE(CalculateLn ), TURN_OFF_DERIVATOR(TakeLnDerivative   ), 1, "ln"       , 3},
    {OPERATOR_EXP      ,            {"exp", 3} , TURN_OFF_CALCULATE(CalculateExp), TURN_OFF_DERIVATOR(TakeExpDerivative  ), 1, "exp"      , 3},
    {OPERATOR_TG       ,            {"tg" , 2} , TURN_OFF_CALCULATE(CalculateTg ), TURN_OFF_DERIVATOR(TakeTgDerivative   ), 1, "tg"       , 3},
    {OPERATOR_CTG      ,            {"ctg", 3} , TURN_OFF_CALCULATE(CalculateCtg), TURN_OFF_DERIVATOR(TakeCtgDerivative  ), 1, "ctg"      , 3},
};
const size_t OPERATION_COUNT = sizeof(OPERATION_INFO) / sizeof(OPERATION_INFO[0]);

#pragma GCC diagnostic warning "-Wwrite-strings"

#undef CONDITIONAL_COMPILATION

#endif // OPERATION_INFO_H