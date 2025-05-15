#ifndef __DEF_HH__
#define __DEF_HH__


#define CHANNEL_NUM 4
#define PACKAGE_NUM 4
#define DIE_NUM 2
#define PLANE_NUM 2
#define BLOCK_NUM 32
#define PAGE_NUM 128
#define PAGE_SIZE 16384

#define LOG2_CEIL(x) ( \
    ((x) <= 1) ? 0 : \
    ((x) <= 2) ? 1 : \
    ((x) <= 4) ? 2 : \
    ((x) <= 8) ? 3 : \
    ((x) <= 16) ? 4 : \
    ((x) <= 32) ? 5 : \
    ((x) <= 64) ? 6 : \
    ((x) <= 128) ? 7 : \
    ((x) <= 256) ? 8 : \
    ((x) <= 512) ? 9 : \
    ((x) <= 1024) ? 10 : -1)

#define CHANNEL_BITS LOG2_CEIL(CHANNEL_NUM)
#define PACKAGE_BITS LOG2_CEIL(PACKAGE_NUM)
#define DIE_BITS     LOG2_CEIL(DIE_NUM)
#define PLANE_BITS   LOG2_CEIL(PLANE_NUM)
#define BLOCK_BITS   LOG2_CEIL(BLOCK_NUM)

#define MAPPINGLBN 1

#define LBN_NUM ( CHANNEL_NUM * PACKAGE_NUM * DIE_NUM * PLANE_NUM * BLOCK_NUM )
#define LBN_SIZE ( PAGE_SIZE * PAGE_NUM )

#define LBN2PLANE(LBA)  ( (LBA >> BLOCK_BITS) % PLANE_NUM )
#define LBN2DIE(LBA)    ( (LBA >> (PLANE_BITS + BLOCK_BITS)) % DIE_NUM )
#define LBN2PACKAGE(LBA)( (LBA >> (DIE_BITS + PLANE_BITS + BLOCK_BITS)) % PACKAGE_NUM )
#define LBN2CH(LBA)     ( (LBA >> (PACKAGE_BITS + DIE_BITS + PLANE_BITS + BLOCK_BITS)) % CHANNEL_NUM )



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
            uint8_t reserved1[62];   // 61 bytes
        };
    };
    
    mappingEntry entry[MAPPING_TABLE_ENTRIES]; 
    uint8_t reserved2[PAGE_SIZE - (MAPPING_TABLE_ENTRIES * sizeof(mappingEntry)) - sizeof(header)]; // 填滿16KB

    mappingTablePerPage() {
        memset(this, 0xFF, sizeof(mappingTablePerPage));
    }
};
#pragma pack(pop)

static_assert(sizeof(mappingTablePerPage) == 16384, "MappingTablePage must be 16KB");


#endif // __DEF_HH__