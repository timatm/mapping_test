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

    parent->children[node->filename] = std::make_unique<TreeNode>(node->filename,node->levelInfo,node->channelInfo,node->rangeMin,node->rangeMax);
    

}

bool Tree::node_move(TreeNode *target,TreeNode *destination){
    if (!destination || !target) return false;

    std::unique_ptr<TreeNode> moving = std::move(target);
    source->children.erase(it);                      // 移除舊位置

    moving->parent = target;                         // 更新 parent
    target->children[childName] = std::move(moving); // 加入新位置

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
