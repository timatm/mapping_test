#ifndef __IMS_INTERFACE__HH__
#define  __IMS_INTERFACE__HH__


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
#include "mapping_table.hh"
#include "print.hh"


class IMS_interface{
public:
    
    int write_sstable(hostInfo request,uint8_t *buffer);
    int read_sstable(hostInfo request ,uint8_t *buffer);
    int search_key(int key);
    int allocate_block(hostInfo request);
    int write_log(valueLogInfo request,uint8_t *buffer);
    int read_log(valueLogInfo request,uint8_t *buffer); 
private:

};



#endif