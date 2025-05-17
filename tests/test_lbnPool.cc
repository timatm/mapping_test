#include <gtest/gtest.h>
#include "../src/lbn_pool.hh"

TEST(LBNPool, InsertAndGet) {
    LBNPool mapMgr;
    mapMgr.insert_usedLBNList(0);
    EXPECT_TRUE(mapMgr.get_usedLBNList(0));
}

TEST(LBNPool, InsertAndGetMultiple) {
    LBNPool mapMgr;
    mapMgr.insert_usedLBNList(0);
    mapMgr.insert_usedLBNList(1);
    EXPECT_TRUE(mapMgr.get_usedLBNList(0));
    EXPECT_TRUE(mapMgr.get_usedLBNList(1));
}

TEST(LBNPool, Remove) {
    LBNPool mapMgr;
    mapMgr.insert_usedLBNList(0);
    EXPECT_TRUE(mapMgr.get_usedLBNList(0));
    mapMgr.remove_usedLBNList(0);
    EXPECT_FALSE(mapMgr.get_usedLBNList(0));
}

TEST(LBNPool, RemoveMultiple) {
    LBNPool mapMgr;
    mapMgr.insert_usedLBNList(0);
    mapMgr.insert_usedLBNList(1);
    EXPECT_TRUE(mapMgr.get_usedLBNList(0));
    EXPECT_TRUE(mapMgr.get_usedLBNList(1));
    mapMgr.remove_usedLBNList(0);
    EXPECT_FALSE(mapMgr.get_usedLBNList(0));
    EXPECT_TRUE(mapMgr.get_usedLBNList(1));
}

TEST(LBNPool, RemoveNonExistent) {
    LBNPool mapMgr;
    mapMgr.insert_usedLBNList(0);
    EXPECT_TRUE(mapMgr.get_usedLBNList(0));
    mapMgr.remove_usedLBNList(1); // Attempt to remove a non-existent LBN
    EXPECT_TRUE(mapMgr.get_usedLBNList(0)); // Should still be present
}

TEST(LBNPool, InsertFreeLBN) {
    LBNPool mapMgr;
    mapMgr.insert_freeLBNList(0);
    EXPECT_TRUE(mapMgr.get_freeLBNList(0));
}
TEST(LBNPool, InsertFreeLBNMultiple) {
    LBNPool mapMgr;
    mapMgr.insert_freeLBNList(0);
    mapMgr.insert_freeLBNList(1);
    EXPECT_TRUE(mapMgr.get_freeLBNList(0));
    EXPECT_TRUE(mapMgr.get_freeLBNList(1));
}
TEST(LBNPool, RemoveFreeLBN) {
    LBNPool mapMgr;
    mapMgr.insert_freeLBNList(0);
    EXPECT_TRUE(mapMgr.get_freeLBNList(0));
    mapMgr.remove_freeLBNList(0);
    EXPECT_FALSE(mapMgr.get_freeLBNList(0));
}

TEST(LBNPool, RemoveFreeLBNMultiple) {
    LBNPool mapMgr;
    mapMgr.insert_freeLBNList(0);
    mapMgr.insert_freeLBNList(1);
    EXPECT_TRUE(mapMgr.get_freeLBNList(0));
    EXPECT_TRUE(mapMgr.get_freeLBNList(1));
    mapMgr.remove_freeLBNList(0);
    EXPECT_FALSE(mapMgr.get_freeLBNList(0));
    EXPECT_TRUE(mapMgr.get_freeLBNList(1));
}
TEST(LBNPool, RemoveFreeLBNNonExistent) {
    LBNPool mapMgr;
    mapMgr.insert_freeLBNList(0);
    EXPECT_TRUE(mapMgr.get_freeLBNList(0));
    mapMgr.remove_freeLBNList(1); // Attempt to remove a non-existent LBN
    EXPECT_TRUE(mapMgr.get_freeLBNList(0)); // Should still be present
}