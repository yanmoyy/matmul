#include "matrix.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>

static size_t get_element_size(m_type_t type);
static const char *type_to_string(m_type_t type);
static char *concat(const char *s1, const char *s2);

Matrix *new_matrix(size_t row, size_t col, m_type_t type, const char *name)
{
    if (row <= 0 || col <= 0) {
        ERROR("new_matrix: Invalid dimensions\n");
        return NULL;
    }

    Matrix *m = malloc(sizeof(Matrix));
    if (!m) {
        ERROR("Failed to allocate Matrix struct\n");
        return NULL;
    }

    m->row = row;
    m->col = col;
    m->type = type;

    if (name && *name != '\0') {
        m->name = strdup(name);
        if (!m->name) {
            ERROR("Failed to duplicate matrix name\n");
            free(m);
            return NULL;
        }
    } else {
        m->name = NULL;
    }

    size_t elem_size = get_element_size(type);
    if (elem_size == 0) {
        ERROR("new_matrix: Invalid type\n");
        goto cleanup;
    }

    size_t total_bytes = elem_size * row * col;
    if (total_bytes / elem_size != row * col) {
        ERROR("new_matrix: potential overflow\n");
        goto cleanup;
    }

    m->data = calloc(row * col, elem_size);
    if (!m->data) {
        ERROR("Failed to allocate memory\n");
        goto cleanup;
    }

    return m;
cleanup:
    free_matrix(&m);
    return NULL;
}

Matrix *new_matrix_with_2D_array(size_t row, size_t col, m_type_t type, const char *name, void *array)
{
    Matrix *m = new_matrix(row, col, type, name);
    if (m == NULL) {
        ERROR("new_matrix_with_2D_array: new_matrix failed\n");
        return NULL;
    }
    memcpy(m->data, array, row * col * get_element_size(type));
    return m;
}

// Returns number of characters written (excluding null terminator),
// or negative value on error / truncation
int matrix_to_string(const Matrix *m, char *buf, size_t bufsize)
{
    if (!m || !buf || bufsize == 0) {
        return -1;
    }

    char *p = buf;
    char *end = buf + bufsize;
    int total = 0;

// Helper to append safely
#define APPEND(fmt, ...)                                  \
    do {                                                  \
        int n = snprintf(p, end - p, fmt, ##__VA_ARGS__); \
        if (n < 0 || p + n >= end)                        \
            return -total - 1; /* truncation */           \
        p += n;                                           \
        total += n;                                       \
    } while (0)

    const char *type_str;
    switch (m->type) {
    case M_TYPE_INT:
        type_str = "int";
        break;
    case M_TYPE_FLOAT:
        type_str = "float";
        break;
    case M_TYPE_DOUBLE:
        type_str = "double";
        break;
    default:
        type_str = "unknown";
        break;
    }

    APPEND("Matrix (%s): %zu × %zu, type = %s\n",
        m->name ? m->name : "NULL",
        m->row, m->col, type_str);

    if (!m->data || m->row == 0 || m->col == 0) {
        APPEND(" (empty)\n");
        return total;
    }

    APPEND("Data:\n");

    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->col; j++) {
            size_t idx = i * m->col + j;
            switch (m->type) {
            case M_TYPE_INT:
                APPEND("%8d ", ((const int *)m->data)[idx]);
                break;
            case M_TYPE_FLOAT:
                APPEND("%10.4f ", ((const float *)m->data)[idx]);
                break;
            case M_TYPE_DOUBLE:
                APPEND("%12.6f ", ((const double *)m->data)[idx]);
                break;
            default:
                APPEND("<?> ");
                break;
            }
        }
        APPEND("\n");
    }

    return total;
}

Matrix *matrix_multiply(Matrix *m1, Matrix *m2)
{
    // check the row, col size
    if (m1->col != m2->row) {
        ERROR("multiply: m1(%zu x %zu) m2(%zu x %zu) size mismatch\n", m1->row, m1->col, m2->row, m2->col);
        return NULL;
    }

    if (m1->type != m2->type) {
        ERROR("multiply: m1(%s) m2(%s) type mismatch\n", type_to_string(m1->type), type_to_string(m2->type));
        return NULL;
    }

    char *name = concat(m1->name, m2->name);
    Matrix *m3 = new_matrix(m1->row, m2->col, m1->type, name);
    free(name); // IMPORTANT: free name before return

    int n = m1->col;

    // sum += A[row][k] * B[k][col]; try to flat array
#define MULTIPLY(type)                                            \
    do {                                                          \
        const type *f1 = m1->data;                                \
        const type *f2 = m2->data;                                \
        type *f3 = m3->data;                                      \
        type sum = 0;                                             \
        for (int k = 0; k < n; k++) {                             \
            sum += f1[row * m1->col + k] * f2[k * m2->col + col]; \
        }                                                         \
        f3[row * m3->col + col] = sum;                            \
    } while (0)

    for (size_t row = 0; row < m3->row; row++) {
        for (size_t col = 0; col < m3->col; col++) {
            switch (m3->type) {
            case M_TYPE_INT:
                MULTIPLY(int);
                break;
            case M_TYPE_FLOAT:
                MULTIPLY(float);
                break;
            case M_TYPE_DOUBLE:
                MULTIPLY(double);
                break;
            default:
                ERROR("multiply: wrong type\n");
                return NULL;
            }
        }
    }

#undef MULTIPLY
    return m3;
}

void print_matrix(const Matrix *m)
{
    if (!m) {
        printf("(null matrix)\n");
        return;
    }

    char buf[8192]; // adjust size based on your needs
    int len = matrix_to_string(m, buf, sizeof buf);

    if (len >= 0) {
        fputs(buf, stdout);
    } else {
        printf("[matrix output truncated or error (%d)]\n", len);
        printf("Matrix: %zu × %zu, type = ?\n", m->row, m->col);
    }
}

// free matrix and make the pointer to null
void free_matrix(Matrix **m_ptr)
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

static size_t get_element_size(m_type_t type)
{
    switch (type) {
    case M_TYPE_INT:
        return sizeof(int);
    case M_TYPE_FLOAT:
        return sizeof(float);
    case M_TYPE_DOUBLE:
        return sizeof(double);
    default:
        return 0;
    }
}

static const char *type_to_string(m_type_t type)
{
    switch (type) {
    case M_TYPE_INT:
        return "int";
    case M_TYPE_FLOAT:
        return "float";
    case M_TYPE_DOUBLE:
        return "double";
    default:
        return "none";
    }
}

static char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// Returns length of name written (excluding \0), or -1 on error/truncation
int get_matrix_name(const char *filepath, char *out_buf, size_t bufsize)
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
        return -1; // buffer too small
    }

    strncpy(out_buf, basename, name_len);
    out_buf[name_len] = '\0';
    return (int)name_len;
}
