#include <cstdlib>
#include <iostream>

#include "persistence.hh"
#include "mapping_table.hh"
int Persistence::readMappingTable(uint64_t lbn,uint8_t *buffer,size_t size) {
    int err;
    if (buffer == nullptr) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return OPERATION_FAILURE;
    }
    if (size != PAGE_SIZE){
        pr_debug("[ERROR] Memory allocation failed.");
        return OPERATION_FAILURE;
    }
    err = disk.read(lbn, buffer);
    if(err){
        return OPERATION_FAILURE;
    }
    return OPERATION_SUCCESS;
}

int Persistence::flushMappingTable(std::unordered_map<std::string, uint64_t>& mappingTable) {
    uint64_t lpn = 0;
    uint8_t *buffer = (uint8_t*)malloc(PAGE_SIZE);
    int err;
    if(!ENABLE_DISK){
        return OPERATION_SUCCESS;
    }
    if (!buffer) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return OPERATION_FAILURE;
    }
    memset(buffer, 0xFF, PAGE_SIZE);

    auto *page = reinterpret_cast<mappingTablePerPage*>(buffer);
    size_t idx = 0;
    for (const auto& pair : mappingTable) {
        if (idx == MAPPING_TABLE_ENTRIES){
            page->entry_num = idx;
            page->nextPage = 1;
            err = disk.write(lpn, buffer);
            memset(buffer, 0xFF, PAGE_SIZE);
            lpn++;
            idx = 0;
        }
        mappingEntry &entry = page->entry[idx++];
        strncpy(entry.fileName, pair.first.c_str(), sizeof(entry.fileName) - 1);
        entry.lbn = pair.second;
    }
    if(idx > 0) {
        page->entry_num = idx;
        page->nextPage = 1;
        err = disk.write(lpn, buffer);
    }
    free(buffer);
    return OPERATION_SUCCESS;
}

int  Persistence::readSStable(uint64_t lbn,uint8_t *buffer,size_t size){
    int err;
    if (buffer == nullptr) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return OPERATION_FAILURE;
    }
    if (size != BLOCK_SIZE){
        pr_debug("[ERROR] Memory allocation failed.");
        return OPERATION_FAILURE;
    }
    err = disk.readBlock(lbn, buffer);
    if(err){
        return OPERATION_FAILURE;
    }
    return OPERATION_SUCCESS;
}
int Persistence::writeSStable(uint64_t lbn,uint8_t *buffer,size_t size){
    int err;
    if(!ENABLE_DISK){
        return OPERATION_SUCCESS;
    }
    if (buffer == nullptr) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return OPERATION_FAILURE;
    }
    if (size != BLOCK_SIZE){
        pr_debug("[ERROR] Memory allocation failed.");
        return OPERATION_FAILURE;
    }
    disk.writeBlock(lbn, buffer);
    if(err){
        return OPERATION_FAILURE;
    }
    return OPERATION_SUCCESS;
}

int Persistence::readSStablePage(uint64_t lpn,uint8_t *buffer,size_t size){
    int err;
    if (buffer == nullptr) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return OPERATION_FAILURE;
    }
    if (size != BLOCK_SIZE){
        pr_debug("[ERROR] Memory allocation failed.");
        return OPERATION_FAILURE;
    }
    err = disk.read(lpn,buffer);
    if(err){
        return OPERATION_FAILURE;
    }
    return OPERATION_SUCCESS;
}

