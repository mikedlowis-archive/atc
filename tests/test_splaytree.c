#include "atf.h"
#include "splaytree.h"

TEST_SUITE(SplayTree) {
    /* Verify: splaytree_create
     *************************************************************************/
    TEST(Verify_Create_allocates_and_initializes_an_empty_tree) {
        splaytree_t* tree = splaytree_create((comp_fn_t)0x1234);
        CHECK(tree->compare == (comp_fn_t)0x1234);
        CHECK(tree->root    == NULL);
        splaytree_destroy(tree);
    }

    /* Verify: splaytree_create
     *************************************************************************/
}
