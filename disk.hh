#ifndef __DISK_H__
#define __DISK_H__
#include <string>
#include <unordered_map>
#include <cstdint>
#include "def.hh"


class Disk {
public:
    void open(const std::string& filename);
    void close();
    int read(uint64_t lpn, uint8_t * buffer);
    int write(uint64_t lpn, const uint8_t * buffer);
    int writeSStable(uint64_t lbn,uint8_t *buffer);
    int readSStable(uint64_t lbn,uint8_t *buffer);
    int removeSStable(uint64_t lbn);
private:
    FILE* file = nullptr;
    uint64_t pageSize = PAGE_SIZE; // Default block size
    uint64_t blockSize = PAGE_SIZE * PAGE_NUM;
    uint64_t LBN2LPN(uint64_t lbn);
};
#endif // __DISK_H__
extern Disk disk;
// #define CHANNEL_NUM 8
// #define PACKAGE_NUM 4
// #define DIE_NUM 2
// #define PLANE_NUM 2
// #define BLOCK_NUM 64
// #define PAGE_NUM 128
// #define PAGE_SIZE 16384