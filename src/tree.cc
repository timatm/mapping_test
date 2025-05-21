#include "tree.hh"


void Tree::insert_tree(std::unique_ptr<TreeNode> subtree) {
    std::string filename = subtree->filename;
    subtree->parent = root.get();
    root->children[filename] = std::move(subtree);
}

void Tree::insert_node(TreeNode *parent,TreeNode *node){
    TreeNode* cur = parent;
    std::vector<TreeNode*> moveList;
    for (auto& [filename, upChild] : cur->children) 
    {
        TreeNode* child = upChild.get();
        if (!child) continue;

        if (child->rangeMin <= node->rangeMin &&
            child->rangeMax >= node->rangeMax && 
            cur->levelInfo == (node->levelInfo-1) )  
        {
            moveList.push_back(cur);
        }
    }
    std::unique_ptr<TreeNode> new_node = std::make_unique<TreeNode>(node->filename,node->levelInfo,node->channelInfo,node->rangeMin,node->rangeMax);
    for(auto& child : moveList) 
    {
        std::string filename = child->filename;
        child->parent = node;
        new_node->children[filename] = std::move(child->children[filename]);
    }
    parent->children[node->filename] = std::move(new_node);
    
}

bool Tree::node_move(std::unique_ptr<TreeNode>* target, std::unique_ptr<TreeNode>* destination) {
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
