#ifndef __DEF_HH__
#define __DEF_HH__


#define CHANNEL_NUM 4
#define PACKAGE_NUM 4
#define DIE_NUM 2
#define PLANE_NUM 2
#define BLOCK_NUM 32
#define PAGE_NUM 128
#define PAGE_SIZE 16384

#define MAPPINGLBN 1

#define LBN_NUM ( CHANNEL_NUM * PACKAGE_NUM * DIE_NUM * PLANE_NUM * BLOCK_NUM )
#define LBN_SIZE ( PAGE_SIZE * PAGE_NUM )

#define INVALIDLBN 0xFFFFFFFFFFFFFFFF
#include <cstdint>
#include <cstring>

#define MAX_FILENAME_LENGTH 56

struct mappingEntry {
    char fileName[MAX_FILENAME_LENGTH];
    uint64_t lbn;

    mappingEntry() : lbn(0xFFFFFFFFFFFFFFFF) {
        memset(fileName, 0, sizeof(fileName));
    }
};
#define MAPPING_TABLE_ENTRIES ( (PAGE_SIZE / sizeof(mappingEntry))-1 ) // 16384 / 64(mapping entry) = 128 , 128 - 1(header) = 127


#pragma pack(push, 1)
struct mappingTablePerPage {
    
    union {
        uint8_t header[sizeof(mappingEntry)];
         // 完整 raw data
        struct {
            uint8_t nextPage;       // 1 byte
            uint8_t entry_num;     // 1 bytes
            uint8_t reserved[62];   // 61 bytes
        };
    };
    
    mappingEntry entry[MAPPING_TABLE_ENTRIES]; 
    uint8_t reserved[16384 - (MAPPING_TABLE_ENTRIES * sizeof(mappingEntry)) - sizeof(header)]; // 填滿16KB

    mappingTablePerPage() {
        memset(this, 0xFF, sizeof(mappingTablePerPage));
    }
};
#pragma pack(pop)

static_assert(sizeof(mappingTablePerPage) == 16384, "MappingTablePage must be 16KB");


#endif // __DEF_HH__