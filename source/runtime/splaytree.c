/**
  @file splaytree.c
  @brief See header for details
*/
#include "splaytree.h"
#include <stdlib.h>

splaytree_t* splaytree_create(void)
{
    splaytree_t* splaytree = (splaytree_t*)malloc(sizeof(splaytree_t));
    splaytree->root = NULL;
    return splaytree;
}

static void destroy_tree(node_t* node)
{
    if (NULL != node) {
        destroy_tree(node->left);
        destroy_tree(node->right);
        free(node);
    }
}

void splaytree_destroy(splaytree_t* tree)
{
    destroy_tree(tree->root);
    free(tree);
}

static node_t* create_node(node_type_t tag, void* value)
{
    node_t* node = (node_t*)malloc(sizeof(node_t*));
    node->tag = tag;
    node->ptr.raw = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static uintptr_t get_start_addr(node_t* curr)
{
    if (curr->tag == SEGMENT)
        return (uintptr_t)(curr->ptr.segment->start);
    else
        return (uintptr_t)(curr->ptr.block->data);
}

static uintptr_t get_end_addr(node_t* curr)
{
    if (curr->tag == SEGMENT)
        return (uintptr_t)(curr->ptr.segment->end);
    else
        return (uintptr_t)(curr->ptr.block->data + curr->ptr.block->size);
}

static node_t** next_node(node_t* curr, uintptr_t address)
{
    uintptr_t curr_address = get_start_addr(curr);
    if (address < curr_address)
        return &(curr->left);
    else
        return &(curr->right);
}

void splaytree_insert(splaytree_t* tree, node_type_t tag, void* value)
{
    /* Get the address of the start of the range */
    uintptr_t address;
    if (SEGMENT == tag)
        address = (uintptr_t)((segment_t*)value)->start;
    else
        address = (uintptr_t)((block_t*)value)->data;
    /* Insert the item into the tree */
    if (tree->root == NULL)
        tree->root = create_node(tag, value);
    else {
        node_t** current = &(tree->root);
        while(*current != NULL) {
            current = next_node(*current, address);
        }
        *current = create_node(tag, value);
    }
}

node_type_t splaytree_lookup(splaytree_t* tree, uintptr_t address, void** value)
{
    node_type_t tag = NONE;
    node_t** current = &(tree->root);
    while(*current != NULL) {
        uintptr_t start = get_start_addr(*current);
        uintptr_t end   = get_end_addr(*current);
        if ((start <= address) && (address < end)) {
            *value = (*current)->ptr.raw;
            break;
        } else if (start < address) {
            current = &((*current)->left);
        } else {
            current = &((*current)->right);
        }
    }
    return tag;
}

void splaytree_delete(splaytree_t* tree, uintptr_t address)
{
    (void)tree;
    (void)address;
}

