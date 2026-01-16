#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const size_t n;    // number of elements
    const size_t size; // size of each element
    const void *data;
} Array;

Array arr_new(size_t n, size_t size);
void *arr_get(Array *arr, size_t at);
bool arr_update(Array *arr, size_t at, void *value);
void arr_free(Array *arr);

bool arr_out_range(Array *arr, size_t index);

#endif
