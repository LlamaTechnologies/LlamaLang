#include "semantic_analyzer.hpp"
#include "semantic_error_msgs.hpp"
#include "ast_nodes.hpp"
#include <stdarg.h>

static bool is_ret_stmnt(const AstNode* stmnt);

static const AstNode* get_expr_type(const AstNode* expr);
static std::unordered_map<const AstNode*, const AstNode*> cached_expr_types;


Table* Table::create_child(const std::string& in_name) {
    return &children_scopes.emplace_back(in_name, this);
}

void Table::remove_last_child() {
    children_scopes.erase(children_scopes.end());
}

void Table::add_symbol(const std::string& in_name, const SymbolType in_type, const AstNode* in_data) {
    Symbol symbol = Symbol(in_name, in_type, in_data);
    symbols.emplace(in_name, symbol);
}

bool SemanticAnalyzer::analizeFuncProto(const AstNode* in_proto_node) {
    assert(in_proto_node != nullptr);
    assert(in_proto_node->node_type == AstNodeType::AstFuncProto);

    const AstFuncProto& func_proto = in_proto_node->function_proto;
    auto ret_type = func_proto.return_type;

    if (ret_type->ast_type.type_id == AstTypeId::Struct) {
        add_semantic_error(ret_type, ERROR_STRUCTS_UNSUPORTED);
        return false;
    }

    auto str_name = std::string(func_proto.name);

    symbol_table->add_symbol(str_name, SymbolType::FUNCTION, in_proto_node);
    symbol_table = symbol_table->create_child(std::string(func_proto.name));

    return true;
}

bool SemanticAnalyzer::analizeFuncBlock(const AstBlock& in_func_block, AstFuncDef& in_function) {
    auto ret_type = in_function.proto->function_proto.return_type;

    size_t errors_before = errors.size();

    for (auto stmnt : in_func_block.statements) {
        if (is_ret_stmnt(stmnt)) {
            auto expr_type = get_expr_type(stmnt->unary_expr.expr);
            //check_type(expr_type, ret_type);
        }
        if (stmnt->node_type == AstNodeType::AstVarDef) {
            analizeVarDef(stmnt, false);
        }
    }

    size_t errors_after = errors.size();

    return errors_before == errors_after;
}

bool SemanticAnalyzer::analizeVarDef(const AstNode* in_node, const bool is_global) {
    assert(in_node != nullptr);
    assert(in_node->node_type == AstNodeType::AstVarDef);

    const AstVarDef& var_def = in_node->var_def;
    auto var_name = std::string(var_def.name);

    if (is_global) {
        if (!var_def.initializer) {
            add_semantic_error(in_node, ERROR_GLOBAL_NEED_INITIALIZER, var_def.name);
            return false;
        }

        global_symbol_table->add_symbol(var_name, SymbolType::VARIABLE, in_node);

        return true;
    }


    /* Local variable */

    if (var_def.initializer) {
        // TODO: check assign types
        return false;
    }
    
    symbol_table->add_symbol(var_name, SymbolType::VARIABLE, in_node);

    return true;
}

bool SemanticAnalyzer::analizeExpr(const AstNode* in_expr) {
    return false;
}

void SemanticAnalyzer::check_type(const AstNode* type_node0, const AstNode* type_node1) {
    assert(type_node0 != nullptr);
    assert(type_node1 != nullptr);
    assert(type_node0->node_type == AstNodeType::AstType);
    assert(type_node1->node_type == AstNodeType::AstType);

    const AstType& type0 = type_node0->ast_type;
    const AstType& type1 = type_node1->ast_type;

    if (type1.type_id == type0.type_id) {
        if (type1.type_id == AstTypeId::Void || type1.type_id == AstTypeId::Bool) {
            return;
        }

        if (type1.type_id == AstTypeId::Pointer || type1.type_id == AstTypeId::Array) {
            check_type(type0.child_type, type1.child_type);
            return;
        }
        else {
            switch (type1.type_id) {
            case AstTypeId::Integer:
                if (type1.type_info->bit_size != type0.type_info->bit_size) {
                    add_semantic_error(type_node0, ERROR_TYPES_SIZE_MISMATCH);
                }
                if (type1.type_info->is_signed != type0.type_info->is_signed) {
                    add_semantic_error(type_node0, ERROR_INTEGERS_SIGN_MISMATCH);
                }
                return;
            case AstTypeId::FloatingPoint:
                if (type1.type_info->bit_size != type0.type_info->bit_size) {
                    add_semantic_error(type_node0, ERROR_TYPES_SIZE_MISMATCH);
                }
                return;
            default:
                UNREACHEABLE;
            }
        }
    }

    add_semantic_error(type_node0, ERROR_TYPES_MISMATCH);
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

const AstNode* get_expr_type(const AstNode* expr) {
    if (cached_expr_types.find(expr) != cached_expr_types.end()) {
        return cached_expr_types.at(expr);
    }
    // TODO(pablo96): get expr type
    return nullptr;
}

