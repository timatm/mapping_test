#include "sstable_map.hh"
#include <iostream>

void SSMappingTable::init_mapping_table(){
    uint64_t mappingPageLBN = MAPPINGLBN;
    recoverMappingTable(mappingPageLBN);
    
    for (const auto& pair : mappingTable) {
        usedLBNList.insert(pair.second);
    }

    for (uint64_t lbn = 0; lbn < LBN_NUM; lbn++) {
        if (usedLBNList.find(lbn) == usedLBNList.end()) {
            freeLBNList.push_back(lbn);
        }
    }
}


void SSMappingTable::recoverMappingTable(uint64_t lbn) {
    uint8_t *buffer = (uint8_t *)malloc(PAGE_SIZE);
    if (buffer == nullptr) {
        std::cerr << "[ERROR] Memory allocation failed.\n";
        return;
    }
    disk.read(lbn, buffer);
    mappingTablePerPage *page = (mappingTablePerPage *)buffer;
    for(int i = 0; i < page->entry_num; i++) {
        mappingEntry *entry = &page->entry[i];
        if (entry->lbn != 0xFFFFFFFFFFFFFFFF) {
            std::string filename(entry->fileName);
            uint64_t lbn = entry->lbn;
            mappingTable[filename] = lbn;
        }
    }
    if(page->nextPage != 0xFF) {
        recoverMappingTable(lbn+1);
    }
}

void SSMappingTable::flushMappingTable() {
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

void SSMappingTable::insert(const std::string& filename, uint64_t lbn) {
    if (lbn >= totalBlocks) {
        std::cerr << "[ERROR] insert_mapping_entry: lbn out of range. lbn=" << lbn << "\n";
        return;
    }
    auto it = mappingTable.find(filename);
    if (it != mappingTable.end()) {
        std::cout << "[INFO] File " << filename << " already exists. Overwriting lbn.\n";
        
        it->second = lbn;
        
    } else {
        mappingTable[filename] = lbn;
        std::cout << "[INFO] Inserted file " << filename << " with lbn " << lbn << ".\n";
    }    
}
void SSMappingTable::remove(const std::string& filename) {
    mappingTable.erase(filename);
}

uint64_t SSMappingTable::getLBN(const std::string& filename) {
    auto it = mappingTable.find(filename);
    if (it != mappingTable.end()) {
        return it->second;
    } else {
        std::cerr << "[ERROR] File " << filename << " not found.\n";
        return INVALIDLBN;
    }
}

int SSMappingTable::writeSStable(char* filename){

}






void SSMappingTable::print_mapping(mappingTablePerPage *page) {
    std::cout << "===== Mapping Table Page =====\n";
    size_t count = 0;
    for (size_t i = 0; i < page->entry_num; i++) {
        mappingEntry *entry = &page->entry[i];
        std::cout << " [" << std::setw(3) << count++ << "] "
                  << "FileName: " << entry->fileName
                  << " | LBN: " << entry->lbn << "\n";
    }

    std::cout << "================================\n";
}
