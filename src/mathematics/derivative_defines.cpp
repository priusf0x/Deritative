#include "derivative_defines.h"

#include "derivative.h"
#include "Assert.h"

ssize_t
AddNode(derivative_t      derivative,
        ssize_t           current_node,
        expression_s*     expr,
        edge_dir_e        dir)
{
    ASSERT(derivative != NULL);
    ASSERT(expr != NULL);

    node_s new_node = {.parent_index      = current_node,
                       .parent_connection = dir,
                       .right_index       = NO_LINK,
                       .left_index        = NO_LINK,
                       .node_value        = *expr,
                       .index_in_tree     = NO_LINK};

    if (TreeAddNode(derivative->ariphmetic_tree, &new_node) != 0)
    {
        // return TAKE_DERIVATIVE_TREE_ERROR;
    }

    return NO_LINK;
}