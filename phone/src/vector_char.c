#include "vector_char.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define INIT_CAPACITY 16

bool vector_init(vector_char *v) {
    v->items = malloc(sizeof(char) * INIT_CAPACITY);
    if (v->items == NULL) {
        return false;
    }
    v->length = 0;
    v->capacity = INIT_CAPACITY;
    return true;
}

void vector_free(vector_char *v) {
    free(v->items);
}

bool vector_add(vector_char *v, char x) {
    if (v->length == v->capacity) {
        if (v->capacity == SIZE_MAX/sizeof(char)) {
            return false;
        }
        if (v->capacity > SIZE_MAX / (2 * sizeof(char))) {
            v->capacity = SIZE_MAX/sizeof(char);
        }
        v->capacity *= 2;
        char *newitems;
        newitems = realloc(v->items, sizeof(char) * v->capacity);
        if (newitems == NULL) {
            return false;
        }
        v->items = newitems;
    }
    v->items[v->length] = x;
    v->length++;
    return true;
}

bool vector_is_empty(vector_char *v) {
    if (v->length == 0)
        return true;
    else 
        return false;
}