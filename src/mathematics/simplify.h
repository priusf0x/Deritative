#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include <stdlib.h>

#include "derivative.h"

enum simplify_return_e
{
    SIMPLIFY_RETURN_SUCCESS,
    SIMPLIFY_RETURN_TREE_ERROR,
    SIMPLIFY_RETURN_INCORRECT_VALUE
};

simplify_return_e
SimplifyNeutralMultipliers(derivative_t derivative,
                           ssize_t       current_node);

#endif // SIMPLIFY_H