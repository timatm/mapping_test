#include "lbn_pool.hh"
#include "def.hh"
#include <unordered_set>
#include <array>
#include <vector>
#include <queue>
#include <cstdint>
#include <iostream>

// freeLBNList 操作
void LBNPool::insert_freeLBNList(uint64_t lbn) {
    uint64_t channel = LBN2CH(lbn);
    freeLBNList[channel].push_back(lbn);
}

bool LBNPool::remove_freeLBNList(uint64_t lbn) {
    uint64_t channel = LBN2CH(lbn);
    auto& deque = freeLBNList[channel];
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        if (*it == lbn) {
            deque.erase(it);
            return true;
        }
    }
    return false; // not found
}

bool LBNPool::get_freeLBNList(uint64_t lbn) {
    uint64_t channel = LBN2CH(lbn);
    auto& deque = freeLBNList[channel];
    for (const auto& val : deque) {
        if (val == lbn)
            return true;
    }
    return false;
}

uint64_t LBNPool::pop_freeLBNList(int ch){
    uint64_t lbn = freeLBNList[ch].front();
    freeLBNList[ch].pop_front();
    return lbn;
}

// usedLBNList 操作
void LBNPool::insert_usedLBNList(uint64_t lbn) {
    usedLBNList.insert(lbn);
}

bool LBNPool::remove_usedLBNList(uint64_t lbn) {
    return usedLBNList.erase(lbn) > 0;
}

bool LBNPool::get_usedLBNList(uint64_t lbn) {
    return usedLBNList.find(lbn) != usedLBNList.end();
}

// 附加：debug print
void LBNPool::print() {
    std::cout << "=== Used LBN List ===\n";
    for (auto lbn : usedLBNList)
        std::cout << lbn << " ";
    std::cout << "\n";

    std::cout << "=== Free LBN List ===\n";
    for (size_t ch = 0; ch < CHANNEL_NUM; ++ch) {
        std::cout << "Channel " << ch << ": ";
        for (auto lbn : freeLBNList[ch])
            std::cout << lbn << " ";
        std::cout << "\n";
    }
}

uint64_t LBNPool::worst_policy(){
    uint64_t lbn = INVALIDLBN;
    for (int ch = 0;ch < CHANNEL_NUM;ch++){
        if (freeLBNList[ch].size() != 0){
            lbn = pop_freeLBNList(ch);
            insert_usedLBNList(lbn);
            return lbn;
        }
    }
    return INVALIDLBN;
}

uint64_t LBNPool::RRpolicy(){
    uint64_t lbn = INVALIDLBN;
    int ch = (lastChannel + 1) % CHANNEL_NUM;
    while(lbn == INVALIDLBN){
        if(freeLBNList[ch].size() != 0){
            lbn = pop_freeLBNList(ch);
            insert_usedLBNList(lbn);
            return lbn;
        }
        ch = (ch + 1)% CHANNEL_NUM;
        if(ch == lastChannel+1){
            std::cout << "LBN pool(RRpolicy) doesn't have free LBN" << std::endl;
            
        }
        return INVALIDLBN;
    }
}

uint64_t LBNPool::level2CH(int level){
    uint64_t lbn = INVALIDLBN;
    if (level < 0 || level >= CHANNEL_NUM) {
        std::cout << "Invalid level index: " << level << std::endl;
        return INVALIDLBN;
    }
    if(freeLBNList[level].size() != 0){
        lbn = pop_freeLBNList(level);
        insert_usedLBNList(lbn);
        return lbn;
    }
    else{
        std::cout << "LBN pool(level2CH) doesn't have free LBN" << std::endl;
        
    }
    return INVALIDLBN;
}


uint64_t LBNPool::best(std::queue<int> CHpriority){
    uint64_t lbn = INVALIDLBN;
    int ch = 0;
    while(CHpriority.size() != 0){
        ch = CHpriority.front();
        CHpriority.pop();
        if(freeLBNList[ch].size() != 0){
            lbn = pop_freeLBNList(ch);
            insert_usedLBNList(lbn);
            return lbn;
        }
    }
    if(CHpriority.size() == 0){
        std::cout << "LBN pool(best) doesn't have free LBN" << std::endl;
        return INVALIDLBN;
    }
    return INVALIDLBN;
}

uint64_t LBNPool::select_lbn(int type){
    uint64_t lbn = 0;
    switch(type){
        case WROSTCASE:

        case RR:

        case LEVEL2CH:

        case BEST:

        default:
            std::cout << "The type of policy is invalid ,check your pass parameter" << std::endl;
    }
    
    return lbn;
}