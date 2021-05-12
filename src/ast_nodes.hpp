#pragma once
#include <vector>
#include <string>

// ast nodes
struct AstDirective;

struct AstFuncDef;
struct AstFuncProto;
struct AstParamDecl;
struct AstBlock;

struct AstVarDef;
struct AstSymbol;
struct AstFuncCallExpr;
struct AstBinaryExpr;
struct AstUnaryExpr;


// ast nodes enum
enum class NodeType {
    AstDirective,
    AstFuncDef,
    AstFuncProto,
    AstParamDecl,
    AstBlock,
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
    NodeType node_type;


    // actual node
    union {
        AstDirective*       directive;      // # dir_name args*
        AstFuncDef*         function_def;   // function definition
        AstFuncProto*       function_proto; // fn name L_PAREN param_decl (, param_decl)* R_PAREN
        AstParamDecl*       param_decl;     // name type
        AstBlock*           block;          // L_CURLY statements R_CURLY
        AstVarDef*          var_def;        // name type

        AstNode*            grouped_expr;   // L_PAREN expr R_PAREN
        AstUnaryExpr*       unary_expr;     // unary_op expr
        AstBinaryExpr*      binary_expr;    // expr binary_op expr
        AstSymbol*          var_ref;        // symbol_name
        AstFuncCallExpr*    func_call;      // func_name L_PAREN (expr (, expr)*)? R_PAREN
    } data;
};

struct AstDirective {
    enum class DirectiveType {
        Run,
        Load,
        CompTimeOnly
    };

    DirectiveType directive_type;
    std::vector<const std::string> arguments;
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
    std::string symbol;
    AstNode*    type;
};

struct AstSymbol {
    std::string symbol;
};

struct AstFuncCallExpr {
    AstNode* fn_ref;
    std::vector<AstNode*> params;
};

enum class BinaryExprType {
    ADD,    // expr + expr
    SUB,    // expr - expr
    MUL,    // expr * expr
    DIV,    // expr / expr
    MOD,    // expr % expr
    ASSIGN, // expr = expr
};

struct AstBinaryExpr {
    AstNode* op1;
    BinaryExprType bin_op;
    AstNode* op2;
};


enum class UnaryExprOp {
    INC,    // ++  primaryExpr
    DEC,    // --  primaryExpr
    NEG,    // -   primaryExpr  
    RET     // ret Expr
};

struct AstUnaryExpr {
    UnaryExprOp op;
    AstNode* primary_expr;
};

