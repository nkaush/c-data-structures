#include "graph.h"
#include "dictionary.h"
#include "set.h"

typedef struct _graph_callbacks {
    hash_function_type vertex_hash_function;
    compare vertex_comp;
    copy_constructor_type vertex_key_copy_constructor;
    destructor_type vertex_key_destructor;
    copy_constructor_type vertex_value_copy_constructor;
    destructor_type vertex_value_destructor;
    copy_constructor_type edge_value_copy_constructor;
    destructor_type edge_value_destructor;
} graph_callbacks;

/**
 * We can iterate through the adjacent edges by iterating through the dictionary
 * of edges since the dictionary is indexed by the edge keys of edges 
 * neighboring this vertex.
 */
typedef struct _vertex {
    void* value;
    set* antineighbors;
    dictionary* outbound_edges; // dictionary of vertex key -> edge value
    destructor_type vertex_value_destructor;
} vertex;

void* vertex_create(void* ptr) {
    graph_callbacks* cbs = (graph_callbacks*) ptr;
    vertex* v = malloc(sizeof(vertex));
    v->vertex_value_destructor = cbs->vertex_value_destructor;
    v->value = NULL;

    v->antineighbors = set_create(
        cbs->vertex_hash_function, cbs->vertex_comp,
        cbs->vertex_key_copy_constructor, cbs->vertex_key_destructor
    );

    v->outbound_edges = dictionary_create(
        cbs->vertex_hash_function, cbs->vertex_comp, 
        cbs->vertex_key_copy_constructor, cbs->vertex_key_destructor, 
        cbs->edge_value_copy_constructor, cbs->edge_value_destructor
    );

    return (void*) v;
}

void vertex_destroy(void* ptr) {
    vertex* v = (vertex*) ptr;

    if ( v->value )
        v->vertex_value_destructor(v->value);

    dictionary_destroy(v->outbound_edges);
    set_destroy(v->antineighbors);
    free(v);
}

struct graph {
    dictionary* vertices; // dictionary of vertex key -> vertex value
    size_t edge_count;
    graph_callbacks callbacks; // callbacks function pointers
};

// Member Functions:

graph* graph_create(hash_function_type vertex_hash_function,
                    compare vertex_comp,

                    copy_constructor_type vertex_key_copy_constructor,
                    destructor_type vertex_key_destructor,

                    copy_constructor_type vertex_value_copy_constructor,
                    destructor_type vertex_value_destructor,

                    copy_constructor_type edge_value_copy_constructor,
                    destructor_type edge_value_destructor) {
    graph* g = malloc(sizeof(graph));
    g->vertices = dictionary_create(
        vertex_hash_function, vertex_comp, vertex_key_copy_constructor, 
        vertex_key_destructor, vertex_create, vertex_destroy
    );

    g->callbacks.vertex_hash_function = vertex_hash_function;
    g->callbacks.vertex_comp = vertex_comp;
    g->callbacks.vertex_key_copy_constructor = vertex_key_copy_constructor;
    g->callbacks.vertex_key_destructor = vertex_key_destructor;
    g->callbacks.vertex_value_copy_constructor = vertex_value_copy_constructor;
    g->callbacks.vertex_value_destructor = vertex_value_destructor;
    g->callbacks.edge_value_copy_constructor = edge_value_copy_constructor;
    g->callbacks.edge_value_destructor = edge_value_destructor;

    return g;
}

void graph_destroy(graph* this) {
    dictionary_destroy(this->vertices);
    free(this);
}

// Graph Connectivity Functions:

bool graph_adjacent(graph* this, void* s, void* t) {
    vertex* v = dictionary_get(this->vertices, s);

    if ( !v ) return false;

    return dictionary_contains(v->outbound_edges, t);
}

vector* graph_neighbors(graph* this, void* s) {
    vertex* v = dictionary_get(this->vertices, s);

    if ( !v ) return shallow_vector_create();

    return dictionary_keys(v->outbound_edges);;
}

