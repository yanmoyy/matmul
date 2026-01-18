#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdio.h>

typedef enum {
    M_TYPE_INT,
    M_TYPE_FLOAT,
    M_TYPE_DOUBLE,
} m_type_t;

typedef enum {
    INITIAL,
    ROW_STEP,
    COL_STEP,
    MAT_STEP,
    FINISH,
} m_step_t;

typedef struct Matrix {
    m_type_t type;
    size_t row;
    size_t col;
    char *name;
    void *data; // 2D (array of array)
} Matrix;

Matrix *new_matrix(size_t row, size_t col, m_type_t type, const char *name);
Matrix *new_matrix_with_2D_array(size_t row, size_t col, m_type_t type, const char *name, void *array);

Matrix *matrix_multiply(Matrix *m1, Matrix *m2);

int matrix_to_string(const Matrix *m, char *buf, size_t bufsize);
void print_matrix(const Matrix *m);
void free_matrix(Matrix **m);

// helper
int get_matrix_name(const char *filepath, char *out_buf, size_t bufsize);

#endif
