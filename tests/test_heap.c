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

    /* Verify: heap_start_collection
     *************************************************************************/
    TEST(Verify_start_collection_prepares_the_heap_for_garbage_collection) {
        heap_t* heap = heap_create();
        splaytree_t* blocks = heap->blocks;
        heap_start_collection(heap);
        CHECK(heap->blocks != blocks);
        CHECK(heap->greylist == blocks);
        heap_destroy(heap);
    }

    /* Verify: heap_finish_collection
     *************************************************************************/
    TEST(Verify_finish_collection_cleans_up_after_garbage_collection) {
        heap_t* heap = heap_create();
        splaytree_t* blocks = heap->blocks;
        heap_start_collection(heap);
        heap_finish_collection(heap);
        CHECK(blocks != heap->blocks);
        heap_destroy(heap);
    }

    TEST(Verify_finish_collection_moves_segments_from_the_full_list_to_the_available_list_when_some_blocks_are_reclaimed) {
        heap_t* heap = heap_create();
        segment_t* seg = segment_create(2, NULL);
        seg->blockmap[0] = 42;
        splaytree_t* blocks = heap->blocks;
        heap->heaps[1].full = seg;
        heap_start_collection(heap);
        heap_finish_collection(heap);
        CHECK(blocks != heap->blocks);
        CHECK(heap->heaps[1].available == seg);
        CHECK(heap->heaps[1].full == NULL);
        heap_destroy(heap);
    }

    TEST(Verify_finish_collection_moves_segments_from_the_full_list_to_the_available_list_when_some_blocks_are_reclaimed) {
        heap_t* heap = heap_create();
        segment_t* seg2 = segment_create(2, NULL);
        segment_t* seg1 = segment_create(2, seg2);
        seg1->blockmap[0] = 0;
        seg2->blockmap[0] = 42;
        splaytree_t* blocks = heap->blocks;
        heap->heaps[1].full = seg1;
        heap_start_collection(heap);
        heap_finish_collection(heap);
        CHECK(blocks != heap->blocks);
        CHECK(heap->heaps[1].available == seg2);
        CHECK(heap->heaps[1].full == seg1);
        heap_destroy(heap);
    }

    TEST(Verify_finish_collection_frees_empty_segments_in_the_full_list) {
        heap_t* heap = heap_create();
        segment_t* seg = segment_create(2, NULL);
        splaytree_t* blocks = heap->blocks;
        heap->heaps[1].full = seg;
        heap_start_collection(heap);
        heap_finish_collection(heap);
        CHECK(blocks != heap->blocks);
        CHECK(heap->heaps[1].available == NULL);
        CHECK(heap->heaps[1].full == NULL);
        heap_destroy(heap);
    }

    TEST(Verify_finish_collection_frees_empty_segments_in_the_full_list) {
        heap_t* heap = heap_create();
        segment_t* seg2 = segment_create(2, NULL);
        segment_t* seg1 = segment_create(2, seg2);
        seg1->blockmap[0] = 0;
        splaytree_t* blocks = heap->blocks;
        heap->heaps[1].full = seg1;
        heap_start_collection(heap);
        heap_finish_collection(heap);
        CHECK(blocks != heap->blocks);
        CHECK(heap->heaps[1].available == NULL);
        CHECK(heap->heaps[1].full == seg1);
        heap_destroy(heap);
    }

    TEST(Verify_finish_collection_does_nothing_when_full_list_contains_only_full_segments) {
        heap_t* heap = heap_create();
        segment_t* seg = segment_create(2, NULL);
        seg->blockmap[0] = 0;
        splaytree_t* blocks = heap->blocks;
        heap->heaps[1].full = seg;
        heap_start_collection(heap);
        heap_finish_collection(heap);
        CHECK(blocks != heap->blocks);
        CHECK(heap->heaps[1].available == NULL);
        CHECK(heap->heaps[1].full == seg);
        heap_destroy(heap);
    }
}
