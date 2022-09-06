#include "graph.h"
#include "testlib.h"
#include <stdio.h>

graph* string_graph_create(void) {
    return graph_create(
        string_hash_function, string_compare, 
        string_copy_constructor, string_destructor,
        string_copy_constructor, string_destructor, 
        string_copy_constructor, string_destructor
    );
}

TEST(test_graph_adjacent, {
    graph* g = string_graph_create();

    graph_add_vertex(g, "a");
    graph_add_vertex(g, "b");
    graph_add_vertex(g, "c");

    graph_add_edge(g, "a", "b");
    graph_add_edge(g, "b", "c");

    ASSERT( graph_adjacent(g, "a", "b") );
    ASSERT( graph_adjacent(g, "b", "c") );

    ASSERT( !graph_adjacent(g, "a", "c") );
    ASSERT( !graph_adjacent(g, "b", "a") );
    ASSERT( !graph_adjacent(g, "c", "a") );
    ASSERT( !graph_adjacent(g, "c", "b") );

    graph_remove_vertex(g, "b");

    ASSERT( !graph_adjacent(g, "a", "b") );
    ASSERT( !graph_adjacent(g, "b", "c") );
    ASSERT( !graph_adjacent(g, "a", "c") );
    ASSERT( !graph_adjacent(g, "b", "a") );
    ASSERT( !graph_adjacent(g, "c", "a") );
    ASSERT( !graph_adjacent(g, "c", "b") );

    graph_destroy(g);
})

TEST(test_graph_neighbors, {
    graph* g = string_graph_create();

    graph_add_vertex(g, "a");
    graph_add_vertex(g, "b");
    graph_add_vertex(g, "c");

    graph_add_edge(g, "a", "b");
    graph_add_edge(g, "a", "c");
    graph_add_edge(g, "b", "c");

    vector* an = graph_neighbors(g, "a");
    vector* bn = graph_neighbors(g, "b");
    vector* cn = graph_neighbors(g, "c");

    ASSERT( vector_size(an) == 2 );
    VECTOR_FOR_EACH(an, n, {
        ASSERT( !strcmp((char*) n, "b") || !strcmp((char*) n, "c") );
    });

    ASSERT( vector_size(bn) == 1 );
    VECTOR_FOR_EACH(bn, n, {
        ASSERT( !strcmp((char*) n, "c") );
    });

    ASSERT( vector_size(cn) == 0 );

    vector_destroy(an);
    vector_destroy(bn);
    vector_destroy(cn);
    graph_destroy(g);
})

TEST(test_graph_antineighbors, {

})

TEST(test_graph_vertex_degree, {

})

TEST(test_graph_vertex_antidegree, {

})

TEST(test_graph_vertices, {

})

TEST(test_graph_vertex_count, {

})

TEST(test_graph_contains_vertex, {

})

TEST(test_graph_add_vertex, {

})

TEST(test_graph_remove_vertex, {

})

TEST(test_graph_get_vertex_value, {

})

TEST(test_graph_set_vertex_value, {

})

TEST(test_graph_edge_count, {

})

TEST(test_graph_add_edge, {

})

TEST(test_graph_remove_edge, {

})

TEST(test_graph_get_edge_value, {

})

TEST(test_graph_set_edge_value, {

})

#if defined(__APPLE__) && defined(DEBUG)
#include <unistd.h>
void check_leaks(void) {
    char cmd[100];
    sprintf(cmd, "leaks --list %d", getpid());
    printf("%s\n", cmd);
    system(cmd);
}
#endif

int main(int argc, char** argv) {
    (void) argc;
#if defined(__APPLE__) && defined(DEBUG)
    atexit(check_leaks);
#endif
    #define NUM_TESTS 17

    int (*tests[NUM_TESTS])(void) = {
        test_graph_adjacent,
        test_graph_neighbors,
        test_graph_antineighbors,
        test_graph_vertex_degree,
        test_graph_vertex_antidegree,
        test_graph_vertices,
        test_graph_vertex_count,
        test_graph_contains_vertex,
        test_graph_add_vertex,
        test_graph_remove_vertex,
        test_graph_get_vertex_value,
        test_graph_set_vertex_value,
        test_graph_edge_count,
        test_graph_add_edge,
        test_graph_remove_edge,
        test_graph_get_edge_value,
        test_graph_set_edge_value
    };

    printf("Running %d test cases from %s...\n\n", NUM_TESTS, argv[0]);

    int return_code = 0;
    for (int i = 0; i < NUM_TESTS; ++i) {
        return_code += tests[i]();
    }

    printf("\n");

    return return_code;
}
