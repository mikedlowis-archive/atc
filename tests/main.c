#include "atf.h"

int ATC_Main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    RUN_EXTERN_TEST_SUITE(SplayTree);
    RUN_EXTERN_TEST_SUITE(Segment);
    return PRINT_TEST_RESULTS();
}
