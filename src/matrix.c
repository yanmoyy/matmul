#include "matrix.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *concat(const char *s1, const char *s2);

Matrix *matrix_new(size_t row, size_t col, const char *name)
{
    if (row <= 0 || col <= 0 || row > MAX_ROW || col > MAX_COL) {
        ERROR("new_matrix: Invalid dimensions (%zu, %zu)\n", row, col);
        return NULL;
    }

    if (name == NULL) {
        ERROR("new_matrix: name is null\n");
        return NULL;
    }

    if (strlen(name) > MAX_NAME_LEN) {
        ERROR("new_matrix: Invalid name length (%s)", name);
        return NULL;
    }

    Matrix *m = malloc(sizeof(Matrix));
    if (!m) {
        ERROR("Failed to allocate Matrix struct\n");
        return NULL;
    }

    m->row = row;
    m->col = col;
    m->name = strdup(name);
    if (!m->name) {
        ERROR("Failed to duplicate matrix name\n");
        free(m);
        return NULL;
    }

    size_t total_bytes = sizeof(double) * row * col;
    if (total_bytes / sizeof(double) != row * col) {
        ERROR("new_matrix: potential overflow\n");
        matrix_free(&m);
        return NULL;
    }

    m->data = (double *)calloc(row * col, sizeof(double));
    if (!m->data) {
        ERROR("Failed to allocate memory\n");
        matrix_free(&m);
        return NULL;
    }

    return m;
}

Matrix *matrix_new_with_array(size_t row, size_t col, const char *name, void *array)
{
    Matrix *m = matrix_new(row, col, name);
    if (m == NULL) {
        ERROR("new_matrix_with_2D_array: new_matrix failed\n");
        return NULL;
    }
    memcpy(m->data, array, row * col * sizeof(double));
    return m;
}

Matrix *matrix_load_from_file(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file) {
        ERROR("file doesn't exist\n");
        return NULL;
    }

    int row = 0, col = 0;
    fscanf(file, "%d %d", &row, &col);

    if (row <= 0 || col <= 0 || row > MAX_ROW || col > MAX_COL) {
        ERROR("Invalid matrix dimentsions: %d x %d\n", row, col);
        fclose(file);
        return NULL;
    }

    char name[MAX_NAME_LEN];
    int len = file_path_to_name(filepath, name, sizeof name);
    if (len < 0) {
        ERROR("Failed to get matrix name: %s\n", filepath);
        fclose(file);
        return NULL;
    }

    Matrix *m = matrix_new(row, col, name);
    if (!m) {
        ERROR("Failed to new matrix\n");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < row * col; i++) {
        fscanf(file, "%lg", &m->data[i]);
    }

    return m;
}

Matrix *matrix_multiply(Matrix *m1, Matrix *m2)
{
    // check the row, col size
    if (m1->col != m2->row) {
        ERROR("multiply: m1(%zu x %zu) m2(%zu x %zu) size mismatch\n", m1->row, m1->col, m2->row, m2->col);
        return NULL;
    }

    if (m1->name == NULL || m2->name == NULL) {
        ERROR("multiply: NULL name detected\n");
        return NULL;
    }

    char *name = concat(m1->name, m2->name);
    Matrix *m3 = matrix_new(m1->row, m2->col, name);
    free(name); // IMPORTANT: free name before return

    int n = m1->col;

    // sum += A[row][k] * B[k][col]; try to flat array
    double *f1 = m1->data;
    double *f2 = m2->data;
    double *f3 = m3->data;
    for (size_t row = 0; row < m3->row; row++) {
        for (size_t col = 0; col < m3->col; col++) {
            double sum = 0;
            for (int k = 0; k < n; k++) {
                sum += f1[row * m1->col + k] * f2[k * m2->col + col];
            }
            f3[row * m3->col + col] = sum;
        }
    }

    return m3;
}

void matrix_print(const Matrix *m)
{
    if (!m) {
        printf("(null matrix)\n");
        return;
    }
    if (m->name) {
        printf("%s =\n", m->name);
    } else {
        printf("NULL =\n");
    }
    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->col; j++) {
            printf("%g ", m->data[i * m->col + j]);
        }
        printf("\n");
    }
}

// free matrix and make the pointer to null
void matrix_free(Matrix **m_ptr)
{
    if (!m_ptr || !*m_ptr) {
        return;
    }
    Matrix *m = *m_ptr;
    if (m->data) {
        free(m->data);
    }
    if (m->name) {
        free(m->name);
    }
    free(m);
    *m_ptr = NULL;
}

static char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// Returns length of name written (excluding \0), or -1 on error/truncation
int file_path_to_name(const char *filepath, char *out_buf, size_t bufsize)
{
    if (!filepath || !out_buf || bufsize == 0) {
        ERROR("Invalid arguments\n");
        return -1;
    }

    const char *basename = strrchr(filepath, '/');
    basename = basename ? basename + 1 : filepath;

    const char *last_dot = strrchr(basename, '.');
    size_t name_len = last_dot && last_dot > basename
        ? (size_t)(last_dot - basename)
        : strlen(basename);

    if (name_len >= bufsize) {
        ERROR("Buffer too small");
        return -1;
    }

    strncpy(out_buf, basename, name_len);
    out_buf[name_len] = '\0';

    return (int)name_len;
}

// check the files exist
bool all_input_files_exist(char *filepaths[], int count)
{
    for (int i = 0; i < count; i++) {
        FILE *file = fopen(filepaths[i], "r");
        if (!file) {
            return false;
        }
        fclose(file);
    }
    return true;
}