vector* graph_antineighbors(graph* this, void* t) {
    vertex* v = dictionary_get(this->vertices, t);

    if ( !v ) return shallow_vector_create();

    return set_elements(v->antineighbors);
}

size_t graph_vertex_degree(graph* this, void* s) {
    vertex* v = dictionary_get(this->vertices, s);

    if ( !v ) return 0;
    
    return dictionary_size(v->outbound_edges);
}

size_t graph_vertex_antidegree(graph* this, void* t) {
    vertex* v = dictionary_get(this->vertices, t);

    if ( !v ) return 0;
    
    return set_cardinality(v->antineighbors);
}

// Vertex Functions:

vector* graph_vertices(graph* this) {
    return dictionary_keys(this->vertices);
}

size_t graph_vertex_count(graph* this) {
    return dictionary_size(this->vertices);
}

bool graph_contains_vertex(graph* this, void* key) {
    return dictionary_contains(this->vertices, key);
}

void graph_add_vertex(graph* this, void* key) {
    if ( !graph_contains_vertex(this, key) )
        dictionary_set(this->vertices, key, &this->callbacks);
}

void graph_remove_vertex(graph* this, void* key) {
    if ( graph_contains_vertex(this, key) ) {
        vertex* v = dictionary_get(this->vertices, key);

        // for each of my antineighbors, remove me from their outbound edges
        SET_FOR_EACH(v->antineighbors, an, {
            vertex* n = dictionary_get(this->vertices, an);
            dictionary_remove(n->outbound_edges, key);
        });

        // for each of my neighbors, remove me from their antineighbors
        vector* neighbors = graph_neighbors(this, key);
        VECTOR_FOR_EACH(neighbors, n, {
            vertex* nv = dictionary_get(this->vertices, n);
            set_remove(nv->antineighbors, key);
        });
        vector_destroy(neighbors);

        dictionary_remove(this->vertices, key);
    }
}

void* graph_get_vertex_value(graph* this, void* key) {
    vertex* v = dictionary_get(this->vertices, key);
    return v ? v->value : NULL;
}

void graph_set_vertex_value(graph* this, void* key, void* value) {
    vertex* v = dictionary_get(this->vertices, key);

    if ( v )
        v->value = this->callbacks.vertex_value_copy_constructor(value);
}

// Edge Functions:

size_t graph_edge_count(graph* this) {
    return this->edge_count;
}

void graph_add_edge(graph* this, void* s, void* t) {
    vertex* vs = dictionary_get(this->vertices, s);
    vertex* vt = dictionary_get(this->vertices, t);

    if ( !vs || !vt ) return;
    
    if ( !dictionary_contains(vs->outbound_edges, t) ) {
        dictionary_set(vs->outbound_edges, t, NULL);
        set_add(vt->antineighbors, s);
        ++this->edge_count;
    }
}

void graph_remove_edge(graph* this, void* s, void* t) {
    vertex* vs = dictionary_get(this->vertices, s);
    vertex* vt = dictionary_get(this->vertices, t);

    if ( !vs || !vt ) return;
    
    if ( dictionary_contains(vs->outbound_edges, t) ) {
        dictionary_remove(vs->outbound_edges, t);
        set_remove(vt->antineighbors, s);
        --this->edge_count;
    }
}

void* graph_get_edge_value(graph* this, void* s, void* t) {
    vertex* v = dictionary_get(this->vertices, s);

    if ( !v ) return NULL;
    
    return dictionary_get(v->outbound_edges, t);
}

void graph_set_edge_value(graph* this, void* s, void* t, void* value) {
    vertex* v = dictionary_get(this->vertices, s);

    if ( !v ) return;
    else if ( !dictionary_contains(v->outbound_edges, t) ) return;
    
    dictionary_set(v->outbound_edges, t, value);
}

graph* shallow_graph_create(void) {
    return graph_create(
        shallow_hash_function, shallow_compare, 
        shallow_copy_constructor, shallow_destructor, 
        shallow_copy_constructor, shallow_destructor, 
        shallow_copy_constructor, shallow_destructor
    );
}
