#include "tree.hh"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
extern Tree tree;

bool Tree::detect_overlap(int level, int rangeMin, int rangeMax) {
    auto& nodes = level_map[level];
    auto dummy = std::make_shared<TreeNode>("dummy", level, rangeMin, rangeMax);

    auto it = nodes.lower_bound(dummy);

    // 往前檢查一個（因為 lower_bound() 可能略過 overlap 節點）
    if (it != nodes.begin()) {
        auto prev = std::prev(it);
        if (rangeMin <= (*prev)->rangeMax && rangeMax >= (*prev)->rangeMin)
            return true;
    }

    // 往後檢查從 lower_bound 開始的節點，只要 rangeMin ≤ rangeMax
    while (it != nodes.end() && (*it)->rangeMin <= rangeMax) {
        if (rangeMin <= (*it)->rangeMax) return true;
        ++it;
    }

    return false;
}

void Tree::insert_node(std::shared_ptr<TreeNode> node){
    int level = node->levelInfo;
    int rangeMin = node->rangeMin;
    int rangeMax = node->rangeMax;
    bool overlap = detect_overlap(level,rangeMin,rangeMax);
    if(!overlap){
        level_map[level].insert(node);
    }
    else{
        pr_debug("Insert node key range is error,key range overlap");
    }
}

void Tree::remove_node(std::shared_ptr<TreeNode> node){
    int level = node->levelInfo;
    for()
}





 


