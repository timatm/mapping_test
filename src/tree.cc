#include "tree.hh"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
extern Tree tree;
void Tree::insert_node(std::shared_ptr<TreeNode> node) {
    int err = 0;

    
    std::queue<std::shared_ptr<TreeNode>> checkList;    // Record the points that need to be checked
    std::unordered_set<TreeNode*> visited;              // Record the points that have visited
    std::unordered_set<TreeNode*> candidateSet;         // Record the points that is the candidate parents

    checkList.push(root);

    while (!checkList.empty()) {
        std::shared_ptr<TreeNode> cur = checkList.front();
        checkList.pop();

        if (!cur || visited.count(cur.get())) continue;
        visited.insert(cur.get());

        for (auto& [filename, child] : cur->children) {
            if (!child) continue;

            // 範圍必須包含，並且層級要比 node 小
            if (child->rangeMin <= node->rangeMax &&
                child->rangeMax >= node->rangeMin) {
                if(child->children.empty() || child->levelInfo == node->levelInfo-1){
                    candidateSet.insert(child.get());
                }
                
            }

            // 若層級還未到底，可以繼續往下搜尋
            if (child->levelInfo < node->levelInfo - 1) {
                checkList.push(child);
            }
        }
    }

    if (candidateSet.empty()) {
        root->children[node->filename] = node;
        node->parent.push_back(root);
        return;
    }


    for (TreeNode* parentNode : candidateSet) {
        auto parent = parentNode->shared_from_this();
        parent->children[node->filename] = node;
        node->parent.push_back(std::weak_ptr<TreeNode>(parent));  // 多父母情況
    }

    return ;
}

// need test
void Tree::remove_node(std::shared_ptr<TreeNode> node) {
    if (!node) return;

    // 1️⃣ 遍歷子節點，向上推給符合範圍的父節點
    for (auto& [childName, child] : node->children) {
        if (!child) continue;

        for (auto& wp : node->parent) {
            if (auto parent = wp.lock()) {
                // 判斷 key range 是否包含 child
                if (parent->rangeMin <= child->rangeMin &&
                    parent->rangeMax >= child->rangeMax) {

                    // 父節點接管這個子節點
                    parent->children[child->filename] = child;

                    // 確保 child 不會重複記錄 parent
                    bool alreadyLinked = false;
                    for (auto& wpp : child->parent) {
                        if (auto p = wpp.lock(); p && p.get() == parent.get()) {
                            alreadyLinked = true;
                            break;
                        }
                    }
                    if (!alreadyLinked) {
                        child->parent.push_back(parent);
                    }
                }
            }
        }
    }

    // 2️⃣ 將 node 從所有父親的 children 中移除
    for (auto& wp : node->parent) {
        if (auto parent = wp.lock()) {
            parent->children.erase(node->filename);
        }
    }

    // 3️⃣ 清空 node 對父母與子節點的連結（會自動釋放 if ref_count == 0）
    node->parent.clear();
    node->children.clear();
}

std::queue<std::shared_ptr<TreeNode>> Tree::search_key(int key) {
    std::queue<std::shared_ptr<TreeNode>> list;
    std::shared_ptr<TreeNode> cur = root;
    int level = 1;
    while (cur) {
        bool found = false;

        for (auto& [filename, child] : cur->children) {
            if (child->rangeMin <= key && child->rangeMax >= key &&child->levelInfo == level) {
                list.push(child);
                cur = child;
                found = true;
                level++;
                break;
            }
        }

        if (!found) break; 
    }

    return list;
}


std::shared_ptr<TreeNode> Tree::find_node(std::string filename, std::shared_ptr<TreeNode> cur){
    if (!cur) return nullptr;
    if(cur->filename == filename) return cur;

    for (auto& [name, upChild] : cur->children) {
        if (!upChild) continue;
        auto found = find_node(filename, upChild);
        if (found) return found;
    }
    return nullptr;
}

std::queue<std::shared_ptr<TreeNode>> Tree::search_key_range(int min,int max){
    std::queue<std::shared_ptr<TreeNode>> list;
    std::queue<std::shared_ptr<TreeNode>> checkList;
    std::unordered_set<TreeNode*> visited;
    checkList.push(root);

    while (!checkList.empty()) {
        std::shared_ptr<TreeNode> cur = checkList.front();
        checkList.pop();
        if (!cur || visited.count(cur.get())) continue;
        visited.insert(cur.get());

        if (cur->rangeMin <= max && cur->rangeMax >= min) {
            list.push(cur);
        }

        for (auto& [filename, child] : cur->children) {
            if (child && child->rangeMin <= max && child->rangeMax >= min) {
                checkList.push(child);
            }
        }
    }

    return list;
}
    
std::vector<int> Tree::get_next_ch_list(hostInfo * info){
    std::string filename = info->filename;
    int level = info->levelInfo;
    int rangeMin = info->rangeMin;
    int rangeMax = info->rangeMax;
    std::vector<int> chList;

}




