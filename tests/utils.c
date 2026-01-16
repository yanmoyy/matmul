#include "utils.h"
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

void test_success(void)
{
    printf("→ %s passed ✓\n", tests_total == 1 ? "Test" : "Test"); // Singular/plural if needed
    tests_passed++;
}

void test_fail(const char *msg)
{
    printf("\n[FAIL] %s\n", msg);
    exit(1); // Or return a non-zero code if you prefer no exit
}

void test_summary(void)
{
    printf("\n=== Summary ===\n");
    printf("Passed: %d/%d tests ✓\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("All tests completed successfully!\n");
    } else {
        printf("Some tests failed! Check the output above.\n");
    }
}
