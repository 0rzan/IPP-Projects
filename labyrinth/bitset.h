#ifndef BITSET_H
#define BITSET_H

#include <stddef.h>
#include <stdbool.h>

typedef struct bitset{
    size_t *array;
    size_t length;
} bitset;

void bitset_init(bitset *, size_t);
bool bitset_check(bitset *, size_t);
void bitset_change(bitset *, size_t, bool);
void bitset_free(bitset *);

#endif