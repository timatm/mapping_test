#include "mapping_table.hh"
#include <iomanip>
#include <algorithm>

extern Tree tree;
void mapping::init_mapping_table(){
    uint64_t mappingPageLBN = MAPPINGLBN;
    size_t size = PAGE_SIZE;
    uint8_t * buffer  = (uint8_t*)malloc(sizeof(uint8_t) * size);
    mappingTablePerPage *mappingTablePtr = (mappingTablePerPage *)persistenceManager.readMappingTable(mappingPageLBN,buffer,size);
    
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
        mappingTablePtr = (mappingTablePerPage *)persistenceManager.readMappingTable(mappingPageLBN,buffer,size);
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

int mapping::write_sstable(hostInfo request,char *buffer){
    int err = -1;
    std::string filename = request.filename;
    int level = request.levelInfo;
    int rangeMin = request.rangeMin;
    int rangeMax = request.rangeMax;
    // Check if the file already exists in the mapping table
    if (mappingTable.count(filename)) {
        pr_info("ERROR: Mapping already exists, refusing to overwrite file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }
    if (!buffer) {
        pr_info("ERROR: Null buffer provided for write to file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }
    uint64_t lbn = lbnPoolManager.select_lbn(DISPATCH_POLICY,request);
    if (lbn == INVALIDLBN) {
        pr_info("Failed to allocate LBN for file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }
    pr_info("Allocated LBN %lu for file: %s", lbn, filename.c_str());
    err = persistenceManager.disk.writeBlock(lbn, (uint8_t *)buffer);
    pr_info("Wrote data to LBN %lu for file: %s", lbn, filename.c_str());
    if(err == OPERATION_SUCCESS){
        pr_info("Write block to LBN %lu for file: %s successfully", lbn, filename.c_str());
        insert_mapping(filename, lbn);
        std::shared_ptr<TreeNode> node = std::make_shared<TreeNode>(filename, level, -1, rangeMin, rangeMax);
        node->channelInfo = LBN2CH(lbn);
        tree.insert_node(node);
        pr_info("Inserted node for file: %s at level %d, channel %d, range [%d, %d]", 
        filename.c_str(), level, node->channelInfo, rangeMin, rangeMax);
    } else {
        pr_info("Failed to write block to LBN %lu for file: %s", lbn, filename.c_str());
        return OPERATION_FAILURE;
    }
    return OPERATION_SUCCESS;
}

int mapping::read_sstable(hostInfo request, char *buffer) {
    std::string filename = request.filename;
    if (mappingTable.count(filename) == 0) {
        pr_info("ERROR: File %s not found in mapping table", filename.c_str());
        return OPERATION_FAILURE;
    }
    if (!buffer) {
        pr_info("ERROR: Null buffer provided to read file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }
    uint64_t lbn = mappingTable[filename];
    int err = persistenceManager.disk.readBlock(lbn, (uint8_t *)buffer);
    if (err == OPERATION_SUCCESS) {
        pr_info("Read data from LBN %lu for file: %s successfully", lbn, filename.c_str());
    } else {
        pr_info("Failed to read block from LBN %lu for file: %s", lbn, filename.c_str());
        return OPERATION_FAILURE;
    }
    return OPERATION_SUCCESS;
}

int mapping::search_key(int key) {
    if(key < 0){
        return OPERATION_FAILURE;
    }
    std::queue<std::shared_ptr<TreeNode>> list = tree.search_key(key);
    
}