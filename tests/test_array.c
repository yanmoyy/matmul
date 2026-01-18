#include "array.h"
#include "test_utils.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static bool test_failed = false;

static void test_int_array()
{
    test_start("Integer array (push_back + growth)");
    Array *arr = arr_new(0, sizeof(int));

    T_ASSERT(arr != NULL);
    T_ASSERT(arr->len == 0);
    T_ASSERT(arr->cap >= 1);
    T_ASSERT(arr->d_size == sizeof(int));
    T_ASSERT(arr->data != NULL);

    int values[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    size_t num = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < num; i++) {
        bool ok = arr_append(arr, &values[i]);
        T_ASSERT(ok);
        T_ASSERT(arr->len == i + 1);
    }
    T_ASSERT((arr->cap & (arr->cap - 1)) == 0); // power of 2
    T_ASSERT(arr->cap >= arr->len);
    for (size_t i = 0; i < arr->len; i++) {
        int *got = arr_get(arr, i);
        T_ASSERT(got != NULL);
        T_ASSERT(*got == values[i]);
    }
    T_ASSERT(arr_get(arr, arr->len) == NULL);
    T_ASSERT(!arr_append(arr, NULL));

cleanup:
    arr_free(&arr);

    T_CHECK_TEST_FAIL("Integer array failed");
}

static void test_double_array()
{
    test_start("Double array");

    Array *arr = arr_new(3, sizeof(double));
    T_ASSERT(arr != NULL);

    double vals[] = { 1.414213562, 2.718281828, 3.141592653 };
    for (size_t i = 0; i < 3; i++) {
        bool ok = arr_append(arr, &vals[i]);
        T_ASSERT(ok);
    }
    for (size_t i = 0; i < arr->len; i++) {
        double *got = arr_get(arr, i);
        T_ASSERT(got != NULL);
        T_ASSERT(DOUBLE_EQ(*got, vals[i]));
    }
    double new_val = 42.195;
    bool set_ok = arr_set(arr, 1, &new_val);
    T_ASSERT(set_ok);
    double *updated = arr_get(arr, 1);
    T_ASSERT(DOUBLE_EQ(*updated, 42.195));

cleanup:
    arr_free(&arr);

    T_CHECK_TEST_FAIL("Double array failed");
}

typedef struct {
    int id;
    char name[20];
    float score;
} Student;

static void test_struct_array()
{
    test_start("Struct array + growth");
    Array *arr = arr_new(2, sizeof(Student));
    T_ASSERT(arr != NULL);

    Student s1 = { 101, "Alice Kim", 95.5f };
    Student s2 = { 102, "Bob Park", 88.0f };
    Student s3 = { 103, "Charlie Lee", 92.75f };
    Student s4 = { 104, "Dana Choi", 97.25f };
    T_ASSERT(arr_append(arr, &s1));
    T_ASSERT(arr_append(arr, &s2));
    T_ASSERT(arr_append(arr, &s3));
    T_ASSERT(arr_append(arr, &s4));
    T_ASSERT(arr->len == 4);
    T_ASSERT(arr->cap >= 4);
    T_ASSERT((arr->cap & (arr->cap - 1)) == 0);
    Student *got = arr_get(arr, 2);
    T_ASSERT(got != NULL);
    T_ASSERT(got->id == 103);
    T_ASSERT(strcmp(got->name, "Charlie Lee") == 0);
    T_ASSERT(fabs(got->score - 92.75f) < 0.001f);
cleanup:
    arr_free(&arr);

    T_CHECK_TEST_FAIL("Struct array failed");
}

void run_array_tests()
{
    printf("\n=== Dynamic Array Tests ===\n");
    test_int_array();
    test_double_array();
    test_struct_array();
    printf("\nArray tests finished.\n");
}
