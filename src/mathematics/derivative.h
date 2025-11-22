#ifndef ARIPHMETIC_OPERATIONS_H
#define ARIPHMETIC_OPERATIONS_H

#include <stdint.h>

#include "tree.h"
#include "expression.h"
#include "my_string.h"

// ======================== ARIPHMETIC_FUNCTION_ENUM ==========================

enum derivative_return_e 
{
    DERITATIVE_RETURN_SUCCESS,
    DERITATIVE_RETURN_SYSTEM_CALL_ERROR,
    DERITATIVE_RETURN_TREE_ERROR,
    DERITATIVE_RETURN_OPEN_FILE_ERROR,
    DERITATIVE_RETURN_READ_FILE_ERROR,
    DERITATIVE_RETURN_CLOSE_FILE_ERROR,
    DERITATIVE_RETURN_EMPTY_FILE,
    DERITATIVE_RETURN_ALLOCATION_ERROR,
    DERITATIVE_RETURN_MISSING_SYMBOL
};

// ========================== STRUCTS_AND_UNIONS ==============================

struct derivative_s
{
    tree_t ariphmetic_tree;
    char*  buffer;
};

typedef derivative_s* derivative_t;

// ========================== ARIPHMETIC_FUNCTIONS ============================

derivative_return_e DerivativeInit(derivative_t* deritative, size_t start_tree_size, const char* file_name);
derivative_return_e DerivativeDestroy(derivative_t* deritative);

// ============================================================================

#endif // ARIPHMETIC_OPERATIONS_H