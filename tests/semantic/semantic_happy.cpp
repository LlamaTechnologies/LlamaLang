#include <gtest/gtest.h>
#include "../../src/error.hpp"
#include "../../src/ast_nodes.hpp"
#include "../../src/semantic_analyzer.hpp"


//==================================================================================
//          SEMANTIC TYPES ANALYSIS
//==================================================================================

TEST(SemanticTypes, BoolVoid) {
    std::vector<Error> errors;
    auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Bool;
    node_type_0->ast_type.type_info = new TypeInfo();
    node_type_0->ast_type.type_info->bit_size = 1;
    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Bool;
    node_type_1->ast_type.type_info = new TypeInfo();
    node_type_1->ast_type.type_info->bit_size = 1;


    SemanticAnalyzer analizer(errors);
    analizer.check_type_compat(node_type_0, node_type_1, node_expr);


    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, StructsUnsuported) {
    std::vector<Error> errors;
    auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Struct;
    node_type_0->ast_type.type_info = new TypeInfo();
    node_type_0->ast_type.type_info->bit_size = 1;
    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Struct;
    node_type_1->ast_type.type_info = new TypeInfo();
    node_type_1->ast_type.type_info->bit_size = 1;


    SemanticAnalyzer analizer(errors);

    ASSERT_DEATH(analizer.check_type_compat(node_type_0, node_type_1, node_expr), "");
}

TEST(SemanticTypes, Pointer) {
    std::vector<Error> errors;
    auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

    auto node_child_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_child_type_0->ast_type.type_id = AstTypeId::Integer;
    node_child_type_0->ast_type.type_info = new TypeInfo();
    node_child_type_0->ast_type.type_info->bit_size = 32;
    node_child_type_0->ast_type.type_info->is_signed = true;

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Pointer;
    node_type_0->ast_type.child_type = node_child_type_0;

    auto node_child_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_child_type_1->ast_type.type_id = AstTypeId::Integer;
    node_child_type_1->ast_type.type_info = new TypeInfo();
    node_child_type_1->ast_type.type_info->bit_size = 32;
    node_child_type_1->ast_type.type_info->is_signed = true;

    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Pointer;
    node_type_1->ast_type.child_type = node_child_type_1;

    SemanticAnalyzer analizer(errors);
    analizer.check_type_compat(node_type_0, node_type_1, node_expr);

    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Array) {
    std::vector<Error> errors;
    auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    
    auto node_child_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_child_type_0->ast_type.type_id = AstTypeId::Integer;
    node_child_type_0->ast_type.type_info = new TypeInfo();
    node_child_type_0->ast_type.type_info->bit_size = 32;
    node_child_type_0->ast_type.type_info->is_signed = true;

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Array;
    node_type_0->ast_type.child_type = node_child_type_0;

    auto node_child_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_child_type_1->ast_type.type_id = AstTypeId::Integer;
    node_child_type_1->ast_type.type_info = new TypeInfo();
    node_child_type_1->ast_type.type_info->bit_size = 32;
    node_child_type_1->ast_type.type_info->is_signed = true;

    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Array;
    node_type_1->ast_type.child_type = node_child_type_1;

    SemanticAnalyzer analizer(errors);
    analizer.check_type_compat(node_type_0, node_type_1, node_expr);

    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Integer) {
    std::vector<Error> errors;
    auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Integer;
    node_type_0->ast_type.type_info = new TypeInfo();
    node_type_0->ast_type.type_info->bit_size = 32;
    node_type_0->ast_type.type_info->is_signed = true;
    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Integer;
    node_type_1->ast_type.type_info = new TypeInfo();
    node_type_1->ast_type.type_info->bit_size = 32;
    node_type_1->ast_type.type_info->is_signed = true;


    SemanticAnalyzer analizer(errors);
    analizer.check_type_compat(node_type_0, node_type_1, node_expr);

    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Float) {
    std::vector<Error> errors;
    auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::FloatingPoint;
    node_type_0->ast_type.type_info = new TypeInfo();
    node_type_0->ast_type.type_info->bit_size = 1;
    node_type_0->ast_type.type_info->is_signed = true;
    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::FloatingPoint;
    node_type_1->ast_type.type_info = new TypeInfo();
    node_type_1->ast_type.type_info->bit_size = 1;
    node_type_1->ast_type.type_info->is_signed = false;

    SemanticAnalyzer analizer(errors);
    analizer.check_type_compat(node_type_0, node_type_1, node_expr);

    ASSERT_EQ(errors.size(), 0L);
}

