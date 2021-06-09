#include <gtest/gtest.h>
#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

//==================================================================================
//          PARSE RETURN STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, RetStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret myVar * (5 + 8)", "Ret stmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_ret_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(value_node->unary_expr.expr, nullptr);
    ASSERT_EQ(value_node->unary_expr.expr->parent, value_node);
}

TEST(ParserHappyStmntTests, RetEmptyStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret ", "RetEmptyStmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_ret_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_EQ(value_node->unary_expr.expr, nullptr);
}

//==================================================================================
//          PARSE ASSIGN STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, AssignStmntTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar = 5 * (6 + 9)", "AssignStmntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_assign_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr); 
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ASSIGN);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstSymbol);
    ASSERT_EQ(value_node->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

//==================================================================================
//          PARSE TYPES
//==================================================================================

TEST(ParserHappyStmntTests, TypeNameParse) {
    std::vector<Error> errors;
    Lexer lexer("MyType", "TypeNameParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_type();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(value_node->ast_type.name, "MyType");
}

TEST(ParserHappyStmntTests, TypeArrayParse) {
    std::vector<Error> errors;
    Lexer lexer("[]MyType", "TypeArrayParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_type();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.type, AstTypeType::Array);
    ASSERT_NE(value_node->ast_type.data_type, nullptr);
    ASSERT_EQ(value_node->ast_type.data_type->parent, value_node);
    ASSERT_EQ(value_node->ast_type.data_type->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.name, "MyType");
}

TEST(ParserHappyStmntTests, TypePointerParse) {
    std::vector<Error> errors;
    Lexer lexer("*MyType", "TypePointerParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_type();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.type, AstTypeType::Pointer);
    ASSERT_NE(value_node->ast_type.data_type, nullptr);
    ASSERT_EQ(value_node->ast_type.data_type->parent, value_node);
    ASSERT_EQ(value_node->ast_type.data_type->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.name, "MyType");
}

//==================================================================================
//          PARSE VARIABLE DEFINITION STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, VarDefSimpleTypeParse) {
    std::vector<Error> errors;
    Lexer lexer("myVar i32", "VarDefSimpleTypeParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_vardef_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
    ASSERT_EQ(value_node->var_def.name, "myVar");
    ASSERT_NE(value_node->var_def.type, nullptr);
    auto type_node = value_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->parent, value_node);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(type_node->ast_type.name, "i32");
}

TEST(ParserHappyStmntTests, VarDefArrayTypeParse) {
    std::vector<Error> errors;
    Lexer lexer("myVar []i32", "VarDefArrayTypeParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_vardef_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
    ASSERT_EQ(value_node->var_def.name, "myVar");
    ASSERT_NE(value_node->var_def.type, nullptr);
    auto type_node = value_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->parent, value_node);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Array);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(data_type_node->parent, type_node);
    ASSERT_EQ(data_type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(data_type_node->ast_type.name, "i32");
}

TEST(ParserHappyStmntTests, VarDefPointerTypeParse) {
    std::vector<Error> errors;
    Lexer lexer("myVar *i32", "VarDefPointerTypeParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_vardef_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
    ASSERT_EQ(value_node->var_def.name, "myVar");
    ASSERT_NE(value_node->var_def.type, nullptr);
    auto type_node = value_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->parent, value_node);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Pointer);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(data_type_node->parent, type_node);
    ASSERT_EQ(data_type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(data_type_node->ast_type.name, "i32");
}


//==================================================================================
//          PARSE ANY STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, StatementVarDefSimpleTypeParse) {
    std::vector<Error> errors;
    Lexer lexer("myVar i32", "VarDefSimpleTypeParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_statement();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
    ASSERT_EQ(value_node->var_def.name, "myVar");
    ASSERT_NE(value_node->var_def.type, nullptr);
    auto type_node = value_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->parent, value_node);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(type_node->ast_type.name, "i32");
}

TEST(ParserHappyStmntTests, StatementVarDefArrayTypeParse) {
    std::vector<Error> errors;
    Lexer lexer("myVar []i32", "VarDefArrayTypeParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_statement();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
    ASSERT_EQ(value_node->var_def.name, "myVar");
    ASSERT_NE(value_node->var_def.type, nullptr);
    auto type_node = value_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->parent, value_node);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Array);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(data_type_node->parent, type_node);
    ASSERT_EQ(data_type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(data_type_node->ast_type.name, "i32");
}

TEST(ParserHappyStmntTests, StatementVarDefPointerTypeParse) {
    std::vector<Error> errors;
    Lexer lexer("myVar *i32", "VarDefPointerTypeParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_statement();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
    ASSERT_EQ(value_node->var_def.name, "myVar");
    ASSERT_NE(value_node->var_def.type, nullptr);
    auto type_node = value_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->parent, value_node);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Pointer);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(data_type_node->parent, type_node);
    ASSERT_EQ(data_type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(data_type_node->ast_type.name, "i32");
}

TEST(ParserHappyStmntTests, StatementAssignStmntTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar = 5 * (6 + 9)", "AssignStmntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_statement();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ASSIGN);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstSymbol);
    ASSERT_EQ(value_node->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyStmntTests, StatementRetStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret myVar * (5 + 8)", "Ret stmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_statement();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(value_node->unary_expr.expr, nullptr);
    ASSERT_EQ(value_node->unary_expr.expr->parent, value_node);
}

TEST(ParserHappyStmntTests, StatementRetEmptyStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret ", "RetEmptyStmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_statement();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_EQ(value_node->unary_expr.expr, nullptr);
}


//==================================================================================
//          PARSE BLOCK STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, BlockEmptyTest) {
    std::vector<Error> errors;
    Lexer lexer("{}", "BlockEmptyTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_block();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->block.statements.size(), 0L);
}

TEST(ParserHappyStmntTests, BlockNoSpaceNearCurliesTest) {
    std::vector<Error> errors;
    Lexer lexer("{myVar i32\nret myvar}", "BlockNoSpaceNearCurliesTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_block();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->block.statements.size(), 2);

    ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
    AstNode* var_def_node = value_node->block.statements.at(0);
    ASSERT_EQ(var_def_node->parent, value_node);
    ASSERT_EQ(var_def_node->var_def.name, "myVar");
    ASSERT_NE(var_def_node->var_def.type, nullptr);
    AstNode* type_node = var_def_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(type_node->ast_type.name, "i32");

    ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
    AstNode* ret_node = value_node->block.statements.at(1);
    ASSERT_EQ(ret_node->parent, value_node);
    ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(ret_node->unary_expr.expr, nullptr);
}

TEST(ParserHappyStmntTests, BlockSpaceNearCurliesTest) {
    std::vector<Error> errors;
    Lexer lexer("{ myVar i32\nret myvar }", "BlockSpaceNearCurliesTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_block();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->block.statements.size(), 2);

    ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
    AstNode* var_def_node = value_node->block.statements.at(0);
    ASSERT_EQ(var_def_node->parent, value_node);
    ASSERT_EQ(var_def_node->var_def.name, "myVar");
    ASSERT_NE(var_def_node->var_def.type, nullptr);
    AstNode* type_node = var_def_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(type_node->ast_type.name, "i32");

    ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
    AstNode* ret_node = value_node->block.statements.at(1);
    ASSERT_EQ(ret_node->parent, value_node);
    ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(ret_node->unary_expr.expr, nullptr);
}

TEST(ParserHappyStmntTests, BlockNewlinesNearCurliesTest) {
    std::vector<Error> errors;
    Lexer lexer("{\n\tmyVar i32\n\tret myvar\n}", "BlockNewlinesNearCurliesTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_block();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->block.statements.size(), 2);

    ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
    AstNode* var_def_node = value_node->block.statements.at(0);
    ASSERT_EQ(var_def_node->parent, value_node);
    ASSERT_EQ(var_def_node->var_def.name, "myVar");
    ASSERT_NE(var_def_node->var_def.type, nullptr);
    AstNode* type_node = var_def_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(type_node->ast_type.name, "i32");

    ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
    AstNode* ret_node = value_node->block.statements.at(1);
    ASSERT_EQ(ret_node->parent, value_node);
    ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(ret_node->unary_expr.expr, nullptr);
}

TEST(ParserHappyStmntTests, BlockSpacesBetweenNewlinesTest) {
    std::vector<Error> errors;
    Lexer lexer("{ \n  \t myVar i32   \n  \t  ret myvar  \n   }", "BlockSpacesBetweenNewlinesTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_block();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->block.statements.size(), 2);

    ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
    AstNode* var_def_node = value_node->block.statements.at(0);
    ASSERT_EQ(var_def_node->parent, value_node);
    ASSERT_EQ(var_def_node->var_def.name, "myVar");
    ASSERT_NE(var_def_node->var_def.type, nullptr);
    AstNode* type_node = var_def_node->var_def.type;
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(type_node->ast_type.name, "i32");

    ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
    AstNode* ret_node = value_node->block.statements.at(1);
    ASSERT_EQ(ret_node->parent, value_node);
    ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(ret_node->unary_expr.expr, nullptr);
}

//==================================================================================
//          PARSE FUNCTION DECLARATION STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, FuncProtoEmptyParamTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc() void", "FuncProtoEmptyParamTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_proto();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(value_node->function_proto.name, "myFunc");
    ASSERT_EQ(value_node->function_proto.params.size(), 0);
    ASSERT_NE(value_node->function_proto.return_type, nullptr);
    auto ret_type_node = value_node->function_proto.return_type;
    ASSERT_EQ(ret_type_node->parent, value_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "void");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}

TEST(ParserHappyStmntTests, FuncProtoSingleParamTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc(param1 i32) void", "FuncProtoSingleParamTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_proto();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(value_node->function_proto.name, "myFunc");
    ASSERT_NE(value_node->function_proto.return_type, nullptr);
    ASSERT_EQ(value_node->function_proto.params.size(), 1L);
    
    auto param_node = value_node->function_proto.params.at(0L);
    ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
    ASSERT_EQ(param_node->parent, value_node);
    ASSERT_EQ(param_node->param_decl.name, "param1");
    ASSERT_NE(param_node->param_decl.type, nullptr);
    ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
    ASSERT_EQ(param_node->param_decl.type->parent, param_node);
    ASSERT_EQ(param_node->param_decl.type->ast_type.name, "i32");
    ASSERT_EQ(param_node->param_decl.type->ast_type.type, AstTypeType::DataType);

    auto ret_type_node = value_node->function_proto.return_type;
    ASSERT_EQ(ret_type_node->parent, value_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "void");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}

TEST(ParserHappyStmntTests, FuncProtoMultiParamTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc(param1 i32, param1 i32, param1 i32) i32", "FuncProtoMultiParamTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_proto();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(value_node->function_proto.name, "myFunc");
    ASSERT_NE(value_node->function_proto.return_type, nullptr);
    ASSERT_EQ(value_node->function_proto.params.size(), 3L);

    for (auto param_node : value_node->function_proto.params) {
        ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
        ASSERT_EQ(param_node->parent, value_node);
        ASSERT_EQ(param_node->param_decl.name, "param1");
        ASSERT_NE(param_node->param_decl.type, nullptr);
        ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
        ASSERT_EQ(param_node->param_decl.type->parent, param_node);
        ASSERT_EQ(param_node->param_decl.type->ast_type.name, "i32");
        ASSERT_EQ(param_node->param_decl.type->ast_type.type, AstTypeType::DataType);
    }

    auto ret_type_node = value_node->function_proto.return_type;
    ASSERT_EQ(ret_type_node->parent, value_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "i32");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}

TEST(ParserHappyStmntTests, FuncProtoMultiLineTest) {
    std::vector<Error> errors;
    Lexer lexer("fn\nmyFunc\n(param1 i32\n, param1 i32,\n param1\ni32)\ni32", "FuncProtoMultiLineTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_proto();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(value_node->function_proto.name, "myFunc");
    ASSERT_NE(value_node->function_proto.return_type, nullptr);
    ASSERT_EQ(value_node->function_proto.params.size(), 3L);

    for (auto param_node : value_node->function_proto.params) {
        ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
        ASSERT_EQ(param_node->parent, value_node);
        ASSERT_EQ(param_node->param_decl.name, "param1");
        ASSERT_NE(param_node->param_decl.type, nullptr);
        ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
        ASSERT_EQ(param_node->param_decl.type->parent, param_node);
        ASSERT_EQ(param_node->param_decl.type->ast_type.name, "i32");
        ASSERT_EQ(param_node->param_decl.type->ast_type.type, AstTypeType::DataType);
    }

    auto ret_type_node = value_node->function_proto.return_type;
    ASSERT_EQ(ret_type_node->parent, value_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "i32");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}


//==================================================================================
//          PARSE FUNCTION DEFINITION STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, FuncDefNoParamsVoidBlockTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc() void {}", "FuncDefNoParamsVoidBlockTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_def();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
    ASSERT_NE(value_node->function_def.block, nullptr);
    ASSERT_NE(value_node->function_def.proto, nullptr);
    ASSERT_EQ(value_node->function_def.block->parent, value_node);
    ASSERT_EQ(value_node->function_def.block->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->function_def.block->block.statements.size(), 0L);

    auto proto_node = value_node->function_def.proto;
    ASSERT_EQ(proto_node->parent, value_node);
    ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(proto_node->function_proto.name, "myFunc");
    ASSERT_EQ(proto_node->function_proto.params.size(), 0L);

    auto ret_type_node = proto_node->function_proto.return_type;
    ASSERT_NE(ret_type_node, nullptr);
    ASSERT_EQ(ret_type_node->parent, proto_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "void");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}

TEST(ParserHappyStmntTests, FuncDefSingleParamsVoidBlockTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc(param1 i32) i32 {}", "FuncDefSingleParamsVoidBlockTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_def();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
    ASSERT_NE(value_node->function_def.block, nullptr);
    ASSERT_NE(value_node->function_def.proto, nullptr);
    ASSERT_EQ(value_node->function_def.block->parent, value_node);
    ASSERT_EQ(value_node->function_def.block->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->function_def.block->block.statements.size(), 0L);

    auto proto_node = value_node->function_def.proto;
    ASSERT_EQ(proto_node->parent, value_node);
    ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(proto_node->function_proto.name, "myFunc");
    ASSERT_NE(proto_node->function_proto.return_type, nullptr);
    ASSERT_EQ(proto_node->function_proto.params.size(), 1L);

    auto param_node = proto_node->function_proto.params.at(0L);
    ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
    ASSERT_EQ(param_node->parent, proto_node);
    ASSERT_EQ(param_node->param_decl.name, "param1");
    ASSERT_NE(param_node->param_decl.type, nullptr);
    ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
    ASSERT_EQ(param_node->param_decl.type->parent, param_node);
    ASSERT_EQ(param_node->param_decl.type->ast_type.name, "i32");
    ASSERT_EQ(param_node->param_decl.type->ast_type.type, AstTypeType::DataType);

    auto ret_type_node = proto_node->function_proto.return_type;
    ASSERT_NE(ret_type_node, nullptr);
    ASSERT_EQ(ret_type_node->parent, proto_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "i32");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}

TEST(ParserHappyStmntTests, FuncDefMultiParamsVoidBlockTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc(param1 i32, param1 i32, param1 i32) i32 {}", "FuncDefMultiParamsVoidBlockTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_def();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
    ASSERT_NE(value_node->function_def.block, nullptr);
    ASSERT_NE(value_node->function_def.proto, nullptr);
    ASSERT_EQ(value_node->function_def.block->parent, value_node);
    ASSERT_EQ(value_node->function_def.block->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(value_node->function_def.block->block.statements.size(), 0L);


    auto proto_node = value_node->function_def.proto;
    ASSERT_EQ(proto_node->parent, value_node);
    ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(proto_node->function_proto.name, "myFunc");
    ASSERT_NE(proto_node->function_proto.return_type, nullptr);
    ASSERT_EQ(proto_node->function_proto.params.size(), 3L);

    for (auto param_node : proto_node->function_proto.params) {
        ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
        ASSERT_EQ(param_node->parent, proto_node);
        ASSERT_EQ(param_node->param_decl.name, "param1");
        ASSERT_NE(param_node->param_decl.type, nullptr);
        ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
        ASSERT_EQ(param_node->param_decl.type->parent, param_node);
        ASSERT_EQ(param_node->param_decl.type->ast_type.name, "i32");
        ASSERT_EQ(param_node->param_decl.type->ast_type.type, AstTypeType::DataType);
    }

    auto ret_type_node = proto_node->function_proto.return_type;
    ASSERT_NE(ret_type_node, nullptr);
    ASSERT_EQ(ret_type_node->parent, proto_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "i32");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}

TEST(ParserHappyStmntTests, FuncDefNoParamsTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc() void {\nmyVar i32\n}", "FuncDefNoParamsTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_def();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
    ASSERT_NE(value_node->function_def.block, nullptr);
    ASSERT_NE(value_node->function_def.proto, nullptr);
    auto block_node = value_node->function_def.block;
    ASSERT_EQ(block_node->parent, value_node);
    ASSERT_EQ(block_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(block_node->block.statements.size(), 1L);

    auto var_def_node = value_node->function_def.block->block.statements.at(0);
    ASSERT_NE(var_def_node, nullptr);
    ASSERT_EQ(var_def_node->parent, block_node);
    ASSERT_EQ(var_def_node->node_type, AstNodeType::AstVarDef);
    ASSERT_EQ(var_def_node->var_def.name, "myVar");
    ASSERT_NE(var_def_node->var_def.type, nullptr);
    ASSERT_EQ(var_def_node->var_def.initializer, nullptr);

    auto type_node = var_def_node->var_def.type;
    ASSERT_EQ(type_node->parent, var_def_node);
    ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(type_node->ast_type.name, "i32");

    auto proto_node = value_node->function_def.proto;
    ASSERT_EQ(proto_node->parent, value_node);
    ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(proto_node->function_proto.name, "myFunc");
    ASSERT_EQ(proto_node->function_proto.params.size(), 0L);

    auto ret_type_node = proto_node->function_proto.return_type;
    ASSERT_NE(ret_type_node, nullptr);
    ASSERT_EQ(ret_type_node->parent, proto_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "void");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}

TEST(ParserHappyStmntTests, FuncDefSingleParamsTest) {
    std::vector<Error> errors;
    Lexer lexer("fn myFunc(param1 i32) i32 {\nret param1 + 5\n}", "FuncDefSingleParamsTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* value_node = parser.parse_function_def();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
    ASSERT_NE(value_node->function_def.block, nullptr);
    ASSERT_NE(value_node->function_def.proto, nullptr);

    auto block_node = value_node->function_def.block;
    ASSERT_EQ(block_node->parent, value_node);
    ASSERT_EQ(block_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(block_node->block.statements.size(), 1L);

    auto ret_node = block_node->block.statements.at(0);
    ASSERT_NE(ret_node, nullptr);
    ASSERT_EQ(ret_node->parent, block_node);
    ASSERT_EQ(ret_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(ret_node->unary_expr.expr, nullptr);
    ASSERT_EQ(ret_node->unary_expr.expr->parent, ret_node);
    ASSERT_EQ(ret_node->unary_expr.expr->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(ret_node->unary_expr.expr->binary_expr.bin_op, BinaryExprType::ADD);

    auto proto_node = value_node->function_def.proto;
    ASSERT_EQ(proto_node->parent, value_node);
    ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(proto_node->function_proto.name, "myFunc");
    ASSERT_NE(proto_node->function_proto.return_type, nullptr);
    ASSERT_EQ(proto_node->function_proto.params.size(), 1L);

    auto param_node = proto_node->function_proto.params.at(0L);
    ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
    ASSERT_EQ(param_node->parent, proto_node);
    ASSERT_EQ(param_node->param_decl.name, "param1");
    ASSERT_NE(param_node->param_decl.type, nullptr);
    ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
    ASSERT_EQ(param_node->param_decl.type->parent, param_node);
    ASSERT_EQ(param_node->param_decl.type->ast_type.name, "i32");
    ASSERT_EQ(param_node->param_decl.type->ast_type.type, AstTypeType::DataType);

    auto ret_type_node = proto_node->function_proto.return_type;
    ASSERT_NE(ret_type_node, nullptr);
    ASSERT_EQ(ret_type_node->parent, proto_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "i32");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}


//==================================================================================
//          PARSE FULL PROGRAM
//==================================================================================

TEST(ParserHappyStmntTests, FullProgramNoNewLineEnd) {
    const char* source_code = 
        "/* Test program */\n" // multiline comment
        "\n"
        ";\n"           // empty statement : discarded
        ";"             // empty statement : discarded
        "myVar i32\n"   // variable definition
        "myVar\n"       // unused variable reference : discarded
        "myVar;\n"      // unused variable reference : discarded
        "\n"
        "fn myFunc() void {\n"
        "\tret\n"
        "}";

    std::vector<Error> errors;
    Lexer lexer(source_code, "FullProgramNoNewLineEnd", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    AstNode* source_code_node = parser.parse();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(source_code_node, nullptr);
    ASSERT_EQ(source_code_node->node_type, AstNodeType::AstSourceCode);
    ASSERT_EQ(source_code_node->source_code.children.size(), 2L);
    auto main_stmnts = source_code_node->source_code.children;

    AstNode* var_def_node = main_stmnts.at(0);
    AstNode* func_def_node = main_stmnts.at(1);
    ASSERT_NE(func_def_node, nullptr);
    ASSERT_EQ(func_def_node->node_type, AstNodeType::AstFuncDef);
    ASSERT_NE(func_def_node->function_def.block, nullptr);
    ASSERT_NE(func_def_node->function_def.proto, nullptr);

    auto block_node = func_def_node->function_def.block;
    ASSERT_EQ(block_node->parent, func_def_node);
    ASSERT_EQ(block_node->node_type, AstNodeType::AstBlock);
    ASSERT_EQ(block_node->block.statements.size(), 1L);

    auto ret_node = block_node->block.statements.at(0);
    ASSERT_NE(ret_node, nullptr);
    ASSERT_EQ(ret_node->parent, block_node);
    ASSERT_EQ(ret_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_EQ(ret_node->unary_expr.expr, nullptr);

    auto proto_node = func_def_node->function_def.proto;
    ASSERT_EQ(proto_node->parent, func_def_node);
    ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
    ASSERT_EQ(proto_node->function_proto.name, "myFunc");
    ASSERT_EQ(proto_node->function_proto.params.size(), 0L);

    auto ret_type_node = proto_node->function_proto.return_type;
    ASSERT_NE(ret_type_node, nullptr);
    ASSERT_EQ(ret_type_node->parent, proto_node);
    ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(ret_type_node->ast_type.name, "void");
    ASSERT_EQ(ret_type_node->ast_type.type, AstTypeType::DataType);
}
