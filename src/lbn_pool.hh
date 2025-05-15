#ifndef __LBN_POOL_HH__
#define __LBN_POOL_HH__


#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <cstdint>
#include <vector>

#include "def.hh"

class LBNPool {
public:
    std::unordered_set<uint64_t> usedLBNList;
    std::array<std::vector<uint64_t>, CHANNEL_NUM> freeLBNList;
    void insert_freeLBNList(uint64_t lbn);
    bool remove_freeLBNList(uint64_t lbn);
    bool get_freeLBNList(uint64_t lbn);

    void insert_usedLBNList(uint64_t lbn);
    bool remove_usedLBNList(uint64_t lbn);
    bool get_usedLBNList(uint64_t lbn);

    void print();
};



#endif