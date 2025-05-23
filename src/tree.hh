#ifndef __TREE_H__
#define __TREE_H__

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include "print.hh"
#include "def.hh"

struct TreeNode{
    std::string filename;
    int levelInfo;
    int channelInfo;
    std::string rangeMin;
    std::string rangeMax;
    std::unordered_map<std::string, std::unique_ptr<TreeNode> > children;
    TreeNode *parent = nullptr;
    TreeNode(std::string name, int level,int ch,std::string min, std::string max):
        filename(std::move(name)),
        levelInfo(level),
        channelInfo(ch),
        rangeMin(min),
        rangeMax(max){}
    TreeNode(std::string name, int level,std::string min, std::string max):
        filename(std::move(name)),
        levelInfo(level),
        channelInfo(-1),
        rangeMin(min),
        rangeMax(max){}
};

class Tree {
public:
    std::unique_ptr<TreeNode> root;      
    static int id_counter;                   

    Tree() :
        root(std::make_unique<TreeNode>("dummy_root", 0, -1, -1, -1, ++id_counter)) {}

    void insert_tree(std::unique_ptr<TreeNode> subtree);
    bool move_node(std::unique_ptr<TreeNode>* target, std::unique_ptr<TreeNode>* destination);
    void insert_node(TreeNode *node);
    void insert_node2parent(TreeNode *parent,TreeNode *node);
    void remove_node(TreeNode *node);


    std::queue<std::string> search_range(std::string key);
    TreeNode * find_node(std::string filename,TreeNode *cur);
    TreeNode * search_insert_parent(TreeNode *node);
    void dumpTree(const TreeNode* node, int indent);
};



#endif