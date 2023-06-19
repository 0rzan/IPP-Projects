#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define INIT_CAPACITY 16

void vector_init(vector *v) {
    v->items = malloc(sizeof(size_t) * INIT_CAPACITY);
    if (v->items == NULL) {
        fprintf(stderr, "ERROR 0\n");
        exit(1);
    }
    v->length = 0;
    v->capacity = INIT_CAPACITY;
}

void vector_free(vector *v) {
    free(v->items);
}

void vector_add(vector *v, size_t x) {
    if (v->length == v->capacity) {
        if (v->capacity == SIZE_MAX/sizeof(size_t)) {
            fprintf(stderr, "ERROR 0\n");
            exit(1);
        }
        if (v->capacity > SIZE_MAX / (2 * sizeof(size_t))) {
            v->capacity = SIZE_MAX/sizeof(size_t);
        }
        v->capacity *= 2;
        v->items = realloc(v->items, sizeof(size_t) * v->capacity);
        if (v->items == NULL) {
            fprintf(stderr, "ERROR 0\n");
            exit(1);
        }
    }
    v->items[v->length] = x;
    v->length++;
}

size_t vector_pop(vector *v) {
    if (v->length > 0) {
        v->length--;
        return v->items[v->length];
    }
}

bool vector_is_empty(vector *v) {
    if (v->length == 0)
        return true;
    else 
        return false;
}

