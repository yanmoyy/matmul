#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_ROW 100
#define MAX_COL 100
#define MAX_NAME_LEN 1000

typedef struct Matrix {
    size_t row;
    size_t col;
    char *name;
    double *data; // double type of data
} Matrix;

// 2nd Version (used by main)
Matrix *matrix_load_from_file(const char *filepath);
void matrix_print(const Matrix *m);
Matrix *matrix_multiply(Matrix *m1, Matrix *m2);
void matrix_free(Matrix **m);

// (used by matrix_test)
Matrix *matrix_new(size_t row, size_t col, const char *name);
Matrix *matrix_new_with_array(size_t row, size_t col, const char *name, void *array);

// Helper functions
bool all_input_files_exist(char *filepaths[], int count);
int file_path_to_name(const char *filepath, char *out_buf, size_t bufsize);
#endif
