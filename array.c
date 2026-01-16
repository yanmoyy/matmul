#include "array.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Array arr_new(size_t initial_cap, size_t elem_size)
{
    Array arr = { 0 };

    arr.d_size = elem_size;
    arr.cap = next_power_of_2(initial_cap);
    arr.data = malloc(arr.cap * elem_size);
    arr.len = 0;

    return arr;
}

void *arr_get(const Array *arr, size_t index)
{
    if (arr_out_of_range(arr, index)) {
        return NULL;
    }
    return (char *)arr->data + index * arr->d_size;
}

bool arr_set(Array *arr, size_t index, const void *value)
{
    if (arr_out_of_range(arr, index) || !value) {
        return false;
    }
    void *dest = (char *)arr->data + index * arr->d_size;
    memcpy(dest, value, arr->d_size);
    return true;
}

bool arr_append(Array *arr, const void *value)
{
    if (!arr || !value) {
        return false;
    }
    if (arr->len >= arr->cap) { // need to grow
        if (arr->cap == 0) {    // freed array
            printf("try to append on freed array!\n");
            return false;
        }
        size_t new_cap = arr->cap * 2;

        if (new_cap / 2 != arr->cap || new_cap > (SIZE_MAX / arr->d_size)) {
            return false; // prevent overflow
        }

        void *new_data = realloc(arr->data, new_cap * arr->d_size);
        if (!new_data) {
            return false;
        }
        arr->data = new_data;
        arr->cap = new_cap;
    }

    void *dest = (char *)arr->data + arr->len * arr->d_size;
    memcpy(dest, value, arr->d_size);
    arr->len++;

    return true;
}

void arr_free(Array *arr)
{
    if (arr && arr->data) {
        free(arr->data);
        arr->data = NULL;
        arr->len = 0;
        arr->cap = 0;
    }
}

bool arr_out_of_range(const Array *arr, size_t index)
{
    return !arr || index >= arr->len;
}

// Helper: smallest power of 2 >= n
size_t next_power_of_2(size_t n)
{
    if (n == 0) {
        return 1;
    }
    n--; // make sure powers-of-2 stay the same

    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;

    if (sizeof(size_t) > 4) {
        n |= n >> 32;
    }

    return n + 1;
}
