#include "array.h"
#include "error.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t next_power_of_2(size_t n);

Array *arr_new(size_t initial_cap, size_t elem_size)
{
    if (elem_size == 0) {
        ERROR("arr_new: element size cannot be zero\n");
        return NULL;
    }

    Array *arr = calloc(1, sizeof(*arr));
    if (!arr) {
        ERROR("arr_new: failed to allocate Array struct\n");
        return NULL;
    }

    arr->d_size = elem_size;
    arr->cap = next_power_of_2(initial_cap ? initial_cap : 8);
    arr->len = 0;

    // Use calloc to zero-initialize (safer for structs/unions in elements)
    arr->data = calloc(arr->cap, elem_size);
    if (!arr->data) {
        ERROR("arr_new: failed to allocate initial data buffer (%zu elements)\n", arr->cap);
        free(arr);
        return NULL;
    }

    return arr;
}

void *arr_get(const Array *arr, size_t index)
{
    if (arr_out_of_range(arr, index)) {
        // Optional: could log here, but usually silent return NULL is fine
        return NULL;
    }
    return (char *)arr->data + index * arr->d_size;
}

bool arr_set(Array *arr, size_t index, const void *value)
{
    if (arr_out_of_range(arr, index)) {
        ERROR("arr_set: index %zu out of range (len=%zu)\n", index, arr ? arr->len : 0);
        return false;
    }
    if (!value) {
        ERROR("arr_set: cannot set NULL value\n");
        return false;
    }

    void *dest = (char *)arr->data + index * arr->d_size;
    memcpy(dest, value, arr->d_size);
    return true;
}

bool arr_append(Array *arr, const void *value)
{
    if (!arr) {
        ERROR("arr_append: array pointer is NULL\n");
        return false;
    }
    if (!value) {
        ERROR("arr_append: cannot append NULL value\n");
        return false;
    }

    if (arr->len >= arr->cap) {
        // Prevent use-after-free style bugs
        if (arr->cap == 0 || arr->data == NULL) {
            ERROR("arr_append: attempt to append to freed or invalid array\n");
            return false;
        }

        // Grow by factor 2, check overflow
        size_t new_cap = arr->cap * 2;
        if (new_cap / 2 != arr->cap) {
            ERROR("arr_append: capacity overflow during growth\n");
            return false;
        }

        size_t new_bytes = new_cap * arr->d_size;
        if (new_bytes / arr->d_size != new_cap) {
            ERROR("arr_append: size_t overflow during realloc calculation\n");
            return false;
        }

        void *new_data = realloc(arr->data, new_bytes);
        if (!new_data) {
            ERROR("arr_append: realloc failed for %zu bytes\n", new_bytes);
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

void arr_free(Array **arr_ptr)
{
    if (!arr_ptr || !*arr_ptr) {
        return;
    }

    Array *arr = *arr_ptr;

    free(arr->data);
    arr->data = NULL; // optional, but good hygiene

    free(arr);
    *arr_ptr = NULL;
}

bool arr_out_of_range(const Array *arr, size_t index)
{
    // Slightly more defensive: check arr != NULL first
    return arr == NULL || index >= arr->len;
}

static size_t next_power_of_2(size_t n)
{
    if (n == 0) {
        return 1;
    }
    n--;
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
