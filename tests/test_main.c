#include "array.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper macro for floating-point comparison
#define DOUBLE_EQ(a, b) (fabs((a) - (b)) < 1e-9)

// Test with integers
static void test_int_array(void)
{
    printf("\n[TEST] Integer array (push_back + growth)\n");

    Array *arr = arr_new(0, sizeof(int));
    assert(arr->len == 0);
    assert(arr->cap >= 1); // should have some minimum capacity
    assert(arr->d_size == sizeof(int));
    assert(arr->data != NULL);

    int values[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    size_t num = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < num; i++) {
        bool ok = arr_append(arr, &values[i]);
        assert(ok);
        assert(arr->len == i + 1);
    }

    // Check capacity is power of 2 and >= len
    assert((arr->cap & (arr->cap - 1)) == 0); // is power of 2
    assert(arr->cap >= arr->len);

    // Verify content
    for (size_t i = 0; i < arr->len; i++) {
        int *got = arr_get(arr, i);
        assert(got != NULL);
        assert(*got == values[i]);
        printf("arr[%zu] = %d\n", i, *got);
    }

    // Out of range
    assert(arr_get(arr, arr->len) == NULL);
    assert(!arr_append(arr, NULL)); // null value

    arr_free(&arr);
    assert(arr == NULL);

    printf("→ Integer test passed ✓\n");
}

// Test with doubles
static void test_double_array(void)
{
    printf("\n[TEST] Double array\n");

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
        printf("arr[%zu] = %.9f\n", i, *got);
    }

    // Test set (overwrite)
    double new_val = 42.195;
    bool set_ok = arr_set(arr, 1, &new_val);
    assert(set_ok);

    double *updated = arr_get(arr, 1);
    assert(DOUBLE_EQ(*updated, 42.195));

    arr_free(&arr);
    printf("→ Double test passed ✓\n");
}

// Test with struct
typedef struct {
    int id;
    char name[20];
    float score;
} Student;

static void test_struct_array(void)
{
    printf("\n[TEST] Struct array + growth\n");

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
    assert((arr->cap & (arr->cap - 1)) == 0); // power of 2

    // Verify data
    Student *got = arr_get(arr, 2);
    assert(got != NULL);
    assert(got->id == 103);
    assert(strcmp(got->name, "Charlie Lee") == 0);
    assert(fabs(got->score - 92.75f) < 0.001f);

    printf("arr[2] = {id=%d, name=%s, score=%.2f}\n",
        got->id, got->name, got->score);

    arr_free(&arr);
    printf("→ Struct test passed ✓\n");
}

int main(void)
{
    printf("=== Dynamic Array Tests ===\n");

    test_int_array();
    test_double_array();
    test_struct_array();

    printf("\nAll tests completed successfully!\n");
    return 0;
}
