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
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Array);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
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
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Pointer);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
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
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Array);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
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
    ASSERT_EQ(type_node->ast_type.type, AstTypeType::Pointer);
    ASSERT_NE(type_node->ast_type.data_type, nullptr);
    auto data_type_node = type_node->ast_type.data_type;
    ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
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
