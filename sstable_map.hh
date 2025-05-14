#ifndef SS_MAPPING_TABLE_H
#define SS_MAPPING_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>


#include "disk.hh"
#include "def.hh"
class SSMappingTable {
public:
    void init_mapping_table();
    void recoverMappingTable(uint64_t lbn);
    int  writeSStable(char* filename);
    int  readSStable(char* filename);
    void removeSStable();


    void insert(const std::string& filename, uint64_t lbn);

    uint64_t getLBN(const std::string& filenam);

    void remove(const std::string& filename);

    void print_mapping(mappingTablePerPage *page);

private:
    std::unordered_map<std::string, uint64_t> mappingTable;
    std::unordered_set<uint64_t> usedLBNList;
    std::array<std::vector<uint64_t>, CHANNEL_NUM> freeLBNList;


    Disk disk;
    
    void selectPolicy(const std::string& filename, uint64_t lbn);
    void flushMappingTable();


    void insert_freeLBNList();
    void remove_freeLBNList();
    void get_freeLBNList();

    void insert_usedLBNList();
    void remove_usedLBNList();
    void get_usedLBNList();
};

#endif // FILE_MAPPING_TABLE_H
