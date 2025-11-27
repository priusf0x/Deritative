#include "take_derivative.h"

#include "Assert.h"
#include "derivative.h"
#include "latex_dump.h"
#include "tree.h"
#include "tools.h"
#include "latex_dump.h"
#include "simplify.h"

static take_derivative_return_e TakeVarDerivative(derivative_t derivative, 
                                                  ssize_t current_node);

// =============================== DEFINES ====================================

#define RIGHT_INDEX(__X) derivative->ariphmetic_tree->nodes_array[__X].right_index
#define LEFT_INDEX(__X)  derivative->ariphmetic_tree->nodes_array[__X].left_index
#define PARENT_INDEX(__X)  derivative->ariphmetic_tree->nodes_array[__X].parent_index
#define PARENT_CONNECTION(__X)  derivative->ariphmetic_tree->nodes_array[__X].parent_connection

static take_derivative_return_e
TakePlusDerivative(derivative_t derivative,
                   ssize_t       current_node);

static take_derivative_return_e
TakeMinusDerivative(derivative_t derivative,
                    ssize_t       current_node);

static take_derivative_return_e
TakeMulDerivative(derivative_t derivative,
                  ssize_t       current_node);

static take_derivative_return_e
TakeDivDerivative(derivative_t derivative,
                  ssize_t      current_node);

static take_derivative_return_e
TakeSinDerivative(derivative_t derivative,
                  ssize_t       current_node);
    
static take_derivative_return_e
TakeCosDerivative(derivative_t derivative,
                  ssize_t      current_node);
                
static take_derivative_return_e
TakeLogDerivative(derivative_t derivative,
                  ssize_t      current_node);

static take_derivative_return_e
TakeExpDerivative(derivative_t derivative,
                  ssize_t      current_node);

static take_derivative_return_e
TakePowerDerivative(derivative_t derivative,
                    ssize_t      current_node);

// =========================== MAIN_DERIVATIVE ================================

take_derivative_return_e
TakeDerivative(derivative_t derivative,
               ssize_t      current_node)
{
    ASSERT(derivative != NULL);

    #ifndef NDEBUG
        TreeDump(derivative->ariphmetic_tree);
    #endif

    CHECK_INDEX;

    expression_s node_value = derivative->ariphmetic_tree->
                                nodes_array[current_node].node_value;

    if (node_value.expression_type == EXPRESSION_TYPE_CONST)
    {
        derivative->ariphmetic_tree->
            nodes_array[current_node].node_value.expression.constant = 0;

        return TAKE_DERIVATIVE_RETURN_SUCCESS;
    }
    else if (node_value.expression_type == EXPRESSION_TYPE_VAR)
    {
        TakeVarDerivative(derivative, current_node);

        return TAKE_DERIVATIVE_RETURN_SUCCESS;
    }
    else if (node_value.expression_type == EXPRESSION_TYPE_OPERATOR)
    {
        switch(node_value.expression.operation)
        {
            case OPERATOR_PLUS:
                TakePlusDerivative(derivative, current_node);
                break;

            case OPERATOR_MINUS:
                TakeMinusDerivative(derivative, current_node);
                break;

            case OPERATOR_MUL:
                TakeMulDerivative(derivative, current_node);
                break;
            
            case OPERATOR_DIV:
                TakeDivDerivative(derivative, current_node);
                break;

            case OPERATOR_SIN:
                TakeSinDerivative(derivative, current_node);
                break;

            case OPERATOR_COS:
                TakeCosDerivative(derivative, current_node);
                break;

            case OPERATOR_LN:
                TakeLogDerivative(derivative, current_node);
                break;

            case OPERATOR_EXP:
                TakeExpDerivative(derivative, current_node);
                break;

            case OPERATOR_POWER:
                TakePowerDerivative(derivative, current_node);
                break;

            case OPERATOR_UNDEFINED:
                return TAKE_DERIVATIVE_UNDEFINED_FUNCTION;

            default: return TAKE_DERIVATIVE_UNDEFINED_FUNCTION;
        }
    }

    return TAKE_DERIVATIVE_RETURN_SUCCESS;
}

