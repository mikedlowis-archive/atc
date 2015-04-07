/**
  @file splaytree.h
  @brief TODO: Describe this file
*/
#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <stdint.h>
#include "heap.h"

typedef enum { SEGMENT, BLOCK, NONE } node_type_t;

typedef struct node_t {
    node_type_t tag;
    union {
        segment_t* segment;
        block_t* block;
        void* raw;
    } ptr;
    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct splaytree_t {
    node_t*   root;
} splaytree_t;

splaytree_t* splaytree_create(void);

void splaytree_destroy(splaytree_t* tree);

void splaytree_insert(splaytree_t* tree, node_type_t tag, void* value);

node_type_t splaytree_lookup(splaytree_t* tree, uintptr_t address, void** value);

void splaytree_delete(splaytree_t* tree, uintptr_t address);

#endif /* SPLAYTREE_H */
