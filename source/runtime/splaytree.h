/**
  @file splaytree.h
  @brief TODO: Describe this file
*/
#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <stdint.h>

typedef void (*del_fn_t)(void* value);

typedef int (*cmp_fn_t)(uintptr_t key, void* value);

typedef struct node_t {
    void* value;
    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct splaytree_t {
    del_fn_t destroy;
    cmp_fn_t compare;
    node_t*  root;
} splaytree_t;

splaytree_t* splaytree_create(del_fn_t delfn, cmp_fn_t cmp_fn);

void splaytree_destroy(splaytree_t* tree);

void splaytree_insert(splaytree_t* tree, uintptr_t key, void* value);

void* splaytree_lookup(splaytree_t* tree, uintptr_t key);

void* splaytree_delete(splaytree_t* tree, uintptr_t key);

#endif /* SPLAYTREE_H */
