/**
  @file heap.h
  @brief TODO: Describe this file
*/
#ifndef HEAP_H
#define HEAP_H

#include "segment.h"
#include "splaytree.h"

#define MIN_NUM_SLOTS (1u)

#define MAX_NUM_SLOTS (sizeof(uintptr_t) * 8u)

#define HEAP_INDEX_OFFSET (MIN_NUM_SLOTS)

#define NUM_HEAP_STACKS (MAX_NUM_SLOTS)

typedef struct block_t {
    uintptr_t size;
    uintptr_t data[];
} block_t;

typedef struct {
    struct {
        segment_t* available;
        segment_t* full;
    } heaps[NUM_HEAP_STACKS];
    splaytree_t* segments;
    splaytree_t* blocks;
    splaytree_t* greylist;
} heap_t;

heap_t* heap_create(void);

void heap_destroy(heap_t* heap);

void* heap_allocate(heap_t* heap, uintptr_t num_slots);

void heap_start_collection(heap_t* heap);

void heap_finish_collection(heap_t* heap);

void* heap_find_and_mark(heap_t* heap, uintptr_t addr);

#endif /* HEAP_H */
