#pragma once
#include "common_defs.hpp"
#include <vector>
#include <string>
#include <assert.h>
#include <cstddef>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>

// ast nodes
struct Token;
struct AstNode;
struct AstDirective;
struct AstType;
struct AstFuncDef;
struct AstFuncProto;
struct AstParamDecl;
struct AstBlock;
struct AstSourceCode;

struct AstVarDef;
struct AstSymbol;
struct AstFuncCallExpr;
struct AstBinaryExpr;
struct AstUnaryExpr;

enum class DirectiveType {
    Run,
    Load,
    CompTimeOnly
};

const std::string get_directive_type_name(const DirectiveType) noexcept;

struct AstDirective {
    DirectiveType       directive_type;
    std::string_view    argument;
};

struct AstFuncDef {
    AstNode* proto;
    AstNode* block;
    llvm::Function* function = nullptr;
};

struct AstFuncProto {
    std::string_view        name;
    std::vector<AstNode*>   params;
    AstNode*                return_type;
};

struct AstParamDecl {
    AstNode*            type;
    std::string_view    name;
};

struct AstBlock {
    std::vector<AstNode*> statements;

    AstBlock() : statements(std::vector<AstNode*>()) {}
};

struct AstVarDef {
    std::string_view    name;
    AstNode*            type;
    AstNode*            initializer;
};

struct AstSymbol {
    const Token* token;
};

struct AstFuncCallExpr {
    std::string_view        fn_name;
    AstNode*                fn_ref;
    std::vector<AstNode*>   params;
};

enum class BinaryExprType {
    ADD,                // expr +  expr
    SUB,                // expr -  expr
    MUL,                // expr *  expr
    DIV,                // expr /  expr
    MOD,                // expr %  expr
    EQUALS,             // expr == expr
    NOT_EQUALS,         // expr != expr
    GREATER_OR_EQUALS,  // expr >= expr
    LESS_OR_EQUALS,     // expr <= expr
    GREATER,            // expr >  expr
    LESS,               // expr <  expr
    LSHIFT,             // expr << expr
    RSHIFT,             // expr >> expr
    BIT_XOR,            // expr ^  expr
    BIT_AND,            // expr &  expr
    ASSIGN,             // expr =  expr
};

struct AstBinaryExpr {
    AstNode*        op1;
    BinaryExprType  bin_op;
    AstNode*        op2;
};


enum class UnaryExprType {
    INC,    // ++  primaryExpr
    DEC,    // --  primaryExpr
    NEG,    // -   primaryExpr  
    RET     // ret Expr
};

struct AstUnaryExpr {
    UnaryExprType   op;
    AstNode*        expr;
};



struct AstSourceCode {
    std::vector<AstNode*> children;
};


enum class AstTypeId {
    Pointer,
    Array,
    Void,
    Bool,
    Integer,
    FloatingPoint,
    Struct
};

struct TypeInfo {
    std::string_view    name;
    LLVMTypeRef         llvm_type;
    uint32_t            bit_size;
    bool                is_signed;
};

struct AstType {
    AstTypeId       type_id;    // Pointer Array Integer FloatingPoint
    AstNode*        child_type; // Not null if type == (pointer | array)
    TypeInfo*       type_info;  // null if type == (pointer | array)

    AstType() : type_id(AstTypeId::Void), child_type(nullptr), type_info(nullptr) {}
};

// ast nodes enum
enum class AstNodeType {
    AstSourceCode,
    AstDirective,
    AstFuncDef,
    AstFuncProto,
    AstParamDecl,
    AstBlock,
    AstType,
    AstVarDef,
    AstSymbol,
    AstFuncCallExpr,
    AstBinaryExpr,
    AstUnaryExpr
};

// base ast node
struct AstNode {
    AstNode* parent;
    size_t line;
    size_t column;
    AstNodeType node_type;
    std::string file_name;

    // actual node
    // TODO: try to use std::variant instead of union
    //union {
        AstSourceCode   source_code;
        AstDirective    directive;      // # dir_name args*
        AstFuncDef      function_def;   // function definition
        AstFuncProto    function_proto; // fn name L_PAREN param_decl (, param_decl)* R_PAREN
        AstParamDecl    param_decl;     // name type
        AstBlock        block;          // L_CURLY statements R_CURLY
        AstVarDef       var_def;        // name type
        AstType         ast_type;       // type
        AstUnaryExpr    unary_expr;     // unary_op expr
        AstBinaryExpr   binary_expr;    // expr binary_op expr
        AstSymbol       symbol;         // symbol_name
        AstFuncCallExpr func_call;      // func_name L_PAREN (expr (, expr)*)? R_PAREN
    //};

    AstNode(AstNodeType in_node_type, size_t in_line, size_t in_column, std::string in_file_name)
        : parent(nullptr), line(in_line), column(in_column), node_type(in_node_type), file_name(in_file_name) {}

    virtual ~AstNode() {
        if (node_type == AstNodeType::AstBlock) {
            for (auto node : block.statements) {
                delete node;
            }
        } else if (node_type == AstNodeType::AstSourceCode) {
            for (auto node : source_code.children) {
                delete node;
            }
        }else if (node_type == AstNodeType::AstFuncCallExpr) {
            for (auto node : func_call.params) {
                delete node;
            }
        }
    }
};
