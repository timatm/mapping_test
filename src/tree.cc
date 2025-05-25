#include "tree.hh"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>


// TreeCLI.cpp
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "tree.hh"  // 假設你有定義 Tree、TreeNode

Tree tree;
std::unordered_map<std::string, std::shared_ptr<TreeNode>> nodeMap;



// void Tree::insert_tree(std::shared_ptr<TreeNode> subtree) {
//     std::string filename = subtree->filename;
//     subtree->parent = root.get();
//     root->children[filename] = std::move(subtree);
// }

// void Tree::insert_node(TreeNode *node){
//     if( !node ) return;
//     TreeNode *parent = search_insert_parent(node);
//     insert_node2parent (parent,node);
// }

// void Tree::insert_node2parent(TreeNode *parent, TreeNode *node) {
//     std::vector<std::string> moveKeys;

//     // Check all children in parent node
//     // Check whether the key range and level are consistent with being the son of the new node
//     for (auto& [filename, upChild] : parent->children) {
//         TreeNode* child = upChild.get();
//         if (!child) continue;

//         if (child->rangeMin >= node->rangeMin &&
//             child->rangeMax <= node->rangeMax &&
//             parent->levelInfo == node->levelInfo - 1) 
//         {
//             moveKeys.push_back(filename);
//         }
//     }

//     std::shared_ptr<TreeNode> new_node = std::make_unique<TreeNode>(
//         node->filename, node->levelInfo, node->channelInfo, node->rangeMin, node->rangeMax);
//     // Move all childl to new node's children
//     for (const auto& filename : moveKeys) {
//         auto& upChild = parent->children[filename];
//         TreeNode* child = upChild.get();
//         child->parent = new_node.get();
//         new_node->children[filename] = std::move(upChild);
//     }

//     parent->children[node->filename] = std::move(new_node);
// }

// void Tree::remove_node(TreeNode *node) {
//     if (!node) return;
//     TreeNode* parent = node->parent;
//     if (!parent) return; 

//     std::vector<std::string> moveKeys;
//     for (const auto& [filename, child] : node->children) {
//         moveKeys.push_back(filename);
//     }

//     for (const auto& filename : moveKeys) {
//         auto& upChild = node->children[filename];
//         TreeNode* child = upChild.get();
//         if (!child) continue;
//         child->parent = parent;
//         parent->children[child->filename] = std::move(upChild);
//     }

//     parent->children.erase(node->filename);

// }



// bool Tree::move_node(std::shared_ptr<TreeNode>* target, std::shared_ptr<TreeNode>* destination) {
//     if (!target || !destination || !*target || !*destination) return false;

//     std::string key = (*target)->filename;
//     TreeNode *parent = (*target)->parent;

//     (*target)->parent = destination->get();

//     (*destination)->children[key] = std::move(*target);

//     if (parent) {
//         parent->children.erase(key);
//     }
//     return true;
// }
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
            if (child->levelInfo == node->levelInfo - 1 &&
                child->rangeMin <= node->rangeMin &&
                child->rangeMax >= node->rangeMax) {
                candidateSet.insert(child.get());
            }

            // 若層級還未到底，可以繼續往下搜尋
            if (child->levelInfo < node->levelInfo - 1) {
                checkList.push(child);
            }
        }
    }

    for (TreeNode* parentNode : candidateSet) {
        auto parent = parentNode->shared_from_this();
        parent->children[node->filename] = node;
        node->parent.push_back(std::weak_ptr<TreeNode>(parent));  // 多父母情況
    }

    return ;
}


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




TreeNode * Tree::find_node(std::string filename,TreeNode *cur){
    if (!cur) return nullptr;
    if(cur->filename == filename) return cur;
    for(auto & [name, upChild] : cur->children) {
        TreeNode* child = upChild.get();
        if (!child) continue;
        TreeNode* found = find_node(filename, child);
        if (found) return found;
    }
    return nullptr;
}


