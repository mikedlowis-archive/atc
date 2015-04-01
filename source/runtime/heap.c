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

void heap_destroy(heap_t* heap)
{
    int i;
    block_t* current = heap->blocks;
    /* Free all the large blocks */
    while (NULL != current) {
        block_t* deadite = current;
        current = deadite->next;
        free(deadite);
    }
    /* Free all of the small block segments */
    for (i = 0; i < NUM_HEAP_STACKS; i++) {
        segnode_t* curr = heap->heaps[i];
        while (NULL != curr) {
            segnode_t* deadite = curr;
            curr = deadite->next;
            segment_destroy(deadite->segment);
            free(deadite);
        }
    }
    /* Free the heap itself */
    free(heap);
}

static void* allocate_small_block(heap_t* heap, uintptr_t num_slots)
{
    uintptr_t index = (num_slots >= MIN_NUM_SLOTS) ? (num_slots - HEAP_INDEX_OFFSET) : 0;
    segnode_t* node = heap->heaps[index];
    if ((NULL == node) || segment_full(node->segment)) {
        segnode_t* newnode = (segnode_t*)malloc(sizeof(segnode_t));
        newnode->segment = segment_create(num_slots);
        newnode->next = node;
        heap->heaps[index] = newnode;
        node = newnode;
    }
    return segment_alloc(heap->heaps[index]->segment);
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

