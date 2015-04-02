#include "gc.h"
#include "heap.h"

typedef struct {
    uint64_t objmap;
    uint8_t data[];
} obj_t;

static heap_t* heap = NULL;

void gc_init(void* stack_bottom)
{
    (void)stack_bottom;
    heap = heap_create();
}

void* gc_object(uint64_t objmap, size_t num_slots)
{
    (void)objmap;
    return heap_allocate(heap, num_slots+1);
}

void* gc_allocate(size_t size)
{
    size_t slot_sz   = sizeof(uintptr_t);
    size_t remainder = size % slot_sz;
    size_t num_slots = (size / slot_sz) + ((remainder == 0) ? 0 : (slot_sz - remainder));
    return heap_allocate(heap, num_slots + 1);
}

void gc_collect(void)
{
}

void gc_shutdown(void)
{
    heap_destroy(heap);
}

