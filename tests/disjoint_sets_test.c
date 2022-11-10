#include "disjoint_sets.h"
#include "testlib.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

TEST(test_contructor, {
    disjoint_sets* ds = disjoint_sets_create(5);
    for (size_t i = 0; i < 5; ++i)
        ASSERT( disjoint_sets_find(ds, i) == i );

    disjoint_sets_destroy(ds);
})

TEST(test_find_and_union, {
    disjoint_sets* ds = disjoint_sets_create(4);
    disjoint_sets_union(ds, 1, 2);

    ASSERT( disjoint_sets_find(ds, 1) == disjoint_sets_find(ds, 2) );

    disjoint_sets_destroy(ds);
})

TEST(test_find_and_union2, {
    disjoint_sets* ds = disjoint_sets_create(10);
    disjoint_sets_union(ds, 4, 8);
    disjoint_sets_union(ds, 1, 3);
    disjoint_sets_union(ds, 1, 5);
    disjoint_sets_union(ds, 8, 3);
    disjoint_sets_union(ds, 6, 7);
    disjoint_sets_union(ds, 2, 8);
    disjoint_sets_union(ds, 7, 1);

    size_t root = disjoint_sets_find(ds, 1);
    for (size_t i = 2; i <= 8; ++i) 
        ASSERT( root == disjoint_sets_find(ds, i) );

    ASSERT( disjoint_sets_find(ds, 0) != disjoint_sets_find(ds, 9) );

    disjoint_sets_destroy(ds);
})

TEST(test_find_and_union3, {
    disjoint_sets* ds = disjoint_sets_create(8);
    disjoint_sets_union(ds, 1, 2);
    disjoint_sets_union(ds, 1, 3);
    disjoint_sets_union(ds, 4, 5);
    disjoint_sets_union(ds, 4, 6);
    disjoint_sets_union(ds, 3, 6);

    ASSERT( disjoint_sets_find(ds, 4) == disjoint_sets_find(ds, 1) );
    ASSERT( disjoint_sets_find(ds, 0) != disjoint_sets_find(ds, 4) );

    disjoint_sets_destroy(ds);
})

TEST(test_size, {
    disjoint_sets* ds = disjoint_sets_create(4);

    for (size_t i = 0; i < 4; ++i)
        ASSERT( disjoint_sets_size(ds, i) == 1 );

    disjoint_sets_destroy(ds);
})

TEST(test_size2, {
    disjoint_sets* ds = disjoint_sets_create(8);

    disjoint_sets_union(ds, 1, 7);
	disjoint_sets_union(ds, 2, 6);
	disjoint_sets_union(ds, 3, 5);
	disjoint_sets_union(ds, 2, 3);

    ASSERT( disjoint_sets_size(ds, 4) == 1 );

    disjoint_sets_union(ds, 1, 3);
    ASSERT( disjoint_sets_size(ds, 6) == 6 );

    disjoint_sets_destroy(ds);
})

#if defined(__APPLE__) && defined(DEBUG)
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
    #define NUM_TESTS 6

    int (*tests[NUM_TESTS])(void) = {
        test_contructor,
        test_find_and_union,
        test_find_and_union2,
        test_find_and_union3,
        test_size,
        test_size2
    };

    printf("Running %d test cases from %s...\n\n", NUM_TESTS, argv[0]);

    int return_code = 0;
    for (int i = 0; i < NUM_TESTS; ++i) {
        return_code += tests[i]();
    }

    printf("\n");

    return return_code;
}
