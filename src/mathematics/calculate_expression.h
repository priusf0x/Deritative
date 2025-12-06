#ifndef CALCULATE_EXPRESSION_H
#define CALCULATE_EXPRESSION_H

#include <stdlib.h>

#include "derivative.h"

double 
CalculateTree(ssize_t      current_node,
              derivative_t derivative);


bool 
CheckIfNoVar(ssize_t      current_node,
                derivative_t derivative);

#endif // CALCULATE_EXPRESSION_H