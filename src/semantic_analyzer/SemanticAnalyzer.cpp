#include "SemanticAnalyzer.hpp"

namespace llang::semantics {
    SemanticAnalyzer::ErrorList* SemanticAnalyzer::errors = nullptr;
    std::shared_ptr<ast::ProgramNode> SemanticAnalyzer::ast = nullptr;

    SemanticAnalyzer::SemanticAnalyzer(std::shared_ptr<ast::ProgramNode> ast,
                                   ErrorList &errors) {
      SemanticAnalyzer::ast = ast;
      SemanticAnalyzer::errors = &errors;
    }
    bool SemanticAnalyzer::checkNode(ast::Node::ChildType node) {
      switch (node->GetType()) {
      case ast::AST_TYPE::FunctionNode: {
        checkNode(std::static_pointer_cast<ast::FunctionNode, ast::Node>(node));
      } break;
      }
      return false;
    }
    bool
    SemanticAnalyzer::checkNode(std::shared_ptr<ast::FunctionNode> funcNode) {
      auto returnStmnt =
          std::static_pointer_cast<ast::UnaryStatementNode, ast::StatementNode>(
              funcNode->Block.back());
      if (funcNode->ReturnType != "void" &&
          returnStmnt->StmntType != ast::STATEMENT_TYPE::RETURN) {
        error_handling::Error error(funcNode->Line, funcNode->FileName,
                                    "Function " + funcNode->Name +
                                        " needs a return statement");
        errors->emplace_back(error);
        return false;
      }

      if (symbol_table::Symbol::IsPrimitive(funcNode->ReturnType)) {
        auto retType = Primitives::Get(funcNode->ReturnType);

        if (retType == PRIMITIVE_TYPE::VOID) {
          if (returnStmnt->Right->StmntType != ast::STATEMENT_TYPE::EMPTY) {
            error_handling::Error error(funcNode->Line, funcNode->FileName,
                                        "Function " + funcNode->Name +
                                            " needs a return statement");
            errors->emplace_back(error);
            return false;
          }
          return true;
        }

        else if (returnStmnt->Right->StmntType ==
                 ast::STATEMENT_TYPE::CONSTANT) {
          auto constStmnt =
              std::static_pointer_cast<ast::ConstantNode, ast::RightValueNode>(
                  returnStmnt->Right);
          switch (retType) {
          case PRIMITIVE_TYPE::CHAR:
          case PRIMITIVE_TYPE::WCHAR:
          case PRIMITIVE_TYPE::UCHAR:
          case PRIMITIVE_TYPE::BYTE:
          case PRIMITIVE_TYPE::UINT8:
          case PRIMITIVE_TYPE::UINT16:
          case PRIMITIVE_TYPE::UINT32:
          case PRIMITIVE_TYPE::UINT64:
            if (constStmnt->Value[0] == '-') {
              auto name = constStmnt->FileName + ":" + funcNode->Name;
              error_handling::Error error(
                  constStmnt->Line, name,
                  "Function return type is unsigned, signed type is returned");
              errors->emplace_back(error);
              return false;
            }
          case PRIMITIVE_TYPE::SCHAR:
          case PRIMITIVE_TYPE::INT8:
          case PRIMITIVE_TYPE::INT16:
          case PRIMITIVE_TYPE::INT32:
          case PRIMITIVE_TYPE::INT64:
            if (constStmnt->ConstType != ast::CONSTANT_TYPE::INTEGER &&
                constStmnt->ConstType != ast::CONSTANT_TYPE::CHAR) {
              auto name = constStmnt->FileName + ":" + funcNode->Name;
              error_handling::Error error(
                  (int)constStmnt->Line, name,
                  "Function return type is Integer, no integer is returned");
              errors->emplace_back(error);
              return false;
            }
            break;
          case PRIMITIVE_TYPE::FLOAT32:
          case PRIMITIVE_TYPE::FLOAT64:
            if (constStmnt->ConstType != ast::CONSTANT_TYPE::FLOAT) {
              auto name = constStmnt->FileName + ":" + funcNode->Name;
              error_handling::Error error(
                  constStmnt->Line, name,
                  "Function return type is Floating "
                  "point, no floating point is returned");
              errors->emplace_back(error);
              return false;
            }
            break;
          }
        } else // Is returning a variable | check variable type
        {
        }
      }
      // Is Custom or built-in Type
      else {
        // Is returning a constant
        if (returnStmnt->Right->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
          auto constStmnt =
              std::static_pointer_cast<ast::ConstantNode, ast::RightValueNode>(
                  returnStmnt->Right);
          auto returnType = constStmnt->ConstType;
          if (funcNode->ReturnType ==
                  GetEnumValueName<built_in::BUILTIN_TYPE,
                                   built_in::BUILTIN_TYPE::STRING>() &&
              returnType != ast::CONSTANT_TYPE::STRING) {
            auto name = constStmnt->FileName + ":" + funcNode->Name;
            error_handling::Error error(
                constStmnt->Line, name,
                "Function return type is string, no string type is returned");
            errors->emplace_back(error);
            return false;
          }
        }
      }
      return true;
    }
    } // namespace llang::semantics