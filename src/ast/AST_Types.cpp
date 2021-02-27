#include "AST_Types.hpp"
#include <vector>
#include "../CppReflection.hpp"
#include "Node.hpp"

#define PUSH_VAL(enum_type, value)\
astTypes[size_t(value)] = GetEnumClassValueName<enum_type, value>()

namespace llang::ast
{
    static bool init = false;
    static auto astTypes = std::vector<std::string>(size_t(AST_TYPE::COUNT));

    std::string GetAstTypeName(AST_TYPE type) {
        if( !init ) {
            init = true;
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(AssignNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(BinaryOperationNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(ConstantNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(FunctionCallNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(FunctionDefNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(StatementNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(ProgramNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(UnaryOperationNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(VariableDeclNode));
            PUSH_VAL(AST_TYPE, GET_AST_TYPE(VariableRefNode));
        }
        return astTypes.at(size_t(type));

    }
}
