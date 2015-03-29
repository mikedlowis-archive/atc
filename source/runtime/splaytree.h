/**
  @file splaytree.h
  @brief TODO: Describe this file
*/
#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <stdint.h>

typedef struct node_t {
    uintptr_t value;
    struct node_t* parent;
    struct node_t* left;
    struct node_t* right;
} node_t;

typedef int (*comp_fn_t)(uintptr_t a, uintptr_t b);

typedef struct splaytree_t {
    comp_fn_t compare;
    node_t*   root;
} splaytree_t;

splaytree_t* splaytree_create(comp_fn_t cfn);

void splaytree_destroy(splaytree_t* tree);

void splaytree_insert(splaytree_t* tree);

uintptr_t splaytree_lookup(splaytree_t* tree, uintptr_t value);

#endif /* SPLAYTREE_H */
