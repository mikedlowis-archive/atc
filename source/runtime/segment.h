/**
  @file segment.h
  @brief TODO: Describe this file
*/
#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_BLOCKS    (256u)

#define BLOCKMAP_SIZE (sizeof(uint16_t) + (NUM_BLOCKS / sizeof(uint16_t)))

typedef struct {
    uintptr_t blocksize;
    uintptr_t* start;
    uintptr_t* end;
    uint16_t blockmap[BLOCKMAP_SIZE];
} segment_t;

segment_t* segment_create(uintptr_t blocksize);

void segment_destroy(segment_t* seg);

void* segment_alloc(segment_t* seg);

#endif /* SEGMENT_H */
