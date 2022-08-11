#pragma once
#include <stdio.h>

#define BOLDGREEN "\033[1m\033[32m"
#define BOLDRED   "\033[1m\033[31m"
#define RESET     "\033[0m"

#define TEST(__testname__, __testcode__)            \
    static int __testname__(void) {                 \
        int __assertions_failed__ = 0;              \
        int __num_assertions__ = 0;                 \
        printf("%s ... ", #__testname__ );          \
        __testcode__;                               \
        if ( !__assertions_failed__ ) {             \
            printf(BOLDGREEN"PASSED"RESET"\n");     \
        } else {                                    \
            printf("\t(%d/%d cases passed)\n",      \
                __num_assertions__ - __assertions_failed__, \
                __num_assertions__);                \
        }                                           \
        return __assertions_failed__;               \
    }

#define ASSERT(__condition__)                                    \
    ++__num_assertions__;                                        \
    if (!(__condition__)) {                                      \
        if ( !__assertions_failed__ ) {                          \
            printf(BOLDRED"FAILED\n"RESET);                      \
        }                                                        \
        printf("\t"BOLDRED"FAILED"RESET" %s\n", #__condition__); \
        ++__assertions_failed__;                                 \
    }
