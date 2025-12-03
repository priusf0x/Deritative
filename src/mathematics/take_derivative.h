#ifndef TAKE_DERIVATIVE_H
#define TAKE_DERIVATIVE_H

#include "derivative.h"

ssize_t
TakeExpressionDerivative(derivative_t derivative, ssize_t current_node);

derivative_return_e 
TakeDerivative(derivative_t derivative, char* variable_name);

#endif //TAKE_DERIVATIVE_H