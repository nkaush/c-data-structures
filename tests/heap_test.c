#include "heap.h"

#include <stdlib.h>
#include <stdio.h>

#if defined(__APPLE__) && defined(DEBUG)
#include <unistd.h>
void check_leaks(void) {
    char cmd[100];
    sprintf(cmd, "leaks --list %d", getpid());
    printf("%s\n", cmd);
    system(cmd);
}
#endif

int main(int argc, char **argv) {
    (void) argc;
#if defined(__APPLE__) && defined(DEBUG)
    atexit(check_leaks);
#endif

    heap *h = heap_create(int_compare);

    for (int i = 20; i > 0; --i) {
        int *p = malloc(sizeof(int));
        *p = i;

        heap_push(h, p);
    }

    for (int i = 1; i <= 5; ++i) {
        int *p = heap_pop(h);
        printf("%d\n", *p);
        free(p);
    }

    for (int i = 40; i > 20; --i) {
        int *p = malloc(sizeof(int));
        *p = i;

        heap_push(h, p);
    }

    while ( !heap_is_empty(h) ) {
        int *p = heap_pop(h);
        printf("%d\n", *p);
        free(p);
    }

    heap_destroy(h);

    return 0;
}