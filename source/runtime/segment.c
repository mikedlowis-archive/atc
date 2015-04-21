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

bool segment_empty(segment_t* seg) {
    return (UINT16_MAX == seg->blockmap[0]);
}

static void* alloc_block(segment_t* seg, uintptr_t root_idx, uintptr_t submap_idx) {
    void* object = NULL;
    uintptr_t submap_mask = (1u << submap_idx);
    if (0u != (seg->blockmap[root_idx + 1] & submap_mask)) {
        /* Get the object's pointer */
        uintptr_t offset = seg->blocksize * ((root_idx * 16u) + submap_idx);
        object = seg->start + offset;
        /* Mark it as allocated */
        seg->blockmap[root_idx + 1] &= ~(submap_mask);
        if (0u == seg->blockmap[root_idx + 1])
            seg->blockmap[0] &= ~(1u << root_idx);
    }
    return object;
}

void* segment_alloc(segment_t* seg) {
    void* obj = NULL;
    /* Check if the segment has any free blocks */
    if (!segment_full(seg)) {
        /* Find the free block */
        uint8_t root_idx   = get_free_index(seg->blockmap[0]);
        uint8_t submap_idx = get_free_index(seg->blockmap[root_idx + 1]);
        /* Allocate and return its address */
        obj = alloc_block(seg, root_idx, submap_idx);
    }
    return obj;
}

void segment_clear_map(segment_t* seg)
{
    while (NULL != seg) {
        memset(seg->blockmap, 0xFFu, sizeof(seg->blockmap));
        seg = seg->next;
    }
}

void* segment_find_and_mark(segment_t* seg, uintptr_t addr) {
    void* obj = NULL;
    uintptr_t start = (uintptr_t)&(seg->start[0]);
    uintptr_t end   = (uintptr_t)seg->end;
    if ((start <= addr) && (addr < end)) {
        uintptr_t block_num  = (addr - start) / (seg->blocksize * sizeof(uintptr_t));
        uintptr_t root_idx   = block_num / 16u;
        uintptr_t submap_idx = block_num % 16u;
        obj = alloc_block(seg, root_idx, submap_idx);
    }
    return obj;
}

