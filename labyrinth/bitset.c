#include "bitset.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

void bitset_init(bitset *b, size_t n) {
    b->length = n;
    b->array = calloc(n / (8 * sizeof(size_t)) + 1, sizeof(size_t));
    if (b->array == NULL) {
        fprintf(stderr, "ERROR 0\n");
        exit(1);
    }
}

bool bitset_check(bitset *b, size_t k) {
    size_t ind = k / (8 * sizeof(size_t));
    size_t r = k % (8 * sizeof(size_t));
    return b->array[ind] & ((size_t)1 << r);
}

void bitset_free(bitset *b) {
    free(b->array);
}

void bitset_change(bitset *b, size_t k, bool q) {
    size_t ind = k / (8 * sizeof(size_t));
    size_t r = k % (8 * sizeof(size_t));
    if (q) {
        b->array[ind] |= ((size_t)1 << r);
    }
    else {
        b->array[ind] &= ~((size_t)1 << r);
    }
}

