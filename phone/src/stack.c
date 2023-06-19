#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define INIT_CAPACITY 16

bool stack_init(stack *s) {
    s->items = malloc(sizeof(entry) * INIT_CAPACITY);
    if (s->items == NULL) {
        return false;
    }
    s->length = 0;
    s->capacity = INIT_CAPACITY;
    return true;
}

void stack_free(stack *s) {
    free(s->items);
}

bool stack_add(stack *s, entry x) {
    if (s->length == s->capacity) {
        if (s->capacity == SIZE_MAX/sizeof(entry)) {
            return false;
        }
        if (s->capacity > SIZE_MAX / (2 * sizeof(entry))) {
            s->capacity = SIZE_MAX/sizeof(entry);
        }
        s->capacity *= 2;
        s->items = realloc(s->items, sizeof(entry) * s->capacity);
        if (s->items == NULL) {
            return false;
        }
    }
    s->items[s->length] = x;
    s->length++;
    return true;
}

bool stack_is_empty(stack *s) {
    if (s->length == 0)
        return true;
    else 
        return false;
}