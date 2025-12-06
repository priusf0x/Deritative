#include "calculate_expression.h"

#include <math.h>

#include "Assert.h"
#include "derivative.h"
#include "derivative_defines.h"
#include "tools.h"

#define CALCULATE__(_N0DE_) CalculateTree((_N0DE_), derivative)

// =========================== CHECK_FUNCTION =================================

bool 
CheckIfNoVar(ssize_t      current_node,
             derivative_t derivative)
{
    ASSERT(derivative != NULL); 

    if (current_node == NO_LINK)
    {
        return true;
    }

    bool current_cond = NODE(current_node)->node_value.expression_type 
                                != EXPRESSION_TYPE_VAR;

    if (!current_cond)
    {
        return false; 
    }

    bool ch_cond = CheckIfNoVar(R_O, derivative) 
                    && CheckIfNoVar(L_O, derivative);

    if (!ch_cond)
    {
        return false;
    }
    
    return true; 
}


// ============================ CALCULATION_RULES =============================

static double
CalculateSum(derivative_t derivative, ssize_t current_node)
{ return CALCULATE__(L_O) + CALCULATE__(R_O); } 

static double
CalculateSub(derivative_t derivative, ssize_t current_node)
{ return CALCULATE__(L_O) - CALCULATE__(R_O); } 

static double
CalculateMul(derivative_t derivative, ssize_t current_node)
{ return CALCULATE__(L_O) * CALCULATE__(R_O); } 

static double
CalculateDiv(derivative_t derivative, ssize_t current_node)
{ return CALCULATE__(L_O) / CALCULATE__(R_O); } 

static double
CalculateSin(derivative_t derivative, ssize_t current_node)
{ return sin(CALCULATE__(L_O)); } 

static double
CalculateCos(derivative_t derivative, ssize_t current_node)
{ return cos(CALCULATE__(L_O)); } 

static double
CalculatePow(derivative_t derivative, ssize_t current_node)
{ return pow(CALCULATE__(L_O) ,CALCULATE__(R_O)); } 

static double
CalculateLn(derivative_t derivative, ssize_t current_node)
{ return log(CALCULATE__(L_O)); } 

static double
CalculateExp(derivative_t derivative, ssize_t current_node)
{ return exp(CALCULATE__(L_O)); } 

static double
CalculateTg(derivative_t derivative, ssize_t current_node)
{ return tan(CALCULATE__(L_O)); } 

static double
CalculateCtg(derivative_t derivative, ssize_t current_node)
{ return 1 / tan(CALCULATE__(L_O)); } 

#undef CALCULATE__

#define _CALCULATION_
#include "operation_info.h"

// ================================= CALCULATION ==============================

double 
CalculateTree(ssize_t      current_node,
              derivative_t derivative)
{
    ASSERT(derivative != NULL);

    if (current_node == NO_LINK)
    {
        return 0;
    }

    if (current_node == 0)
    {
        current_node = NODE(0)->left_index;
    }    
    
    expression_s expr = NODE(current_node)->node_value;
    
    switch (expr.expression_type)
    {
        case EXPRESSION_TYPE_CONST:
            return GET_CONST_VAL__(current_node);
        
        case EXPRESSION_TYPE_VAR:
            return GetVariableValue( expr.expression.variable.variable_name, 
                                        derivative->name_table);

        case EXPRESSION_TYPE_OPERATOR:
            return OPERATION_INFO[expr.expression.operation]
                                .calc_function(derivative, current_node);

        case EXPRESSION_TYPE_UNDEFINED:
        default: return 0;
    }

    return 0;
}