#include "latex_dump.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Assert.h"
#include "derivative.h"
#include "tools.h"
#include "derivative_defines.h"
#include "operation_info.h"

static const char* latex_log_file_name = "logs/latex_dump.tex"; 

void static 
WriteConstInFile(derivative_t deritative,
                 size_t       current_node,
                 FILE*        output_file);

void static 
WriteVarInFile(const derivative_t deritative,
               size_t             current_node,
               FILE*              output_file);

static FILE*
GetLatexLogFile();

// ============================= LOGGING_IN_FILE ==============================

static void
WriteExpression(derivative_t deritative,
                ssize_t      current_node,
                FILE*        output_file);

void
LogDeritativeInLatex(derivative_t derivative,
                     ssize_t      current_node,
                     FILE*        output_file)
{
    ASSERT(derivative != NULL);
    
    if (output_file == NULL)
    {
        output_file = GetLatexLogFile();
        if (output_file == NULL)
        {   
            return;
        }
    }   
    
    if (current_node == 0)
    {
        current_node = NODE(0)->left_index;
    }

    fprintf(output_file, "\\begin{align}\n");
    WriteExpression(derivative, (ssize_t) current_node, output_file);
    fprintf(output_file, "\n\\end{align}\n");
}

// =========================== FILE_USAGE =====================================

static FILE* 
GetLatexLogFile()
{
    static FILE* maxim_gorohov_file = fopen(latex_log_file_name, "w+");
    return maxim_gorohov_file;
}

void 
SetLogFileName(const char* new_name)
{
    ASSERT(new_name != NULL);

    latex_log_file_name = new_name;
}


// ============================ DUMP_HELPERS ==================================

void 
StartLatexDocument(FILE* output_file)
{
    if (output_file == NULL)
    {
        output_file = GetLatexLogFile();
        if (output_file == NULL)
        {   
            return;
        }
    }

    const char* header_text = 
    "\\documentclass[a4paper,10pt]{article}\n"
    "\\usepackage[left=2cm,right=2cm,top=2cm,bottom=2cm]{geometry}\n"
    "\\usepackage[utf8]{inputenc}\n"
    "\\usepackage[T2A]{fontenc}\n"
    "\\usepackage[russian]{babel}\n"
    "\\usepackage{amsmath}\n"
    "\\begin{document} \n\n";  
    
    fprintf(output_file, "%s", header_text);
}

void 
EndLatexDocument(FILE* output_file)
{
    if (output_file == NULL)
    {
        output_file = GetLatexLogFile();
        if (output_file == NULL)
        {   
            return;
        }
    }

    const char* end_text = "\\end{document}\n";

    fprintf(output_file, "%s", end_text);
    fclose(output_file);

    #ifdef TURN_GROQ
        SystemCall("python src/groq/groq_help.py");
    #endif 

    SystemCall("pdflatex %s -f y -interaction=nonstopmode -halt-on-error \
            -file-line-error 2 > latex_output.log", latex_log_file_name);
    SystemCall("rm *.aux *.log >latex_output.log");
}

// ========================= WRITE_FUNCTIONS_HELPERS ==========================

void static 
PrintOneArgFunction(derivative_t derivative,
                    size_t       current_node,
                    FILE*        output);

void static 
WriteConstInFile(derivative_t deritative,
                 size_t       current_node, 
                 FILE*        output_file)
{
    ASSERT(deritative  != NULL);
    ASSERT(output_file != NULL);
    
    double number = deritative->ariphmetic_tree->
                nodes_array[current_node].node_value.expression.constant;

    if(CheckIfInteger(number))
    {
        fprintf(output_file, "%ld", (long) number);
    }
    else
    {
        fprintf(output_file, "%f", number);
    }
}

void static 
WriteVarInFile(const derivative_t deritative,
               size_t             current_node,
               FILE*              output_file)
{
    ASSERT(deritative != NULL);
    ASSERT(output_file != NULL);

    string_s string = deritative->ariphmetic_tree->nodes_array[current_node]
                            .node_value.expression.variable.variable_name;
    PrintString(&string, output_file);
}

