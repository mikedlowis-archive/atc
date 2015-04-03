/**
  @file heap.c
  @brief See header for details
*/
#include "heap.h"
#include <stdlib.h>

heap_t* heap_create(void)
{
    return (heap_t*)calloc(1u, sizeof(heap_t));;
}

static void destroy_large_blocks(block_t* current) {
    while (NULL != current) {
        block_t* deadite = current;
        current = current->next;
        free(deadite);
    }
}

void heap_destroy(heap_t* heap)
{
    unsigned int i;
    /* Free all the large blocks */
    destroy_large_blocks(heap->blocks);
    destroy_large_blocks(heap->greylist);
    /* Free all of the small block segments */
    for (i = 0; i < NUM_HEAP_STACKS; i++) {
        segment_destroy(heap->heaps[i]);
    }
    /* Free the heap itself */
    free(heap);
}

static void* allocate_small_block(heap_t* heap, uintptr_t num_slots)
{
    uintptr_t index = (num_slots >= MIN_NUM_SLOTS) ? (num_slots - HEAP_INDEX_OFFSET) : 0;
    segment_t* current = heap->heaps[index];
    if ((NULL == current) || segment_full(current)) {
        heap->heaps[index] = segment_create(num_slots, current);
    }
    return segment_alloc(heap->heaps[index]);
}

static void* allocate_large_block(heap_t* heap, uintptr_t num_slots)
{
    block_t* blk = (block_t*)malloc(sizeof(block_t) + (num_slots * sizeof(uintptr_t)));
    blk->next = heap->blocks;
    heap->blocks = blk;
    return (&blk->data[0]);
}

void* heap_allocate(heap_t* heap, uintptr_t num_slots)
{
    void* obj = NULL;
    if (num_slots <= MAX_NUM_SLOTS) {
        obj = allocate_small_block(heap, num_slots);
    } else {
        obj = allocate_large_block(heap, num_slots);
    }
    return obj;
}

void heap_start_collection(heap_t* heap)
{
    heap->greylist = heap->blocks;
    heap->blocks = NULL;
    for (uintptr_t i = 0; i < NUM_HEAP_STACKS; i++) {
        for(segment_t* curr = heap->heaps[i]; curr != NULL; curr = curr->next) {
            segment_clear_map(heap->heaps[i]);
        }
    }
}

void heap_finish_collection(heap_t* heap)
{
    destroy_large_blocks(heap->greylist);
}

void* heap_find_and_mark(heap_t* heap, uintptr_t* addr)
{
    (void)heap;
    (void)addr;
    return NULL;
}

