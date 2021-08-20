#include "ast_nodes.hpp"

static std::vector<const char *> directives_keywords = { "run", "load", "compile_only" };

const std::string get_directive_type_name(const DirectiveType directive_type) noexcept {
  assert(directive_type <= DirectiveType::CompTimeOnly);
  return directives_keywords.at((size_t)directive_type);
}

static std::vector<const char *> unary_operators_symbols = {
  "++", "--", "-", "!", "~",
};

const std::string get_unary_op_symbol(const UnaryExprType op_type) noexcept {
  assert(op_type < UnaryExprType::RET);
  return unary_operators_symbols[(size_t)op_type];
}

AstFuncDef::~AstFuncDef() {
  if (block) {
    delete block;
    block = nullptr;
  }
  if (proto) {
    delete proto;
    proto = nullptr;
  }
}

AstFuncProto::~AstFuncProto() {
  if (!params.empty()) {
    for (AstNode *&param : params) {
      if (param) {
        delete param;
        param = nullptr;
      }
    }
  }

  // Types are deleted by the type allocator
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

AstFuncCallExpr::~AstFuncCallExpr() {
  if (!params.empty()) {
    for (AstNode *&param : params) {
      if (param) {
        delete param;
        param = nullptr;
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
}

AstType::~AstType() {
  if (child_type) {
    delete child_type;
    child_type = nullptr;
  }

  if (type_info) {
    delete type_info;
    type_info = nullptr;
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
