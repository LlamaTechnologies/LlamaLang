#include "ast_nodes.hpp"

#include "lexer.hpp"

static const std::vector<const char *> directives_keywords = { "LOAD", "MAIN", "RUN", "COMPILE", "FN_TYPE" };

const std::string get_directive_type_name(const DirectiveType directive_type) noexcept {
  assert(directive_type <= DirectiveType::FN_TYPE);
  return directives_keywords.at((size_t)directive_type);
}

static const std::vector<const char *> unary_operators_symbols = {
  "++", "--", "-", "!", "~",
};

const std::string get_unary_op_symbol(const UnaryExprType op_type) noexcept {
  assert(op_type < UnaryExprType::RET);
  return unary_operators_symbols[(size_t)op_type];
}

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

AstFnProto::~AstFnProto() {
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
