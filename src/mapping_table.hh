#ifndef __MAPPING_TABLE_HH__
#define __MAPPING_TABLE_HH__
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <cstdint>

#include <vector>
#include <iostream>
#include "def.hh"
#include "lbn_pool.hh"
#include "persistence.hh"
#include "tree.hh"

class Mapping{
    LBNPool& lbnPoolManager;
public:
    Mapping(LBNPool& p) : lbnPoolManager(p) {}
    std::unordered_map<std::string, uint64_t> mappingTable;
    void init_mapping_table();

    void insert_mapping(const std::string& filename, uint64_t lbn);

    uint64_t getLBN(const std::string& filenam);

    void remove_mapping(const std::string& filename);

    void dump_mapping(mappingTablePerPage *page);

    
};

#endif // __MAPPING_TABLE_HH__