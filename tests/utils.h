#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

void test_start(const char *test_name);
void test_success(void);
void test_fail(const char *msg) __attribute__((noreturn));
void test_summary(void);

#endif
