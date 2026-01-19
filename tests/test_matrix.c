#include "matrix.h"
#include "test_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static bool test_failed = false;

static bool check_matrix_meta(const Matrix *m, size_t row, size_t col, m_type_t type, const char *name)
{
    if (m == NULL
        || m->row != row
        || m->col != col
        || m->type != type) {
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
        return false;
    }

#undef CHECK
    if (mismatch_found) {
        print_matrix(m);
        return false;
    }
    return true;
}

static void test_new_matrix_basic()
{
    test_start("new_matrix - basic allocation");

    Matrix *m1 = NULL, *m2 = NULL;
    m1 = new_matrix(3, 4, M_TYPE_INT, "test");
    T_ASSERT(check_matrix_meta(m1, 3, 4, M_TYPE_INT, "test"));

    int expected[3][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    T_ASSERT(check_matrix_data(m1, expected));

    // check null name
    m2 = new_matrix(3, 4, M_TYPE_INT, NULL);
    T_ASSERT(check_matrix_meta(m2, 3, 4, M_TYPE_INT, NULL));

cleanup:
    free_matrix(&m1);
    free_matrix(&m2);

    T_CHECK_TEST_FAIL("new_matrix failed");
}

static void test_new_matrix_with_2D_array()
{
    test_start("new_matrix_with_2D_array");

    Matrix *m1 = NULL, *m2 = NULL, *m3 = NULL;

    int data_int[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    m1 = new_matrix_with_2D_array(2, 3, M_TYPE_INT, "test", data_int);
    T_ASSERT(check_matrix_meta(m1, 2, 3, M_TYPE_INT, "test"));
    T_ASSERT(check_matrix_data(m1, data_int));

    float data_float[2][3] = {
        { 1.25f, -2.50f, 3.75f },
        { 10.00f, 0.125f, -5.5f }
    };
    m2 = new_matrix_with_2D_array(2, 3, M_TYPE_FLOAT, "test", data_float);
    T_ASSERT(check_matrix_meta(m2, 2, 3, M_TYPE_FLOAT, "test"));
    T_ASSERT(check_matrix_data(m2, data_float));

    double data_double[2][3] = {
        { 1.25, -2.50, 3.75 },
        { 10.00, 0.125, -5.5 }
    };
    m3 = new_matrix_with_2D_array(2, 3, M_TYPE_DOUBLE, "test", data_double);
    T_ASSERT(check_matrix_meta(m3, 2, 3, M_TYPE_DOUBLE, "test"));
    T_ASSERT(check_matrix_data(m3, data_double));

cleanup:
    free_matrix(&m1);
    free_matrix(&m2);
    free_matrix(&m3);

    T_CHECK_TEST_FAIL("new_matrix_with_2D_array failed");
}

static void test_matrix_to_string()
{
    test_start("matrix_to_string");

    Matrix *m1 = NULL, *m2 = NULL;

    int data_int[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };

    m1 = new_matrix_with_2D_array(2, 3, M_TYPE_INT, "test", data_int);
    T_ASSERT(check_matrix_meta(m1, 2, 3, M_TYPE_INT, "test"));

    char buf[1024] = { 0 };
    int len = matrix_to_string(m1, buf, sizeof buf);
    T_ASSERT(len > 0);

    const char *expected = "Matrix (test): 2 × 3, type = int\n"
                           "Data:\n"
                           "       1        2        3 \n"
                           "       4        5        6 \n";

    if (strcmp(buf, expected) != 0) {
        print_string_diff(buf, expected);
        T_ASSERT(false);
        return;
    }

    // test matrix_to_string with NULL name
    m2 = new_matrix_with_2D_array(2, 3, M_TYPE_INT, NULL, data_int);
    T_ASSERT(check_matrix_meta(m2, 2, 3, M_TYPE_INT, NULL));

    len = matrix_to_string(m2, buf, sizeof buf);
    T_ASSERT(len > 0);

    expected = "Matrix (NULL): 2 × 3, type = int\n"
               "Data:\n"
               "       1        2        3 \n"
               "       4        5        6 \n";

    if (strcmp(buf, expected) != 0) {
        print_string_diff(buf, expected);
        T_ASSERT(false);
    }

cleanup:
    free_matrix(&m1);
    free_matrix(&m2);

    T_CHECK_TEST_FAIL("matrix_to_string failed");
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

    T_ASSERT(check_matrix_meta(result, 2, 2, M_TYPE_INT, "m1m2"));
    T_ASSERT(check_matrix_data(result, expected));

cleanup:
    free_matrix(&m1_);
    free_matrix(&m2_);
    free_matrix(&result);

    T_CHECK_TEST_FAIL("matrix_multiply failed");
}

static void test_copy_matrix()
{
    test_start("copy_matrix");

    int m1[2][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };

    Matrix *m1_ = new_matrix_with_2D_array(2, 2, M_TYPE_INT, "m1", m1);
    Matrix *result = copy_matrix(m1_);

    T_ASSERT(check_matrix_meta(result, 2, 2, M_TYPE_INT, "m1"));
    T_ASSERT(check_matrix_data(result, m1));

cleanup:
    free_matrix(&m1_);
    free_matrix(&result);

    T_CHECK_TEST_FAIL("copy_matrix failed");
}

void run_matrix_tests()
{
    printf("\n=== Matrix Tests ===\n");
    test_new_matrix_basic();
    test_matrix_to_string();
    test_new_matrix_with_2D_array();
    test_get_matrix_name();
    test_multiply_matrices();
    test_copy_matrix();

    printf("\nMatrix tests finished.\n");
}
