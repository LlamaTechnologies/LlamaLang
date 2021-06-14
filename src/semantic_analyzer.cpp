#include "semantic_analyzer.hpp"
#include <stdarg.h>

static bool is_ret_stmnt(const AstNode* stmnt);

void Table::add_child(Table& in_child) {
    children_scopes.push_back(in_child);
}

void Table::remove_last_child() {
    children_scopes.erase(children_scopes.end());
}

void Table::add_symbol(const std::string& in_name, const SymbolType in_type) {
    Symbol symbol(in_name, in_type);
    symbols.emplace(in_name, symbol);
}

bool SemanticAnalyzer::analizeFuncProto(const AstFuncProto& in_func_proto, AstFuncDef* in_function) {
    return true;
}

bool SemanticAnalyzer::analizeFuncBlock(const AstBlock& in_func_block, AstFuncDef& in_function) {
    auto ret_type = in_function.proto->function_proto.return_type;
    size_t errors_before = errors.size();

    for (auto stmnt : in_func_block.statements) {
        if (is_ret_stmnt(stmnt)) {
            // TODO: check if type is of type ret_type
        }
        if (stmnt->node_type == AstNodeType::AstVarDef) {
            analizeVarDef(stmnt, false);
        }
    }

    size_t errors_after = errors.size();

    return errors_before == errors_after;
}

bool SemanticAnalyzer::analizeVarDef(const AstNode* in_node, const bool is_global) {
    const AstVarDef& var_def = in_node->var_def;
    /*
    if (is_global && !var_def.initializer) {
        add_semantic_error(in_node, ERROR_GLOBAL_NEED_INITIALIZER, var_def.name);
        return false;
    }

    if (in_var_def.initializer) {
        // TODO: check assign types
        return false;
    }
    */

    return true;
}

bool SemanticAnalyzer::analizeExpr(const AstNode* in_expr) {
    return false;
}

void SemanticAnalyzer::add_semantic_error(const AstNode* in_node, const char* in_msg, ...) {
    va_list ap, ap2;
    va_start(ap, in_msg);
    va_copy(ap2, ap);

    int len1 = snprintf(nullptr, 0, in_msg, ap);
    assert(len1 >= 0);

    std::string msg;
    msg.reserve(len1);

    int len2 = snprintf(msg.data(), len1, in_msg, ap2);
    assert(len2 == len1);

    va_end(ap);

    Error error(ERROR_TYPE::ERROR,
        in_node->line,
        in_node->column,
        in_node->file_name, msg);
}

bool is_ret_stmnt(const AstNode* stmnt) {
    return stmnt->node_type == AstNodeType::AstUnaryExpr && stmnt->unary_expr.op == UnaryExprType::RET;
}
