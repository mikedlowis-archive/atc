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
//    TEST(Verify_Insert_will_insert_into_an_empty_tree)
//    {
//        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
//        splaytree_insert(tree, 42, (void*)42);
//        CHECK((void*)42 == tree->root->value);
//        splaytree_destroy(tree);
//    }
//
//    TEST(Verify_Insert_will_insert_to_the_left_of_root_when_less_than)
//    {
//        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
//        splaytree_insert(tree, 42, (void*)42);
//        splaytree_insert(tree, 41, (void*)41);
//        CHECK((void*)41 == tree->root->value);
//        CHECK((void*)42 == tree->root->right->value);
//        splaytree_destroy(tree);
//    }

    TEST(Verify_Insert_will_insert_to_the_right_of_root_when_greater_than)
    {
        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
        splaytree_insert(tree, 42, (void*)42);
        splaytree_insert(tree, 43, (void*)43);
        printf("root->value: %p\n", tree->root->value);
        CHECK((void*)43 == tree->root->value);
        CHECK((void*)42 == tree->root->left->value);
        splaytree_destroy(tree);
    }

    /* Verify: splaytree_lookup
     *************************************************************************/
//    TEST(Verify_Lookup_will_find_the_item_at_the_root)
//    {
//        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
//        splaytree_insert(tree, 42, (void*)42);
//        CHECK((void*)42 == splaytree_lookup(tree, 42));
//        splaytree_destroy(tree);
//    }
//
//    TEST(Verify_Lookup_will_find_the_item_left_of_root_when_less_than)
//    {
//        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
//        splaytree_insert(tree, 42, (void*)42);
//        splaytree_insert(tree, 41, (void*)41);
//        CHECK((void*)41 == splaytree_lookup(tree, 41));
//        splaytree_destroy(tree);
//    }
//
//    TEST(Verify_Lookup_will_find_the_item_right_of_root_when_greater_than)
//    {
//        splaytree_t* tree = splaytree_create((del_fn_t)del_int, (cmp_fn_t)cmp_int);
//        splaytree_insert(tree, 42, (void*)42);
//        splaytree_insert(tree, 43, (void*)43);
//        CHECK((void*)43 == splaytree_lookup(tree, 43));
//        splaytree_destroy(tree);
//    }
//
//    /* Verify: splaytree_delete
//     *************************************************************************/
//    TEST(Verify_Delete_will_delete_the_item_at_the_root)
//    {
//        CHECK(false);
//    }
//
//    TEST(Verify_Delete_will_delete_the_item_left_of_root_when_less_than)
//    {
//        CHECK(false);
//    }
//
//    TEST(Verify_Delete_will_delete_the_item_right_of_root_when_greater_than)
//    {
//        CHECK(false);
//    }
}
