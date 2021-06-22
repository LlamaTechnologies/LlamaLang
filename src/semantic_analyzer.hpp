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
    Table*      parent;
    std::string name;

    Table(const std::string& in_name, Table* in_parent) : parent(in_parent), name(in_name) {}

    Table& create_child(const std::string& in_name);

    void remove_last_child();

    void add_symbol(const std::string& in_name, const SymbolType in_type);

private:
    std::vector<Table>                      children_scopes;
    std::unordered_map<std::string, Symbol> symbols;
};

class SemanticAnalyzer {
    Table global_symbol_table = Table("global_scope", nullptr);
    Table symbol_table = global_symbol_table;
    std::vector<Error> errors;
public:
    SemanticAnalyzer(std::vector<Error>& in_errors) : errors(in_errors) {}

    bool analizeFuncProto(const AstNode* in_func_proto);
    bool analizeFuncBlock(const AstBlock& in_func_block, AstFuncDef& in_function);
    bool analizeVarDef(const AstNode* in_node, const bool is_global);

private:
    bool analizeExpr(const AstNode* in_expr);
    void check_type(const AstNode* type_node0, const AstNode* type_node1);

    void add_semantic_error(const AstNode* in_node, const char* in_msg, ...);
};