//==================================================================================
//          SEMANTIC VARIABLES DEFINITION
//==================================================================================

TEST(SemanticVariableDefinitions, GlobalVariable) {
    bool is_global = true;
    std::vector<Error> errors;

    auto i32_type_node = new AstNode(AstNodeType::AstType, 0, 0, "");
    i32_type_node->ast_type.type_id = AstTypeId::Integer;
    i32_type_node->ast_type.type_info = new TypeInfo();
    i32_type_node->ast_type.type_info->bit_size = 32;
    i32_type_node->ast_type.type_info->is_signed = true;
    i32_type_node->ast_type.type_info->name = "i32";

    BigInt integer_value;
    bigint_init_signed(&integer_value, 0L);

    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;
    const_value_node->const_value.integer = integer_value;

    auto var_def_node= new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    var_def_node->var_def.type = i32_type_node;
    var_def_node->var_def.name = "global_var";
    var_def_node->var_def.initializer = const_value_node;


    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticVariableDefinitions, LocalVariableNoInit) {
    bool is_global = false;
    std::vector<Error> errors;

    auto i32_type_node = new AstNode(AstNodeType::AstType, 0, 0, "");
    i32_type_node->ast_type.type_id = AstTypeId::Integer;
    i32_type_node->ast_type.type_info = new TypeInfo();
    i32_type_node->ast_type.type_info->bit_size = 32;
    i32_type_node->ast_type.type_info->is_signed = true;
    i32_type_node->ast_type.type_info->name = "i32";

    auto var_def_node= new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    var_def_node->var_def.type = i32_type_node;
    var_def_node->var_def.name = "local_var";
    var_def_node->var_def.initializer = nullptr;

    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

//==================================================================================
//          SEMANTIC EXPRESSIONS
//==================================================================================

TEST(SemanticExpressions, ConstantValue) {
    BigInt integer_value;
    bigint_init_signed(&integer_value, 0L);

    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;
    const_value_node->const_value.integer = integer_value;

    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.analizeExpr(const_value_node);

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, ResolveKnownVariableSymbol) {
    // given: variable definition
    auto i32_type_node = new AstNode(AstNodeType::AstType, 0, 0, "");
    i32_type_node->ast_type.type_id = AstTypeId::Integer;
    i32_type_node->ast_type.type_info = new TypeInfo();
    i32_type_node->ast_type.type_info->bit_size = 32;
    i32_type_node->ast_type.type_info->is_signed = true;
    i32_type_node->ast_type.type_info->name = "i32";

    auto var_def_node= new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    var_def_node->var_def.type = i32_type_node;
    auto var_name = var_def_node->var_def.name = "my_var";

    // given: symbol node
    auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
    symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

    // given: configured analizer
    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);
    analizer.analizeVarDef(var_def_node, false);

    // when: call to analize_expr
    bool is_valid = analizer.analizeExpr(symbol_node);

    // then:
    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, UnaryExprBool) {
    // given: expr
    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::BOOL;
    const_value_node->const_value.boolean = false;

    // given: unary expr
    auto unary_epxr_node= new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
    unary_epxr_node->unary_expr.op = UnaryExprType::NOT;
    unary_epxr_node->unary_expr.expr = const_value_node;

    // given: analizer
    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);

    // when: call to analize_expr 
    // with: a bool expresion
    // with: a unary operator valid for the expr type
    bool is_valid = analizer.analizeExpr(unary_epxr_node);

    // then:
    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, UnaryExprNumber) {
    // given: lexed integer number
    BigInt integer_value;
    bigint_init_signed(&integer_value, 0L);

    // given: expr
    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;
    const_value_node->const_value.integer = integer_value;

    // given: unary expr
    auto unary_epxr_node= new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
    unary_epxr_node->unary_expr.op = UnaryExprType::NEG;
    unary_epxr_node->unary_expr.expr = const_value_node;

    // given: analizer
    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);

    // when: call to analize_expr 
    // with: a number expresion
    // with: a unary operator valid for the expr type
    bool is_valid = analizer.analizeExpr(unary_epxr_node);

    // then:
    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, UnaryExprNumberBitwiseOp) {
    // given: lexed integer number
    BigInt integer_value;
    bigint_init_signed(&integer_value, 0L);

    // given: expr
    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;
    const_value_node->const_value.integer = integer_value;

    // given: unary expr
    auto unary_epxr_node= new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
    unary_epxr_node->unary_expr.op = UnaryExprType::BIT_INV;
    unary_epxr_node->unary_expr.expr = const_value_node;

    // given: analizer
    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);

    // when: call to analize_expr 
    // with: a number expresion
    // with: a unary operator valid for the expr type
    bool is_valid = analizer.analizeExpr(unary_epxr_node);

    // then:
    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, BinaryExprBitShift) {
    // given: lexed integer number
    BigInt integer_value;
    bigint_init_signed(&integer_value, 1L);

    // given: expr
    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;
    const_value_node->const_value.integer = integer_value;

    // given: biary expr
    auto binary_epxr_node= new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
    // bin_expr -> 1 << 1
    binary_epxr_node->binary_expr.bin_op = BinaryExprType::LSHIFT;
    binary_epxr_node->binary_expr.left_expr = const_value_node;
    binary_epxr_node->binary_expr.right_expr = const_value_node;

    // given: analizer
    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);

    // when: call to analize_expr 
    // with: 2 constants expresions as child expr
    // with: right expr of type integer
    bool is_valid = analizer.analizeExpr(binary_epxr_node);

    // then:
    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, BinaryExprBoolOperator) {
    // given: variable definition
    auto i32_type_node = new AstNode(AstNodeType::AstType, 0, 0, "");
    i32_type_node->ast_type.type_id = AstTypeId::Integer;
    i32_type_node->ast_type.type_info = new TypeInfo();
    i32_type_node->ast_type.type_info->bit_size = 32;
    i32_type_node->ast_type.type_info->is_signed = true;
    i32_type_node->ast_type.type_info->name = "i32";

    auto var_def_node= new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    var_def_node->var_def.type = i32_type_node;
    auto var_name = var_def_node->var_def.name = "my_var";

    // given: l_expr -> symbol node
    auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
    symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

    // given: r_expr -> constant integer
    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;

    // given: biary expr -> my_var == SOME_INT
    auto binary_epxr_node= new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
    binary_epxr_node->binary_expr.bin_op = BinaryExprType::EQUALS;
    binary_epxr_node->binary_expr.left_expr = symbol_node;
    binary_epxr_node->binary_expr.right_expr = const_value_node;

    // given: analizer
    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);
    bool is_valid_var_def = analizer.analizeVarDef(var_def_node, false);

    // when: call to analize_expr 
    // with: symbol as left expr
    // with: constant as right expr
    bool is_valid = analizer.analizeExpr(binary_epxr_node);

    // then:
    ASSERT_EQ(is_valid_var_def, true);
    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, BinaryExprAssignOperator) {
    // given: variable definition
    auto i32_type_node = new AstNode(AstNodeType::AstType, 0, 0, "");
    i32_type_node->ast_type.type_id = AstTypeId::Integer;
    i32_type_node->ast_type.type_info = new TypeInfo();
    i32_type_node->ast_type.type_info->bit_size = 32;
    i32_type_node->ast_type.type_info->is_signed = true;
    i32_type_node->ast_type.type_info->name = "i32";

    auto var_def_node= new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    var_def_node->var_def.type = i32_type_node;
    auto var_name = var_def_node->var_def.name = "my_var";

    // given: l_expr -> symbol node
    auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
    symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

    // given: r_expr -> constant integer
    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;

    // given: biary expr -> my_var = SOME_INT
    auto binary_epxr_node= new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
    binary_epxr_node->binary_expr.bin_op = BinaryExprType::ASSIGN;
    binary_epxr_node->binary_expr.left_expr = symbol_node;
    binary_epxr_node->binary_expr.right_expr = const_value_node;

    // given: analizer
    std::vector<Error> errors;
    SemanticAnalyzer analizer(errors);
    bool is_valid_var_def = analizer.analizeVarDef(var_def_node, false);

    // when: call to analize_expr 
    // with: symbol as left expr
    // with: constant as right expr
    bool is_valid = analizer.analizeExpr(binary_epxr_node);

    // then:
    ASSERT_EQ(is_valid_var_def, true);
    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(is_valid, true);
}