void static 
WriteOperationInFile(derivative_t derivative,
                     size_t       current_node,
                     FILE*        output_file);

void static 
PrintOneArgFunction(derivative_t derivative,
                    size_t       current_node,
                    FILE*        output)
{
    ASSERT(derivative != NULL);

    operations_e node_op = NODE(current_node)->node_value.expression.operation;

    fprintf(output, "%s{(", OPERATION_INFO[node_op].latex_dump_name);
    WriteExpression(derivative, NODE(current_node)->left_index, output);
    fprintf(output, ")}");
}

static bool 
CheckIfBracket(derivative_t derivative,
               ssize_t      current_node,
               ssize_t      next_node)
{    
    if (NODE(next_node)->node_value.expression_type 
            != EXPRESSION_TYPE_OPERATOR)
    {
        return false;
    }

    operations_e node_op = NODE(current_node)->node_value.
                                            expression.operation;
    operations_e next_node_op = NODE(next_node)->node_value.
                                                expression.operation;

    if (node_op == next_node_op)
    {
        return false;
    }

    if (OPERATION_INFO[next_node_op].priority 
            > OPERATION_INFO[node_op].priority)
    {
        return false;
    }

    return true;
}

void static 
PrintTwoArgFunction(derivative_t derivative,
                    size_t      current_node,
                    FILE*        output)
{
    ASSERT(derivative != NULL);

    operations_e node_op = NODE(current_node)->node_value.expression.operation;
    bool bracket_l = CheckIfBracket(derivative, (ssize_t) current_node, L_O);
    bool bracket_r = CheckIfBracket(derivative, (ssize_t) current_node, R_O);
   
    fprintf(output, "{{");
    if (bracket_l)
    {fprintf(output, "(");}
    WriteExpression(derivative, NODE(current_node)->left_index, output);
    if (bracket_l)
    {fprintf(output, ")");}
    fprintf(output, "}");
    fprintf(output, "%s", OPERATION_INFO[node_op].latex_dump_name);
    fprintf(output, "{");
    if (bracket_r)
    {fprintf(output, "(");}
    WriteExpression(derivative, NODE(current_node)->right_index, output);
    if (bracket_r)
    {fprintf(output, ")");}
    fprintf(output, "}}");
}

void static 
WriteOperationInFile(derivative_t derivative,
                     size_t       current_node,
                     FILE*        output_file)
{
    ASSERT(derivative != NULL);
    ASSERT(output_file != NULL);

    operations_e node_op = NODE(current_node)->node_value.expression.operation;
    size_t argc = OPERATION_INFO[node_op].argc;

    switch(argc)
    {
        case 1:
            PrintOneArgFunction(derivative, current_node, output_file);
            break;
        case 2:
            PrintTwoArgFunction(derivative, current_node, output_file);
            break;

        case 0:
        default: return;
    }
}

// ============================ WRITE_FUNCTIONS ===============================

static void
WriteExpression(derivative_t deritative,
                ssize_t      current_node,
                FILE*        output_file) 
{
    ASSERT(deritative != NULL);
    ASSERT(output_file != NULL);
    if (current_node == NO_LINK)
    {
        return;
    }
    node_s node = deritative->ariphmetic_tree->nodes_array[current_node];

    switch(node.node_value.expression_type)
    {
        case EXPRESSION_TYPE_CONST:
            WriteConstInFile(deritative, (size_t) current_node, output_file);
            break;
        
        case EXPRESSION_TYPE_VAR:
            WriteVarInFile(deritative, (size_t) current_node, output_file);            
            break;

        case EXPRESSION_TYPE_OPERATOR:
            WriteOperationInFile(deritative, (size_t) current_node, output_file);
            break;

        case EXPRESSION_TYPE_UNDEFINED:
            break;

        default: break;
    }
}