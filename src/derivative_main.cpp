#include "derivative.h"

#include "tree.h"
#include "simple_parser.h"
#include "my_string.h"
#include "tools.h"
#include "latex_dump.h"
#include "take_derivative.h"
#include "simplify.h"
#include "recursive_decent.h"

static const char* formula_file_name = "pletnev.zov";

//make functions 
// 3 priority Fill function, calculation with vars
// 4 GRRRRRROROOOOOOOQQQQQ 

int
main()
{
    const size_t start_tree_size = 1; 
    derivative_t derivative = NULL;

    StartLatexDocument(NULL);

    int error_number = 0;

    if ((error_number = DerivativeInit(&derivative, start_tree_size, 
                        formula_file_name)) != DERIVATIVE_RETURN_SUCCESS)
    {   
        fprintf(stderr, "Init Error: %d\n", error_number);

        return error_number;
    }

    if ((error_number = ConvertToGraph(derivative)) != DERIVATIVE_RETURN_SUCCESS)
    {
        fprintf(stderr, "Read Error: %d\n", error_number);

        return error_number;
    }   

    LogDeritativeInLatex(derivative, 0, NULL);

    if ((error_number = TakeDerivative(derivative, "x") // need to be added gui inteface??? 
            != DERIVATIVE_RETURN_SUCCESS))
    {
        fprintf(stderr, "Derivate Error: %d\n", error_number);

        return error_number;
    }

    if ((error_number = SimplifyGraph(derivative)) != DERIVATIVE_RETURN_SUCCESS)
    {
        printf("Simplify Error:%d\n", error_number);

        return error_number;
    }

    LogDeritativeInLatex(derivative, 0, NULL);
    DerivativeDestroy(&derivative);
    EndLatexDocument(NULL);

    return 0;
} 


