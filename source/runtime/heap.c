/**
  @file heap.c
  @brief See header for details
*/
#include "heap.h"
#include <stdlib.h>

static int range_compare(uintptr_t val, uintptr_t start, uintptr_t end)
{
    if (val < start)
        return -1;
    else if (val >= end)
        return 1;
    else
        return 0;
}

static int segment_compare(uintptr_t key, segment_t* seg)
{
    uintptr_t start = (uintptr_t)(seg->start);
    uintptr_t end   = (uintptr_t)(seg->end);
    return range_compare(key, start, end);
}

static int block_compare(uintptr_t key, block_t* block)
{
    uintptr_t start = (uintptr_t)(block->data);
    uintptr_t end   = (uintptr_t)(start + block->size);
    return range_compare(key, start, end);
}

heap_t* heap_create(void)
{
    heap_t* heap = (heap_t*)calloc(1u, sizeof(heap_t));
    heap->segments = splaytree_create((del_fn_t)segment_destroy, (cmp_fn_t)segment_compare);
    heap->blocks   = splaytree_create((del_fn_t)free, (cmp_fn_t)block_compare);
    heap->greylist = NULL;
    return heap;
}

void heap_destroy(heap_t* heap)
{
    /* Free all the large blocks */
    splaytree_destroy(heap->segments);
    splaytree_destroy(heap->blocks);
    splaytree_destroy(heap->greylist);
    /* Free all of the small block segments */
    for (unsigned int i = 0; i < NUM_HEAP_STACKS; i++) {
        segment_destroy(heap->heaps[i].available);
    }
    /* Free the heap itself */
    free(heap);
}

static void* allocate_small_block(heap_t* heap, uintptr_t num_slots)
{
    void* object;
    uintptr_t index = (num_slots >= MIN_NUM_SLOTS) ? (num_slots - HEAP_INDEX_OFFSET) : 0;
    /* If we dont have any available segments, allocate a new one */
    if (NULL == heap->heaps[index].available) {
        heap->heaps[index].available = segment_create(num_slots, heap->heaps[index].available);
        splaytree_insert(heap->segments, (uintptr_t)&(heap->heaps[index].available->start[0]), heap->heaps[index].available);
    }
    /* Allocate the object */
    object = segment_alloc(heap->heaps[index].available);
    /* If we filled it up then move it to the full list */
    if (segment_full(heap->heaps[index].available)) {
        segment_t* current = heap->heaps[index].available;
        heap->heaps[index].available = heap->heaps[index].available->next;
        current->next = heap->heaps[index].full;
        heap->heaps[index].full = current;
    }
    return object;
}

static void* allocate_large_block(heap_t* heap, uintptr_t num_slots)
{
    uintptr_t size = (num_slots * sizeof(uintptr_t));
    block_t* blk = (block_t*)malloc(sizeof(block_t) + size);
    blk->size = size;
    splaytree_insert(heap->blocks, (uintptr_t)&blk->data[0], blk);
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
    heap->blocks   = splaytree_create((del_fn_t)free, (cmp_fn_t)block_compare);
}

void heap_finish_collection(heap_t* heap)
{
    /* All blocks remaining in the greylist now are unreachable so free them */
    splaytree_destroy(heap->greylist);
    /* Now iterate over the sub heaps and make sure the full/available lists are correct */
    for (unsigned int i = 0; i < NUM_HEAP_STACKS; i++) {
        segment_t* prev = NULL;
        segment_t* curr = heap->heaps[i].full;
        while(curr != NULL) {
            if (segment_empty(curr)) {
                segment_t* deadite = curr;
                if (NULL != prev) {
                    prev->next = curr->next;
                    curr = curr->next;
                }
                deadite->next = NULL;
                segment_destroy(deadite);
            } else if (!segment_full(curr)) {
                if (NULL != prev) {
                    prev->next = curr->next;
                    curr = curr->next;
                }
                curr->next = heap->heaps[i].available;
                heap->heaps[i].available = curr;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
        segment_destroy(heap->heaps[i].available);
    }
}

void* heap_find_and_mark(heap_t* heap, uintptr_t addr)
{
    void* obj = NULL;
    segment_t* seg = splaytree_lookup(heap->segments, addr);
    if (NULL != seg) {
        obj = segment_find_and_mark(seg, addr);
    } else {
        block_t* block = splaytree_delete(heap->greylist, addr);
        if (NULL != block) {
            splaytree_insert(heap->blocks, addr, block);
            obj = &block->data[0];
        }
    }
    return obj;
}

