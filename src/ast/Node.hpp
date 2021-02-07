#pragma once
#include <string>
#include <vector>
#include <memory>

namespace llang::ast {
    struct Node {
        std::vector<std::shared_ptr<Node>> children;
        std::string fileName;
        long line;

        virtual void ToString(std::string& str, const int tabLevel) const = 0;
    };

    inline void ToString(std::shared_ptr<const Node> node, std::string& str, const int tabLevel) {
        for (auto child : node->children) {
            child->ToString(str, tabLevel);
        }
    }
}
