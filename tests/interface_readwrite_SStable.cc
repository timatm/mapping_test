#include <gtest/gtest.h>
#include <cstring>
#include <string>
#include <memory>

#include "../src/def.hh"
#include "../src/IMS_interface.hh"
#include "../src/mapping_table.hh"
#include "../src/persistence.hh"
#include "../src/lbn_pool.hh"
#include "../src/tree.hh"
#include "../src/IMS_interface.cc"
#include "../src/mapping_table.cc"


constexpr const char* test_file = "test.bin";
// 測試專用 Fixture 類別
class IMSInterfaceTest : public ::testing::Test {
protected:
    IMS_interface ims;
    std::string filename = test_file;
    void SetUp() override {
        persistenceManager.disk.open(filename);
        lbnPoolManager.init_lbn_pool();
        
        tree = Tree();  // 重置整棵樹
        
    }

    void TearDown() override {
        persistenceManager.disk.close();
        std::remove(filename.c_str());
    }
    hostInfo make_request(const std::string& filename, int level, int rmin, int rmax) {
        hostInfo h(filename, level, -1, rmin, rmax);
        return h;
    }
};

// ✅ 成功寫入再讀取
TEST_F(IMSInterfaceTest, WriteThenReadSuccess) {
    uint8_t write_buf[BLOCK_SIZE];
    uint8_t read_buf[BLOCK_SIZE];
    std::memset(write_buf, 0x42, BLOCK_SIZE);
    std::memset(read_buf, 0x00, BLOCK_SIZE);
    std::string test_filename = "001.sst";
    hostInfo req = make_request(test_filename, 1, 0, 10);
    ASSERT_EQ(ims.write_sstable(req, write_buf), OPERATION_SUCCESS);
    ASSERT_EQ(ims.read_sstable(req, read_buf), OPERATION_SUCCESS);
    ASSERT_EQ(std::memcmp(write_buf, read_buf, BLOCK_SIZE), 0);
    ASSERT_EQ(req.lbn, mappingManager.getLBN(test_filename));
    EXPECT_TRUE(lbnPoolManager.get_usedLBNList(req.lbn));
    EXPECT_FALSE(lbnPoolManager.get_freeLBNList(req.lbn));
    auto node = tree.find_node(test_filename);
    EXPECT_TRUE(node != nullptr);
}

// ❌ buffer 為 nullptr 時寫入失敗
TEST_F(IMSInterfaceTest, WriteNullBufferFails) {
    std::string test_filename = "001.sst";
    hostInfo req = make_request(test_filename, 1, 0, 10);
    ASSERT_EQ(ims.write_sstable(req, nullptr), OPERATION_FAILURE);
}


// ❌ buffer 為 nullptr 時讀取失敗
TEST_F(IMSInterfaceTest, ReadNullBufferFails) {
    std::string test_filename = "001.sst";
    hostInfo req = make_request(test_filename, 1, 0, 10);
    mappingManager.mappingTable[test_filename] = 1234; // 模擬已存在 entry
    ASSERT_EQ(ims.read_sstable(req, nullptr), OPERATION_FAILURE);
}

// ❌ 讀取不存在的 mapping 項目
TEST_F(IMSInterfaceTest, ReadNonExistentFileFails) {
    hostInfo req = make_request("nonexistent.sst", 0, 0, 0);
    uint8_t buffer[BLOCK_SIZE];
    ASSERT_EQ(ims.read_sstable(req, buffer), OPERATION_FAILURE);
}

// ❌ 重複寫入相同 filename，應該失敗（mappingTable 已存在）
TEST_F(IMSInterfaceTest, DuplicateWriteFails) {
    uint8_t buffer[BLOCK_SIZE];
    std::memset(buffer, 0x66, BLOCK_SIZE);
    std::string test_filename = "001.sst";
    hostInfo req = make_request(test_filename, 1, 0, 10);

    ASSERT_EQ(ims.write_sstable(req, buffer), OPERATION_SUCCESS);
    ASSERT_EQ(ims.write_sstable(req, buffer), OPERATION_FAILURE); // 第二次應該失敗
}
