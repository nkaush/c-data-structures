#include "disjoint_sets.h"
#include <stdlib.h>
#include <string.h>

struct disjoint_sets {
    ssize_t* elements;
    size_t num_elems;
};

disjoint_sets* disjoint_sets_create(size_t size) {
    disjoint_sets* ds = malloc(sizeof(disjoint_sets));
    ds->elements = calloc(size, sizeof(ssize_t));
    memset(ds->elements, -1, size * sizeof(ssize_t));
    ds->num_elems = size; 
    return ds;
}

void disjoint_sets_destroy(disjoint_sets* ds) {
    free(ds->elements);
    free(ds);
}

size_t disjoint_sets_find(disjoint_sets* ds, size_t elem) {
    if (ds->elements[elem] < 0) {
        return elem;
    }

    ds->elements[elem] = disjoint_sets_find(ds, ds->elements[elem]);
    return ds->elements[elem];
}

void disjoint_sets_union(disjoint_sets* ds, size_t a, size_t b) {
    size_t root1 = disjoint_sets_find(ds, a);
    size_t root2 = disjoint_sets_find(ds, b);
    ssize_t new_size = ds->elements[root1] + ds->elements[root2];
    
    // If elements[root1] is less than (more negative), it is the
    // larger set; we union the smaller set, root2, with root1.
    if ( ds->elements[root1] < ds->elements[root2] ) {
        ds->elements[root2] = root1;
        ds->elements[root1] = new_size;
    } else { // Otherwise, do the opposite:
        ds->elements[root1] = root2;
        ds->elements[root2] = new_size;
    }
}

size_t disjoint_sets_size(disjoint_sets* ds, size_t elem) {
    return -1 * ds->elements[disjoint_sets_find(ds, elem)];
}

ssize_t disjoint_sets_findroot(disjoint_sets* ds) {
    for (size_t i = 0; i < ds->num_elems; ++i) {
        if (ds->elements[i] < 0) return i;
    }

    return -1;
}