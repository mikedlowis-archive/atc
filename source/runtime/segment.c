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

segment_t* segment_create(uintptr_t num_slots, segment_t* next)
{
    size_t segsize = sizeof(uintptr_t) * num_slots * NUM_BLOCKS;
    segment_t* seg = (segment_t*)malloc(sizeof(segment_t) + segsize);
    seg->next = next;
    seg->blocksize = num_slots;
    seg->end = (seg->start + (num_slots * NUM_BLOCKS));
    segment_clear_map(seg);
    return seg;
}

void segment_destroy(segment_t* seg)
{
    segment_t* curr = seg;
    while (NULL != curr) {
        segment_t* deadite = curr;
        curr = curr->next;
        free(deadite);
    }
}

bool segment_full(segment_t* seg) {
    return (0u == seg->blockmap[0]);
}

void* segment_alloc(segment_t* seg) {
    void* obj = NULL;
    /* Check if the segment has any free blocks */
    if (!segment_full(seg)) {
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

void segment_clear_map(segment_t* seg)
{
    memset(seg->blockmap, 0xFFu, sizeof(seg->blockmap));
}

