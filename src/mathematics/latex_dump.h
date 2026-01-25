#ifndef LATEX_DUMP_H
#define LATEX_DUMP_H

#include <stdlib.h>
#include "derivative.h"

// ========================= FILE_LATEX_FEATURES ==============================

void SetLogFileName(const char* new_name);
void LogDeritativeInLatex(derivative_t deritative, ssize_t current_node,
                             FILE* output_file);
void LogAssignment(derivative_t derivative, ssize_t l_value,
                   ssize_t r_value, FILE* output_file);
void StartLatexDocument(FILE* output_file);
void EndLatexDocument(derivative_t derivative, FILE* output_file);

// ============================================================================

#endif // LATEX_DUMP_H