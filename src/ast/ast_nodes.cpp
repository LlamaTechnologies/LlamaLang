#include "ast_nodes.hpp"

#include "../semantics/Types.hpp"
#include "../tokenizer/Tokenizer.hpp"

#include <algorithm>

#define LL_DEFAULT_TYPE TypesRepository::get().get_type("void")

static const std::vector<const char *> directives_keywords = { "LOAD", "MAIN", "RUN", "COMPILE", "FN_TYPE" };

const std::string get_directive_type_name(const DirectiveType directive_type) noexcept {
  LL_ASSERT(directive_type <= DirectiveType::FN_TYPE);
  return directives_keywords.at((size_t)directive_type);
}

static const std::vector<const char *> types_id_names = { "VOID",    "BOOL",  "INTEGER", "FLOATING_POINT",
                                                          "POINTER", "ARRAY", "STRUCT",  "UNKNOWN" };

const std::string_view get_type_id_name(AstTypeId in_type_id) noexcept {
  LL_ASSERT(in_type_id <= AstTypeId::UNKNOWN);
  return types_id_names.at((size_t)in_type_id);
}

const std::string_view get_type_id_name_lower_case(AstTypeId in_type_id) noexcept {
  LL_ASSERT(in_type_id <= AstTypeId::UNKNOWN);

  static std::vector<std::string> types_id_names_lower_case;
  if (types_id_names_lower_case.empty()) {
    for (std::string str : types_id_names) {
      std::transform(str.begin(), str.end(), str.begin(), ::tolower);
      types_id_names_lower_case.push_back(str);
    }
  }

  return types_id_names_lower_case.at((size_t)in_type_id);
}

static const std::vector<const char *> unary_operators_symbols = { "++", "--", "-", "!", "~", "&", "*", "ret" };

const std::string get_unary_op_symbol(const UnaryExprType op_type) noexcept {
  LL_ASSERT(op_type < UnaryExprType::RET);
  return unary_operators_symbols[(size_t)op_type];
}

AstType::AstType()
    : AstNode(AstNodeType::AST_TYPE, 0L, 0L, "predefined"), child_type(nullptr), type_info(LL_DEFAULT_TYPE) {}

AstType::AstType(const size_t in_line, const size_t in_column, const std::string_view in_file_name)
    : AstNode(AstNodeType::AST_TYPE, in_line, in_column, in_file_name), child_type(nullptr),
      type_info(LL_DEFAULT_TYPE) {}

AstType::AstType(const TypeInfo *in_type)
    : AstNode(AstNodeType::AST_TYPE, 0L, 0L, "predefined"), child_type(nullptr), type_info(in_type) {}

AstType::AstType(const TypeInfo *in_type, const size_t in_line, const size_t in_column,
                 const std::string_view in_file_name)
    : AstNode(AstNodeType::AST_TYPE, in_line, in_column, in_file_name), child_type(nullptr), type_info(in_type) {}

AstFnDef::~AstFnDef() {
  if (block) {
    delete block;
    block = nullptr;
  }
  if (proto) {
    delete proto;
    proto = nullptr;
  }
}

AstCtrlStmnt::~AstCtrlStmnt() {
  // loop ref is owned by parser
  if (this->label) {
    delete[] label;
  }
}

AstFnProto::~AstFnProto() {
  if (!params.empty()) {
    for (AstParamDef *param : params) {
      if (param) {
        delete param;
        param = nullptr;
      }
    }
  }

  // Types are deleted by the type allocator
}

AstIfStmnt::~AstIfStmnt() {
  if (condition_expr) {
    delete condition_expr;
    condition_expr = nullptr;
  }

  if (true_block) {
    delete true_block;
    true_block = nullptr;
  }

  if (false_block) {
    delete false_block;
    false_block = nullptr;
  }
}

AstLoopStmnt::~AstLoopStmnt() {
  if (initializer_block) {
    delete initializer_block;
    initializer_block = nullptr;
  }

  if (condition_expr) {
    delete condition_expr;
    condition_expr = nullptr;
  }

  if (header_block) {
    delete header_block;
    header_block = nullptr;
  }

  if (content_block) {
    delete content_block;
    content_block = nullptr;
  }

  if (footer_block) {
    delete footer_block;
    footer_block = nullptr;
  }
}

AstBlock::~AstBlock() {
  if (!statements.empty()) {
    for (AstNode *&stmnt : statements) {
      if (stmnt) {
        delete stmnt;
        stmnt = nullptr;
      }
    }
  }
}

AstVarDef::~AstVarDef() {
  if (initializer) {
    delete initializer;
    initializer = nullptr;
  }
  // Types are deleted by the type allocator
}

AstFnCallExpr::~AstFnCallExpr() {
  if (!args.empty()) {
    for (AstNode *&arg : args) {
      if (arg) {
        delete arg;
        arg = nullptr;
      }
    }
  }
}

AstBinaryExpr::~AstBinaryExpr() {
  if (left_expr || right_expr) {
    // same nodes as left and right expr
    if (left_expr == right_expr) {
      delete right_expr;
      right_expr = nullptr;
    } else {
      if (left_expr) {
        delete left_expr;
        left_expr = nullptr;
      }
      if (right_expr) {
        delete right_expr;
        right_expr = nullptr;
      }
    }
  }
}

AstUnaryExpr::~AstUnaryExpr() {
  if (expr) {
    delete expr;
    expr = nullptr;
  }
}

AstSourceCode::~AstSourceCode() {
  if (!children.empty()) {
    for (AstNode *&child : children) {
      if (child) {
        delete child;
        child = nullptr;
      }
    }
  }

  if (lexer) {
    delete lexer;
  }
}

TypeInfo::~TypeInfo() {}

AstType::~AstType() {
  if (child_type) {
    delete child_type;
    child_type = nullptr;
  }
}

AstConstValue::~AstConstValue() {
  /*
  if ((type == ConstValueType::INT || type == ConstValueType::FLOAT) && number) {
    delete[] number;
    number = nullptr;
  }
  */
}
