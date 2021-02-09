#include "AST_Types.hpp"
#include <vector>
#include "../CppReflection.hpp"

#define PUSH_VAL(enum_type, value)\
astTypes[size_t(value)] = GetEnumClassValueName<enum_type, value>()

namespace llang::ast {
	static bool init = false;
	static auto astTypes = std::vector<std::string>(size_t(AST_TYPE::COUNT));

	std::string GetAstTypeName(AST_TYPE type) { 
		if (init)
			return astTypes.at(size_t(type));
        PUSH_VAL(AST_TYPE, AST_TYPE::ProgramNode);
        PUSH_VAL(AST_TYPE, AST_TYPE::RightValueNode);
        PUSH_VAL(AST_TYPE, AST_TYPE::BinaryStatementNode);
        PUSH_VAL(AST_TYPE, AST_TYPE::ConstantNode);
        PUSH_VAL(AST_TYPE, AST_TYPE::FunctionNode);
        PUSH_VAL(AST_TYPE, AST_TYPE::StatementNode);
        PUSH_VAL(AST_TYPE, AST_TYPE::UnaryStatementNode);
        PUSH_VAL(AST_TYPE, AST_TYPE::VariableDeclNode);
	}
}
