#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#define ERROR(format, ...) fprintf(stderr, "Error: " format, ##__VA_ARGS__)
#endif
