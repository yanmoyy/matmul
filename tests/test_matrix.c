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

static void check_multiply_and_cleanup(Matrix *m1, Matrix *m2, int exp_r, int exp_c,
    const char *exp_n, void *exp_data)
{
    Matrix *res = matrix_multiply(m1, m2);

    if (exp_data == NULL) {
        T_ASSERT(res == NULL);
    } else {
        T_ASSERT(res != NULL);
        T_ASSERT(check_matrix_meta(res, exp_r, exp_c, exp_n));
        T_ASSERT(check_matrix_data(res, exp_data));
    }

cleanup:
    matrix_free(&m1);
    matrix_free(&m2);
    matrix_free(&res);
}

static void test_multiply_matrices()
{
    test_start("multiply_matrices");

    // Case: Standard 2x3 * 3x2
    {
        double a[2][3] = {
            { 1, 2, 3 },
            { 4, 5, 6 },
        };
        double b[3][2] = {
            { 1, 4 },
            { 2, 5 },
            { 3, 6 },
        };
        double exp[2][2] = {
            { 14, 32 },
            { 32, 77 },
        };
        Matrix *m1 = matrix_new_with_array(2, 3, "m1", a);
        Matrix *m2 = matrix_new_with_array(3, 2, "m2", b);
        check_multiply_and_cleanup(m1, m2, 2, 2, "m1m2", exp);
    }

    // Case: Identity
    {
        double a[2][2] = {
            { 5, 6 },
            { 7, 8 },
        };
        double id[2][2] = {
            { 1, 0 },
            { 0, 1 },
        };
        Matrix *m1 = matrix_new_with_array(2, 2, "A", a);
        Matrix *m2 = matrix_new_with_array(2, 2, "I", id);
        check_multiply_and_cleanup(m1, m2, 2, 2, "AI", a);
    }

    // Case: Dimension mismatch
    {
        double a[2][3] = {
            { 1, 2, 3 },
            { 4, 5, 6 },
        };
        double b[2][2] = {
            { 1, 4 },
            { 2, 5 },
        };
        void *exp = NULL;
        Matrix *m1 = matrix_new_with_array(2, 3, "m1", a);
        Matrix *m2 = matrix_new_with_array(2, 2, "m2", b);
        check_multiply_and_cleanup(m1, m2, 2, 2, "m1m2", exp);
    }

    // Case: Large-ish numbers
    {
        double a[2][2] = {
            { 1000000.0, -200000.0 },
            { 0.0001, 999999.0 },
        };
        double b[2][2] = {
            { 0.5, 2.0 },
            { 1.0, -0.5 },
        };
        double exp[2][2] = {
            { 300000.0, 2100000.0 },
            { 999999.00005, -499999.4998 },
        };

        Matrix *m1 = matrix_new_with_array(2, 2, "m1", a);
        Matrix *m2 = matrix_new_with_array(2, 2, "m2", b);
        check_multiply_and_cleanup(m1, m2, 2, 2, "m1m2", exp);
    }

    T_CHECK_TEST_FAIL("matrix_multiply suite failed");
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
