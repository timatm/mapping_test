#include <gtest/gtest.h>
#include <cstring>
#include <unordered_map>
#include <string>

#include "../src/persistence.hh"
#include "../src/mapping_table.hh"
#include "../src/disk.hh"
constexpr const char* test_file = "test_disk.bin";

class PersistenceTest : public ::testing::Test {
protected:
    Persistence persistence;
    std::string filename = test_file;

    void SetUp() override {
        persistence.disk.open(filename);
    }

    void TearDown() override {
        persistence.disk.close();
        std::remove(filename.c_str());
    }

    void fill_buffer(uint8_t* buffer, uint8_t value, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            buffer[i] = value;
        }
    }
};

// 測試 writeSStable / readSStable
TEST_F(PersistenceTest, ReadWriteSStableBlock) {
    uint8_t write_buf[BLOCK_SIZE];
    uint8_t read_buf[BLOCK_SIZE];
    fill_buffer(write_buf, 0xA5, BLOCK_SIZE);

    ASSERT_EQ(persistence.flushSStable(2, write_buf, BLOCK_SIZE), OPERATION_SUCCESS);
    ASSERT_EQ(persistence.readSStable(2, read_buf, BLOCK_SIZE), OPERATION_SUCCESS);
    ASSERT_EQ(memcmp(write_buf, read_buf, BLOCK_SIZE), 0);
}

// 測試單頁讀取 readSStablePage
TEST_F(PersistenceTest, ReadWriteSStablePage) {
    uint8_t write_buf[BLOCK_SIZE];
    uint8_t read_buf[PAGE_SIZE];
    fill_buffer(write_buf, 0x3C, BLOCK_SIZE);
    
    uint8_t expect[PAGE_SIZE];
    fill_buffer(expect, 0x3C, PAGE_SIZE);

    ASSERT_EQ(persistence.flushSStable(3, write_buf, BLOCK_SIZE), 0);
    ASSERT_EQ(persistence.readSStablePage(3, read_buf, PAGE_SIZE), OPERATION_SUCCESS);
    ASSERT_EQ(memcmp(expect, read_buf, PAGE_SIZE), 0);
}

// 測試 mappingTable 的 flush / read 功能
TEST_F(PersistenceTest, ReadWriteMappingTable) {
    std::unordered_map<std::string, uint64_t> mappingTable = {
        {"file1.sst", 10},
        {"file2.sst", 11},
        {"file3.sst", 12}
    };

    ASSERT_EQ(persistence.flushMappingTable(mappingTable), OPERATION_SUCCESS);

    uint8_t buffer[PAGE_SIZE];
    ASSERT_EQ(persistence.readMappingTable(0, buffer, PAGE_SIZE), OPERATION_SUCCESS);
}
