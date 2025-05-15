#ifndef __PERSISTENCE_CC__
#define __PERSISTENCE_CC__


#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include "def.hh"
#include "disk.hh"
class persistence {

public:
    void flushMappingTable(std::unordered_map<std::string, uint64_t>&);
    uint8_t * readMappingTable(uint64_t lbn);
    Disk disk;

private:
};

#endif // __PERSISTENCE_CC__