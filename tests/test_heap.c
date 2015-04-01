#include "atf.h"
#include "heap.h"

TEST_SUITE(Heap) {
    /* Verify: heap_create
     *************************************************************************/
    TEST(Verify_Create_allocates_and_initializes_a_heap) {
        heap_t* heap = heap_create();
        CHECK(heap != NULL);

        CHECK(NULL != heap_allocate(heap, 63));
        heap_destroy(heap);
    }

    /* Verify: segment_alloc
     *************************************************************************/
}
