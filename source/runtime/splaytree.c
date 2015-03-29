/**
  @file splaytree.c
  @brief See header for details
*/
#include "splaytree.h"
#include <stdlib.h>

static node_t* create_tree(uintptr_t val) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->value  = val;
    node->parent = NULL;
    node->left   = NULL;
    node->right  = NULL;
    return node;
}

static void destroy_tree(node_t* node) {
    if (NULL != node) {
        destroy_tree(node->left);
        destroy_tree(node->right);
    }
}

splaytree_t* splaytree_create(comp_fn_t cfn)
{
    splaytree_t* splaytree = (splaytree_t*)malloc(sizeof(splaytree_t));
    splaytree->compare = cfn;
    splaytree->root    = NULL;
    return splaytree;
}

void splaytree_destroy(splaytree_t* tree)
{
    destroy_tree(tree->root);
    free(tree);
}

void splaytree_insert(splaytree_t* tree, uintptr_t value)
{
    if (tree->root == NULL) {
        tree->root = create_tree(value);
    } else {
        node_t* curr = tree->root;
        while (1) {
            int cmp = tree->compare(value, curr->value);
            if (0 == cmp) {
                break;
            } else if (cmp < 1) {
                if (NULL == curr->left) {
                    curr->left = create_tree(value);
                } else {
                    curr = curr->left;
                }
            } else if (cmp > 1) {
                if (NULL == curr->right) {
                    curr->right = create_tree(value);
                } else {
                    curr = curr->right;
                }
            }
        }
    }
}

uintptr_t splaytree_lookup(splaytree_t* tree, uintptr_t value)
{
    (void)tree;
    (void)value;
    return 0;
}


