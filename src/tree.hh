#ifndef __TREE_H__
#define __TREE_H__

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <queue>
#include "print.hh"
#include "def.hh"

struct TreeNode : public std::enable_shared_from_this<TreeNode>{
    std::string filename;
    int levelInfo;
    int channelInfo;
    // std::string rangeMin;
    // std::string rangeMax;
    // std::unordered_map<std::string, std::shared_ptr<TreeNode> > children;
    // std::vector<std::weak_ptr<TreeNode>> parent;
    // TreeNode(std::string name, int level,int ch,std::string min, std::string max):
    //     filename(std::move(name)),
    //     levelInfo(level),
    //     channelInfo(ch),
    //     rangeMin(min),
    //     rangeMax(max){}
    // TreeNode(std::string name, int level,std::string min, std::string max):
    //     TreeNode(std::move(name), level, -1, std::move(min), std::move(max)){}

    int rangeMin;
    int rangeMax;
    std::unordered_map<std::string, std::shared_ptr<TreeNode> > children;
    std::vector<std::weak_ptr<TreeNode>> parent;
    TreeNode(std::string name, int level,int ch,int min, int max):
        filename(std::move(name)),
        levelInfo(level),
        channelInfo(ch),
        rangeMin(min),
        rangeMax(max){}
    TreeNode(std::string name, int level,int min, int max):
        TreeNode(std::move(name), level, -1, min,max){}
};

class Tree {
public:
    std::shared_ptr<TreeNode> root;           

    Tree() :
        root(std::make_shared<TreeNode>("dummy_root", 0, -1, 0,100)) {}

    void insert_node(std::shared_ptr<TreeNode> node);
    void remove_node(std::shared_ptr<TreeNode> node);


    std::queue<std::string> search_range(std::string key);
    TreeNode * find_node(std::string filename,TreeNode *cur);
    void dumpGraph(const std::shared_ptr<TreeNode>& node,
               int indent = 0,
               std::unordered_set<const TreeNode*>* visited = nullptr);
};



#endif