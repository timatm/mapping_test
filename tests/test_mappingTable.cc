#include <gtest/gtest.h>
#include "../src/mapping_table.hh"

TEST(MappingTable , InsertAndGet){
    mapping mapMrg;
    mapMrg.insert("0001.sstable",0);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
}

TEST(MappingTable , InsertAndGetMultiple){
    mapping mapMrg;
    mapMrg.insert("0001.sstable",0);
    mapMrg.insert("0002.sstable",1);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.mappingTable["0002.sstable"],1);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.getLBN("0002.sstable"),1);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(1),true);
}
TEST(MappingTable , Remove){
    mapping mapMrg;
    mapMrg.insert("0001.sstable",0);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    mapMrg.remove("0001.sstable");
    EXPECT_EQ(mapMrg.mappingTable.find("0001.sstable"),mapMrg.mappingTable.end());
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),false);
}
TEST(MappingTable , RemoveMultiple){
    mapping mapMrg;
    mapMrg.insert("0001.sstable",0);
    mapMrg.insert("0002.sstable",1);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.mappingTable["0002.sstable"],1);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.getLBN("0002.sstable"),1);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(1),true);
    mapMrg.remove("0001.sstable");
    EXPECT_EQ(mapMrg.mappingTable.find("0001.sstable"),mapMrg.mappingTable.end());
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),false);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(1),true);
}
TEST(MappingTable , RemoveNonExistent){
    mapping mapMrg;
    mapMrg.insert("0001.sstable",0);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    mapMrg.remove("0002.sstable"); // Attempt to remove a non-existent LBN
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true); // Should still be present
}
