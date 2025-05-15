#include "lbn_pool.hh"
#include "def.hh"
#include <unordered_set>
#include <array>
#include <vector>
#include <cstdint>
#include <iostream>

// freeLBNList 操作
void LBNPool::insert_freeLBNList(uint64_t lbn) {
    uint64_t channel = LBN2CH(lbn);
    freeLBNList[channel].push_back(lbn);
}

bool LBNPool::remove_freeLBNList(uint64_t lbn) {
    uint64_t channel = LBN2CH(lbn);
    auto& vec = freeLBNList[channel];
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it == lbn) {
            vec.erase(it);
            return true;
        }
    }
    return false; // not found
}

bool LBNPool::get_freeLBNList(uint64_t lbn) {
    uint64_t channel = LBN2CH(lbn);
    auto& vec = freeLBNList[channel];
    for (const auto& val : vec) {
        if (val == lbn)
            return true;
    }
    return false;
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
