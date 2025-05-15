#include "mapping_table.hh"
#include <iomanip>

void mapping::init_mapping_table(){
    uint64_t mappingPageLBN = MAPPINGLBN;
    mappingTablePerPage *mappingTablePtr = (mappingTablePerPage *)persistenceManager->readMappingTable(mappingPageLBN);
    
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
        mappingTablePtr = (mappingTablePerPage *)persistenceManager->readMappingTable(mappingPageLBN);
    }


    for (const auto& pair : mappingTable) {
        lbnPoolManager.usedLBNList.insert(pair.second);
    }

    for (uint64_t lbn = 0; lbn < LBN_NUM; lbn++) {
        if (lbnPoolManager.usedLBNList.find(lbn) == lbnPoolManager.usedLBNList.end()) {
            lbnPoolManager.freeLBNList[LBN2CH(lbn)].push_back(lbn);
        }
    }
}
void mapping::print_mapping(mappingTablePerPage *page) {
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