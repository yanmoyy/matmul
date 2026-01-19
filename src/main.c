#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // STEP 1. Check Arguemnts
    if (argc < 3) {
        fprintf(stderr, "Usage: %s file1 file2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    // STEP 2. Check all files exist
    if (!all_input_files_exist(argv + 1, argc - 1)) {
        fprintf(stderr, "Some files do not exist\n");
        return EXIT_FAILURE;
    }

    // STEP 3. Multiply the matrices
    Matrix *result = matrix_load_from_file(argv[1]);
    if (!result) {
        return EXIT_FAILURE;
    }
    matrix_print(result);

    for (int i = 2; i < argc; i++) { // skip first
        Matrix *next = matrix_load_from_file(argv[i]);
        if (!next) {
            matrix_free(&result);
            return EXIT_FAILURE;
        }
        matrix_print(next);

        Matrix *temp = matrix_multiply(result, next);
        if (!temp) {
            matrix_free(&next);
            matrix_free(&result);
            return EXIT_FAILURE;
        }
        matrix_free(&result);
        matrix_free(&next);

        result = temp;
    }

    // STEP 4. print the final result
    matrix_print(result);

    matrix_free(&result);
    return EXIT_SUCCESS;
}
