#include "matrix.h"

void test_print_matrix()
{
    int mat[3][2] = {
        { 1, 2 },
        { 3, 4 },
        { 5, 6 },
    };
    Matrix *m = new_matrix_with_2D_array(3, 2, mat);
    print_matrix(m);
}
