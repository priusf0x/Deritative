#include "take_derivative.h"

#include "Assert.h"
#include "derivative.h"
#include "latex_dump.h"
#include "tree.h"
#include "tools.h"

take_derivative_return_e
TakeDerivative(derivative_t derivative,
               size_t       current_node)