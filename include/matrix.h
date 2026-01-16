#ifndef MATRIX_H
#define MATRIX_H

#include "array.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t row;
    size_t col;
    char *name;
    Array *data; // 2D (array of array)
} Matrix;

enum {
    INITIAL,
    ROW_STEP,
    COL_STEP,
    MAT_STEP,
    FINISH,
} M_FILL_STATE;

Matrix *parse_matrix(size_t row, size_t col, char *name, FILE *file);
void print_matrix(Matrix *matrix);

#endif
