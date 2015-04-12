#include "atf.h"
#include "heap.h"

#include "gc.h"
TEST_SUITE(Heap) {
    /* Verify: heap_create
     *************************************************************************/
    TEST(Verify_Create_allocates_and_initializes_a_heap) {
        heap_t* heap = heap_create();
        CHECK(heap != NULL);
        CHECK(NULL != heap_allocate(heap, 64));
        CHECK(NULL != heap_allocate(heap, 65));
        heap_destroy(heap);
    }

    /* Verify: heap_alloc
     *************************************************************************/
    TEST(Verify_Allocate_should_allocate_a_new_segment_if_the_subheap_is_empty) {
        heap_t* heap = heap_create();
        CHECK(NULL != heap_allocate(heap, 1));
        CHECK(heap->heaps[0].available != NULL);
        heap_destroy(heap);
    }

    TEST(Verify_Allocate_should_allocate_a_new_segment_if_the_current_segment_is_full) {
        heap_t* heap = heap_create();
        CHECK(NULL != heap_allocate(heap, 1));
        heap->heaps[0].available->blockmap[0] = 1 << ((sizeof(uint16_t) * 8) - 1);
        heap->heaps[0].available->blockmap[16] = 1 << ((sizeof(uint16_t) * 8) - 1);
        CHECK(NULL != heap_allocate(heap, 1));
        CHECK(heap->heaps[0].available == NULL);
        CHECK(heap->heaps[0].full != NULL);
        CHECK(heap->heaps[0].full->next == NULL);
        heap_destroy(heap);
    }

    TEST(Verify_Allocate_should_allocate_the_minimum_size_if_num_slots_is_less_than_the_minimum) {
        heap_t* heap = heap_create();
        CHECK(NULL != heap_allocate(heap, 0));
        CHECK(heap->heaps[0].available != NULL);
        heap_destroy(heap);
    }

    TEST(Verify_Allocate_should_allocate_a_large_block_if_the_number_of_slots_is_greater_than_the_max) {
        heap_t* heap = heap_create();
        CHECK(NULL != heap_allocate(heap, 65));
        CHECK(NULL != heap->blocks->root->value);
        heap_destroy(heap);
    }
}
