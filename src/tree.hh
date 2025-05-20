#ifndef __TREE_H__
#define __TREE_H__

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "print.hh"
#include "def.hh"

struct TreeNode{
    std::string filename;
    int levelInfo;
    int channelInfo;
    int rangeMin;
    int rangeMax;
    std::unordered_map<std::string, std::unique_ptr<TreeNode> > children;
    TreeNode *parent = nullptr;
    TreeNode(std::string name, int level,int ch,int min, int max):
        filename(std::move(name)),
        levelInfo(level),
        channelInfo(ch),
        rangeMin(min),
        rangeMax(max){}
    TreeNode(std::string name, int level,int min, int max):
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
    bool node_move(TreeNode *target,TreeNode *destination);
    void insert_node(TreeNode *parent,TreeNode *node);
    TreeNode * search_insert_parent(TreeNode *node);
};





#endif