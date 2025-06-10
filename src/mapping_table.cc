#include "mapping_table.hh"
#include <iomanip>
#include <algorithm>

#include "persistence.hh"
#include "tree.hh"
#include "lbn_pool.hh"
int Mapping::init_mapping_table(uint64_t mappingPageLBN,uint64_t page_num){

    if(mappingPageLBN == INVALIDLBN) {
        pr_info("Invalid mapping page LBN, cannot initialize mapping table");
        return OPERATION_FAILURE;
    }
    size_t size = PAGE_SIZE;
    uint8_t *buffer  = (uint8_t*)malloc(size);
    if (!buffer) {
        pr_info("Failed to allocate buffer for mapping table");
        return OPERATION_FAILURE;
    }
    uint64_t lpn = LBN2LPN(mappingPageLBN);
    for(int page = 0;page < page_num;page++){
        mappingTablePerPage *mappingTablePtr = 
            (mappingTablePerPage *)persistenceManager.readMappingTable(lpn, buffer, size);

        if (!mappingTablePtr) {
            pr_info("Failed to read mapping table at LPN: %lu", lpn);
            free(buffer);
            return OPERATION_FAILURE;
        }

        for (int i = 0; i < mappingTablePtr->entry_num; i++) {
            mappingEntry *entry = &mappingTablePtr->entry[i];

            // Recover mapping table and SStable tree info
            if (entry->lbn != INVALIDLBN) {
                mappingTable[std::string(entry->fileName)] = entry->lbn;
                std::shared_ptr<TreeNode> node = std::make_shared<TreeNode>(entry->fileName,entry->level, entry->channel, entry->minRange, entry->maxRange);
                tree.insert_node(node);
            }
        }
        lpn++;
    }
    free(buffer);
    return OPERATION_SUCCESS;
}


void Mapping::insert_mapping(const std::string& filename, uint64_t lbn) {
    if (mappingTable.find(filename) != mappingTable.end()) {
        std::cerr << "File already exists in the mapping table , update mapping to " << lbn << "\n";
    }

    lbnPoolManager.remove_freeLBNList(mappingTable[filename]);
    mappingTable[filename] = lbn;
    lbnPoolManager.insert_usedLBNList(lbn);
    return;
}

uint64_t Mapping::getLBN(const std::string& filename){
    return mappingTable[filename];
}

void Mapping::remove_mapping(const std::string& filename) {
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


void Mapping::dump_mapping(mappingTablePerPage *page) {
    pr_info("Dumping mapping table to page...");
    pr_info("===== Mapping Table Page =====");
    size_t count = 0;
    for (size_t i = 0; i < page->entry_num; i++) {
        mappingEntry *entry = &page->entry[i];
        pr_info("Entry %zu: FileName: %s -> LBN: %lu", count, entry->fileName, entry->lbn);
    }
    pr_info("================================");
}


int Mapping::flush_mapping_table(){
    
}