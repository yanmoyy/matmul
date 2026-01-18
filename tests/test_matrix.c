#include "matrix.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void assert_matrix(const Matrix *m, size_t row, size_t col, m_type_t type, const char *name)
{
    assert(m != NULL);
    assert(m->row == row);
    assert(m->col == col);
    assert(m->type == type);
    assert(m->name != NULL);
    if (name == NULL) {
        assert(strcmp(m->name, "(null)") == 0);
    } else {
        assert(strcmp(m->name, name) == 0);
    }
}

static void free_and_check(Matrix **m)
{
    free_matrix(m);
    assert(*m == NULL);
}

void test_new_matrix_basic()
{
    test_start("new_matrix - basic allocation");
    Matrix *m = new_matrix(3, 4, M_TYPE_INT, "test");
    assert_matrix(m, 3, 4, M_TYPE_INT, "test");

    // check data is all zero
    for (size_t i = 0; i < m->row * m->col; i++) {
        assert(((int *)m->data)[i] == 0);
    }

    free_and_check(&m);

    // check null name
    m = new_matrix(3, 4, M_TYPE_INT, NULL);
    assert_matrix(m, 3, 4, M_TYPE_INT, NULL);
    free_and_check(&m);

    test_success();
}

void test_new_matrix_with_2D_array_double()
{
    test_start("new_matrix_with_2D_array - double 3×3");

    double data[3][3] = {
        { 1.25, -2.50, 3.75 },
        { 10.00, 0.125, -5.5 },
        { 7.50, 8.75, 9.00 }
    };

    Matrix *m = new_matrix_with_2D_array(3, 3, M_TYPE_DOUBLE, "test", data);
    assert_matrix(m, 3, 3, M_TYPE_DOUBLE, "test");

    double *flat = (double *)m->data;
    double expected[] = { 1.25, -2.50, 3.75, 10.00, 0.125, -5.5, 7.50, 8.75, 9.00 };

    int ok = 1;
    for (int i = 0; i < 9; i++) {
        if (flat[i] != expected[i]) {
            printf("Data mismatch at %d: got %f, want %f\n", i, flat[i], expected[i]);
            ok = 0;
            break;
        }
    }
    if (ok) {
        test_success();
    } else {
        test_fail("double data copy failed");
    }
    free_and_check(&m);
}
void test_matrix_to_string()
{
    test_start("matrix_to_string");

    int data_int[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };

    Matrix *m = new_matrix_with_2D_array(2, 3, M_TYPE_INT, "test", data_int);
    assert_matrix(m, 2, 3, M_TYPE_INT, "test");

    char buf[1024] = { 0 };
    int len = matrix_to_string(m, buf, sizeof buf);
    assert(len > 0);

    const char *expected = "Matrix: 2 × 3, type = int, name = test\n"
                           "Data:\n"
                           "       1        2        3 \n"
                           "       4        5        6 \n"
                           "\n";

    if (strcmp(buf, expected) != 0) {
        print_string_diff(buf, expected);
        test_fail("matrix_to_string failed");
        return;
    }
    free_and_check(&m);

    test_success();
}

void run_matrix_tests()
{
    printf("\n=== Matrix Tests ===\n");
    test_new_matrix_basic();
    // test_print_matrix();
    test_matrix_to_string();
    // test_new_matrix_with_2D_array_int();
    // test_print_matrix_double();
    // test_parse_matrix_dummy();   // uncomment when you have a test file

    printf("\nMatrix tests finished.\n");
}
