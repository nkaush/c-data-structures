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

int test_graph_neighbors(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_antineighbors(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_vertex_degree(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_vertex_antidegree(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_vertices(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_vertex_count(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_contains_vertex(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_add_vertex(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_remove_vertex(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_get_vertex_value(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_set_vertex_value(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_edge_count(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_add_edge(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_remove_edge(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_get_edge_value(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

int test_graph_set_edge_value(void) {
    int assertions_failed = 0;

    

    return assertions_failed;
}

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
}
