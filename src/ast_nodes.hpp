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
    DirectiveType       directive_type;
    std::string_view    argument;
};

struct AstFuncDef {
    AstNode* proto;
    AstNode* block;
};

struct AstFuncProto {
    std::string_view        name;
    std::vector<AstNode*>   params;
    AstNode*                return_type;
    AstNode*                fn_def_node;
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


enum class AstTypeType {
    Pointer,
    Array,
    DataType
};

struct AstType {
    AstTypeType             type;   //pointer | Array | DataType
    union {
        AstNode*            data_type; // actual type (Pointer|Array)
        std::string_view    name;   // name of the type
    };

    AstType() : type(AstTypeType::DataType), data_type(nullptr) {}
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


    // actual node
    union {
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
    };


    AstNode(AstNodeType in_node_type, size_t in_line, size_t in_column)
        : parent(nullptr), line(in_line), column(in_column), node_type(in_node_type) {}

    AstNode(const AstNode& other)
        : parent(other.parent), line(other.line), column(other.column), node_type(other.node_type) {
        switch (other.node_type)
        {
        case AstNodeType::AstSourceCode:
            source_code = other.source_code;
            break;
        case AstNodeType::AstDirective:
            directive = other.directive;
            break;
        case AstNodeType::AstFuncDef:
            function_def = other.function_def;
            break;
        case AstNodeType::AstFuncProto:
            function_proto = other.function_proto;
            break;
        case AstNodeType::AstParamDecl:
            param_decl = other.param_decl;
            break;
        case AstNodeType::AstBlock:
            block = other.block;
            break;
        case AstNodeType::AstType:
            ast_type = other.ast_type;
            break;
        case AstNodeType::AstVarDef:
            var_def = other.var_def;
            break;
        case AstNodeType::AstSymbol:
            symbol = other.symbol;
            break;
        case AstNodeType::AstFuncCallExpr:
            func_call = other.func_call;
            break;
        case AstNodeType::AstBinaryExpr:
            binary_expr = other.binary_expr;
            break;
        case AstNodeType::AstUnaryExpr:
            unary_expr = other.unary_expr;
            break;
        default:
            UNREACHEABLE;
        }
    }

    virtual ~AstNode() {}

    AstNode& operator=(const AstNode& other) {
        if (this != &other) // not a self-assignment
        {
            parent      = other.parent;
            line        = other.line;
            column      = other.column;
            node_type   = other.node_type;

            switch (other.node_type)
            {
            case AstNodeType::AstSourceCode:
                source_code = other.source_code;
                break;
            case AstNodeType::AstDirective:
                directive = other.directive;
                break;
            case AstNodeType::AstFuncDef:
                function_def = other.function_def;
                break;
            case AstNodeType::AstFuncProto:
                function_proto = other.function_proto;
                break;
            case AstNodeType::AstParamDecl:
                param_decl = other.param_decl;
                break;
            case AstNodeType::AstBlock:
                block = other.block;
                break;
            case AstNodeType::AstType:
                ast_type = other.ast_type;
                break;
            case AstNodeType::AstVarDef:
                var_def = other.var_def;
                break;
            case AstNodeType::AstSymbol:
                symbol = other.symbol;
                break;
            case AstNodeType::AstFuncCallExpr:
                func_call = other.func_call;
                break;
            case AstNodeType::AstBinaryExpr:
                binary_expr = other.binary_expr;
                break;
            case AstNodeType::AstUnaryExpr:
                unary_expr = other.unary_expr;
                break;
            default:
                UNREACHEABLE;
            }
        }
        return *this;
    }
};
