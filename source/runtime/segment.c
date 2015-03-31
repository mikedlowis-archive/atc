/**
  @file segment.c
  @brief See header for details
*/
#include "segment.h"
#include <stdlib.h>
#include <string.h>

static uint8_t get_free_index(int32_t bitmap) {
    static const uint8_t lookup[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    return lookup[((uint32_t)((bitmap & -bitmap) * 0x077CB531U)) >> 27];
}

segment_t* segment_create(uintptr_t blocksize)
{
    segment_t* seg = (segment_t*)malloc(sizeof(segment_t));
    seg->blocksize = blocksize;
    seg->start = (uintptr_t*)malloc(sizeof(uintptr_t) * blocksize * NUM_BLOCKS);
    seg->end = (seg->start + (blocksize * NUM_BLOCKS));
    memset(seg->blockmap, 0xFFu, sizeof(seg->blockmap));
    return seg;
}

void segment_destroy(segment_t* seg)
{
    free(seg->start);
    free(seg);
}

void* segment_alloc(segment_t* seg) {
    void* obj = NULL;
    /* Check if the segment has any free blocks */
    if (0u != seg->blockmap[0]) {
        /* Find the free block */
        uint8_t root_idx   = get_free_index(seg->blockmap[0]);
        uint8_t submap_idx = get_free_index(seg->blockmap[root_idx + 1]);
        /* Calculate it's address */
        uintptr_t offset = seg->blocksize * ((root_idx * 16u) + submap_idx);
        obj = seg->start + offset;
        /* Mark it allocated */
        seg->blockmap[root_idx + 1] &= ~(1u << submap_idx);
        if (0u == seg->blockmap[root_idx + 1])
            seg->blockmap[0] &= ~(1u << root_idx);
    }
    return obj;
}
