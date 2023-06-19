#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct vector {
    size_t *items;
    size_t length;
    size_t capacity;
} vector;

void vector_init(vector *);
void vector_add(vector *, size_t);
size_t vector_pop(vector *);
void vector_free(vector *);
bool vector_is_empty(vector *);

#endif