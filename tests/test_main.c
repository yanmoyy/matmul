#include "test_utils.h"

void run_array_tests();
void run_matrix_tests();

int main()
{
    run_array_tests();
    run_matrix_tests();
    test_summary(); // Replaces the hardcoded message
    return 0;
}
