#include "semantic_analyzer.hpp"
#include "semantic_error_msgs.hpp"
#include "ast_nodes.hpp"
#include "Types.hpp"
#include "common_defs.hpp"
#include <stdarg.h>

static bool is_ret_stmnt(const AstNode* stmnt);
static const AstNode* get_best_type(const AstNode* type_node0, const AstNode* type_node1);

Table* Table::create_child(const std::string& in_name) {
    return &children_scopes.emplace_back(in_name, this);
}

bool Table::has_child(const std::string& in_name) {
    return symbols.find(in_name) != symbols.end();
}

const Symbol& Table::get_child(const std::string& in_name) {
#ifdef LL_DEBUG
    if (has_child(in_name))
#endif
        return symbols.at(in_name);
#ifdef LL_DEBUG
    UNREACHEABLE;
#endif
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
            check_type_compat(expr_type, ret_type, stmnt);
        } else if (stmnt->node_type == AstNodeType::AstVarDef) {
            analizeVarDef(stmnt, false);
        } else {
            analizeExpr(stmnt);
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

    if (var_def.initializer) {
        auto expr_type = get_expr_type(var_def.initializer);
        if (!check_type_compat(var_def.type, expr_type, in_node))
            return false;
    }

    if (is_global) {
        if (!var_def.initializer) {
            add_semantic_error(in_node, ERROR_GLOBAL_NEED_INITIALIZER, var_def.name);
            return false;
        }

        global_symbol_table->add_symbol(var_name, SymbolType::VARIABLE, in_node);

        return true;
    }

    symbol_table->add_symbol(var_name, SymbolType::VARIABLE, in_node);

    return true;
}

bool SemanticAnalyzer::analizeExpr(const AstNode* in_expr) {
        switch (in_expr->node_type) {
        case AstNodeType::AstBinaryExpr: {
            auto bin_expr = in_expr->binary_expr;
            switch (bin_expr.bin_op) {
                case BinaryExprType::LSHIFT:
                case BinaryExprType::RSHIFT:
                {
                    if (!analizeExpr(bin_expr.op1) || !analizeExpr(bin_expr.op2))
                        return false;
                    
                    // right value should be an integer
                    auto r_expr_type_node = get_expr_type(bin_expr.op2);
                    if (r_expr_type_node->ast_type.type_id != AstTypeId::Integer) {
                        return false;
                    }

                    // now we depend on left expr beeing ok
                    return get_expr_type(bin_expr.op1);
                }
                case BinaryExprType::EQUALS:
                case BinaryExprType::NOT_EQUALS:
                case BinaryExprType::GREATER_OR_EQUALS:
                case BinaryExprType::LESS_OR_EQUALS:
                case BinaryExprType::GREATER:
                case BinaryExprType::LESS:
                    // We just check their expressions are good
                    return analizeExpr(bin_expr.op1) && analizeExpr(bin_expr.op2);
                case BinaryExprType::ADD:
                case BinaryExprType::SUB:
                case BinaryExprType::MUL:
                case BinaryExprType::DIV:
                case BinaryExprType::MOD:
                case BinaryExprType::ASSIGN:
                {
                    if (!analizeExpr(bin_expr.op1) || !analizeExpr(bin_expr.op2))
                        return false;
                    
                    auto l_expr_type_node = get_expr_type(bin_expr.op1);
                    auto r_expr_type_node = get_expr_type(bin_expr.op2);
                    
                    // We check they have compatible types
                    return check_type_compat(l_expr_type_node, r_expr_type_node, in_expr);
                }
                default:
                    UNREACHEABLE;
            }
        }
        case AstNodeType::AstUnaryExpr: {
            auto unary_expr = in_expr->unary_expr;
            if (unary_expr.op == UnaryExprType::RET)
                UNREACHEABLE;

            auto type_node = get_expr_type(unary_expr.expr);

            // Overloading operators is not supported
            if (type_node->ast_type.type_id == AstTypeId::Struct) {
                auto operator_symbol = get_unary_op_symbol(unary_expr.op);
                add_semantic_error(in_expr, ERROR_UNSUPORTED_UNARY_OP_STRUCT_EXPR, operator_symbol);
                return false;
            }
            // If it is a boolean expr only the NOT operator is valid
            if (type_node->ast_type.type_id == AstTypeId::Bool && unary_expr.op != UnaryExprType::NOT) {
                auto operator_symbol = get_unary_op_symbol(unary_expr.op);
                add_semantic_error(in_expr, ERROR_UNSUPORTED_OP_BOOL_EXPR, operator_symbol);
                return false;
            }
            // If it is not boolean expr the NOT operator is invalid
            if (type_node->ast_type.type_id != AstTypeId::Bool && unary_expr.op == UnaryExprType::NOT) {
                add_semantic_error(in_expr, ERROR_UNSUPORTED_OP_NOT_BOOL_EXPR);
                return false;
            } 

            return analizeExpr(unary_expr.expr);
        }
        case AstNodeType::AstFuncCallExpr: {
            return resolve_function_variable(std::string(in_expr->func_call.fn_name), in_expr) != nullptr;
        }
        case AstNodeType::AstSymbol: {
            auto name = std::string(in_expr->symbol.cached_name);
            return resolve_function_variable(name, in_expr) != nullptr;
        }
        case AstNodeType::AstConstValue: {
            switch (in_expr->const_value.type)
            {
            case ConstValueType::BOOL:
            case ConstValueType::INT:
            case ConstValueType::FLOAT:
            case ConstValueType::CHAR:
                return true;
            default:
                UNREACHEABLE;
            }
        }
        default:
            UNREACHEABLE;
        }
        return false;
}

