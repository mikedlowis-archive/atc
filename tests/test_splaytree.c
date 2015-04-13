#include "atf.h"
#include "splaytree.h"
#include <stdio.h>

static int cmp_int(uintptr_t key, uintptr_t val) {
    return (key == val) ? 0 : (key < val) ? -1 : 1;
}

static void del_int(uintptr_t val) {
    (void)val;
}

TEST_SUITE(SplayTree) {
    /* Verify: splaytree_create
     *************************************************************************/
    TEST(Verify_Create_allocates_and_initializes_an_empty_tree) {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        CHECK(tree->destroy == (del_fn_t)del_int);
        CHECK(tree->compare == (cmp_fn_t)cmp_int);
        CHECK(tree->root == NULL);
        splaytree_destroy(tree);
    }

    /* Verify: splaytree_insert
     *************************************************************************/
    TEST(Verify_Insert_will_insert_into_an_empty_tree)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        CHECK((void*)42 == tree->root->value);
        splaytree_destroy(tree);
    }

    TEST(Verify_Insert_will_insert_does_nothing_if_item_already_present)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 42, (void*)42);
        CHECK((void*)42 == tree->root->value);
        CHECK(NULL == tree->root->left);
        CHECK(NULL == tree->root->right);
        splaytree_destroy(tree);
    }

    TEST(Verify_Insert_will_insert_to_the_left_of_root_when_less_than)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 41, (void*)41);
        CHECK((void*)41 == tree->root->value);
        CHECK((void*)42 == tree->root->right->value);
        splaytree_destroy(tree);
    }

    TEST(Verify_Insert_will_insert_to_the_right_of_root_when_greater_than)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 43, (void*)43);
        CHECK((void*)43 == tree->root->value);
        CHECK((void*)42 == tree->root->left->value);
        splaytree_destroy(tree);
    }

    /* Verify: splaytree_lookup
     *************************************************************************/
    TEST(Verify_Lookup_will_find_the_item_at_the_root)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        CHECK((void*)42 == splaytree_lookup(tree, 42));
        splaytree_destroy(tree);
    }

    TEST(Verify_Lookup_will_return_null_if_tree_is_empty)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        CHECK(NULL == splaytree_lookup(tree, 43));
        splaytree_destroy(tree);
    }

    TEST(Verify_Lookup_will_return_null_if_value_is_not_present)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        CHECK(NULL == splaytree_lookup(tree, 43));
        splaytree_destroy(tree);
    }

    TEST(Verify_Lookup_will_find_the_item_left_of_root_when_less_than)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 41, (void*)41);
        CHECK((void*)41 == splaytree_lookup(tree, 41));
        splaytree_destroy(tree);
    }

    TEST(Verify_Lookup_will_find_the_item_right_of_root_when_greater_than)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 43, (void*)43);
        CHECK((void*)43 == splaytree_lookup(tree, 43));
        splaytree_destroy(tree);
    }

    /* Verify: splaytree_delete
     *************************************************************************/
    TEST(Verify_Delete_will_delete_the_item_at_the_root)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        CHECK((void*)42 == splaytree_delete(tree, 42));
        CHECK(tree->root == NULL);
        splaytree_destroy(tree);
    }

    TEST(Verify_Delete_will_delete_the_smallest_item)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 43, (void*)43);
        CHECK((void*)42 == splaytree_delete(tree, 42));
        CHECK(tree->root != NULL);
        CHECK(tree->root->value == (void*)43);
        splaytree_destroy(tree);
    }

    TEST(Verify_Delete_will_delete_the_largest_item)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 43, (void*)43);
        CHECK((void*)43 == splaytree_delete(tree, 43));
        CHECK(tree->root != NULL);
        CHECK(tree->root->value == (void*)42);
        splaytree_destroy(tree);
    }

    TEST(Verify_Delete_will_delete_the_item_to_the_left_of_root)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 41, (void*)41);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 43, (void*)43);
        CHECK((void*)41 == splaytree_delete(tree, 41));
        CHECK(tree->root != NULL);
        CHECK(tree->root->value == (void*)42);
        splaytree_destroy(tree);
    }

    TEST(Verify_Delete_will_delete_an_item_from_a_tree_with_depth_3)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 40, (void*)40);
        splaytree_insert(tree, 41, (void*)41);
        splaytree_insert(tree, 43, (void*)43);
        splaytree_insert(tree, 44, (void*)44);
        splaytree_insert(tree, 42, (void*)42);
        CHECK((void*)42 == splaytree_delete(tree, 42));
        CHECK(tree->root != NULL);
        CHECK(tree->root->value == (void*)41);
        CHECK(tree->root->left != NULL);
        CHECK(tree->root->left->value == (void*)40);
        CHECK(tree->root->left->left == NULL);
        CHECK(tree->root->left->right == NULL);
        CHECK(tree->root->right != NULL);
        CHECK(tree->root->right->value == (void*)43);
        CHECK(tree->root->right->right != NULL);
        CHECK(tree->root->right->right->value == (void*)44);
        CHECK(tree->root->right->left == NULL);
        splaytree_destroy(tree);
    }

    TEST(Verify_Delete_should_do_nothing_when_tree_empty)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        CHECK(NULL == splaytree_delete(tree, 42));
        splaytree_destroy(tree);
    }
}
