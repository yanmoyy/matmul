#include "array.h"
#include <stdlib.h>
#include <string.h>

// create new array
Array arr_new(size_t n, size_t size)
{
    Array arr = {
        .n = n,
        .size = size,
        .data = malloc(size * n),
    };
    return arr;
}

// get value of elemente, return null if failed
void *arr_get(Array *arr, size_t at)
{
    if (arr_out_range(arr, at)) {
        return NULL;
    }

    return (char *)arr->data + (at * arr->size);
}

// update array, return false when failed (out of range)
bool arr_update(Array *arr, size_t at, void *value)
{
    if (arr_out_range(arr, at)) {
        return false;
    }

    void *dest = (char *)arr->data + (at * arr->size);
    memcpy(dest, value, arr->size);
    return true;
}

// free array
void arr_free(Array *arr)
{
    free((void *)arr->data);
    arr->data = NULL;
}

// utilities
bool arr_out_range(Array *arr, size_t index)
{
    return index >= arr->n;
}
