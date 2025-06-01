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

#include "tree.hh"
#include "def.hh"
#include "print.hh"
class LBNPool {
public:
    std::array<std::deque<uint64_t>, CHANNEL_NUM> usedLBNList;
    std::array<std::deque<uint64_t>, CHANNEL_NUM> freeLBNList;
    std::queue<uint64_t> valueLogList;
    int lastUsedChannel;

    void init_lbn_pool();
    void insert_freeLBNList(uint64_t lbn);
    bool remove_freeLBNList(uint64_t lbn);
    bool get_freeLBNList(uint64_t lbn);
    uint64_t pop_freeLBNList(int ch);

    void insert_usedLBNList(uint64_t lbn);
    bool remove_usedLBNList(uint64_t lbn);
    bool get_usedLBNList(uint64_t lbn);

    void insert_valueLogList(uint64_t lbn);
    void remove_valueLogList(uint64_t lbn);
    uint64_t allocate_valueLog_block();

    std::array<int,CHANNEL_NUM> calculate_channel_usage(std::queue<std::shared_ptr<TreeNode>>);
    template<typename T>
    uint64_t select_lbn(int type,T info){
        uint64_t lbn = 0;
        switch(type){
            case WROSTCASE:
                lbn = worst_policy();
                break;
            case RR:
                lbn = RRpolicy();
                break;
            case LEVEL2CH:
                lbn = level2CH(info);
                break;
            case MYPOLICY:
                lbn = my_policy(info);
                break;
            default:
                pr_info("The type of policy is invalid ,check your pass parameter");
                return INVALIDLBN;
        }
        
        return lbn;
    }
    void print();
    uint64_t worst_policy();
    uint64_t RRpolicy();
    uint64_t level2CH(hostInfo info);
    uint64_t my_policy(hostInfo info);
    enum{
        WROSTCASE = 0,
        RR        = 1,
        LEVEL2CH  = 2,
        MYPOLICY      = 3
    };
};

extern LBNPool lbnPoolManager; 

#endif