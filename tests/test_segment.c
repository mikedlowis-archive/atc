#include "atf.h"
#include "segment.h"

TEST_SUITE(Segment) {
    /* Verify: segment_create
     *************************************************************************/
    TEST(Verify_Create_allocates_and_initializes_a_segment) {
        unsigned int i;
        segment_t* seg = segment_create(2u, NULL);
        CHECK(seg->blocksize == 2u);
        CHECK(seg->start != NULL);
        CHECK(seg->end == (seg->start + (NUM_BLOCKS * 2u)));
        for (i = 0; i < BLOCKMAP_SIZE; i++)
            CHECK(seg->blockmap[i] == UINT16_MAX);
        segment_destroy(seg);
    }

    /* Verify: segment_alloc
     *************************************************************************/
    TEST(Verify_alloc_allocates_the_first_block) {
        segment_t* seg = segment_create(2u, NULL);
        void* obj = segment_alloc(seg);
        CHECK(obj == seg->start);
        segment_destroy(seg);
    }

    TEST(Verify_alloc_allocates_the_second_block) {
        segment_t* seg = segment_create(2u, NULL);
        seg->blockmap[1] = UINT16_MAX-1;
        void* obj = segment_alloc(seg);
        CHECK(obj == &seg->start[2]);
        segment_destroy(seg);
    }

    TEST(Verify_alloc_allocates_the_sixteenth_block) {
        segment_t* seg = segment_create(2u, NULL);
        seg->blockmap[0] = UINT16_MAX-1;
        seg->blockmap[1] = 0u;
        void* obj = segment_alloc(seg);
        CHECK(obj == &seg->start[16*2]);
        segment_destroy(seg);
    }

    TEST(Verify_alloc_allocates_blocks_consecutively) {
        segment_t* seg = segment_create(2u, NULL);
        void* obj1 = segment_alloc(seg);
        void* obj2 = segment_alloc(seg);
        void* obj3 = segment_alloc(seg);
        CHECK(obj1 == &seg->start[0]);
        CHECK(obj2 == &seg->start[2]);
        CHECK(obj3 == &seg->start[4]);
        CHECK(seg->blockmap[0] == UINT16_MAX);
        CHECK(seg->blockmap[1] == UINT16_MAX-7u);
        segment_destroy(seg);
    }

    TEST(Verify_alloc_allocates_blocks_consecutively_across_bitmap_boundaries) {
        segment_t* seg = segment_create(2u, NULL);
        seg->blockmap[1] = 1 << 15;
        void* obj1 = segment_alloc(seg);
        void* obj2 = segment_alloc(seg);
        CHECK(obj1 == &seg->start[2*15]);
        CHECK(obj2 == &seg->start[2*16]);
        CHECK(seg->blockmap[0] == UINT16_MAX-1);
        CHECK(seg->blockmap[1] == 0u);
        CHECK(seg->blockmap[2] == UINT16_MAX-1);
        segment_destroy(seg);
    }

    TEST(Verify_alloc_returns_NULL_if_no_blocks_available) {
        segment_t* seg = segment_create(2u, NULL);
        seg->blockmap[0] = 0;
        void* obj = segment_alloc(seg);
        CHECK(obj == NULL);
        segment_destroy(seg);
    }
}
