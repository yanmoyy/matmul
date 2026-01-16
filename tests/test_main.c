#include "utils.h"
#include <stdio.h>

void test_int_array(void);
void test_double_array(void);
void test_struct_array(void);

int main(void)
{
    printf("=== Dynamic Array Tests ===\n");
    test_int_array();
    test_double_array();
    test_struct_array();

    test_summary(); // Replaces the hardcoded message
    return 0;
}
