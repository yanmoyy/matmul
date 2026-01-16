#ifndef MATRIX_H
#define MATRIX_H

#include "array.h"
#include <stddef.h>

typedef struct {
    size_t row;
    size_t col;
    Array *data; // 2D (array of array)
} Matrix;

#endif
