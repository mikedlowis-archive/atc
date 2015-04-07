#include "atf.h"
#include "splaytree.h"

TEST_SUITE(SplayTree) {
    /* Verify: splaytree_create
     *************************************************************************/
    TEST(Verify_Create_allocates_and_initializes_an_empty_tree) {
        splaytree_t* tree = splaytree_create();
        CHECK(tree->root    == NULL);
        splaytree_destroy(tree);
    }

    /* Verify: splaytree_create
     *************************************************************************/
}
