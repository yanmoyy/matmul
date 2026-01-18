#include "array.h"
#include "error.h"
#include "matrix.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 100 // maximum rows of matrix
#define MAX_COL 100 // maximum cols of matrix
#define MAX_NUM 100 // maximum number of matrix

bool add_file(Array *files, const char *filename);
void close_files(Array *files);

int main(int argc, char *argv[])
{
    //   STEP 1. Check Arguemnts
    if (argc < 3) {
        ERROR("Usage: %s file1 file2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int file_num = argc - 1;
    if (file_num > MAX_NUM) {
        ERROR("Too many files (%d > %d)\n", file_num, MAX_NUM);
        exit(EXIT_FAILURE);
    }

    // STEP 2. Open the files
    Array *files = arr_new(file_num, sizeof(FILE *));
    if (!files) {
        ERROR("arr_new failed\n");
        exit(EXIT_FAILURE);
    }

    bool success = false;

    for (int i = 0; i < file_num; i++) {
        char *filename = argv[i + 1];
        bool success = add_file(files, filename);
        if (!success) {
            ERROR("Failed to open file: %s\n", filename);
            goto cleanup;
        }
    }

    for (int i = 0; i < file_num; i++) {
        FILE **file_ptr = arr_get(files, i);
        if (file_ptr == NULL || *file_ptr == NULL) {
            ERROR("Failed to get file: %s\n", argv[i + 1]);
            goto cleanup;
        }

        int row = 0, col = 0;
        FILE *file = *file_ptr;
        fscanf(file, "%d %d", &row, &col);

        if (row <= 0 || col <= 0 || row > MAX_ROW || col > MAX_COL) {
            ERROR("Invalid matrix dimensions: %d x %d\n", row, col);
            goto cleanup;
        }

        char name[1024];
        int ret = get_matrix_name(argv[i + 1], name, sizeof name);
        if (ret < 0) {
            ERROR("Failed to get matrix name: %s\n", argv[i + 1]);
            goto cleanup;
        }

        // TODO: handle multiple types?
        int data[row][col];
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                fscanf(file, "%d", &data[i][j]);
            }
        }

        Matrix *m = new_matrix_with_2D_array(row, col, M_TYPE_INT, name, data);
        print_matrix(m);
        free_matrix(&m);
    }
    success = true;
cleanup:
    close_files(files);
    arr_free(&files);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

// add file to list, return true if the file exist and no error
bool add_file(Array *files, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }
    return arr_append(files, &file);
}

void close_files(Array *files)
{
    for (size_t i = 0; i < files->len; i++) {
        FILE *f = *(FILE **)arr_get(files, i);
        if (f) {
            fclose(f);
        }
    }
}
