#include <gtest/gtest.h>
#include "../src/lbn_pool.hh"

TEST(MappingTableTest, InsertAndGet) {
    LBNPool mapMgr;
    mapMgr.insert_usedLBNList(0);
    EXPECT_TRUE(mapMgr.get_usedLBNList(0));
}
