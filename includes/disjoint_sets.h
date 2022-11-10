#pragma once
#include <sys/types.h>

/**
 * Struct representing a disjoint set
 */
typedef struct disjoint_sets disjoint_sets;

disjoint_sets* disjoint_sets_create(size_t size);

void disjoint_sets_destroy(disjoint_sets* ds);

size_t disjoint_sets_find(disjoint_sets* ds, size_t elem);

void disjoint_sets_union(disjoint_sets* ds, size_t a, size_t b);

size_t disjoint_sets_size(disjoint_sets* ds, size_t elem);

ssize_t disjoint_sets_findroot(disjoint_sets* ds);