#pragma once
#include <string>
#include <vector>
#include <memory>
#include "AST_Types.hpp"

namespace llang::ast {
    struct Node {
        using Action = void (*)(std::shared_ptr<RegisterAstType>);
        std::vector<std::shared_ptr<RegisterAstType>> children;
        std::string fileName;
        long line;

        virtual void ToString(std::string& str, const int tabLevel) const = 0;

        void ForEachDeep(Action action) {
            for (auto child : children) {
                action(child);
                ForEachDeep(action);
            }
        }
    };

    inline void ToString(std::shared_ptr<const Node> node, std::string& str, const int tabLevel) {
        for (auto child : node->children) {
            child->ToString(str, tabLevel);
        }
    }
}
