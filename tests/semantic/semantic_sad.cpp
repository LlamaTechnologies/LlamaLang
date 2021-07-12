#include <gtest/gtest.h>
#include "../../src/error.hpp"
#include "../../src/ast_nodes.hpp"
#include "../../src/semantic_analyzer.hpp"


//==================================================================================
//          SEMANTIC TYPES ANALYSIS
//==================================================================================

TEST(SemanticTypes, DistinctTypes) {
    std::vector<Error> errors;
    auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Bool;
    node_type_0->ast_type.type_info = new TypeInfo();
    node_type_0->ast_type.type_info->bit_size = 1;
    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Integer;
    node_type_1->ast_type.type_info = new TypeInfo();
    node_type_1->ast_type.type_info->bit_size = 32;


    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.check_type_compat(node_type_0, node_type_1, node_expr);


    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(is_valid, false);
}

TEST(SemanticTypes, PointerDistinctTypes) {
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
    node_child_type_1->ast_type.type_id = AstTypeId::FloatingPoint;
    node_child_type_1->ast_type.type_info = new TypeInfo();
    node_child_type_1->ast_type.type_info->bit_size = 32;
    node_child_type_1->ast_type.type_info->is_signed = true;

    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Pointer;
    node_type_1->ast_type.child_type = node_child_type_1;

    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.check_type_compat(node_type_0, node_type_1, node_expr);

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(is_valid, false);
}

TEST(SemanticTypes, ArrayDistinctTypes) {
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
    node_child_type_1->ast_type.type_id = AstTypeId::FloatingPoint;
    node_child_type_1->ast_type.type_info = new TypeInfo();
    node_child_type_1->ast_type.type_info->bit_size = 32;
    node_child_type_1->ast_type.type_info->is_signed = true;

    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Array;
    node_type_1->ast_type.child_type = node_child_type_1;

    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.check_type_compat(node_type_0, node_type_1, node_expr);

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(is_valid, false);
}

//==================================================================================
//          SEMANTIC VARIABLES DEFINITION
//==================================================================================

TEST(SemanticVariableDefinitions, GlobalVariableNoInit) {
    bool is_global = true;
    std::vector<Error> errors;

    auto i32_type_node = new AstNode(AstNodeType::AstType, 0, 0, "");
    i32_type_node->ast_type.type_id = AstTypeId::Integer;
    i32_type_node->ast_type.type_info = new TypeInfo();
    i32_type_node->ast_type.type_info->bit_size = 32;
    i32_type_node->ast_type.type_info->is_signed = true;
    i32_type_node->ast_type.type_info->name = "i32";

    auto var_def_node= new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    var_def_node->var_def.type = i32_type_node;
    var_def_node->var_def.name = "global_var";
    var_def_node->var_def.initializer = nullptr;

    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(is_valid, false);
}

TEST(SemanticVariableDefinitions, LocalVariableTypeMismatch) {
    bool is_global = false;
    std::vector<Error> errors;

    auto f32_type_node = new AstNode(AstNodeType::AstType, 0, 0, "");
    f32_type_node->ast_type.type_id = AstTypeId::FloatingPoint;
    f32_type_node->ast_type.type_info = new TypeInfo();
    f32_type_node->ast_type.type_info->bit_size = 32;
    f32_type_node->ast_type.type_info->is_signed = true;
    f32_type_node->ast_type.type_info->name = "f32";

    BigInt integer_value;
    bigint_init_signed(&integer_value, 0L);

    auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
    const_value_node->const_value.type = ConstValueType::INT;
    const_value_node->const_value.integer = integer_value;

    auto var_def_node= new AstNode(AstNodeType::AstVarDef, 0, 0, "");
    var_def_node->var_def.type = f32_type_node;
    var_def_node->var_def.name = "local_var";
    var_def_node->var_def.initializer = const_value_node;

    SemanticAnalyzer analizer(errors);
    bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(is_valid, false);
}