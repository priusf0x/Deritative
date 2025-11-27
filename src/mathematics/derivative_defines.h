#ifndef DERIVATIVE_DEFINES_H

ssize_t
AddNode(derivative_t      derivative,
        ssize_t           current_node,
        expression_s*     expr,
        edge_dir_e        dir);

#define TURN_ON_CAST _Pragma("GCC diagnostic warning \"-Wuseless-cast\"") \
                     _Pragma("GCC diagnostic warning \"-Wconversion\"")\
                     _Pragma("GCC diagnostic warning \"-Wimplicit-fallthrough\"")
#define TURN_OFF_CAST _Pragma("GCC diagnostic ignored \"-Wuseless-cast\"")\
                      _Pragma("GCC diagnostic ignored \"-Wconversion\"")\
                      _Pragma("GCC diagnostic ignored \"-Wimplicit-fallthrough\"")

#define INIT_EXPR(__TYPE, __VALUE)\
    do {switch(__TYPE)\
    {\
        TURN_OFF_CAST\
        case EXPRESSION_TYPE_CONST:\
            expr__.expression.constant = (double) __VALUE;\
            break;\
        case EXPRESSION_TYPE_OPERATOR:\
            expr__.expression.operation = (operations_e) __VALUE;\
            break;\
        case EXPRESSION_TYPE_VAR:\
            expr__.expression.variable = (char) __VALUE;\
        case EXPRESSION_TYPE_UNDEFINED:\
        default: return TAKE_DERIVATIVE_INCORRECT_EXPRESSION;\
        TURN_ON_CAST\
    }} while(0);

#define DELETE(__X) DeleteSubgraph(derivative->ariphmetic_tree, __X);

#define ADD_NODE(__PARENT, __TYPE, __VALUE, __HOW) \
    do {expression_s expr__ = {.expression_type = __TYPE};\
        INIT_EXPR(__TYPE, __VALUE);\
        return AddNode(derivative, __PARENT, \)\
        } while (0)

#define COPY(__DST, __SRC, __HOW) \
    do {\
        ssize_t SRC_COPY = __SRC;\
        CopySubgraph(derivative->ariphmetic_tree, &SRC_COPY, \__SRC, __HOW)\
        return SRC_COPY;\
        } while (0)

#define D(__X) \
    do {COPY(NO_LINK, __X, EDGE_DIR_NO_DIRECTION)\  
        output = TakeDerivative(derivative, \
                 COPY(NO_LINK, __X, EDGE_DIR_NO_DIRECTION))\
        } while (0)

#define cR COPY(NO_LINK, current_node, EDGE_DIR_NO_DIRECTION)
#define cL COPY(NO_LINK, current_node, EDGE_DIR_NO_DIRECTION)

#endif // DERIVATIVE_DEFINES