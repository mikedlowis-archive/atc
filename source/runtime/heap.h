/**
  @file heap.h
  @brief TODO: Describe this file
*/
#ifndef HEAP_H
#define HEAP_H

#include "segment.h"

#define NUM_HEAP_STACKS ((sizeof(uintptr_t) * 8u) - 3u)

typedef struct segnode_t {
    segment_t* segment;
    struct segnode_t* next;
} segnode_t;

typedef struct {
    uintptr_t bytes_allocated;
    struct {
        segnode_t* first;
        segnode_t* last;
    } heaps[NUM_HEAP_STACKS];
} heap_t;

heap_t* heap_create(void);

void heap_destroy(heap_t* heap);

void* heap_allocate(size_t block_size);

#endif /* HEAP_H */
