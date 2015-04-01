/**
  @file heap.c
  @brief See header for details
*/
#include "heap.h"
#include <stdlib.h>

heap_t* heap_create(void)
{
    heap_t* heap = (heap_t*)calloc(1u, sizeof(heap_t));
    return heap;
}

void heap_destroy(heap_t* heap)
{
    int i;
    for (i = 0; i < NUM_HEAP_STACKS; i++) {
        segnode_t* curr = heap->heaps[i];
        while (NULL != curr) {
            segnode_t* deadite = curr;
            curr = deadite->next;
            segment_destroy(deadite->segment);
            free(deadite);
        }
    }
    free(heap);
}

void* allocate_small_block(heap_t* heap, uintptr_t num_slots)
{
    uintptr_t index = num_slots - HEAP_INDEX_OFFSET;
    printf("index: %d, %d\n", (unsigned int)index, (unsigned int)num_slots);
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

void* allocate_large_block(heap_t* heap, uintptr_t num_slots)
{
    return NULL;
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

