#include "utils.h"
#include <stdio.h>

void test_int_array();
void test_double_array();
void test_struct_array();
void test_print_matrix();

int main()
{
    printf("=== Dynamic Array Tests ===\n");
    test_int_array();
    test_double_array();
    test_struct_array();

    printf("=== Matrix Tests ===\n");
    test_print_matrix();

    test_summary(); // Replaces the hardcoded message
    return 0;
}
