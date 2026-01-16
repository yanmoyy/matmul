#include "array.h"
#include "matrix.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 100 // maximum rows of matrix
#define MAX_COL 100 // maximum cols of matrix
#define MAX_NUM 100 // maximum number of matrix

void exit_with_fail(const char *format, ...);
bool add_file(Array *files, const char *filename);
void close_files(Array *files);
Matrix *parse_file_to_matrix(FILE **file_ptr);

int main(int argc, char *argv[])
{
    // STEP 1. Check Arguemnts
    if (argc < 3) {
        exit_with_fail("Usage: %s file1 file2 ...\n", argv[0]);
    }

    int file_num = argc - 1;
    if (file_num > MAX_NUM) {
        exit_with_fail("Too many files (%d > %d)\n", file_num, MAX_NUM);
    }

    // STEP 2. Open the files
    Array *files = arr_new(file_num, sizeof(FILE *));

    for (int i = 0; i < file_num; i++) {
        char *filename = argv[i + 1];
        bool success = add_file(files, filename);
        if (!success) {
            close_files(files);
            exit_with_fail("Error: failed to open %s\n", filename);
        }
    }

    printf("All %zu files exist!\n", files->len);

    for (int i = 0; i < file_num; i++) {
        FILE **file_ptr = arr_get(files, i);
        if (file_ptr == NULL || *file_ptr == NULL) {
            close_files(files);
            exit_with_fail("Something wrong on %d'th file\n", i + 1);
        }

        int row = 0, col = 0;
        FILE *file = *file_ptr;
        fscanf(file, "%d %d", &row, &col);

        if (row <= 0 || col <= 0) {
            close_files(files);
            exit_with_fail("Something wrong on %d'th file\n", i + 1);
        }

        printf("row: %d\n", row);
        printf("col: %d\n", col);

        Matrix *m = parse_matrix(row, col, argv[i], file);
        print_matrix(m);
    }

    close_files(files);
    return 0;
}

// printf wraper for exiting on failure
void exit_with_fail(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
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

// Matrix *parse_file_to_matrix(FILE **file_ptr)
// {
//     int row, col;
//     fscanf(*file_ptr, "%d %d", row, col);
//     printf("row: %d\n", row);
//     printf("col: %d\n", col);
// }
