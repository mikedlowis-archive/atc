#include "gc.h"
#include <stdlib.h>

/*****************************************************************************/

typedef struct {
    uint64_t objmap;
    uint8_t data[];
} obj_t;

void gc_init(void* stack_bottom)
{
}

void* gc_alloc(uint64_t objmap, size_t num_slots)
{
    obj_t* obj;
    if (num_slots <= (sizeof(uint64_t) - 1u)) {
        obj = (void*)malloc(sizeof(obj_t) + (num_slots * sizeof(uintptr_t)));
    } else {
        obj = (void*)malloc(sizeof(obj_t) + (num_slots * sizeof(uintptr_t)));
    }
    obj->objmap = objmap;
    obj++;
    return (void*)obj;
}

void gc_collect(void)
{
}

void gc_shutdown(void)
{
}

/*****************************************************************************/

