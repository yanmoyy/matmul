#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t len;    // current number of elements
    size_t cap;    // current allocated capacity
    size_t d_size; // size of each element
    void *data;    // pointer to allocated memory (non-const!)
} Array;

Array *arr_new(size_t initial_cap, size_t elem_size);
void *arr_get(const Array *arr, size_t index);
bool arr_append(Array *arr, const void *value);            // append
bool arr_set(Array *arr, size_t index, const void *value); // update existing
void arr_free(Array **arr);
bool arr_out_of_range(const Array *arr, size_t index);

// Optional helpers
size_t next_power_of_2(size_t n);

#endif
