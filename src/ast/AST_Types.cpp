#include "AST_Types.hpp"
#include <vector>
#include "../CppReflection.hpp"
#include "Node.hpp"

#define GET_VAL(enum_type, value)\
GetEnumClassValueName<enum_type, value>()

namespace llang::ast
{
    static bool init = false;
    static std::vector<std::string> astTypes = {
        GET_VAL(AST_TYPE, GET_AST_TYPE(AssignNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(BinaryOperationNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(ConstantNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(FunctionCallNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(FunctionDefNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(StatementNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(ProgramNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(UnaryOperationNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(VariableDefNode)),
        GET_VAL(AST_TYPE, GET_AST_TYPE(VariableRefNode))
    };

    std::string GetAstTypeName(AST_TYPE type) {
        return astTypes.at(size_t(type));
    }
}
