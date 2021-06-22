#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "error.hpp"

struct AstFuncProto;
struct AstFuncDef;
struct AstBlock;
struct AstNode;

enum class SymbolType {
    VARIABLE,
    FUNCTION
};

struct Symbol {
    const std::string& name;
    const SymbolType  type;

    Symbol(const std::string& in_name, const SymbolType in_type)
        : name(in_name), type(in_type) {}
};

struct Table {
    std::string                             name;
    std::vector<Table>                      children_scopes;
    std::unordered_map<std::string, Symbol> symbols;

    Table(const std::string& in_name) : name(in_name) {}

    void add_child(Table& in_child);

    void remove_last_child();

    void add_symbol(const std::string& in_name, const SymbolType in_type);
};

class SemanticAnalyzer {
    Table symbol_table = Table("global_scope");
    std::vector<Error> errors;
public:
    SemanticAnalyzer(std::vector<Error>& in_errors) : errors(in_errors) {}

    bool analizeFuncProto(const AstFuncProto& in_func_proto, AstFuncDef* in_function);
    bool analizeFuncBlock(const AstBlock& in_func_block, AstFuncDef& in_function);
    bool analizeVarDef(const AstNode* in_node, const bool is_global);

private:
    bool analizeExpr(const AstNode* in_expr);

    void add_semantic_error(const AstNode* in_node, const char* in_msg, ...);
};
