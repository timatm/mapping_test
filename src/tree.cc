#include "tree.hh"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>


void Tree::insert_tree(std::unique_ptr<TreeNode> subtree) {
    std::string filename = subtree->filename;
    subtree->parent = root.get();
    root->children[filename] = std::move(subtree);
}

void Tree::insert_node(TreeNode *node){
    if( !node ) return;
    TreeNode *parent = search_insert_parent(node);
    insert_node2parent (parent,node);
}

void Tree::insert_node2parent(TreeNode *parent, TreeNode *node) {
    std::vector<std::string> moveKeys;

    // Check all children in parent node
    // Check whether the key range and level are consistent with being the son of the new node
    for (auto& [filename, upChild] : parent->children) {
        TreeNode* child = upChild.get();
        if (!child) continue;

        if (child->rangeMin >= node->rangeMin &&
            child->rangeMax <= node->rangeMax &&
            parent->levelInfo == node->levelInfo - 1) 
        {
            moveKeys.push_back(filename);
        }
    }

    std::unique_ptr<TreeNode> new_node = std::make_unique<TreeNode>(
        node->filename, node->levelInfo, node->channelInfo, node->rangeMin, node->rangeMax);
    // Move all childl to new node's children
    for (const auto& filename : moveKeys) {
        auto& upChild = parent->children[filename];
        TreeNode* child = upChild.get();
        child->parent = new_node.get();
        new_node->children[filename] = std::move(upChild);
    }

    parent->children[node->filename] = std::move(new_node);
}

void Tree::remove_node(TreeNode *node) {
    if (!node) return;
    TreeNode* parent = node->parent;
    if (!parent) return; 

    std::vector<std::string> moveKeys;
    for (const auto& [filename, child] : node->children) {
        moveKeys.push_back(filename);
    }

    for (const auto& filename : moveKeys) {
        auto& upChild = node->children[filename];
        TreeNode* child = upChild.get();
        if (!child) continue;
        child->parent = parent;
        parent->children[child->filename] = std::move(upChild);
    }

    parent->children.erase(node->filename);

}



bool Tree::move_node(std::unique_ptr<TreeNode>* target, std::unique_ptr<TreeNode>* destination) {
    if (!target || !destination || !*target || !*destination) return false;

    std::string key = (*target)->filename;
    TreeNode *parent = (*target)->parent;

    (*target)->parent = destination->get();

    (*destination)->children[key] = std::move(*target);

    if (parent) {
        parent->children.erase(key);
    }
    return true;
}


TreeNode * Tree::find_node(std::string filename){
    
    for(auto& [filename , child] :root->children){
        TreeNode *cur = root.get();
        
    }
}

TreeNode* Tree::search_insert_parent(TreeNode* node)
{
    if (!root) return nullptr;
    TreeNode* cur = root.get();

    while (cur && cur->levelInfo < node->levelInfo)
    {
        bool found = false;

        for (auto& [name, upChild] : cur->children) 
        {
            TreeNode* child = upChild.get();
            if (!child) continue;

            if (child->rangeMin <= node->rangeMin &&
                child->rangeMax >= node->rangeMax)  
            {
                cur   = child;
                found = true;
                break;
            }
        }

        if (!found) break;
    }
    return cur; 
}



void Tree::dumpTree(const TreeNode* node, int indent = 0) {
    if (!node) return;

    // 印出縮排與節點資訊
    std::cout << std::string(indent * 2, ' ')
              << "- " << node->filename
              << " (Level: " << node->levelInfo
              << ", Channel: " << node->channelInfo
              << ", Range: [" << node->rangeMin << ", " << node->rangeMax << "])"
              << std::endl;

    // 遞迴處理子節點
    for (const auto& [key, childPtr] : node->children) {
        dumpTree(childPtr.get(), indent + 1);
    }
}