// ========================== TAKING_DERIVATIVES ==============================

static take_derivative_return_e
TakeVarDerivative(derivative_t derivative,
                  ssize_t      current_node)
{
    ASSERT(derivative != NULL);

    derivative->ariphmetic_tree->
        nodes_array[current_node].node_value.expression_type = EXPRESSION_TYPE_CONST;

    derivative->ariphmetic_tree->
        nodes_array[current_node].node_value.expression.constant = 1;

    return TAKE_DERIVATIVE_RETURN_SUCCESS;
}

// =========================== OPERATION_DERIVATION ===========================

// ============================= FUNCTION_DERIVATIVES =========================

// static take_derivative_return_e
// TakePlusDerivative(derivative_t derivative,
//                    ssize_t       current_node)
// {
//     ASSERT(derivative != NULL);

//     CHECK_INDEX;

//     D(RIGHT_INDEX(current_node));
//     D(LEFT_INDEX(current_node));

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakeMinusDerivative(derivative_t derivative,
//                     ssize_t       current_node)
// {
//     ASSERT(derivative != NULL);

//     CHECK_INDEX;

//     D(RIGHT_INDEX(current_node));
//     D(LEFT_INDEX(current_node));

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakeMulDerivative(derivative_t derivative,
//                   ssize_t      current_node)
// {
//     ASSERT(derivative != NULL);

//     CHECK_INDEX;

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                   OPERATOR_PLUS, EDGE_DIR_LEFT);

//     current_node = PARENT_INDEX(current_node);

//     COPY(current_node, LEFT_INDEX(current_node), 
//          EDGE_DIR_RIGHT);

//     D(RIGHT_INDEX(RIGHT_INDEX(current_node)));
//     D(LEFT_INDEX(LEFT_INDEX(current_node)));
    
//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakeDivDerivative(derivative_t derivative,
//                   ssize_t      current_node)
// {
//     ASSERT(derivative != NULL);

//     CHECK_INDEX;

//     CHANGE_OPER(OPERATOR_MUL);

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                   OPERATOR_MINUS, EDGE_DIR_LEFT);

//     current_node = PARENT_INDEX(current_node);

//     COPY(current_node, LEFT_INDEX(current_node), EDGE_DIR_RIGHT);

//     D(LEFT_INDEX(LEFT_INDEX(current_node)));
//     D(RIGHT_INDEX(RIGHT_INDEX(current_node)));

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                   OPERATOR_DIV, EDGE_DIR_LEFT);
    
//     current_node = PARENT_INDEX(current_node);

//     ADD_NODE(current_node, EXPRESSION_TYPE_OPERATOR,
//              OPERATOR_POWER, EDGE_DIR_RIGHT);

//     COPY(RIGHT_INDEX(current_node), RIGHT_INDEX(LEFT_INDEX(LEFT_INDEX(current_node))), 
//          EDGE_DIR_LEFT);

//     ADD_NODE(RIGHT_INDEX(current_node), EXPRESSION_TYPE_CONST, 
//                                      2, EDGE_DIR_RIGHT);

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakeSinDerivative(derivative_t derivative,
//                   ssize_t      current_node)
// {
//     ASSERT(derivative != 0);

//     CHECK_INDEX;

//     CHANGE_OPER(OPERATOR_COS);

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                   OPERATOR_MUL, EDGE_DIR_RIGHT);

//     current_node = PARENT_INDEX(current_node);

//     COPY(current_node, LEFT_INDEX(RIGHT_INDEX(current_node)),
//          EDGE_DIR_LEFT);

//     D(LEFT_INDEX(current_node));

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakeCosDerivative(derivative_t derivative,
//                   ssize_t      current_node)
// {
//     ASSERT(derivative != 0);

//     CHANGE_OPER(OPERATOR_SIN);

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                   OPERATOR_MUL, EDGE_DIR_RIGHT);

//     current_node = PARENT_INDEX(current_node);

//     COPY(current_node, LEFT_INDEX(RIGHT_INDEX(current_node)),
//          EDGE_DIR_LEFT);

//     D(LEFT_INDEX(current_node));
    
//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                   OPERATOR_MUL, EDGE_DIR_RIGHT);

