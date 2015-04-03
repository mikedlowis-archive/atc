#include "gc.h"
#include "heap.h"
#include <assert.h>
#include <stdlib.h>
#include <setjmp.h>

typedef struct {
    uint64_t objmap;
    uint8_t data[];
} obj_t;

typedef struct root_t {
    struct root_t* next;
    void* address;
    size_t size;
} root_t;

static heap_t* Heap = NULL;
static uintptr_t* Stack_Bottom = NULL;
static root_t* Roots = NULL;

void gc_init(uintptr_t* stack_bottom)
{
    Stack_Bottom = stack_bottom;
    Heap = heap_create();
}

void gc_shutdown(void)
{
    heap_destroy(Heap);
}

void gc_add_root(void* address, size_t size)
{
    root_t* root = (root_t*)malloc(sizeof(root_t));
    root->address = address;
    root->size = size;
    root->next = Roots;
    Roots = root;
}

void* gc_object(uint64_t objmap, size_t num_slots)
{
    obj_t* obj = heap_allocate(Heap, num_slots+1);
    obj->objmap = objmap;
    obj++;
    return obj;
}

void* gc_allocate(size_t size)
{
    size_t slot_sz   = sizeof(uintptr_t);
    size_t num_slots = (size + slot_sz - 1) / slot_sz;
    return heap_allocate(Heap, num_slots + 1);
}

static void gc_scan_object(void* object) {
    (void)object;
}

static void gc_scan_region(uintptr_t* start, uintptr_t* stop) {
    for (; start < stop; start++) {
        obj_t* obj = (obj_t*)heap_find_and_mark(Heap, start);
        if (NULL != obj)
            gc_scan_object(obj);
    }
}

static void gc_scan_stack(void) {
    /* Setup pointers to the stack top and bottom */
    uintptr_t* stack_bot = Stack_Bottom;
    uintptr_t* stack_top = (uintptr_t*)&stack_top;
    /* Scan the stack and mark any live objects */
    if (stack_bot <= stack_top)
        gc_scan_region(stack_bot, stack_top);
    else
        gc_scan_region(stack_top, stack_bot);
}

static void gc_scan_roots(void) {
    for (root_t* root = Roots; root != NULL; root = root->next)
        gc_scan_region(root->address, root->address + (root->size / sizeof(uintptr_t)));
}

void gc_collect(void)
{
    jmp_buf registers;
    heap_start_collection(Heap);
    (void)setjmp(registers); // Save off register sets
    gc_scan_stack();
    gc_scan_roots();
    gc_scan_object(NULL);
    heap_finish_collection(Heap);
}
