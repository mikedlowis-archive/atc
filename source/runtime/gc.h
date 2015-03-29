/**
  @file gc.h
  @brief TODO: Describe this file
*/
#ifndef GC_H
#define GC_H

#include <stddef.h>
#include <stdint.h>

void gc_init(void* stack_bottom);

void* gc_alloc(uint64_t objmap, size_t size);

void gc_collect(void);

void gc_shutdown(void);

#endif /* GC_H */