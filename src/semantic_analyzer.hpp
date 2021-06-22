#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "error.hpp"

struct AstFuncDef;
struct AstBlock;
struct AstNode;

enum class SymbolType {
    VARIABLE,
    FUNCTION
};

struct Symbol {
    const std::string&  name;
    const SymbolType    type;
    const AstNode*      data_node;

    Symbol(const std::string&in_name, const SymbolType in_type, const AstNode* in_data)
        : name(in_name), type(in_type), data_node(in_data) {}
};

struct Table {
    using SymbolMap = std::unordered_map<std::string, Symbol>;

    Table*      parent;
    std::string name;
    

    Table(const std::string& in_name, Table* in_parent) : parent(in_parent), name(in_name) {}

    Table* create_child(const std::string& in_name);

    void remove_last_child();

    void add_symbol(const std::string& in_name, const SymbolType in_type, const AstNode* in_data);

private:
    std::vector<Table>  children_scopes;
    SymbolMap           symbols;
};

class SemanticAnalyzer {
    Table* global_symbol_table = new Table("global_scope", nullptr);
    Table* symbol_table = global_symbol_table;
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
