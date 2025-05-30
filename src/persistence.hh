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
    Disk disk;
    void flushMappingTable(std::unordered_map<std::string, uint64_t>&);
    mappingTablePerPage * readMappingTable(uint64_t lbn,uint8_t *buffer,size_t size);
    SStableFormat * readSStable(uint64_t lbn,uint8_t *buffer,size_t size);
    int writeSStable(uint64_t lbn,uint8_t *buffer,size_t size);
    pageFormat * readSStablePage(uint64_t lpn,uint8_t *buffer,size_t size);
    // TODO log(blob log) need to add 
private:
};

#endif // __PERSISTENCE_CC__