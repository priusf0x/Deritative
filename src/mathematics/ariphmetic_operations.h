#ifndef ARIPHMETIC_OPERATIONS_H
#define ARIPHMETIC_OPERATIONS_H

#include <stdint.h>

// ============================ TREE_FUNCTIONS ================================


static akinator_return_e 
ReadFileData(akinator_t  akinator,
             const char* file_name)

// ========================== STRUCTS_AND_UNIONS ==============================

enum expression_type_e
{
    EXPRESSION_TYPE_CONST,
    EXPRESSION_TYPE_VAR,
    EXPRESSION_TYPE_OPERATOR
};

enum operations_e 
{
    OPERATOR_PLUS  = 0,
    OPERATOR_MINUS = 1,
    OPERATOR_MUL   = 2,
    OPERATOR_DIV   = 3
};

const char* const  OPERATION_STR_ARRAY[] = 
    {"plus",
    "minus",
    "multiply",
    "divide"};
const size_t OPERATION_COUNT = sizeof(OPERATION_STR_ARRAY) / sizeof(OPERATION_STR_ARRAY[0]);

union expression_u
{
    double        constant;
    operations_e  operation;
    unsigned char variable;     
};

struct expression_s
{
    expression_u      expression;          
    expression_type_e expression_type;
};

// ============================================================================

#endif // ARIPHMETIC_OPERATIONS_H