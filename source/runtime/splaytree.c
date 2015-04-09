/**
  @file splaytree.c
  @brief See header for details
*/
#include "splaytree.h"
#include <stdlib.h>

static node_t* create_node(void* value)
{
    node_t* node = (node_t*)malloc(sizeof(node_t*));
    node->value  = value;
    node->left   = NULL;
    node->right  = NULL;
    return node;
}

static node_t** next_node(splaytree_t* tree, node_t* curr, uintptr_t address)
{
    if (tree->compare(address, curr->value) < 0)
        return &(curr->left);
    else
        return &(curr->right);
}

static void destroy_node(splaytree_t* tree, node_t* node) {
    if (NULL != node) {
        destroy_node(tree, node->left);
        destroy_node(tree, node->right);
        //tree->destroy(node->value);
        free(node);
    }
}

splaytree_t* splaytree_create(del_fn_t delfn, cmp_fn_t cmp_fn)
{
    splaytree_t* tree = (splaytree_t*)malloc(sizeof(splaytree_t));
    tree->destroy = delfn;
    tree->compare = cmp_fn;
    tree->root = NULL;
    return tree;
}

void splaytree_destroy(splaytree_t* tree)
{
    if (NULL != tree) {
        destroy_node(tree, tree->root);
        free(tree);
    }
}

void splaytree_insert(splaytree_t* tree, uintptr_t key, void* value)
{
    /* Insert the item into the tree */
    if (tree->root == NULL) {
        tree->root = create_node(value);
    } else {
        node_t** current = &(tree->root);
        while(*current != NULL) {
            current = next_node(tree, *current, key);
        }
        *current = create_node(value);
    }
}

void* splaytree_lookup(splaytree_t* tree, uintptr_t key)
{
    node_t* current = tree->root;
    while (current != NULL) {
        int cmp_val = tree->compare(key, current->value);
        if (cmp_val == 0)
            return current->value;
        else if (cmp_val < 0)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

void* splaytree_delete(splaytree_t* tree, uintptr_t key)
{
    (void)tree;
    (void)key;
    return NULL;
}

