/**
  @file splaytree.c
  @brief See header for details
*/
#include "splaytree.h"
#include <stdlib.h>
#include <stdio.h>

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

typedef enum {
    LEFT = 0, RIGHT
} direction_t;

static node_t* rotate(node_t* node, direction_t direction){
    node_t* root  = node;
    node_t* pivot = (direction == LEFT) ? root->right : root->left;
    if (direction == LEFT) {
        root->right = pivot->left;
        pivot->left = root;
    } else {
        root->left = pivot->right;
        pivot->right = root;
    }
    return pivot;
}

void print_tree(node_t* tree) {
    if (tree) {
        printf("(%lu ", (uintptr_t)tree->value);
        if (tree->left) {
            print_tree(tree->left);
            printf(" ");
        } else
            printf("nil ");
        if (tree->right)
            print_tree(tree->right);
        else
            printf("nil");
        printf(")");
    }
}

static void splay(splaytree_t* tree, uintptr_t key) {
    node_t  subroots = {0, 0, 0};
    node_t* subleft  = &subroots;
    node_t* subright = &subroots;
    node_t* root     = tree->root;
    if (NULL != root) {
        while (1) {
            int cmp = tree->compare(key, root->value);
            if (cmp < 0) {
                if (NULL == root->left) break;
                if (tree->compare(key, root->left->value) < 0) {
                    root = rotate(root, RIGHT);
                    if (NULL == root->left) break;
                }
                subright->left = root;
                subright = root;
                root = root->left;
            } else if (cmp > 0) {
                if (NULL == root->right) break;
                if (tree->compare(key, root->right->value) < 0) {
                    root = rotate(root, LEFT);
                    if (NULL == root->right) break;
                }
                subleft->right = root;
                subleft = root;
                root = root->right;
            } else {
                break;
            }
        }
    }

    /* assemble */
    subleft->right = root->left;
    subright->left = root->right;
    root->left  = subroots.right;
    root->right = subroots.left;

    /* Set the root */
    tree->root = root;
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
        //destroy_node(tree, tree->root);
        (void)destroy_node;
        free(tree);
    }
}

void splaytree_insert(splaytree_t* tree, uintptr_t key, void* value)
{
    (void)next_node;
    if (tree->root == NULL) {
        tree->root = create_node(value);
    } else {
        /* Splay the closest node in value to the root */
        puts("before splay: ");
        print_tree(tree->root);
        puts("");
        /* Splay the new node to the top of the tree */
        splay(tree, key);
        puts("after splay: ");
        print_tree(tree->root);
        puts("");

        /* Compare the key to the new root */
        int cmp = tree->compare(key, tree->root->value);
        if (cmp < 0) {
           node_t* newroot  = create_node(value);
           printf("newroot1: %lu\n", (uintptr_t)newroot->value);
           newroot->left    = tree->root->left;
           newroot->right   = tree->root;
           tree->root->left = NULL;
           tree->root       = newroot;
        } else if (cmp > 0) {
           node_t* newroot   = create_node(value);
           newroot->right    = tree->root->right;
           newroot->left     = tree->root;
           printf("newroot2: %lu\n", (uintptr_t)newroot->value);
           tree->root->right = NULL;
           printf("newroot2: %lu\n", (uintptr_t)newroot->value);
           tree->root        = newroot;
        } else { /* do nothing, item already present */ }
        printf("values: %lu %lu\n", (uintptr_t)value, (uintptr_t)(tree->root->value));
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

