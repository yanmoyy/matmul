#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdio.h>

enum m_type {
    INT,
    FLOAT,
    DOUBLE,
} M_TYPE;

enum {
    INITIAL,
    ROW_STEP,
    COL_STEP,
    MAT_STEP,
    FINISH,
} M_FILL_STATE;

typedef struct {
    enum type type;
    size_t row;
    size_t col;
    char *name;
    void *data; // 2D (array of array)
} Matrix;

Matrix *parse_matrix(size_t row, size_t col, char *name, FILE *file);
Matrix *new_matrix_with_2D_array(size_t row, size_t col, void *array);

void print_matrix(Matrix *matrix);

#endif
