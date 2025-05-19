#ifndef __LBN_POOL_HH__
#define __LBN_POOL_HH__


#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <cstdint>
#include <vector>
#include <deque>
#include <queue>

#include "def.hh"
#include "print.hh"
class LBNPool {
public:
    std::unordered_set<uint64_t> usedLBNList;
    std::array<std::deque<uint64_t>, CHANNEL_NUM> freeLBNList;
    int lastChannel;

    void init_lbn_pool();
    void insert_freeLBNList(uint64_t lbn);
    bool remove_freeLBNList(uint64_t lbn);
    bool get_freeLBNList(uint64_t lbn);
    uint64_t pop_freeLBNList(int ch);

    void insert_usedLBNList(uint64_t lbn);
    bool remove_usedLBNList(uint64_t lbn);
    bool get_usedLBNList(uint64_t lbn);

    template<typename T>
    uint64_t select_lbn(int,T);
    void print();
    uint64_t worst_policy();
    uint64_t RRpolicy();
    uint64_t level2CH(int level);
    uint64_t best(std::queue<int>);
    enum{
        WROSTCASE = 0,
        RR        = 1,
        LEVEL2CH  = 2,
        BEST      = 3
    };
};



#endif