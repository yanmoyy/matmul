#include "matrix.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// TODO: need to handle memory leak when the test failed!!!

static void assert_matrix_meta(const Matrix *m, size_t row, size_t col, m_type_t type, const char *name)
{
    assert(m != NULL);
    assert(m->row == row);
    assert(m->col == col);
    assert(m->type == type);
    if (name == NULL) {
        assert(m->name == NULL);
    } else {
        assert(strcmp(m->name, name) == 0);
    }
}

static void assert_matrix_data(const Matrix *m, const void *expected)
{
    if (!m || !m->data || !expected) {
        test_fail("assert_matrix_data: NULL pointer");
        return;
    }

    size_t count = m->row * m->col;
    int mismatch_found = 0;

#define CHECK(type, fmt)                                                                           \
    do {                                                                                           \
        const type *got = (const type *)m->data;                                                   \
        const type *exp = (const type *)expected;                                                  \
        for (size_t i = 0; i < count; i++) {                                                       \
            if (got[i] != exp[i]) {                                                                \
                if (!mismatch_found) {                                                             \
                    printf("Data mismatch at index (%zu, %zu): got= " fmt ", expected= " fmt "\n", \
                        i / m->col, i % m->col, got[i], exp[i]);                                   \
                    mismatch_found = 1;                                                            \
                    break;                                                                         \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
    } while (0)

    switch (m->type) {
    case M_TYPE_INT:
        CHECK(int, "%d");
        break;
    case M_TYPE_FLOAT:
        CHECK(float, "%g");
        break;
    case M_TYPE_DOUBLE:
        CHECK(double, "%g");
        break;
    default:
        test_fail("Unsupported matrix type");
        return;
    }

#undef CHECK
    if (mismatch_found) {
        print_matrix(m);
        test_fail("matrix mismatched");
    }
}

static void free_and_check(Matrix **m)
{
    free_matrix(m);
    assert(*m == NULL);
}

static void test_new_matrix_basic()
{
    test_start("new_matrix - basic allocation");
    Matrix *m = new_matrix(3, 4, M_TYPE_INT, "test");
    assert_matrix_meta(m, 3, 4, M_TYPE_INT, "test");

    int expected[3][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    assert_matrix_data(m, expected);
    free_and_check(&m);

    // check null name
    m = new_matrix(3, 4, M_TYPE_INT, NULL);
    assert_matrix_meta(m, 3, 4, M_TYPE_INT, NULL);
    free_and_check(&m);

    test_success();
}

static void test_new_matrix_with_2D_array()
{
    test_start("new_matrix_with_2D_array");

    int data_int[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    Matrix *m = new_matrix_with_2D_array(2, 3, M_TYPE_INT, "test", data_int);
    assert_matrix_meta(m, 2, 3, M_TYPE_INT, "test");
    assert_matrix_data(m, data_int);
    free_and_check(&m);

    float data_float[2][3] = {
        { 1.25f, -2.50f, 3.75f },
        { 10.00f, 0.125f, -5.5f }
    };
    m = new_matrix_with_2D_array(2, 3, M_TYPE_FLOAT, "test", data_float);
    assert_matrix_meta(m, 2, 3, M_TYPE_FLOAT, "test");
    assert_matrix_data(m, data_float);
    free_and_check(&m);

    double data_double[2][3] = {
        { 1.25, -2.50, 3.75 },
        { 10.00, 0.125, -5.5 }
    };
    m = new_matrix_with_2D_array(2, 3, M_TYPE_DOUBLE, "test", data_double);
    assert_matrix_meta(m, 2, 3, M_TYPE_DOUBLE, "test");
    assert_matrix_data(m, data_double);
    free_and_check(&m);

    test_success();
}

static void test_matrix_to_string()
{
    test_start("matrix_to_string");

    int data_int[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };

    Matrix *m = new_matrix_with_2D_array(2, 3, M_TYPE_INT, "test", data_int);
    assert_matrix_meta(m, 2, 3, M_TYPE_INT, "test");

    char buf[1024] = { 0 };
    int len = matrix_to_string(m, buf, sizeof buf);
    assert(len > 0);

    const char *expected = "Matrix (test): 2 × 3, type = int\n"
                           "Data:\n"
                           "       1        2        3 \n"
                           "       4        5        6 \n";

    if (strcmp(buf, expected) != 0) {
        print_string_diff(buf, expected);
        test_fail("matrix_to_string failed");
        return;
    }

    free_and_check(&m);

    // test matrix_to_string with NULL name

    m = new_matrix_with_2D_array(2, 3, M_TYPE_INT, NULL, data_int);
    assert_matrix_meta(m, 2, 3, M_TYPE_INT, NULL);
    len = matrix_to_string(m, buf, sizeof buf);
    assert(len > 0);

    expected = "Matrix (NULL): 2 × 3, type = int\n"
               "Data:\n"
               "       1        2        3 \n"
               "       4        5        6 \n";

    if (strcmp(buf, expected) != 0) {
        print_string_diff(buf, expected);
        free_matrix(&m);
        test_fail("matrix_to_string failed");
    }

    free_and_check(&m);
    test_success();
}

static void test_get_matrix_name()
{
    test_start("get_matrix_name");

    typedef struct {
        const char *filepath;
        const char *expected;
        bool is_error;
    } test_case_t;

    const test_case_t tests[] = {
        { "matrix/A.txt", "A", false },
        { "/data/B.csv", "B", false },
        { "simple.dat", "simple", false },
        { "folder/sub/no-extension", "no-extension", false },
        { "weird.name.with.dots.txt",
            "weird.name.with.dots",
            false },
        { "./C", "C", false },
        { "justname", "justname", false },
        { NULL, NULL, true }
    };

    size_t test_count = sizeof tests / sizeof *tests;

    for (size_t i = 0; i < test_count; i++) {
        char buf[1024];
        int ret = get_matrix_name(tests[i].filepath, buf, sizeof buf);
        if (tests[i].is_error) {
            assert(ret == -1);
            continue;
        }
        assert(ret == (int)strlen(tests[i].expected));
        assert(strcmp(buf, tests[i].expected) == 0);
    }

    test_success();
}

static void test_multiply_matrices()
{
    test_start("multiply_matrices");
    int m1[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    int m2[3][2] = {
        { 1, 4 },
        { 2, 5 },
        { 3, 6 }
    };
    int expected[2][2] = {
        { 14, 32 },
        { 32, 77 }
    };

    Matrix *m1_ = new_matrix_with_2D_array(2, 3, M_TYPE_INT, "m1", m1);
    Matrix *m2_ = new_matrix_with_2D_array(3, 2, M_TYPE_INT, "m2", m2);

    Matrix *result = matrix_multiply(m1_, m2_);

    assert_matrix_meta(result, 2, 2, M_TYPE_INT, "m1m2");
    assert_matrix_data(result, expected);

    free_and_check(&m1_);
    free_and_check(&m2_);
    free_and_check(&result);

    test_success();
}

void run_matrix_tests()
{
    printf("\n=== Matrix Tests ===\n");
    test_new_matrix_basic();
    test_matrix_to_string();
    test_new_matrix_with_2D_array();
    test_get_matrix_name();
    test_multiply_matrices();

    printf("\nMatrix tests finished.\n");
}
