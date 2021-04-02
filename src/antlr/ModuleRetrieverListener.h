
// Generated from ../../ModuleRetriever.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "ModuleRetrieverParser.h"


namespace llang {

/**
 * This interface defines an abstract listener for a parse tree produced by ModuleRetrieverParser.
 */
class  ModuleRetrieverListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterValidSource(ModuleRetrieverParser::ValidSourceContext *ctx) = 0;
  virtual void exitValidSource(ModuleRetrieverParser::ValidSourceContext *ctx) = 0;

  virtual void enterModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext *ctx) = 0;
  virtual void exitModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext *ctx) = 0;

  virtual void enterIncludeDirective(ModuleRetrieverParser::IncludeDirectiveContext *ctx) = 0;
  virtual void exitIncludeDirective(ModuleRetrieverParser::IncludeDirectiveContext *ctx) = 0;

  virtual void enterAnyToken(ModuleRetrieverParser::AnyTokenContext *ctx) = 0;
  virtual void exitAnyToken(ModuleRetrieverParser::AnyTokenContext *ctx) = 0;


};

}  // namespace llang
