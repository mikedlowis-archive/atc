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
    uintptr_t size = (num_slots * sizeof(uintptr_t));
    block_t* blk = (block_t*)malloc(sizeof(block_t) + size);
    blk->next = heap->blocks;
    blk->size = size;
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
    heap->greylist = NULL;
}

static void* subheap_find_and_mark(heap_t* heap, uintptr_t addr) {
    void* obj = NULL;
    for (uintptr_t i = 0; i < NUM_HEAP_STACKS; i++) {
        for(segment_t* curr = heap->heaps[i]; curr != NULL; curr = curr->next) {
            obj = segment_find_and_mark(heap->heaps[i], addr);
            if (NULL != obj) {
                i = NUM_HEAP_STACKS;
                break;
            }
        }
    }
    return obj;
}

static void* block_find_and_mark(heap_t* heap, uintptr_t addr) {
    void* obj = NULL;
    block_t* prev = NULL;
    block_t* curr = heap->greylist;
    while (curr != NULL) {
        uintptr_t start = (uintptr_t)&(curr->data[0]);
        uintptr_t end   = start + curr->size;
        if ((start <= addr) && (addr < end)) {
            /* Remove it from the grey list */
            if (prev == NULL)
                heap->greylist = curr->next;
            else
                prev->next = curr->next;
            /* Add it to the in-use list and break */
            curr->next = heap->blocks;
            heap->blocks = curr->next;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    return obj;
}

void* heap_find_and_mark(heap_t* heap, uintptr_t addr)
{
    void* obj = subheap_find_and_mark(heap, addr);
    if (NULL == obj)
        obj = block_find_and_mark(heap, addr);
    return obj;
}

