#include "compare.h"
#include "vector.h"
#include <stdbool.h>

typedef struct heap heap;

/**
 * @brief Constructs an empty heap. 
 */
heap *heap_create(compare comparator);

/**
 * @brief Constructs a heap with the vector's elements. The vector must not be 
 * modified as the heap will maintain shallow references to each vector element.
 * Runs in O(n) time.
 */
heap *heap_from_vector(compare comparator, vector *elements);

/**
 * @brief Heap destructor.
 */
void heap_destroy(heap *h);

/**
 * Returns, but does not remove, the element with highest priority. 
 * Runs in O(1) time.
 *
 * @return The highest priority element in the entire heap.
 */
void* heap_top(heap *h);

/**
 * Removes the element with highest priority according to the comparator() 
 * functor. Runs in O(log n) time.
 *
 * @return The element with highest priority in the heap.
 */
void* heap_pop(heap *h);

/**
 * Inserts the given element into the heap, restoring the heap property after 
 * the insert as appropriate. Runs in O(log n) time.
 *
 * @param elem The element to be inserted.
 */
void heap_push(heap *h, void *elem);

/**
 * Determines if the given heap is empty.
 *
 * @return Whether or not there are elements in the heap.
 */
bool heap_is_empty(heap *h);

/**
 * @brief Gets the the elements of the heap as a SHALLOW vector. 
 * Runs in O(n) time.
 */
vector* heap_to_shallow_vector(heap *h);