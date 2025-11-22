#include "take_derivative.h"

#include "Assert.h"
#include "derivative.h"
#include "latex_dump.h"
#include "tree.h"
#include "tools.h"

static void TakeConstDerivative(derivative_t derivative, size_t current_node);

// =============================== DEFINES ====================================

#define RIGHT derivative->ariphmetic_tree->nodes_array[current_node].right_index
#define LEFT  derivative->ariphmetic_tree->nodes_array[current_node].left_index

#define D(__X) do { take_derivative_return_e output = TAKE_DERIVATIVE_RETURN_SUCCESS; \
                    if ((output = TakeDerivative(derivative, (__X))) != TAKE_DERIVATIVE_RETURN_SUCCESS)\
                    {\
                        return output;\
                    }\
                } while (0)

// =========================== MAIN_DERIVATIVE ================================

take_derivative_return_e
TakeDerivative(derivative_t derivative,
               ssize_t      current_node)
{
    ASSERT(derivative != NULL);

    if (current_node == NO_LINK)
    {
        return TAKE_DERIVATIVE_INCORRECT_EXPRESSION;
    }

    expression_s node_value = derivative->ariphmetic_tree->nodes_array[current_node].node_value;

    if (node_value.expression_type == EXPRESSION_TYPE_CONST)
    {
        DeleteSubgraph(derivative->ariphmetic_tree, current_node);

        return TAKE_DERIVATIVE_RETURN_SUCCESS;
    }
    else if (node_value.expression_type == EXPRESSION_TYPE_VAR)
    {
        derivative->ariphmetic_tree->
            nodes_array[current_node].node_value.expression_type = EXPRESSION_TYPE_CONST;

        return TAKE_DERIVATIVE_RETURN_SUCCESS;
    }
    else if (node_value.expression_type == EXPRESSION_TYPE_OPERATOR)
    {
        switch(node_value.expression.operation)
        {
            case OPERATOR_PLUS:
                break;

            case OPERATOR_MINUS:
                break;

            case OPERATOR_MUL:
                break;
            
            case OPERATOR_DIV:
                break;

            case OPERATOR_SIN:
                break;

            case OPERATOR_COS:
                break;

            case OPERATOR_UNDEFINED:
                return TAKE_DERIVATIVE_UNDEFINED_FUNCTION;

            default: return TAKE_DERIVATIVE_UNDEFINED_FUNCTION;
        }
    }

    return TAKE_DERIVATIVE_RETURN_SUCCESS;
}

// ========================== TAKING_DERIVATIVES ==============================

static void
TakeConstDerivative(derivative_t derivative,
                    size_t       current_node)
{
    ASSERT(derivative != NULL);

    derivative->ariphmetic_tree->
        nodes_array[current_node].node_value.expression_type = EXPRESSION_TYPE_CONST;

    derivative->ariphmetic_tree->
        nodes_array[current_node].node_value.expression.constant = 1;
}

// =========================== OPERATION_DERIVATION ===========================

static take_derivative_return_e
TakePlusDerivative(derivative_t derivative,
                   size_t       current_node)
{
    ASSERT(derivative != NULL);

    D(RIGHT);
    D(LEFT);

    return TAKE_DERIVATIVE_RETURN_SUCCESS;
}

static take_derivative_return_e
TakeMinusDerivative(derivative_t derivative,
                    size_t       current_node)
{
    ASSERT(derivative != NULL);

    D(RIGHT);
    D(LEFT);

    return TAKE_DERIVATIVE_RETURN_SUCCESS;
}

static take_derivative_return_e
TakeMulDerivative(derivative_t derivative,
                  size_t       current_node)
{
    ASSERT(derivative != NULL);

    D(RIGHT);
    D(LEFT);

    return TAKE_DERIVATIVE_RETURN_SUCCESS;
}
