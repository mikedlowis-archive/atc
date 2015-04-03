/**
  @file gc.h
  @brief TODO: Describe this file
*/
#ifndef GC_H
#define GC_H

#include <stddef.h>
#include <stdint.h>

void gc_init(uintptr_t* stack_bottom);

void gc_shutdown(void);

void gc_add_root(void* address, size_t size);

void* gc_object(uint64_t objmap, size_t num_slots);

void* gc_allocate(size_t size);

void gc_collect(void);

#endif /* GC_H */