const AstNode* SemanticAnalyzer::resolve_function_variable(const std::string& in_name, const AstNode* in_parent_node) {
    auto curr_table = symbol_table;
    do {
        if (curr_table->has_child(in_name)) {
            const Symbol& symbol = curr_table->get_child(in_name);
            return symbol.data_node;
        }
        curr_table = curr_table->parent;
    } while (curr_table);


    add_semantic_error(in_parent_node, ERROR_UNKNOWN_SYMBOL, in_name);
    
    return nullptr;
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
    
    errors.push_back(error);
}

bool SemanticAnalyzer::check_type_compat(const AstNode* type_node0, const AstNode* type_node1, const AstNode* expr_node) {
    assert(type_node0 != nullptr);
    assert(type_node1 != nullptr);
    assert(type_node0->node_type == AstNodeType::AstType);
    assert(type_node1->node_type == AstNodeType::AstType);
    assert(expr_node->node_type != AstNodeType::AstType);

    const AstType& type0 = type_node0->ast_type;
    const AstType& type1 = type_node1->ast_type;

    if (type1.type_id == type0.type_id) {
        if (type1.type_id == AstTypeId::Pointer || type1.type_id == AstTypeId::Array)
            return check_type_compat(type0.child_type, type1.child_type, expr_node);
        if (type0.type_id == AstTypeId::Struct)
            // NOTE: Structs No Supported
            // TODO: Add support for structs
            UNREACHEABLE;
        return true;
    }

    // incompatible types
    // void | bool
    // int  | bool
    // int  | void
    // int  | float
    // int  | pointer
    // int  | array
    // float| bool
    // float| void
    // float| array
    // float| pointer
    // array| pointer
    add_semantic_error(expr_node, ERROR_TYPES_MISMATCH);

    return false;
}

const AstNode* SemanticAnalyzer::get_expr_type(const AstNode* expr) {
    switch (expr->node_type) {
    case AstNodeType::AstBinaryExpr: {
        auto bin_expr = expr->binary_expr;
        switch (bin_expr.bin_op) {
        case BinaryExprType::EQUALS:
        case BinaryExprType::NOT_EQUALS:
        case BinaryExprType::GREATER:
        case BinaryExprType::LESS:
        case BinaryExprType::GREATER_OR_EQUALS:
        case BinaryExprType::LESS_OR_EQUALS:
            return get_type_node("bool");
        default:
            auto type0 = get_expr_type(bin_expr.op1);
            auto type1 = get_expr_type(bin_expr.op2);
            if (check_type_compat(type0, type1, expr))
                return get_best_type(type0, type1);
            return nullptr;
        }
        UNREACHEABLE;
    }
    case AstNodeType::AstUnaryExpr: {
        auto unary_expr = expr->unary_expr;
        switch (unary_expr.op) {
        case UnaryExprType::NOT:
            return get_type_node("bool");
        default:
            return get_expr_type(unary_expr.expr);
        }
    }
    case AstNodeType::AstFuncCallExpr: {
        auto func_node = resolve_function_variable(std::string(expr->func_call.fn_name), expr);
        return get_expr_type(func_node);
    }
    case AstNodeType::AstFuncDef: {
        auto proto_node = expr->function_def.proto->function_proto;
        return proto_node.return_type;
    }
    case AstNodeType::AstFuncProto: {
        auto proto_node = expr->function_proto;
        return proto_node.return_type;
    }
    case AstNodeType::AstSymbol: {
        // TODO(pablo96): should make it posible to resolve function names as func pointers
        auto name = std::string(expr->symbol.cached_name);
        auto var_node = resolve_function_variable(name, expr);
        if (!var_node) {
            return nullptr;
        }
        if (var_node->node_type == AstNodeType::AstVarDef) {
            return var_node->var_def.type;
        }
        if (var_node->node_type == AstNodeType::AstFuncDef) {
            return var_node->function_def.proto->function_proto.return_type;
        }
        if (var_node->node_type == AstNodeType::AstFuncProto) {
            return var_node->function_proto.return_type;
        }
        UNREACHEABLE;
    }
    case AstNodeType::AstConstValue: {
        auto const_value = expr->const_value;
        switch (const_value.type) {
        case ConstValueType::BOOL:
            return get_type_node("bool");
        case ConstValueType::FLOAT:
            return get_type_node("f128");
        case ConstValueType::INT:
            if (const_value.integer.is_negative)
                return get_type_node("i128");
            return get_type_node("u128");
        case ConstValueType::CHAR:
            return get_type_node("u32");
        default:
            UNREACHEABLE;
        }
    }
    default:
        UNREACHEABLE;
    }
}

bool is_ret_stmnt(const AstNode* stmnt) {
    return stmnt->node_type == AstNodeType::AstUnaryExpr && stmnt->unary_expr.op == UnaryExprType::RET;
}

const AstNode* get_best_type(const AstNode* type_node0, const AstNode* type_node1) {
    assert(type_node0 != nullptr);
    assert(type_node1 != nullptr);
    assert(type_node0->node_type == AstNodeType::AstType);
    assert(type_node1->node_type == AstNodeType::AstType);

    const AstType& type0 = type_node0->ast_type;
    const AstType& type1 = type_node1->ast_type;

    if (type1.type_id == AstTypeId::Pointer || type1.type_id == AstTypeId::Array) {
        return get_best_type(type0.child_type, type1.child_type);
    }
    
    return type0.type_info->bit_size > type1.type_info->bit_size ? type_node0 : type_node1;
}

