#include "IMS_interface.hh"

Tree tree;
Mapping mappingManager(lbnPoolManager);

int IMS_interface::write_sstable(hostInfo request,uint8_t *buffer){
    int err = OPERATION_FAILURE;

    std::string filename = request.filename;
    int level = request.levelInfo;
    int rangeMin = request.rangeMin;
    int rangeMax = request.rangeMax;
    // Check if the file already exists in the mapping table
    if (mappingManager.mappingTable.count(filename)) {
        pr_info("ERROR: Mapping already exists, refusing to overwrite file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }
    if (!buffer) {
        pr_info("ERROR: Null buffer provided for write to file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }

    // select_lbn() has been inserted the node ,so if operation isn't success ,the node need to remove from tree
    uint64_t lbn = lbnPoolManager.select_lbn(DISPATCH_POLICY,request);
    auto node = tree.find_node(filename,level,rangeMin,rangeMax);
    if (lbn == INVALIDLBN) {
        tree.remove_node(node);
        pr_info("Failed to allocate LBN for file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }
    if(!node){
        pr_info("Find node is error,filename: %s ", filename.c_str());
        return OPERATION_FAILURE;
    }
    pr_info("Allocated LBN %lu for file: %s", lbn, filename.c_str());
    err = persistenceManager.writeSStable(lbn, (uint8_t *)buffer, BLOCK_SIZE);
    pr_info("Wrote data to LBN %lu for file: %s", lbn, filename.c_str());

    if(err == OPERATION_SUCCESS){
        pr_info("Write block to LBN %lu for file: %s successfully", lbn, filename.c_str());
        node->channelInfo = LBN2CH(lbn);
        mappingManager.insert_mapping(filename, lbn);
    } 
    else {
        tree.remove_node(node);
        pr_info("Failed to write block to LBN %lu for file: %s", lbn, filename.c_str());
        return OPERATION_FAILURE;
    }

    return OPERATION_SUCCESS;
}

int IMS_interface::read_sstable(hostInfo request, uint8_t *buffer) {
    std::string filename = request.filename;
    if (mappingManager.mappingTable.count(filename) == 0) {
        pr_info("ERROR: File %s not found in mapping table", filename.c_str());
        return OPERATION_FAILURE;
    }
    if (!buffer) {
        pr_info("ERROR: Null buffer provided to read file: %s", filename.c_str());
        return OPERATION_FAILURE;
    }
    uint64_t lbn = mappingManager.mappingTable[filename];
    int err =  persistenceManager.readSStable(lbn, (uint8_t *)buffer, BLOCK_SIZE);

    if (err == OPERATION_SUCCESS) {
        pr_info("Read data from LBN %lu for file: %s successfully", lbn, filename.c_str());
    } else {
        pr_info("Failed to read block from LBN %lu for file: %s", lbn, filename.c_str());
        return OPERATION_FAILURE;
    }
    return OPERATION_SUCCESS;
}

int IMS_interface::search_key(int key) {
    if(key < 0){
        return OPERATION_FAILURE;
    }
    // The queue is sorted from low to high level.
    std::queue<std::shared_ptr<TreeNode>> list = tree.search_key(key);
}


// TODO now is not complete still need to finish  
// This function is used to allocate a block for value log
int IMS_interface::allocate_block(hostInfo request){
    uint64_t lbn;
    lbn = lbnPoolManager.allocate_valueLog_block();
    if(lbn == INVALIDLBN){
        pr_info("Allocate value log block is failed, need to check policy or free block list");
        return OPERATION_FAILURE;
    }

    return OPERATION_SUCCESS;
}


// TODO now not complete still need to modify
int IMS_interface::write_log(valueLogInfo request,uint8_t *buffer){
    int err = OPERATION_FAILURE;
    uint64_t lpn = LBN2LPN(request.lbn) + request.page_offset;
    err = disk.write(lpn,buffer);
    if( err != OPERATION_SUCCESS){
        pr_info("Write value log is fail");
    }
    return err;
}
int IMS_interface::read_log(valueLogInfo request,uint8_t *buffer){
    int err = OPERATION_FAILURE;
    uint64_t lpn = LBN2LPN(request.lbn) + request.page_offset;
    err = disk.read(lpn,buffer);
    if( err != OPERATION_SUCCESS){
        pr_info("Write value log is fail");
    }
    return err;
}