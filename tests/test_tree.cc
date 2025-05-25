#include <gtest/gtest.h>

#include "../src/tree.hh"

TEST(TreeTest, InsertTest){
    Tree* treeMgr = new Tree();
    TreeNode *s1 = new TreeNode("0001.sstable",1,"ab","cdd");
    TreeNode *s2 = new TreeNode("0002.sstable",1,"cff","eef");
    TreeNode *s3 = new TreeNode("0003.sstable",2,"a","agg");
    TreeNode *s4 = new TreeNode("0004.sstable",2,"ah","az");
    TreeNode *s5 = new TreeNode("0005.sstable",2,"ca","d");
    TreeNode *s6 = new TreeNode("0006.sstable",3,"ab","aef");
    TreeNode *s7 = new TreeNode("0007.sstable",3,"cbe","cgg");
    treeMgr->insert_node(s1);
    treeMgr->insert_node(s2);
    treeMgr->insert_node(s3);
    treeMgr->insert_node(s4);
    treeMgr->insert_node(s5);
    treeMgr->insert_node(s6);
    treeMgr->insert_node(s7);
    TreeNode *root = treeMgr->root.get();
    for(auto &[filename,child] :root->children){

    }
}