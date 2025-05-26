#include "tree.hh"
#include "tree.cc"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <fstream>
#include <sstream>


Tree tree;
std::unordered_map<std::string, std::shared_ptr<TreeNode>> nodeMap;



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

void print_all_children(const std::shared_ptr<TreeNode>& node,
                        int indent = 0,
                        std::unordered_set<const TreeNode*>* visited = nullptr) {
    if (!node) return;

    bool localVisited = false;
    if (!visited) {
        visited = new std::unordered_set<const TreeNode*>();
        localVisited = true;
    }

    if (visited->count(node.get())) return;
    visited->insert(node.get());

    // 印出自己
    std::cout << std::string(indent * 2, ' ') << "↳ " << node->filename << "\n";

    // 遞迴印出子節點
    for (const auto& [name, child] : node->children) {
        print_all_children(child, indent + 1, visited);
    }

    if (localVisited) {
        delete visited;
    }
}


void loadTestFile(const std::string& filename,
                  Tree& tree,
                  std::unordered_map<std::string, std::shared_ptr<TreeNode>>& nodeMap) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "❌ Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "insert") {
            std::string name;
            int min ,max;
            int level;
            iss >> name >> level >> min >> max;
            if (name.empty() || min < 0 || max < 0 || iss.fail()) {
                std::cerr << "⚠️  Invalid insert format at line " << lineNumber << ": " << line << "\n";
                continue;
            }

            auto node = std::make_shared<TreeNode>(name, level, min, max);
            tree.insert_node(node);
            nodeMap[name] = node;

        } else if (cmd == "r") {
            std::string name;
            iss >> name;
            auto it = nodeMap.find(name);
            if (it != nodeMap.end()) {
                tree.remove_node(it->second);
                nodeMap.erase(it);
            } else {
                std::cerr << "⚠️  Node not found for removal: " << name << "\n";
            }

        } else if (cmd == "dump") {
            std::cout << "\n🔍 Tree structure:\n";
            dumpGraph(tree.root);
        }
        else if (cmd == "children") {
            std::string name;
            iss >> name;
            auto it = nodeMap.find(name);
            if (it != nodeMap.end()) {
                std::cout << "🧭 Subtree of: " << name << "\n";
                print_all_children(it->second);
            } else {
                std::cerr << "⚠️  Node not found: " << name << "\n";
            }
        }
        else if (cmd == "search") {
            int key = -1;
            iss >> key;
            if( key < 0){
                std::cout << "Search key is invalid" << std::endl;
            }
            else{
                std::cout << "Search List (input key:" << key << ")" << std::endl;
                std::queue<std::shared_ptr<TreeNode>> list = tree.search_key(key);
                while(!list.empty()){
                    std::shared_ptr<TreeNode> cur = list.front();
                    list.pop();
                    std::cout << "● " << cur->filename << " [L]:" << cur->levelInfo << " [Key range]:" << cur->rangeMin << "~" << cur->rangeMax << std::endl;
    
                }
            }
            

        }
        else {
            std::cerr << "⚠️  Unknown command at line " << lineNumber << ": " << cmd << "\n";
        }
    }
}

int main() {
    std::string file = "case.txt";
    loadTestFile(file, tree, nodeMap);
    return 0;
}