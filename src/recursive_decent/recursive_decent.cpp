#include "recursive_decent.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "Assert.h"
#include "buffer.h"
#include "color.h"
#include "derivative_defines.h"
#include "derivative.h"
#include "expression.h"
#include "tree.h"
#include "tools.h"
#include "operation_info.h"

// ============================= HELPERS ======================================

#define CURRENT_STRING (derivative->buffer->buffer \
                        + derivative->buffer->current_position)

static void 
SkipSpacesInBuffer(buffer_t buffer)
{
    buffer->current_position = SkipSpaces(buffer->buffer, 
                                            buffer->current_position);
}

static void 
SkipNSymbols(buffer_t buffer,
             size_t   n)
{
    buffer->current_position += n;
}

static void 
ReadVarInBuffer(variable_s* var,
                buffer_t buffer)
{
    ASSERT(buffer);

    ReadVarString(buffer->buffer + buffer->current_position,
                  &var->variable_name);

    buffer->current_position += var->variable_name.string_size;
    SkipSpacesInBuffer(buffer);

}

struct key_words_function_s 
{
    const char*  function_name;
    size_t       name_len;
    operations_e operation;
};

// ============================= GET_FUNCTIONS ================================

static ssize_t GetE(derivative_t derivative);
static ssize_t GetT(derivative_t derivative);
static ssize_t GetP(derivative_t derivative);
static ssize_t GetN(derivative_t derivative);

static operations_e 
CheckIfFunction(derivative_t derivative)
{
    string_s compare_string = {};

    for (size_t index = 1; index < OPERATION_COUNT; index++)
    {
        compare_string = OPERATION_INFO[index].operation_name;
        if (StrCmpWithEnding(CURRENT_STRING, compare_string.string_source, 
                compare_string.string_size, " \r\n\t("))
        {
            SkipNSymbols(derivative->buffer, compare_string.string_size);
            SkipSpacesInBuffer(derivative->buffer);

            return OPERATION_INFO[index].operation;
        }
    }

    return OPERATOR_UNDEFINED;
}

static ssize_t 
GetN(derivative_t derivative)
{
    RETURN_NO_LINK_IF_ERROR;

    #ifndef NDEBUG
        BufferDump(derivative->buffer);
    #endif

    char* current_string = CURRENT_STRING; 
    char* endptr = NULL;

    double value = strtod(current_string, &endptr);

    size_t new_position = (size_t) (endptr - derivative->buffer->buffer);
    if (new_position == derivative->buffer->current_position)
    {
        derivative->error = DERIVATIVE_RETURN_READ_ERROR;
        
        return NO_LINK;
    }

    derivative->buffer->current_position = new_position;
    
    SkipSpacesInBuffer(derivative->buffer);

    return CONST__(value);
}

static ssize_t
GetP(derivative_t derivative)
{
    RETURN_NO_LINK_IF_ERROR;

    char current_symbol = *CURRENT_STRING;
    ssize_t return_value = NO_LINK;
    operations_e possible_op = OPERATOR_UNDEFINED;

    RETURN_NO_LINK_IF_ERROR;

    #ifndef NDEBUG
        BufferDump(derivative->buffer);
    #endif 

    if (current_symbol == '(')
    {
        SkipNSymbols(derivative->buffer, 1);
        SkipSpacesInBuffer(derivative->buffer);

        return_value = GetE(derivative);

        if (*CURRENT_STRING != ')')
        {
            return NO_LINK;
        }

        SkipNSymbols(derivative->buffer, 1);
        SkipSpacesInBuffer(derivative->buffer);
        
        return return_value;
    }    
    else if ((possible_op = CheckIfFunction(derivative)) 
                != OPERATOR_UNDEFINED)
    {
        if (*CURRENT_STRING != '(')
        {
            return NO_LINK;
        }

        SkipNSymbols(derivative->buffer, 1);
        SkipSpacesInBuffer(derivative->buffer);

        return_value = GetE(derivative);

        if (*CURRENT_STRING != ')')
        {
            return NO_LINK;
        }

        SkipNSymbols(derivative->buffer, 1);
        SkipSpacesInBuffer(derivative->buffer);
        
        return DerivativeAddOperation(derivative, return_value, NO_LINK, 
                                      possible_op);
    }
    else if (CheckIfSymbVar(*CURRENT_STRING))
    {
        variable_s var = {};
        ReadVarInBuffer(&var, derivative->buffer);
        return VAR__(&var);
    }
    else 
    {
        return GetN(derivative);
    }
}

static ssize_t
GetT(derivative_t derivative)
{
    RETURN_NO_LINK_IF_ERROR;

    ssize_t last_add = GetP(derivative);

    char last_symbol = *CURRENT_STRING;
    char operation = 0;

    while((last_symbol == '*') || (last_symbol == '/')
          || (last_symbol == '^'))
    {
        RETURN_NO_LINK_IF_ERROR;

        #ifndef NDEBUG
            BufferDump(derivative->buffer);
        #endif

        operation = last_symbol;

        SkipNSymbols(derivative->buffer, 1);
        SkipSpacesInBuffer(derivative->buffer);

        switch(operation)
       {
            case '*':
                last_add = MUL__(last_add, GetP(derivative));
                break;

            case '/':
                last_add = DIV__(last_add, GetP(derivative));
                break;

            case '^':
                last_add = POW__(last_add, GetP(derivative));
                break;

            default: derivative->error = DERIVATIVE_RETURN_READ_ERROR;  
                     return NO_LINK;
        }

        last_symbol = *CURRENT_STRING;
    }

    return last_add; 
} 

static ssize_t
GetE(derivative_t derivative)
{
    RETURN_NO_LINK_IF_ERROR;    

    ssize_t last_add = GetT(derivative);

    char last_symbol = *CURRENT_STRING;
    char operation = 0;

    while((last_symbol == '+') || (last_symbol == '-'))
    {
        RETURN_NO_LINK_IF_ERROR;

        #ifndef NDEBUG
            BufferDump(derivative->buffer);
        #endif

        operation = last_symbol;

        SkipNSymbols(derivative->buffer, 1);
        SkipSpacesInBuffer(derivative->buffer);

        switch(operation)
        {
            case '+':
                last_add = SUM__(last_add, GetT(derivative));
                break;

            case '-':
                last_add = SUB__(last_add, GetT(derivative));
                break;

            default: derivative->error = DERIVATIVE_RETURN_READ_ERROR;  
                     return NO_LINK;
        }

        last_symbol = *CURRENT_STRING;
    }

    return last_add;
}

derivative_return_e
ConvertToGraph(derivative_t derivative)
{ 
    ASSERT(derivative);

    ssize_t readen_node = GetE(derivative);

    if ((readen_node == NO_LINK) || IF_DERIVATIVE_FAILED)
    {
        const char* error_read_message = RED      "Error was occupied while reading." 
                                         STANDARD "Buffer dump:\n" ;

        fprintf(stderr, "%s", error_read_message);
        BufferDump(derivative->buffer);
    }

    if (ForceConnect(derivative->ariphmetic_tree, readen_node, 0,
                                            EDGE_DIR_LEFT) != TREE_RETURN_SUCCESS)
    {
        return DERIVATIVE_RETURN_TREE_ERROR;
    }

    #ifndef NDEBUG
    BufferDump(derivative->buffer);
    #endif 

    return DERIVATIVE_RETURN_SUCCESS; 
}

#undef CURRENT_STRING
