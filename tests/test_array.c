#include "array.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <string.h>

// Helper macro for floating-point comparison
#define DOUBLE_EQ(a, b) (fabs((a) - (b)) < 1e-9)

void test_int_array()
{
    test_start("Integer array (push_back + growth)"); // Replaces printf
    Array *arr = arr_new(0, sizeof(int));
    assert(arr->len == 0);
    assert(arr->cap >= 1);
    assert(arr->d_size == sizeof(int));
    assert(arr->data != NULL);
    int values[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    size_t num = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < num; i++) {
        bool ok = arr_append(arr, &values[i]);
        assert(ok);
        assert(arr->len == i + 1);
    }
    assert((arr->cap & (arr->cap - 1)) == 0); // power of 2
    assert(arr->cap >= arr->len);
    for (size_t i = 0; i < arr->len; i++) {
        int *got = arr_get(arr, i);
        assert(got != NULL);
        assert(*got == values[i]);
        // optional: printf("arr[%zu] = %d\n", i, *got);
    }
    assert(arr_get(arr, arr->len) == NULL);
    assert(!arr_append(arr, NULL));
    arr_free(&arr);
    assert(arr == NULL);
    test_success(); // Replaces printf
}

void test_double_array()
{
    test_start("Double array");
    Array *arr = arr_new(3, sizeof(double));
    double vals[] = { 1.414213562, 2.718281828, 3.141592653 };
    for (size_t i = 0; i < 3; i++) {
        bool ok = arr_append(arr, &vals[i]);
        assert(ok);
    }
    for (size_t i = 0; i < arr->len; i++) {
        double *got = arr_get(arr, i);
        assert(got != NULL);
        assert(DOUBLE_EQ(*got, vals[i]));
        // optional: printf("arr[%zu] = %.9f\n", i, *got);
    }
    double new_val = 42.195;
    bool set_ok = arr_set(arr, 1, &new_val);
    assert(set_ok);
    double *updated = arr_get(arr, 1);
    assert(DOUBLE_EQ(*updated, 42.195));
    arr_free(&arr);
    test_success();
}

typedef struct {
    int id;
    char name[20];
    float score;
} Student;

void test_struct_array()
{
    test_start("Struct array + growth");
    Array *arr = arr_new(2, sizeof(Student));
    Student s1 = { 101, "Alice Kim", 95.5f };
    Student s2 = { 102, "Bob Park", 88.0f };
    Student s3 = { 103, "Charlie Lee", 92.75f };
    Student s4 = { 104, "Dana Choi", 97.25f };
    assert(arr_append(arr, &s1));
    assert(arr_append(arr, &s2));
    assert(arr_append(arr, &s3));
    assert(arr_append(arr, &s4));
    assert(arr->len == 4);
    assert(arr->cap >= 4);
    assert((arr->cap & (arr->cap - 1)) == 0);
    Student *got = arr_get(arr, 2);
    assert(got != NULL);
    assert(got->id == 103);
    assert(strcmp(got->name, "Charlie Lee") == 0);
    assert(fabs(got->score - 92.75f) < 0.001f);
    arr_free(&arr);
    test_success();
}
