#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>

#define T_ASSERT(expr)                           \
    do {                                         \
        if (!(expr)) {                           \
            fprintf(stderr,                      \
                "%s:%d: assertion failed: %s\n", \
                __FILE_NAME__, __LINE__, #expr); \
            test_failed = true;                  \
            goto cleanup;                        \
        }                                        \
    } while (0)

#define T_CHECK_TEST_FAIL(msg) \
    do {                       \
        if (test_failed) {     \
            test_fail(msg);    \
        } else {               \
            test_success();    \
        }                      \
    } while (0)

#define FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-6f)
#define DOUBLE_EQ(a, b) (fabs((a) - (b)) < 1e-9)

void test_start(const char *test_name);
void test_success();
void test_fail(const char *msg) __attribute__((noreturn));
void test_summary();

void print_string_diff(const char *got, const char *expected);

#endif
