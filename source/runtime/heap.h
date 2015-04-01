/**
  @file heap.h
  @brief TODO: Describe this file
*/
#ifndef HEAP_H
#define HEAP_H

#include "segment.h"

#define MIN_NUM_SLOTS (2u)

#define MAX_NUM_SLOTS ((sizeof(uintptr_t)*8u) - 1u)

#define HEAP_INDEX_OFFSET (MIN_NUM_SLOTS)

#define NUM_HEAP_STACKS ((sizeof(uintptr_t) * 8u) - 1)

typedef struct segnode_t {
    segment_t* segment;
    struct segnode_t* next;
} segnode_t;

typedef struct {
    uintptr_t bytes_allocated;
    segnode_t* heaps[NUM_HEAP_STACKS];
} heap_t;

heap_t* heap_create(void);

void heap_destroy(heap_t* heap);

void* heap_allocate(heap_t* heap, uintptr_t num_slots);

#endif /* HEAP_H */
