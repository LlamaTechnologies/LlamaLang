#include <gtest/gtest.h>
#include "../../src/error.hpp"
#include "../../src/ast_nodes.hpp"
#include "../../src/semantic_analyzer.hpp"


//==================================================================================
//          SEMANTIC TYPES ANALYSIS
//==================================================================================

TEST(SemanticTypes, BoolVoid) {
    std::vector<Error> errors;

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Bool;
    node_type_0->ast_type.type_info = new TypeInfo();
    node_type_0->ast_type.type_info->bit_size = 1;
    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Bool;
    node_type_1->ast_type.type_info = new TypeInfo();
    node_type_1->ast_type.type_info->bit_size = 1;


    SemanticAnalyzer analizer(errors);
    analizer.check_type(node_type_0, node_type_1);

    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, StructsUnsuported) {
    std::vector<Error> errors;

    auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_0->ast_type.type_id = AstTypeId::Struct;
    node_type_0->ast_type.type_info = new TypeInfo();
    node_type_0->ast_type.type_info->bit_size = 1;
    auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
    node_type_1->ast_type.type_id = AstTypeId::Struct;
    node_type_1->ast_type.type_info = new TypeInfo();
    node_type_1->ast_type.type_info->bit_size = 1;


    SemanticAnalyzer analizer(errors);

    ASSERT_DEATH(analizer.check_type(node_type_0, node_type_1), "");
}

TEST(SemanticTypes, Pointer) {
    std::vector<Error> errors;

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
    analizer.check_type(node_type_0, node_type_1);

    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Array) {
    std::vector<Error> errors;
    
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
    analizer.check_type(node_type_0, node_type_1);

    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Integer) {
    std::vector<Error> errors;

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
    analizer.check_type(node_type_0, node_type_1);

    ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Float) {
    std::vector<Error> errors;

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
    analizer.check_type(node_type_0, node_type_1);

    ASSERT_EQ(errors.size(), 0L);
}

