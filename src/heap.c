#include "heap.h"
#define ROOT_INDEX 1

#define HEAP_SIZE(__h) vector_size(__h->elements)
#define HEAP_AT(__h, __idx) vector_get(__h->elements, __idx)
#define LEFT_CHILD(__idx) (__idx * 2)
#define RIGHT_CHILD(__idx) ((__idx * 2) + 1)
#define PARENT(__idx) (__idx / 2)
#define HAS_CHILD(__h, __idx) (LEFT_CHILD(__idx) < HEAP_SIZE(h))
#define HIGHER_PRIORITY(__h, __a, __b) (h->comparator(HEAP_AT(h, __a), HEAP_AT(h, __b)) < 0)

struct heap {
    vector* elements;
    compare comparator;
};

size_t heap_max_priority_child(heap *h, size_t idx) {
    size_t left = LEFT_CHILD(idx);
    size_t right = RIGHT_CHILD(idx);

    if ( right == HEAP_SIZE(h) ) {
        return left;
    }

    return HIGHER_PRIORITY(h, left, right) ? left : right;
}

void heap_heapify_down(heap *h, size_t idx) {
    if (HAS_CHILD(h, idx)) {
        size_t priority_child_idx = heap_max_priority_child(h, idx);

        if (priority_child_idx >= HEAP_SIZE(h)) return;

        if (HIGHER_PRIORITY(h, priority_child_idx, idx)) {
            vector_swap(h->elements, idx, priority_child_idx);
            heap_heapify_down(h, priority_child_idx);
        }
    }
}

void heap_heapify_up(heap *h, size_t idx) {
    if (idx != ROOT_INDEX) {
        if (HIGHER_PRIORITY(h, idx, PARENT(idx))) {
            vector_swap(h->elements, idx, PARENT(idx));
            heap_heapify_up(h, PARENT(idx));
        }
    }
}

heap *heap_create(compare comparator) {
    heap *h = malloc(sizeof(heap));
    h->elements = shallow_vector_create();
    h->comparator = comparator;

    vector_push_back(h->elements, NULL);

    return h;
}

heap *heap_from_vector(compare comparator, vector *elements) {
    heap *h = heap_create(comparator);
    vector_push_back(h->elements, NULL);

    VECTOR_FOR_EACH(elements, elem, {
        vector_push_back(h->elements, elem);
    });

    size_t idx_last_non_child = ROOT_INDEX;
    while ( HAS_CHILD(h, idx_last_non_child) ) {
        idx_last_non_child = LEFT_CHILD(idx_last_non_child);
    }

    for (size_t i = idx_last_non_child; i > 0; --i) {
        heap_heapify_down(h, i);
    }

    return h;
}

void heap_destroy(heap *h) {
    vector_destroy(h->elements);
    free(h);
}

void* heap_top(heap *h) {
    return HEAP_AT(h, ROOT_INDEX);
}

void* heap_pop(heap *h) {
    void *min_value = HEAP_AT(h, ROOT_INDEX);

    vector_swap(h->elements, ROOT_INDEX, HEAP_SIZE(h) - 1);
    vector_pop_back(h->elements);
    heap_heapify_down(h, ROOT_INDEX);

    return min_value;
}

void heap_push(heap *h, void *elem) {
    vector_push_back(h->elements, elem);
    heap_heapify_up(h, HEAP_SIZE(h) - 1);
}

bool heap_is_empty(heap *h) {
    return HEAP_SIZE(h) == 1;
}

vector* heap_to_shallow_vector(heap *h) {
    vector *v = shallow_vector_create();
    VECTOR_FOR_EACH(v, e, {
        vector_push_back(v, e);
    });

    return v;
}