//     current_node = PARENT_INDEX(current_node);
    
//     ADD_NODE(current_node, EXPRESSION_TYPE_CONST,
//              -1, EDGE_DIR_LEFT);

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakeLogDerivative(derivative_t derivative,
//                   ssize_t      current_node)
// {
//     ASSERT(derivative != NULL);

//     CHANGE_OPER(OPERATOR_DIV);

//     COPY(current_node, LEFT_INDEX(current_node), EDGE_DIR_RIGHT);

//     DELETE(LEFT_INDEX(current_node));

//     ADD_NODE(current_node, EXPRESSION_TYPE_CONST, 
//                         1, EDGE_DIR_LEFT);

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR, 
//                   OPERATOR_MUL, EDGE_DIR_RIGHT);

//     current_node = PARENT_INDEX(current_node);
    
//     COPY(current_node, RIGHT_INDEX(RIGHT_INDEX(current_node)), 
//          EDGE_DIR_LEFT);

//     D(LEFT_INDEX(current_node));

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakeExpDerivative(derivative_t derivative,
//                   ssize_t      current_node)
// {
//     ASSERT(derivative != NULL);

//     CHECK_INDEX;

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                   OPERATOR_MUL, EDGE_DIR_RIGHT);

//     current_node = PARENT_INDEX(current_node);

//     COPY(current_node, LEFT_INDEX(RIGHT_INDEX(current_node)), 
//          EDGE_DIR_LEFT);
    
//     D(LEFT_INDEX(current_node));

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// static take_derivative_return_e
// TakePowerDerivative(derivative_t derivative,
//                     ssize_t      current_node)
// {
//     ASSERT(derivative != NULL);

//     CHECK_INDEX;

//     INSERT_BEFORE(current_node, EXPRESSION_TYPE_OPERATOR,
//                     OPERATOR_MUL, EDGE_DIR_LEFT);

//     current_node = PARENT_INDEX(current_node);

//     ADD_NODE(current_node, EXPRESSION_TYPE_OPERATOR,
//                 OPERATOR_PLUS, EDGE_DIR_RIGHT);

//     ADD_NODE(RIGHT_INDEX(current_node), EXPRESSION_TYPE_OPERATOR,
//                 OPERATOR_MUL, EDGE_DIR_LEFT);

//     COPY(LEFT_INDEX(RIGHT_INDEX(current_node)), LEFT_INDEX(LEFT_INDEX(current_node)),
//             EDGE_DIR_LEFT); 
//     COPY(LEFT_INDEX(RIGHT_INDEX(current_node)), RIGHT_INDEX(LEFT_INDEX(current_node)),
//             EDGE_DIR_RIGHT);

//     current_node = RIGHT_INDEX(current_node);

//     COPY(current_node, LEFT_INDEX(current_node), EDGE_DIR_RIGHT);

//     current_node = LEFT_INDEX(current_node);

//     INSERT_BEFORE(LEFT_INDEX(current_node), EXPRESSION_TYPE_OPERATOR,
//                     OPERATOR_DIV, EDGE_DIR_LEFT);
                    
//     COPY(current_node, LEFT_INDEX(current_node), EDGE_DIR_RIGHT);
//     D(LEFT_INDEX(current_node));

//     current_node = RIGHT_INDEX(PARENT_INDEX(current_node));

//     D(RIGHT_INDEX(current_node));
    
//     INSERT_BEFORE(LEFT_INDEX(current_node), EXPRESSION_TYPE_OPERATOR,
//                     OPERATOR_LN, EDGE_DIR_LEFT);

//     return TAKE_DERIVATIVE_RETURN_SUCCESS;
// }

// ============================== UNDEFINITION ================================

#undef RIGHT_INDEX
#undef LEFT_INDEX  
#undef PARENT_INDEX  
#undef PARENT_CONNECTION  
#undef CHECK_INDEX 
#undef INSERT_BEFORE 
#undef COPY 
#undef D 
#undef ADD_NODE
#undef CHANGE_OPER
#undef TURN_ON_CAST
#undef TURN_OFF_CAST 
#undef INIT_EXPR
#undef DELETE