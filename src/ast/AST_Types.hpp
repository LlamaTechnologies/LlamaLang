#pragma once
#include "ProgramNode.hpp"
#include "RightValueNode.hpp"
#include "BinaryStatementNode.hpp"
#include "ConstantNode.hpp"
#include "FunctionNode.hpp"
#include "StatementNode.hpp"
#include "UnaryStatementNode.hpp"
#include "VariableDeclNode.hpp"

#define REGISTER_AST_TYPE(type) public RegisterAstType<type>

#define SET_VALUE(value) value
#define GET_VALUE(type) AST_TYPE::type
namespace llang::ast {
    enum class AST_TYPE {
        SET_VALUE(ProgramNode),
        SET_VALUE(RightValueNode),
        SET_VALUE(BinaryStatementNode),
        SET_VALUE(ConstantNode),
        SET_VALUE(FunctionNode),
        SET_VALUE(StatementNode),
        SET_VALUE(UnaryStatementNode),
        SET_VALUE(VariableDeclNode)
    };

    template<T>
    struct RegisterAstType {
        AST_TYPE AstType = GET_VALUE(T);
    };
}

#undef GET_VALUE(type)
#undef SET_VALUE(value)
