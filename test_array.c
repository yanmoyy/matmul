#include "array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test with primitive types
void test_int_array(void)
{
    printf("\n=== Testing int array ===\n");

    Array arr = arr_new(5, sizeof(int));
    assert(arr.data != NULL);
    assert(arr.n == 5);
    assert(arr.size == sizeof(int));

    // Update some values
    int values[] = { 10, 20, 30, 40, 50 };
    for (size_t i = 0; i < 5; i++) {
        bool ok = arr_update(&arr, i, &values[i]);
        assert(ok);
    }

    // Get and verify
    for (size_t i = 0; i < 5; i++) {
        int *got = arr_get(&arr, i);
        assert(got != NULL);
        assert(*got == values[i]);
        printf("arr[%zu] = %d\n", i, *got);
    }

    // Out of range checks
    assert(arr_get(&arr, 5) == NULL);
    assert(arr_get(&arr, 999) == NULL);
    assert(!arr_update(&arr, 5, &values[0]));
    assert(!arr_update(&arr, (size_t)-1, &values[0]));

    arr_free(&arr);
    assert(arr.data == NULL);
    printf("Int array tests passed ✓\n");
}

// Test with doubles
void test_double_array(void)
{
    printf("\n=== Testing double array ===\n");

    Array arr = arr_new(4, sizeof(double));
    double vals[] = { 1.1, 2.718, 3.14159, 42.0 };

    for (size_t i = 0; i < 4; i++) {
        arr_update(&arr, i, &vals[i]);
    }

    for (size_t i = 0; i < 4; i++) {
        double *got = arr_get(&arr, i);
        assert(got != NULL);
        assert(*got == vals[i]);
        printf("arr[%zu] = %.5f\n", i, *got);
    }

    arr_free(&arr);
    assert(arr.data == NULL);
    printf("Double array tests passed ✓\n");
}

// Test with a struct
typedef struct {
    int id;
    char name[16];
} Person;

void test_struct_array(void)
{
    printf("\n=== Testing struct array ===\n");

    Array arr = arr_new(3, sizeof(Person));

    Person p1 = { 1, "Alice" };
    Person p2 = { 2, "Bob" };
    Person p3 = { 3, "Charlie" };

    arr_update(&arr, 0, &p1);
    arr_update(&arr, 1, &p2);
    arr_update(&arr, 2, &p3);

    Person *got = arr_get(&arr, 1);
    assert(got != NULL);
    assert(got->id == 2);
    assert(strcmp(got->name, "Bob") == 0);
    printf("arr[1] = {id=%d, name=%s}\n", got->id, got->name);

    assert(arr_get(&arr, 3) == NULL);

    arr_free(&arr);
    assert(arr.data == NULL);
    printf("Struct array tests passed ✓\n");
}

int main(void)
{
    printf("=== Starting array tests ===\n");

    test_int_array();
    test_double_array();
    test_struct_array();

    printf("\nAll tests passed successfully!\n");
    return 0;
}
