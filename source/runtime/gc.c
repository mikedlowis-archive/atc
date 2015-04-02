#include "gc.h"
#include "heap.h"

typedef struct {
    uint64_t objmap;
    uint8_t data[];
} obj_t;

static heap_t* heap = NULL;

void gc_init(void* stack_bottom)
{
    heap = heap_create();
}

void* gc_alloc(uint64_t objmap, size_t num_slots)
{
    return heap_allocate(heap, num_slots+1);
}

void gc_collect(void)
{
}

void gc_shutdown(void)
{
    heap_destroy(heap);
}

/*****************************************************************************/

