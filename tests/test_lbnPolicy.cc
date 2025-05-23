#include <gtest/gtest.h>
#include "../src/lbn_pool.hh"
#include "../src/def.hh"
#include <iostream>
#include <iomanip>

#include <queue>
#include <unordered_map>
#include <vector>
TEST(LBN_policy,wrost){
    LBNPool mapMgr;
    uint64_t lbn = INVALIDLBN;
    mapMgr.init_lbn_pool();
    mapMgr.insert_freeLBNList(0);
    mapMgr.insert_freeLBNList(1);
    mapMgr.insert_freeLBNList(2);
    mapMgr.insert_freeLBNList(3);
    mapMgr.insert_freeLBNList(1000);
    mapMgr.insert_freeLBNList(200);
    EXPECT_EQ(mapMgr.worst_policy(), 0);
    EXPECT_TRUE(mapMgr.get_usedLBNList(0));
    EXPECT_FALSE(mapMgr.get_freeLBNList(0));

    EXPECT_EQ(mapMgr.worst_policy(), 1);
    EXPECT_TRUE(mapMgr.get_usedLBNList(1));
    EXPECT_FALSE(mapMgr.get_freeLBNList(1));

    EXPECT_EQ(mapMgr.worst_policy(), 2);
    EXPECT_TRUE(mapMgr.get_usedLBNList(2));
    EXPECT_FALSE(mapMgr.get_freeLBNList(2));

    EXPECT_EQ(mapMgr.worst_policy(), 3);
    EXPECT_TRUE(mapMgr.get_usedLBNList(3));
    EXPECT_FALSE(mapMgr.get_freeLBNList(3));

    EXPECT_EQ(mapMgr.worst_policy(), 200);
    EXPECT_TRUE(mapMgr.get_usedLBNList(200));
    EXPECT_FALSE(mapMgr.get_freeLBNList(200));

    EXPECT_EQ(mapMgr.worst_policy(), 1000);
    EXPECT_TRUE(mapMgr.get_usedLBNList(1000));
    EXPECT_FALSE(mapMgr.get_freeLBNList(1000));
};  




TEST(LBN_policy, RR_policy_adaptive) {
    LBNPool pool;
    pool.init_lbn_pool();

    //Create test LBN list
    std::vector<uint64_t> input = {0, 1, 2, 3, 1000, 200 ,1500,500,2000};
    for (auto l : input) {
        if(l <= LBN_NUM){
            pool.insert_freeLBNList(l);
        }
        else{
            pr_debug("LBN %lu is out of range", l);
            FAIL() << "LBN " << l << " is out of range (0-" << LBN_NUM-1 << ")";
        }
    }
    // caculate the expected LBN
    std::vector<std::queue<uint64_t>> buckets(CHANNEL_NUM);
    for (auto l : input) {
        int ch = LBN2CH(l);
        buckets[ch].push(l);
    }

    // run RR policy to check the result
    int cur = (pool.lastUsedChannel + 1) % CHANNEL_NUM; 
    for (std::size_t i = 0; i < input.size(); ++i) {
        while (buckets[cur].empty())
            cur = (cur + 1) % CHANNEL_NUM;

        uint64_t expected = buckets[cur].front();
        buckets[cur].pop();

        uint64_t got = pool.RRpolicy();
        EXPECT_EQ(got, expected)          << "RRpolicy returned wrong LBN";
        EXPECT_TRUE(pool.get_usedLBNList(got));
        EXPECT_FALSE(pool.get_freeLBNList(got));

        cur = (cur + 1) % CHANNEL_NUM;
    }

    // Check all LBNs are used
    EXPECT_EQ(pool.RRpolicy(), INVALIDLBN);
} 