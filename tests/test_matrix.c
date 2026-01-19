#include "matrix.h"
#include "test_utils.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static bool test_failed = false;

static bool check_matrix_meta(const Matrix *m, size_t row, size_t col, const char *name)
{
    if (m == NULL
        || m->row != row
        || m->col != col) {
        return false;
    }
    if (name == NULL) {
        return m->name == NULL;
    } else {
        return strcmp(m->name, name) == 0;
    }
}

static bool check_matrix_data(const Matrix *m, const void *expected)
{
    if (!m || !m->data || !expected) {
        // null pointer
        return false;
    }

    size_t count = m->row * m->col;
    int mismatch_found = 0;
    double *got = (double *)expected;

    for (size_t i = 0; i < count; i++) {
        if (!(fabs(m->data[i] - got[i]) < 0.00001)) {
            if (!mismatch_found) {
                printf("Data mismatch at index (%zu, %zu): got=%lf, expected=%lf\n",
                    i / m->col, i % m->col, m->data[i], got[i]);
                mismatch_found = 1;
                break;
            }
        }
    }

    if (mismatch_found) {
        matrix_print(m);
        return false;
    }
    return true;
}

static void test_new_matrix_basic()
{
    test_start("new_matrix - basic allocation");

    Matrix *m1 = NULL, *m2 = NULL;
    m1 = matrix_new(3, 4, "test");
    T_ASSERT(check_matrix_meta(m1, 3, 4, "test"));

    double expected[3][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    T_ASSERT(check_matrix_data(m1, expected));

    // check null name
    m2 = matrix_new(3, 4, NULL);
    T_ASSERT(m2 == NULL);

cleanup:
    matrix_free(&m1);
    matrix_free(&m2);
    T_CHECK_TEST_FAIL("new_matrix failed");
}

static void test_new_matrix_with_array()
{
    test_start("new_matrix_with_array");

    Matrix *m1 = NULL, *m2 = NULL, *m3 = NULL;

    double data_double[2][3] = {
        { 1.25, -2.50, 3.75 },
        { 10.00, 0.125, -5.5 }
    };
    m3 = matrix_new_with_array(2, 3, "test", data_double);
    T_ASSERT(check_matrix_meta(m3, 2, 3, "test"));
    T_ASSERT(check_matrix_data(m3, data_double));

cleanup:
    matrix_free(&m1);
    matrix_free(&m2);
    matrix_free(&m3);

    T_CHECK_TEST_FAIL("new_matrix_with_2D_array failed");
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
        int ret = file_path_to_name(tests[i].filepath, buf, sizeof buf);
        if (tests[i].is_error) {
            T_ASSERT(ret == -1);
            continue;
        }
        T_ASSERT(ret == (int)strlen(tests[i].expected));
        T_ASSERT(strcmp(buf, tests[i].expected) == 0);
    }

cleanup:
    T_CHECK_TEST_FAIL("get_matrix_name failed");
}

static void test_multiply_matrices()
{
    test_start("multiply_matrices");
    double m1[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    double m2[3][2] = {
        { 1, 4 },
        { 2, 5 },
        { 3, 6 }
    };
    double expected[2][2] = {
        { 14, 32 },
        { 32, 77 }
    };

    Matrix *m1_ = matrix_new_with_array(2, 3, "m1", m1);
    Matrix *m2_ = matrix_new_with_array(3, 2, "m2", m2);
    Matrix *result = matrix_multiply(m1_, m2_);

    T_ASSERT(check_matrix_meta(result, 2, 2, "m1m2"));
    T_ASSERT(check_matrix_data(result, expected));

cleanup:
    matrix_free(&m1_);
    matrix_free(&m2_);
    matrix_free(&result);

    T_CHECK_TEST_FAIL("matrix_multiply failed");
}

void run_matrix_tests()
{
    printf("\n=== Matrix Tests ===\n");
    test_new_matrix_basic();
    test_new_matrix_with_array();
    test_get_matrix_name();
    test_multiply_matrices();

    printf("\nMatrix tests finished.\n");
}
