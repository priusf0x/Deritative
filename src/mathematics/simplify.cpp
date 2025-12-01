#include "simplify.h"

#include <stdlib.h>

#include "Assert.h"
#include "derivative.h"
#include "tools.h"
#include "expression.h"
#include "derivative_defines.h"

// ======================= SIMPLIFICATION_HELPERS =============================

inline static bool
CheckIfType(const derivative_t derivative,   
            ssize_t            current_node,
            expression_type_e  expression)
{return derivative->ariphmetic_tree->nodes_array[current_node].
                        node_value.expression_type == expression;}
#define CHECK_IF_TYPE(_EXPR_, _NODE_) CheckIfType(derivative, _NODE_, _EXPR_)

inline static bool
CheckIfOp(const derivative_t derivative,   
          ssize_t            current_node,
          operations_e       operation)
{return derivative->ariphmetic_tree->nodes_array[current_node].
                        node_value.expression.operation == operation;}
#define CHECK_OP(_OP_, _NODE_) CheckIfOp(derivative, _NODE_, _OP_)

static bool 
CheckNodeConstAndEq(const derivative_t derivative,
                    ssize_t            node,
                    double             num)
{
    double node_value = derivative->ariphmetic_tree->nodes_array[node].
                                            node_value.expression.constant; 
    return CHECK_IF_TYPE(EXPRESSION_TYPE_CONST, node) && 
                                                CheckIfEqual(node_value, num);
}

#define CHECK_ONE(_NODE_)  (CheckNodeConstAndEq(derivative, _NODE_, (double) 1))
#define CHECK_ZERO(_NODE_) (CheckNodeConstAndEq(derivative, _NODE_, (double) 0))

// =========================== SIMPLIFICATION =================================

// ======================= CONSTANT_SIMPLIFICATION ============================




// ========================== NEUTRAL_SIMPLIFICATION ==========================

static ssize_t
SimplifyMultiplicationOnZero(derivative_t derivative,
                             ssize_t      current_node)
{ REPLACE(CONST(0)); }

static ssize_t
SimplifySumWithZero(derivative_t derivative,
                    ssize_t      current_node)
{
    node_s node = derivative->ariphmetic_tree->nodes_array[current_node];
    fprintf(stderr, "%ld %ld %ld", current_node, node.left_index, node.right_index);

    if (CHECK_ZERO(node.left_index))
    {
        REPLACE(cR);
    }
    else if(CHECK_ZERO(node.right_index))
    {
        REPLACE(cL);
    }

    return NO_LINK;
}

static ssize_t
SimplifySubWithZero(derivative_t derivative,
                    ssize_t      current_node)
{
    node_s node = derivative->ariphmetic_tree->nodes_array[current_node];

    if (CHECK_ZERO(node.left_index))
    {
        REPLACE(MUL(CONST(-1), cR));
    }
    else if(CHECK_ZERO(node.right_index))
    {
        REPLACE(cL);
    }

    return NO_LINK;
}

static ssize_t
SimplifyMulOnOne(derivative_t derivative,
                 ssize_t      current_node)
{
    node_s node = derivative->ariphmetic_tree->nodes_array[current_node];

    if (CHECK_ONE(node.left_index))
    {
        REPLACE(cR);
    }
    else if(CHECK_ONE(node.right_index))
    {
        REPLACE(cL);
    }

    return NO_LINK;
}
#include "color.h"
ssize_t
SimplifyNeutralMultipliers(derivative_t derivative,
                           ssize_t      current_node)
{
    ASSERT(derivative);
 
    RETURN_NO_LINK_IF_ERROR;
    
    if (current_node == NO_LINK)
    {
        return NO_LINK;
    }

    #ifndef NDEBUG
        TreeDump(derivative->ariphmetic_tree);
    #endif

    node_s* node = &(derivative->ariphmetic_tree->nodes_array[current_node]);
    
    ssize_t new_right = SimplifyNeutralMultipliers(derivative, node->right_index);
    node = derivative->ariphmetic_tree->nodes_array + current_node;
    node->right_index = new_right;
    if (new_right != NO_LINK)
    {
        NODE(new_right)->parent_index = current_node;
        NODE(new_right)->parent_connection = EDGE_DIR_RIGHT;
    }

    ssize_t new_left = SimplifyNeutralMultipliers(derivative, node->left_index);
    node = derivative->ariphmetic_tree->nodes_array + current_node;
    node->left_index = new_left;
    if (new_left != NO_LINK)
    {
        node->parent_index = current_node;
        node->parent_connection = EDGE_DIR_LEFT;
    }

    RETURN_NO_LINK_IF_ERROR;

    if (CHECK_IF_TYPE(EXPRESSION_TYPE_OPERATOR, current_node))
    {
        if (CHECK_OP(OPERATOR_MUL, current_node) &&     
            (CHECK_ZERO(node->left_index) || CHECK_ZERO(node->right_index))) 
        {
            return SimplifyMultiplicationOnZero(derivative, current_node);
        }
        else if (CHECK_OP(OPERATOR_PLUS, current_node) &&
                 (CHECK_ZERO(node->left_index) || CHECK_ZERO(node->right_index)))
        {
            return SimplifySumWithZero(derivative, current_node);
        }
        else if (CHECK_OP(OPERATOR_MINUS, current_node) &&
                 (CHECK_ZERO(node->left_index) || CHECK_ZERO(node->right_index)))
        {
            return SimplifySubWithZero(derivative, current_node);
        }   
        else if (CHECK_OP(OPERATOR_MUL, current_node) &&
                 (CHECK_ONE(node->left_index) || CHECK_ONE(node->right_index))) 
        {
            return SimplifyMulOnOne(derivative, current_node);
        }
    }

    return current_node;
}

// ============================ UNDEFINITION ==================================

#undef CHECK_ONE
#undef CHECK_ZERO

// simplify_return_e
// SimplifyDoubles(derivative_t derivative,
//                 ssize_t      current_node)
// {
//     ASSERT(derivative != NULL);

//     simplify_return_e output = SIMPLIFY_RETURN_SUCCESS;
    
//     node_s node = derivative->ariphmetic_tree->nodes_array[current_node];

//     if (node.right_index != NO_LINK)
//     {
//         SimplifyNeutralMultipliers(derivative, node.right_index);
//     }
//     if (node.left_index != NO_LINK)
//     {
//         SimplifyNeutralMultipliers(derivative, node.left_index);
//     }

//     if (CheckIfOperator(derivative, current_node) 
//         && CheckIfConst(derivative, node.right_index)
//         && CheckIfConst(derivative, node.left_index))
//     {
//         switch(node.node_value.expression_type)
//         {
//             case OPERATOR_PLUS:
                

//             case OPERATOR_MINUS:
//                 break;
    
//             case OPERATOR_MUL:
//                 break;
//         }
//     }

//     return SIMPLIFY_RETURN_SUCCESS;
// }

// =============================== HELPERS ====================================
