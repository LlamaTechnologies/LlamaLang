#pragma once
#include <string>
#include <vector>
#include <memory>

#define GET_AST_TYPE(type) AST_TYPE::type

namespace llang
{
    namespace ast
    {
        enum class AST_TYPE;

        /**
         * Base Node type for the ast.
         * IMPORTANT: Remember every time you inherit this struct
         * you have to add the new type to AST_Types.hpp
         */
        struct Node {
            using ChildType = std::shared_ptr<Node>;
            using Action = void ( * )( ChildType );

            std::vector<ChildType> children;
            std::string FileName;
            size_t Line = 0L;

            std::string ToString() const {
                std::string str;
                ToString(str, 0);
                return str;
            }

            virtual void ToString(std::string &str, const int tabLevel) const = 0;

            virtual AST_TYPE GetType() const = 0;

            void ForEachDeep(Action action) {
                for( auto child : children ) {
                    action(child);
                    child->ForEachDeep(action);
                }
            }

        protected:
            Node() = default;
        };
    } // namespace ast

    template <typename T>
    std::shared_ptr<T> constexpr CastNode(ast::Node::ChildType node) {
        return std::static_pointer_cast<T, ast::Node>( node );
    }
}
