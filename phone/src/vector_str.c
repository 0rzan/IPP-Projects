#include "vector_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define INIT_CAPACITY 16

bool vector_str_init(vector_str *v) {
    v->items = malloc(sizeof(char *) * INIT_CAPACITY);
    if (v->items == NULL) {
        return false;
    }
    v->length = 0;
    v->capacity = INIT_CAPACITY;
    return true;
}

void vector_str_free(vector_str *v) {
    for(size_t i = 0; i < v->length; i++) {
        free(v->items[i]);
    }
    free(v->items);
}

bool vector_str_add(vector_str *v, char *x) {
    if (v->length == v->capacity) {
        if (v->capacity == SIZE_MAX/sizeof(char *)) {
            return false;
        }
        if (v->capacity > SIZE_MAX / (2 * sizeof(char *))) {
            v->capacity = SIZE_MAX/sizeof(char *);
        }
        v->capacity *= 2;
        char **newitems;
        newitems = realloc(v->items, sizeof(char *) * v->capacity);
        if (newitems == NULL) {
            return false;
        }
        v->items = newitems;
    }
    v->items[v->length] = x;
    v->length++;
    return true;
}
