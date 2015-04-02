/**
  @file segment.h
  @brief TODO: Describe this file
*/
#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_BLOCKS    (256u)

#define BLOCKMAP_SIZE ((sizeof(uint16_t) * 8) + 1)

typedef struct segment_t {
    struct segment_t* next;
    uintptr_t blocksize;
    uint16_t blockmap[BLOCKMAP_SIZE];
    uintptr_t* end;
    uintptr_t start[];
} segment_t;

segment_t* segment_create(uintptr_t blocksize, segment_t* next);

void segment_destroy(segment_t* seg);

bool segment_full(segment_t* seg);

void* segment_alloc(segment_t* seg);

#endif /* SEGMENT_H */
