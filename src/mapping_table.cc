#include "mapping_table.hh"
#include <iomanip>
#include <algorithm>

Tree tree;
void mapping::init_mapping_table(){
    uint64_t mappingPageLBN = MAPPINGLBN;
    mappingTablePerPage *mappingTablePtr = (mappingTablePerPage *)persistenceManager.readMappingTable(mappingPageLBN);
    
    while(mappingTablePtr->nextPage != 0) {
        for (int i = 0; i < mappingTablePtr->entry_num; i++) {
            mappingEntry *entry = &mappingTablePtr->entry[i];
            if (entry->lbn != 0xFFFFFFFFFFFFFFFF) {
                std::string filename(entry->fileName);
                uint64_t lbn = entry->lbn;
                mappingTable[filename] = lbn;
            }
        }
        mappingPageLBN++;
        mappingTablePtr = (mappingTablePerPage *)persistenceManager.readMappingTable(mappingPageLBN);
    }


    for (const auto& [filename ,lbn] : mappingTable) {
        int ch = LBN2CH(lbn);
        lbnPoolManager.usedLBNList[ch].push_back(lbn);
    }

    for (uint64_t lbn = 0; lbn < LBN_NUM; lbn++) {
        int ch = LBN2CH(lbn);
        auto it = std::find(lbnPoolManager.usedLBNList[ch].begin(), lbnPoolManager.usedLBNList[ch].end(), lbn);
        if (it != lbnPoolManager.usedLBNList[ch].end()) {
            lbnPoolManager.freeLBNList[LBN2CH(lbn)].push_back(lbn);
        }
    }
}

void mapping::insert_mapping(const std::string& filename, uint64_t lbn) {
    if (mappingTable.find(filename) != mappingTable.end()) {
        std::cerr << "File already exists in the mapping table , update mapping to " << lbn << "\n";
    }

    lbnPoolManager.remove_freeLBNList(mappingTable[filename]);
    mappingTable[filename] = lbn;
    lbnPoolManager.insert_usedLBNList(lbn);
    return;
}

uint64_t mapping::getLBN(const std::string& filename){
    return mappingTable[filename];
}

void mapping::remove_mapping(const std::string& filename) {
    auto it = mappingTable.find(filename);
    if (it == mappingTable.end()) {
        pr_info("File \"%s\" does not exist in the mapping table", filename.c_str());
        return; 
    }
    uint64_t lbn = it->second;
    mappingTable.erase(it);
    lbnPoolManager.remove_usedLBNList(lbn);   
    lbnPoolManager.insert_freeLBNList(lbn);    
}


void mapping::dump_mapping(mappingTablePerPage *page) {
    pr_info("Dumping mapping table to page...");
    pr_info("===== Mapping Table Page =====");
    size_t count = 0;
    for (size_t i = 0; i < page->entry_num; i++) {
        mappingEntry *entry = &page->entry[i];
        pr_info("Entry %zu: FileName: %s -> LBN: %lu", count, entry->fileName, entry->lbn);
    }
    pr_info("================================");
}

void mapping::write_sstable(hostInfo request,char *buffer){
    std::string filename = request.filename;
    int level = request.levelInfo;
    int rangeMin = request.rangeMin;
    int rangeMax = request.rangeMax;
    uint64_t lbn = lbnPoolManager.select_lbn(1,request);

}
