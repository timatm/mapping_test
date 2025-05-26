#include <gtest/gtest.h>
#include "../src/mapping_table.hh"

TEST(MappingTable , insert_mappingAndGet){
    mapping mapMrg;
    mapMrg.insert_mapping("0001.sstable",0);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
}

TEST(MappingTable , insert_mappingAndGetMultiple){
    mapping mapMrg;
    mapMrg.insert_mapping("0001.sstable",0);
    mapMrg.insert_mapping("0002.sstable",1);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.mappingTable["0002.sstable"],1);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.getLBN("0002.sstable"),1);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(1),true);
}
TEST(MappingTable , Remove){
    mapping mapMrg;
    mapMrg.insert_mapping("0001.sstable",0);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    mapMrg.remove_mapping("0001.sstable");
    EXPECT_EQ(mapMrg.mappingTable.find("0001.sstable"),mapMrg.mappingTable.end());
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),false);
}
TEST(MappingTable , remove_mappingMultiple){
    mapping mapMrg;
    mapMrg.insert_mapping("0001.sstable",0);
    mapMrg.insert_mapping("0002.sstable",1);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.mappingTable["0002.sstable"],1);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.getLBN("0002.sstable"),1);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(1),true);
    mapMrg.remove_mapping("0001.sstable");
    EXPECT_EQ(mapMrg.mappingTable.find("0001.sstable"),mapMrg.mappingTable.end());
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),false);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(1),true);
}
TEST(MappingTable , remove_mappingNonExistent){
    mapping mapMrg;
    mapMrg.insert_mapping("0001.sstable",0);
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.getLBN("0001.sstable"),0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true);
    mapMrg.remove_mapping("0002.sstable"); // Attempt to remove a non-existent LBN
    EXPECT_EQ(mapMrg.mappingTable["0001.sstable"],0);
    EXPECT_EQ(mapMrg.lbnPoolManager.get_usedLBNList(0),true); // Should still be present
}
