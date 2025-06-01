#include <gtest/gtest.h>

#include "../src/tree.hh"



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


void execute_tree_test(Tree& tree, const std::string& commands, std::ostream& out) {
    std::istringstream iss(commands);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream linestream(line);
        std::string cmd;
        linestream >> cmd;

        if (cmd == "insert") {
            std::string filename;
            int level, min, max;
            linestream >> filename >> level >> min >> max;
            auto node = std::make_shared<TreeNode>(filename, level, min, max);
            tree.insert_node(node);
        }
        else if (cmd == "remove"){
            std::string filename;
            int level, min, max;
            linestream >> filename >> level >> min >> max;
            auto node = tree.find_node(filename,tree.root);
            tree.remove_node(node);
        }
        else if (cmd == "search") {
            int key;
            linestream >> key;
            auto res = tree.search_key(key);
            while (!res.empty()) {
                out << res.front()->filename << " ";
                res.pop();
            }
            out << "\n";
        }
    }
}

TEST(TreeTest, CommandBasedTest) {
    Tree tree;
    std::ostringstream output;

    std::string input = R"(
    insert A 1 5 20
    insert B 1 21 50
    insert C 2 10 30
    insert D 2 40 45
    insert E 3 0 2
    dump
    search 10
    children A
    )";

    execute_tree_test(tree, input, output);

    // 可以直接比較，也可以斷言有包含關鍵字
    EXPECT_TRUE(output.str().find("A") != std::string::npos);
}