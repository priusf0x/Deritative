#include "tree.h"
#include "simple_parser.h"
#include "ariphmetic_operations.h"

// static const char* base_file_name = "base_file.zov";

int
main()
{
    tree_t expression_tree = NULL;
    const size_t start_tree_size = 1; 

    int error_number = 0;

    if ((error_number = TreeInit(&expression_tree,
                                 start_tree_size)) != 0)
    {
       return error_number;
    }


// struct node_s
// {
//     ssize_t     parent_index;
//     edge_dir_e  parent_connection;
//     ssize_t     right_index;
//     ssize_t     left_index;
//     node_data_t node_value;
//     size_t      index_in_tree;
// };

    node_s node_1 = {.parent_index = 0, .parent_connection = EDGE_DIR_LEFT,
                     .right_index  = NO_LINK, .left_index = NO_LINK,
                     .node_value = {.expression = {.constant = 10}, .expression_type = EXPRESSION_TYPE_CONST}};

    TreeAddNode(expression_tree, &node_1);
        

    node_s node_2 = {.parent_index = 1, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = NO_LINK, .left_index = NO_LINK,
                     .node_value = {.expression = {.operation = OPERATOR_MINUS}, .expression_type = EXPRESSION_TYPE_OPERATOR}};

    TreeAddNode(expression_tree, &node_2);

    node_s node_3 = {.parent_index = 2, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = NO_LINK, .left_index = NO_LINK,
                     .node_value = {.expression = {.variable = 'h'}, .expression_type = EXPRESSION_TYPE_VAR}};

    TreeAddNode(expression_tree, &node_3);
    
    TreeDump(expression_tree);

    TreeDestroy(expression_tree); 

    return 0;
} 