// std::queue<std::string> Tree::search_range(std::string key){
//     std::queue<std::string> sstableList;
//     TreeNode* cur = root.get();
    
//     while (cur) {
//         bool found = false;
//         for (auto& [filename, child] : cur->children) {
//             TreeNode* childNode = child.get();
//             if (!childNode) continue;

//             if (childNode->rangeMin <= key && key <= childNode->rangeMax) {
//                 sstableList.push(childNode->filename);
//                 cur = childNode;
//                 found = true;
//                 break;
//             }
//         }
//         if (!found) break; 
//     }

//     return sstableList;
// }


// TreeNode* Tree::search_insert_parent(TreeNode* node)
// {
//     if (!root) return nullptr;
//     TreeNode* cur = root.get();

//     while (cur && cur->levelInfo < node->levelInfo)
//     {
//         bool found = false;

//         for (auto& [name, upChild] : cur->children) 
//         {
//             TreeNode* child = upChild.get();
//             if (!child) continue;

//             if (child->rangeMin <= node->rangeMin &&
//                 child->rangeMax >= node->rangeMax)  
//             {
//                 cur   = child;
//                 found = true;
//                 break;
//             }
//         }

//         if (!found) break;
//     }
//     return cur; 
// }



void dumpGraph(const std::shared_ptr<TreeNode>& node,
               int indent = 0,
               std::unordered_set<const TreeNode*>* visited = nullptr) {
    if (!node) return;

    // 初始化 visited 集合
    bool localVisited = false;
    if (!visited) {
        visited = new std::unordered_set<const TreeNode*>();
        localVisited = true;
    }

    // 避免重複列印已經走過的節點（防止重複或循環）
    if (visited->count(node.get())) return;
    visited->insert(node.get());

    // 印出自己
    std::cout << std::string(indent * 2, ' ') << "● " << node->filename
              << " (L" << node->levelInfo << ", C" << node->channelInfo << ", [" 
              << node->rangeMin << " ~ " << node->rangeMax << "])" << '\n';

    // 印出所有子節點（遞迴）
    for (const auto& [name, child] : node->children) {
        dumpGraph(child, indent + 1, visited);
    }

    // 如果是最外層呼叫者，刪掉 visited（避免記憶體外漏）
    if (localVisited) {
        delete visited;
    }
}

void insert_node_cli() {
    std::string filename;
    int min, max;
    int level;

    std::cout << "Enter filename: ";
    std::cin >> filename;
    std::cout << "Enter level: ";
    std::cin >> level;
    std::cout << "Enter rangeMin: ";
    std::cin >> min;
    std::cout << "Enter rangeMax: ";
    std::cin >> max;

    auto node = std::make_shared<TreeNode>(filename, level, min, max);
    tree.insert_node(node);
    nodeMap[filename] = node;

    std::cout << "✅ Inserted node: " << filename << "\n";
}

void remove_node_cli() {
    std::string filename;
    std::cout << "Enter filename to remove: ";
    std::cin >> filename;

    auto it = nodeMap.find(filename);
    if (it != nodeMap.end()) {
        tree.remove_node(it->second);
        nodeMap.erase(it);
        std::cout << "🗑️ Removed node: " << filename << "\n";
    } else {
        std::cout << "⚠️ Node not found.\n";
    }
}

void print_graph_cli() {
    std::cout << "\n🔍 Tree structure:\n";
    dumpGraph(tree.root);
    std::cout << "\n";
}

int main() {
    std::cout << "=== Tree Test CLI ===\n";
    while (true) {
        std::cout << "\nChoose action:\n";
        std::cout << "1. Insert node\n";
        std::cout << "2. Remove node\n";
        std::cout << "3. Print tree\n";
        std::cout << "0. Exit\n";
        std::cout << "Your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: insert_node_cli(); break;
            case 2: remove_node_cli(); break;
            case 3: print_graph_cli(); break;
            case 0: std::cout << "Bye!\n"; return 0;
            default: std::cout << "❌ Invalid option.\n"; break;
        }
    }
}