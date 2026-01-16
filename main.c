#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 100 // maximum rows of matrix
#define MAX_COL 100 // maximum cols of matrix
#define MAX_NUM 100 // maximum number of matrix

void exit_with_fail(const char *format, ...);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        exit_with_fail("Usage: %s file1 file2 ...\n", argv[0]);
    }

    int file_num = argc - 1;
    if (file_num > MAX_NUM) {
        exit_with_fail("Too many files (%d > %d)\n", file_num, MAX_NUM);
    }

    printf("Hello world!\n");
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

// // add file to list, return true if the file exist and no error
// bool add_file(const char *filename, FILE *array[MAX_NUM])
// {
//     FILE *file = fopen(filename, "r");
//     if (file == NULL) {
//         return false;
//     }
//     return true;
// }
