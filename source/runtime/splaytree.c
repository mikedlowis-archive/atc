/**
  @file splaytree.c
  @brief See header for details
*/
#include "splaytree.h"
#include <stdlib.h>
#include <string.h>

static node_t* create_node(void* value, node_t* left, node_t* right)
{
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->value  = value;
    node->left   = left;
    node->right  = right;
    return node;
}

static void destroy_node(splaytree_t* tree, node_t* node) {
    if (NULL != node) {
        //tree->destroy(node->value);
        destroy_node(tree, node->left);
        destroy_node(tree, node->right);
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
        destroy_node(tree, tree->root);
        (void)destroy_node;
        free(tree);
    }
}

void splaytree_insert(splaytree_t* tree, uintptr_t key, void* value)
{
    if (tree->root == NULL) {
        tree->root = create_node(value, NULL, NULL);
    } else {
        /* Splay the closest node in value to the root */
        splay(tree, key);
        /* Compare the key to the new root */
        node_t* root = tree->root;
        int cmp = tree->compare(key, root->value);
        if (cmp < 0) {
            node_t* newroot  = create_node(value, root->left, root);
            tree->root->left = NULL;
            tree->root       = newroot;
        } else if (cmp > 0) {
            node_t* newroot = create_node(value, root, root->right);
            root->right     = NULL;
            tree->root      = newroot;
        } else { /* do nothing, item already present */ }
    }
}

void* splaytree_lookup(splaytree_t* tree, uintptr_t key)
{
    if (tree->root != NULL) {
        splay(tree, key);
        if (0 == tree->compare(key, tree->root->value))
            return tree->root->value;
    }
    return NULL;
}

void* splaytree_delete(splaytree_t* tree, uintptr_t key)
{
    void* value = NULL;
    if (tree->root != NULL) {
        splay(tree, key);
        if (0 == tree->compare(key, tree->root->value)) {
            node_t* deadite = tree->root;
            if (tree->root->left == NULL) {
                tree->root = deadite->right;
            } else {
                tree->root = deadite->left;
                splay(tree, key);
                tree->root->right = deadite->right;
            }
            /* Return the value and kill the old root */
            value = deadite->value;
            memset(deadite, 0x00, sizeof(node_t));
            destroy_node(tree, deadite);
        }
    }
    return value;
}

