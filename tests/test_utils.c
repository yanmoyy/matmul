#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

// Static counter for summary (shared across tests)
static int tests_passed = 0;
static int tests_total = 0;

void test_start(const char *test_name)
{
    printf("\n[TEST] %s\n", test_name);
    tests_total++; // Increment on start
}

void test_success()
{
    printf("→ %s passed ✓\n", tests_total == 1 ? "Test" : "Tests");
    tests_passed++;
}

void test_fail(const char *msg)
{
    fprintf(stderr, "[FAIL] %s\n", msg);
    exit(1);
}

void test_summary()
{
    printf("\n=== Summary ===\n");
    printf("Passed: %d/%d tests ✓\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("All tests completed successfully!\n");
    } else {
        printf("Some tests failed! Check the output above.\n");
    }
}

void print_string_diff(const char *got, const char *expected)
{
    printf("\nExpected (with visible escapes):\n");
    for (const char *p = expected; *p; p++) {
        if (*p == '\n') {
            printf("\\n\n");
        } else if (*p == ' ') {
            printf("·"); // middle dot for space
        } else {
            printf("%c", *p);
        }
    }
    printf("\n\nGot (with visible escapes):\n");
    for (const char *p = got; *p; p++) {
        if (*p == '\n') {
            printf("\\n\n");
        } else if (*p == ' ') {
            printf("·");
        } else {
            printf("%c", *p);
        }
    }
    printf("\n");
}
