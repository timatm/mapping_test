#include <cstdlib>
#include <iostream>

#include "persistence.hh"
#include "mapping_table.hh"
uint8_t * persistence::readMappingTable(uint64_t lbn) {
    uint8_t *buffer = (uint8_t *)malloc(PAGE_SIZE);
    if (buffer == nullptr) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return nullptr;
    }
    disk.read(lbn, buffer);
    
    return buffer;
}

void persistence::flushMappingTable(std::unordered_map<std::string, uint64_t>& mappingTable) {
    uint64_t lpn = 0;
    uint8_t *buffer = (uint8_t*)malloc(PAGE_SIZE);
    if (!buffer) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return;
    }
    memset(buffer, 0xFF, PAGE_SIZE);

    auto *page = reinterpret_cast<mappingTablePerPage*>(buffer);
    size_t idx = 0;
    for (const auto& pair : mappingTable) {
        if (idx == MAPPING_TABLE_ENTRIES){
            page->entry_num = idx;
            page->nextPage = 1;
            disk.write(lpn, buffer);
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
        disk.write(lpn, buffer);
    }
    free(buffer);
}