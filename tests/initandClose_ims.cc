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
#include "../src/print.hh"

constexpr const char* test_file = "test.bin";
class IMSInterfaceTest : public ::testing::Test {
protected:
    IMS_interface ims;
    std::string filename = test_file;
    void SetUp() override {
        for(uint64_t lbn = 0;lbn < BLOCK_NUM;lbn++){
            lbnPoolManager.insert_freeLBNList(lbn);
        }
        persistenceManager.disk.open(filename);
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

TEST_F(IMSInterfaceTest,initAndClose){
    pr_info("BBBBBBBBBBBBBBBBBBBBBB");
    uint8_t write_buf1[BLOCK_SIZE];
    uint8_t read_buf1[BLOCK_SIZE];
    uint8_t write_buf2[BLOCK_SIZE];
    uint8_t read_buf2[BLOCK_SIZE];
    uint8_t write_buf3[BLOCK_SIZE];
    uint8_t read_buf3[BLOCK_SIZE];

    std::memset(write_buf1, 0x01, BLOCK_SIZE);
    std::memset(write_buf2, 0x02, BLOCK_SIZE);
    std::memset(write_buf3, 0x03, BLOCK_SIZE);

    std::memset(read_buf1, 0x00, BLOCK_SIZE);
    std::memset(read_buf2, 0x00, BLOCK_SIZE);
    std::memset(read_buf3, 0x00, BLOCK_SIZE);

    hostInfo req1("0001.sst", 1,  5, 20);
    hostInfo req2("0002.sst", 1, 22, 45);
    hostInfo req3("0003.sst", 1, 50, 90);
    pr_info("BBBBBBBBBBBBBBBBBBBBBB");
    EXPECT_EQ(ims.write_sstable(req1,write_buf1),OPERATION_SUCCESS);
    EXPECT_EQ(ims.write_sstable(req2,write_buf2),OPERATION_SUCCESS);
    EXPECT_EQ(ims.write_sstable(req3,write_buf3),OPERATION_SUCCESS);


    pr_info("AAAAAAAAAAAAAAAAAAAAAAAAAA");
    EXPECT_EQ(ims.close_IMS(),OPERATION_SUCCESS);
    pr_info("BBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    EXPECT_EQ(ims.init_IMS(),OPERATION_SUCCESS);

    EXPECT_EQ(ims.read_sstable(req1,read_buf1),OPERATION_SUCCESS);
    EXPECT_EQ(std::memcmp(write_buf1,read_buf1,BLOCK_SIZE),0);

    EXPECT_EQ(ims.read_sstable(req2,read_buf2),OPERATION_SUCCESS);
    EXPECT_EQ(std::memcmp(write_buf2,read_buf2,BLOCK_SIZE),0);

    EXPECT_EQ(ims.read_sstable(req3,read_buf3),OPERATION_SUCCESS);
    EXPECT_EQ(std::memcmp(write_buf3,read_buf3,BLOCK_SIZE),0);

    const auto node1 = tree.find_node("0001.sst");
    const auto node2 = tree.find_node("0002.sst");
    const auto node3 = tree.find_node("0003.sst");
    EXPECT_NE(node1, nullptr);
    EXPECT_NE(node2, nullptr);
    EXPECT_NE(node3, nullptr);
    
}