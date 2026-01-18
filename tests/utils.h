#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-6f)
#define DOUBLE_EQ(a, b) (fabs((a) - (b)) < 1e-9)

void test_start(const char *test_name);
void test_success();
void test_fail(const char *msg) __attribute__((noreturn));
void test_summary();

void print_string_diff(const char *got, const char *expected);

#endif
