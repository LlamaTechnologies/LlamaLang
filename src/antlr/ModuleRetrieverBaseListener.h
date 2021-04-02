
// Generated from ../../ModuleRetriever.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "ModuleRetrieverListener.h"


namespace llang {

/**
 * This class provides an empty implementation of ModuleRetrieverListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ModuleRetrieverBaseListener : public ModuleRetrieverListener {
public:

  virtual void enterValidSource(ModuleRetrieverParser::ValidSourceContext * /*ctx*/) override { }
  virtual void exitValidSource(ModuleRetrieverParser::ValidSourceContext * /*ctx*/) override { }

  virtual void enterModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext * /*ctx*/) override { }
  virtual void exitModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext * /*ctx*/) override { }

  virtual void enterMainDirective(ModuleRetrieverParser::MainDirectiveContext * /*ctx*/) override { }
  virtual void exitMainDirective(ModuleRetrieverParser::MainDirectiveContext * /*ctx*/) override { }

  virtual void enterParameterList(ModuleRetrieverParser::ParameterListContext * /*ctx*/) override { }
  virtual void exitParameterList(ModuleRetrieverParser::ParameterListContext * /*ctx*/) override { }

  virtual void enterParameterDecl(ModuleRetrieverParser::ParameterDeclContext * /*ctx*/) override { }
  virtual void exitParameterDecl(ModuleRetrieverParser::ParameterDeclContext * /*ctx*/) override { }

  virtual void enterAnyToken(ModuleRetrieverParser::AnyTokenContext * /*ctx*/) override { }
  virtual void exitAnyToken(ModuleRetrieverParser::AnyTokenContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace llang
