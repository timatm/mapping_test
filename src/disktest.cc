#include "disk.hh"
#include "def.hh"
#include "disk.cc"
#include <iostream>
#include <vector>
#include <cstring>

void print_buffer(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main(int argc, char** argv) {
    Disk disk;
    const std::string filename = "test.disk";
    const uint64_t test_lpn = 0;
    const uint64_t test_lbn = 1;

    // 打开磁盘
    disk.open(filename);

    // 创建写入缓冲区
    std::vector<uint8_t> write_buffer(PAGE_SIZE, 0xAB); // 填充 0xAB
    std::vector<uint8_t> read_buffer(PAGE_SIZE, 0x00);

    std::cout << "Testing write..." << std::endl;
    disk.write(test_lpn, write_buffer.data());

    std::cout << "Testing read..." << std::endl;
    disk.read(test_lpn, read_buffer.data());
    print_buffer(read_buffer.data(), PAGE_SIZE);

    // std::cout << "Testing writeBlock..." << std::endl;
    // std::vector<uint8_t> write_block(PAGE_SIZE * PAGE_NUM, 0xBB); // 整个 block 填 0xBB
    // disk.writeBlock(test_lbn, write_block.data());

    // std::cout << "Testing readBlock..." << std::endl;
    // std::vector<uint8_t> read_block(PAGE_SIZE * PAGE_NUM, 0x00);
    // disk.readBlock(test_lbn, read_block.data());
    // print_buffer(read_block.data(), PAGE_SIZE * PAGE_NUM);

    std::cout << "Testing removeSStable..." << std::endl;
    disk.removeSStable(test_lbn);

    // std::cout << "Verifying removal (readBlock again)..." << std::endl;
    // disk.readBlock(test_lbn, read_block.data());
    // print_buffer(read_block.data(), PAGE_SIZE * PAGE_NUM);

    disk.close();
    std::cout << "Test completed." << std::endl;
    return 0;
}
