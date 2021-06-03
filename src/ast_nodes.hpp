#pragma once
#include "common_defs.hpp"
#include <vector>
#include <string>

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
    DirectiveType   directive_type;
    std::string     argument;
};

struct AstFuncDef {
    AstNode* proto;
    AstNode* block;
};

struct AstFuncProto {
    std::string name;
    std::vector<AstNode*> params;
    AstNode* return_type;
    AstNode* fn_def_node;
};

struct AstParamDecl {
    AstNode* type;
    std::string name;
};

struct AstBlock {
    std::vector<AstNode*> statements;
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
    AstNode* fn_ref;
    std::vector<AstNode*> params;
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
    AstNode* op1;
    BinaryExprType bin_op;
    AstNode* op2;
};


enum class UnaryExprType {
    INC,    // ++  primaryExpr
    DEC,    // --  primaryExpr
    NEG,    // -   primaryExpr  
    RET     // ret Expr
};

struct AstUnaryExpr {
    UnaryExprType op;
    AstNode* expr;
};



struct AstSourceCode {
    std::vector<AstNode*> children;
};


enum class AstTypeType {
    Pointer,
    Array,
    DataType
};

struct AstType {
    AstTypeType type;
    AstNode* data_type;
    std::string name;

    AstType() : type(AstTypeType::DataType), data_type(nullptr), name("") {

    }
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
struct AstNode
{
    AstNode* parent;
    size_t line;
    size_t column;
    AstNodeType node_type;


    // actual node
    union {
        AstSourceCode* source_code;
        AstDirective* directive;        // # dir_name args*
        AstFuncDef* function_def;       // function definition
        AstFuncProto* function_proto;   // fn name L_PAREN param_decl (, param_decl)* R_PAREN
        AstParamDecl* param_decl;       // name type
        AstBlock* block;                // L_CURLY statements R_CURLY
        AstVarDef* var_def;             // name type
        AstType* ast_type;              // type
        AstNode* grouped_expr;          // L_PAREN expr R_PAREN
        AstUnaryExpr* unary_expr;       // unary_op expr
        AstBinaryExpr* binary_expr;     // expr binary_op expr
        AstSymbol*  symbol;             // symbol_name
        AstFuncCallExpr* func_call;     // func_name L_PAREN (expr (, expr)*)? R_PAREN
    } data;


    AstNode(AstNodeType in_node_type, size_t in_line, size_t in_column)
        : parent(nullptr), line(in_line), column(in_column), node_type(in_node_type)
    {
        switch (node_type)
        {
        case AstNodeType::AstSourceCode:
            data.source_code = new AstSourceCode();
            break;
        case AstNodeType::AstDirective:
            data.directive = new AstDirective();
            break;
        case AstNodeType::AstFuncDef:
            data.function_def = new AstFuncDef();
            break;
        case AstNodeType::AstFuncProto:
            data.function_proto = new AstFuncProto();
            break;
        case AstNodeType::AstParamDecl:
            data.param_decl = new AstParamDecl();
            break;
        case AstNodeType::AstBlock:
            data.block = new AstBlock();
            break;
        case AstNodeType::AstVarDef:
            data.var_def = new AstVarDef();
            break;
        case AstNodeType::AstSymbol:
            data.symbol = new AstSymbol();
            break;
        case AstNodeType::AstFuncCallExpr:
            data.func_call = new AstFuncCallExpr();
            break;
        case AstNodeType::AstBinaryExpr:
            data.binary_expr = new AstBinaryExpr();
            break;
        case AstNodeType::AstUnaryExpr:
            data.unary_expr = new AstUnaryExpr();
            break;
        case AstNodeType::AstType:
            data.ast_type = new AstType();
            break;
        default:
            UNREACHEABLE;
        }
    }

    ~AstNode() {
        switch (node_type)
        {
        case AstNodeType::AstSourceCode:
            if (data.source_code)
                delete data.source_code;
            break;
        case AstNodeType::AstDirective:
            if (data.directive)
                delete data.directive;
            break;
        case AstNodeType::AstFuncDef:
            if (data.function_def)
                delete data.function_def;
            break;
        case AstNodeType::AstFuncProto:
            if (data.function_proto)
                delete data.function_proto;
            break;
        case AstNodeType::AstParamDecl:
            if (data.param_decl)
                delete data.param_decl;
            break;
        case AstNodeType::AstBlock:
            if (data.block)
                delete data.block;
            break;
        case AstNodeType::AstType:
            if (data.ast_type)
                delete data.ast_type;
            break;
        case AstNodeType::AstVarDef:
            if (data.var_def)
                delete data.var_def;
            break;
        case AstNodeType::AstSymbol:
            if (data.symbol)
                delete data.symbol;
            break;
        case AstNodeType::AstFuncCallExpr:
            if (data.func_call)
                delete data.func_call;
            break;
        case AstNodeType::AstBinaryExpr:
            if (data.binary_expr)
                delete data.binary_expr;
            break;
        case AstNodeType::AstUnaryExpr:
            if (data.unary_expr)
                delete data.unary_expr;
            break;
        default:
            UNREACHEABLE;
            break;
        }
    }
};
