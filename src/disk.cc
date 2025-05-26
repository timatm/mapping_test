#include "disk.hh"
#include "print.hh"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <iostream>
void Disk::open(const std::string& filename) {
    file = std::fopen(filename.c_str(), "r+b");
    if (!file) {
        // If file does not exist, create it
        file = std::fopen(filename.c_str(), "w+b");
        if (!file) {
            throw std::runtime_error("Failed to open or create file: " + filename);
        }
    }

    // Calculate total blocks
    std::fseek(file, 0, SEEK_END);
    uint64_t fileSize = std::ftell(file);
    pr_info("Total LBN num: %lu", LBN_NUM);
    pr_info("Open disk success");
    std::rewind(file);
}

void Disk::close() {
    if (file) {
        std::fclose(file);
        file = nullptr;
    }
}

int Disk::read(uint64_t lpn, uint8_t * buffer) {
    if (!file) throw std::runtime_error("Disk not opened.");

    uint64_t offset = lpn * PAGE_SIZE;
    if (std::fseek(file, offset, SEEK_SET) != 0) {
        throw std::runtime_error("Seek failed.");
        return -1;
    }

    size_t readBytes = std::fread(buffer, 1, PAGE_SIZE, file);
    if (readBytes != PAGE_SIZE) {
        throw std::runtime_error("Read error or EOF.");
        return -1;
    }
    return 0;
}

int Disk::write(uint64_t lpn,const uint8_t * buffer) {
    if (!file) throw std::runtime_error("Disk not opened.");

    uint64_t offset = lpn * PAGE_SIZE;
    if (std::fseek(file, offset, SEEK_SET) != 0) {
        throw std::runtime_error("Seek failed.");
        return -1;
    }

    size_t writtenBytes = std::fwrite(buffer, 1, PAGE_SIZE, file);
    if (writtenBytes != PAGE_SIZE) {
        throw std::runtime_error("Write error.");
        return -1;
    }
    std::fflush(file);
    return 0;
}

int Disk::writeBlock(uint64_t lbn ,uint8_t *buffer) {
    uint16_t ret = 0;
    uint64_t lpn = LBN2LPN(lbn);
    if (file) {
        for(int i = 0;i < PAGE_NUM;i++){
            uint8_t *page_ptr = buffer + i * PAGE_NUM;
            lpn = lpn + i;
            uint16_t written = write(lpn, page_ptr);
            if (written != 0) {
                pr_info("[ERROR] write block failed at page: %d LPN: %lu", i, lpn);
            }
            ret += written;
        }
    }

    return ret;
}

int Disk::readBlock(uint64_t lbn ,uint8_t *buffer) {
    uint16_t ret = 0;
    uint64_t lpn = LBN2LPN(lbn);
    if (file) {
        for (int i = 0; i < PAGE_NUM; i++) {
        lpn = lpn +1;

        uint8_t *page_ptr = buffer + i * PAGE_SIZE;

        int read_result = read(lpn, page_ptr);
        if (read_result != 0) {
            pr_info("[ERROR] read block failed at page: %d LPN: %lu", i, lpn);
            break;
        }
        ret += read_result;
        }
    }

  return ret;
}


int Disk::removeSStable(uint64_t lbn) {
    if (!file) {
        std::cerr << "[ERROR] Disk not opened.\n";
        return -1;
    }

    uint64_t lpn = LBN2LPN(lbn);
    uint8_t* page_ptr = (uint8_t*)malloc(PAGE_SIZE);
    if (!page_ptr) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return -1;
    }

    // 填滿 0xFF
    memset(page_ptr, 0xFF, PAGE_SIZE);

    int ret = 0;
    for (int i = 0; i < PAGE_NUM; i++) {
        uint64_t target_lpn = lpn + i;
        try {
            write(target_lpn,page_ptr);
            ret++;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] writeSStable failed at page: " << i 
                      << " LPN: " << target_lpn 
                      << " error: " << e.what() << std::endl;
            break;
        }
    }

    free(page_ptr);
    return ret;
}



uint64_t Disk::LBN2LPN(uint64_t lbn) {
    uint64_t lpn = lbn * PAGE_NUM;
    return lpn;
}
