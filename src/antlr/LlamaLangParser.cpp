
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1


#include "LlamaLangVisitor.h"

#include "LlamaLangParser.h"


using namespace antlrcpp;
using namespace llang;
using namespace antlr4;

LlamaLangParser::LlamaLangParser(TokenStream *input) : LlamaLangParserBase(input)
{
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LlamaLangParser::~LlamaLangParser()
{
  delete _interpreter;
}

std::string LlamaLangParser::getGrammarFileName() const
{
  return "LlamaLang.g4";
}

const std::vector<std::string> &LlamaLangParser::getRuleNames() const
{
  return _ruleNames;
}

dfa::Vocabulary &LlamaLangParser::getVocabulary() const
{
  return _vocabulary;
}


//----------------- SourceFileContext ------------------------------------------------------------------

LlamaLangParser::SourceFileContext::SourceFileContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

std::vector<LlamaLangParser::EosContext *> LlamaLangParser::SourceFileContext::eos()
{
  return getRuleContexts<LlamaLangParser::EosContext>();
}

LlamaLangParser::EosContext *LlamaLangParser::SourceFileContext::eos(size_t i)
{
  return getRuleContext<LlamaLangParser::EosContext>(i);
}

std::vector<LlamaLangParser::IncludeDirectiveContext *> LlamaLangParser::SourceFileContext::includeDirective()
{
  return getRuleContexts<LlamaLangParser::IncludeDirectiveContext>();
}

LlamaLangParser::IncludeDirectiveContext *LlamaLangParser::SourceFileContext::includeDirective(size_t i)
{
  return getRuleContext<LlamaLangParser::IncludeDirectiveContext>(i);
}

std::vector<LlamaLangParser::FunctionDefContext *> LlamaLangParser::SourceFileContext::functionDef()
{
  return getRuleContexts<LlamaLangParser::FunctionDefContext>();
}

LlamaLangParser::FunctionDefContext *LlamaLangParser::SourceFileContext::functionDef(size_t i)
{
  return getRuleContext<LlamaLangParser::FunctionDefContext>(i);
}

std::vector<LlamaLangParser::VarDefContext *> LlamaLangParser::SourceFileContext::varDef()
{
  return getRuleContexts<LlamaLangParser::VarDefContext>();
}

LlamaLangParser::VarDefContext *LlamaLangParser::SourceFileContext::varDef(size_t i)
{
  return getRuleContext<LlamaLangParser::VarDefContext>(i);
}


size_t LlamaLangParser::SourceFileContext::getRuleIndex() const
{
  return LlamaLangParser::RuleSourceFile;
}


antlrcpp::Any LlamaLangParser::SourceFileContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitSourceFile(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::SourceFileContext *LlamaLangParser::sourceFile()
{
  SourceFileContext *_localctx = _tracker.createInstance<SourceFileContext>(_ctx, getState());
  enterRule(_localctx, 0, LlamaLangParser::RuleSourceFile);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(91);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);

    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(88);
        includeDirective();
      }

      setState(93);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    }

    setState(100);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);

    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(98);
        _errHandler->sync(this);

        switch (_input->LA(1)) {
        case LlamaLangParser::DIR_BEGIN:
        case LlamaLangParser::FUNC: {
          setState(94);
          functionDef();
          break;
        }

        case LlamaLangParser::IDENTIFIER: {
          setState(95);
          varDef();
          setState(96);
          eos();
          break;
        }

        default:
          throw NoViableAltException(this);
        }
      }

      setState(102);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }

    setState(103);
    eos();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierListContext ------------------------------------------------------------------

LlamaLangParser::IdentifierListContext::IdentifierListContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

std::vector<tree::TerminalNode *> LlamaLangParser::IdentifierListContext::IDENTIFIER()
{
  return getTokens(LlamaLangParser::IDENTIFIER);
}

tree::TerminalNode *LlamaLangParser::IdentifierListContext::IDENTIFIER(size_t i)
{
  return getToken(LlamaLangParser::IDENTIFIER, i);
}

std::vector<tree::TerminalNode *> LlamaLangParser::IdentifierListContext::COMMA()
{
  return getTokens(LlamaLangParser::COMMA);
}

tree::TerminalNode *LlamaLangParser::IdentifierListContext::COMMA(size_t i)
{
  return getToken(LlamaLangParser::COMMA, i);
}


size_t LlamaLangParser::IdentifierListContext::getRuleIndex() const
{
  return LlamaLangParser::RuleIdentifierList;
}


antlrcpp::Any LlamaLangParser::IdentifierListContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitIdentifierList(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::IdentifierListContext *LlamaLangParser::identifierList()
{
  IdentifierListContext *_localctx = _tracker.createInstance<IdentifierListContext>(_ctx, getState());
  enterRule(_localctx, 2, LlamaLangParser::RuleIdentifierList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(105);
    match(LlamaLangParser::IDENTIFIER);
    setState(110);
    _errHandler->sync(this);
    _la = _input->LA(1);

    while (_la == LlamaLangParser::COMMA) {
      setState(106);
      match(LlamaLangParser::COMMA);
      setState(107);
      match(LlamaLangParser::IDENTIFIER);
      setState(112);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionListContext ------------------------------------------------------------------

LlamaLangParser::ExpressionListContext::ExpressionListContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

std::vector<LlamaLangParser::ExpressionContext *> LlamaLangParser::ExpressionListContext::expression()
{
  return getRuleContexts<LlamaLangParser::ExpressionContext>();
}

LlamaLangParser::ExpressionContext *LlamaLangParser::ExpressionListContext::expression(size_t i)
{
  return getRuleContext<LlamaLangParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> LlamaLangParser::ExpressionListContext::COMMA()
{
  return getTokens(LlamaLangParser::COMMA);
}

tree::TerminalNode *LlamaLangParser::ExpressionListContext::COMMA(size_t i)
{
  return getToken(LlamaLangParser::COMMA, i);
}


size_t LlamaLangParser::ExpressionListContext::getRuleIndex() const
{
  return LlamaLangParser::RuleExpressionList;
}


antlrcpp::Any LlamaLangParser::ExpressionListContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitExpressionList(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ExpressionListContext *LlamaLangParser::expressionList()
{
  ExpressionListContext *_localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 4, LlamaLangParser::RuleExpressionList);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(113);
    expression(0);
    setState(118);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);

    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(114);
        match(LlamaLangParser::COMMA);
        setState(115);
        expression(0);
      }

      setState(120);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncludeDirectiveContext ------------------------------------------------------------------

LlamaLangParser::IncludeDirectiveContext::IncludeDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::IncludeDirectiveContext::DIR_BEGIN()
{
  return getToken(LlamaLangParser::DIR_BEGIN, 0);
}

LlamaLangParser::String_Context *LlamaLangParser::IncludeDirectiveContext::string_()
{
  return getRuleContext<LlamaLangParser::String_Context>(0);
}


size_t LlamaLangParser::IncludeDirectiveContext::getRuleIndex() const
{
  return LlamaLangParser::RuleIncludeDirective;
}


antlrcpp::Any LlamaLangParser::IncludeDirectiveContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitIncludeDirective(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::IncludeDirectiveContext *LlamaLangParser::includeDirective()
{
  IncludeDirectiveContext *_localctx = _tracker.createInstance<IncludeDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 6, LlamaLangParser::RuleIncludeDirective);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(121);
    match(LlamaLangParser::DIR_BEGIN);
    setState(122);
    match(LlamaLangParser::T__0);
    setState(123);
    string_();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternDirectiveContext ------------------------------------------------------------------

LlamaLangParser::ExternDirectiveContext::ExternDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::ExternDirectiveContext::DIR_BEGIN()
{
  return getToken(LlamaLangParser::DIR_BEGIN, 0);
}

tree::TerminalNode *LlamaLangParser::ExternDirectiveContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}


size_t LlamaLangParser::ExternDirectiveContext::getRuleIndex() const
{
  return LlamaLangParser::RuleExternDirective;
}


antlrcpp::Any LlamaLangParser::ExternDirectiveContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitExternDirective(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ExternDirectiveContext *LlamaLangParser::externDirective()
{
  ExternDirectiveContext *_localctx = _tracker.createInstance<ExternDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 8, LlamaLangParser::RuleExternDirective);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(125);
    match(LlamaLangParser::DIR_BEGIN);
    setState(126);
    match(LlamaLangParser::T__1);
    setState(127);
    match(LlamaLangParser::IDENTIFIER);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RunDirectiveContext ------------------------------------------------------------------

LlamaLangParser::RunDirectiveContext::RunDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::RunDirectiveContext::DIR_BEGIN()
{
  return getToken(LlamaLangParser::DIR_BEGIN, 0);
}

LlamaLangParser::PrimaryExprContext *LlamaLangParser::RunDirectiveContext::primaryExpr()
{
  return getRuleContext<LlamaLangParser::PrimaryExprContext>(0);
}


size_t LlamaLangParser::RunDirectiveContext::getRuleIndex() const
{
  return LlamaLangParser::RuleRunDirective;
}


antlrcpp::Any LlamaLangParser::RunDirectiveContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitRunDirective(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::RunDirectiveContext *LlamaLangParser::runDirective()
{
  RunDirectiveContext *_localctx = _tracker.createInstance<RunDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 10, LlamaLangParser::RuleRunDirective);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(129);
    match(LlamaLangParser::DIR_BEGIN);
    setState(130);
    match(LlamaLangParser::T__2);
    setState(131);
    primaryExpr(0);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BasicDirectiveContext ------------------------------------------------------------------

LlamaLangParser::BasicDirectiveContext::BasicDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::BasicDirectiveContext::DIR_BEGIN()
{
  return getToken(LlamaLangParser::DIR_BEGIN, 0);
}

tree::TerminalNode *LlamaLangParser::BasicDirectiveContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}


size_t LlamaLangParser::BasicDirectiveContext::getRuleIndex() const
{
  return LlamaLangParser::RuleBasicDirective;
}


antlrcpp::Any LlamaLangParser::BasicDirectiveContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitBasicDirective(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::BasicDirectiveContext *LlamaLangParser::basicDirective()
{
  BasicDirectiveContext *_localctx = _tracker.createInstance<BasicDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 12, LlamaLangParser::RuleBasicDirective);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(133);
    match(LlamaLangParser::DIR_BEGIN);
    setState(134);
    match(LlamaLangParser::IDENTIFIER);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDefContext ------------------------------------------------------------------

LlamaLangParser::FunctionDefContext::FunctionDefContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::FunctionDefContext::FUNC()
{
  return getToken(LlamaLangParser::FUNC, 0);
}

tree::TerminalNode *LlamaLangParser::FunctionDefContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}

LlamaLangParser::SignatureContext *LlamaLangParser::FunctionDefContext::signature()
{
  return getRuleContext<LlamaLangParser::SignatureContext>(0);
}

LlamaLangParser::Type_Context *LlamaLangParser::FunctionDefContext::type_()
{
  return getRuleContext<LlamaLangParser::Type_Context>(0);
}

LlamaLangParser::BlockContext *LlamaLangParser::FunctionDefContext::block()
{
  return getRuleContext<LlamaLangParser::BlockContext>(0);
}

LlamaLangParser::BasicDirectiveContext *LlamaLangParser::FunctionDefContext::basicDirective()
{
  return getRuleContext<LlamaLangParser::BasicDirectiveContext>(0);
}


size_t LlamaLangParser::FunctionDefContext::getRuleIndex() const
{
  return LlamaLangParser::RuleFunctionDef;
}


antlrcpp::Any LlamaLangParser::FunctionDefContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitFunctionDef(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::FunctionDefContext *LlamaLangParser::functionDef()
{
  FunctionDefContext *_localctx = _tracker.createInstance<FunctionDefContext>(_ctx, getState());
  enterRule(_localctx, 14, LlamaLangParser::RuleFunctionDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(137);
    _errHandler->sync(this);

    _la = _input->LA(1);

    if (_la == LlamaLangParser::DIR_BEGIN) {
      setState(136);
      basicDirective();
    }

    setState(139);
    match(LlamaLangParser::FUNC);
    setState(140);
    match(LlamaLangParser::IDENTIFIER);
    setState(141);
    signature();
    setState(142);
    type_();
    setState(143);
    block();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

LlamaLangParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::VarDefContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}

LlamaLangParser::Type_Context *LlamaLangParser::VarDefContext::type_()
{
  return getRuleContext<LlamaLangParser::Type_Context>(0);
}

tree::TerminalNode *LlamaLangParser::VarDefContext::ASSIGN()
{
  return getToken(LlamaLangParser::ASSIGN, 0);
}

LlamaLangParser::ExpressionListContext *LlamaLangParser::VarDefContext::expressionList()
{
  return getRuleContext<LlamaLangParser::ExpressionListContext>(0);
}


size_t LlamaLangParser::VarDefContext::getRuleIndex() const
{
  return LlamaLangParser::RuleVarDef;
}


antlrcpp::Any LlamaLangParser::VarDefContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitVarDef(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::VarDefContext *LlamaLangParser::varDef()
{
  VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
  enterRule(_localctx, 16, LlamaLangParser::RuleVarDef);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(145);
    match(LlamaLangParser::IDENTIFIER);
    setState(146);
    type_();
    setState(149);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      setState(147);
      match(LlamaLangParser::ASSIGN);
      setState(148);
      expressionList();
      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

LlamaLangParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::BlockContext::L_CURLY()
{
  return getToken(LlamaLangParser::L_CURLY, 0);
}

tree::TerminalNode *LlamaLangParser::BlockContext::R_CURLY()
{
  return getToken(LlamaLangParser::R_CURLY, 0);
}

LlamaLangParser::StatementListContext *LlamaLangParser::BlockContext::statementList()
{
  return getRuleContext<LlamaLangParser::StatementListContext>(0);
}


size_t LlamaLangParser::BlockContext::getRuleIndex() const
{
  return LlamaLangParser::RuleBlock;
}


antlrcpp::Any LlamaLangParser::BlockContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::BlockContext *LlamaLangParser::block()
{
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 18, LlamaLangParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    match(LlamaLangParser::L_CURLY);
    setState(153);
    _errHandler->sync(this);

    _la = _input->LA(1);

    if ((((_la & ~ 0x3fULL) == 0) &&
         ((1ULL << _la) & ((1ULL << LlamaLangParser::RETURN)
                           | (1ULL << LlamaLangParser::IDENTIFIER)
                           | (1ULL << LlamaLangParser::L_PAREN)
                           | (1ULL << LlamaLangParser::L_CURLY)
                           | (1ULL << LlamaLangParser::SEMI)
                           | (1ULL << LlamaLangParser::PLUS_PLUS)
                           | (1ULL << LlamaLangParser::MINUS_MINUS)
                           | (1ULL << LlamaLangParser::EXCLAMATION)
                           | (1ULL << LlamaLangParser::PLUS)
                           | (1ULL << LlamaLangParser::MINUS)
                           | (1ULL << LlamaLangParser::STAR)
                           | (1ULL << LlamaLangParser::AMPERSAND)
                           | (1ULL << LlamaLangParser::DECIMAL_LIT)
                           | (1ULL << LlamaLangParser::OCTAL_LIT)
                           | (1ULL << LlamaLangParser::HEX_LIT)
                           | (1ULL << LlamaLangParser::FLOAT_LIT)
                           | (1ULL << LlamaLangParser::DOUBLE_LIT)
                           | (1ULL << LlamaLangParser::RUNE_LIT)
                           | (1ULL << LlamaLangParser::INTERPRETED_STRING_LIT))) != 0)) {
      setState(152);
      statementList();
    }

    setState(155);
    match(LlamaLangParser::R_CURLY);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementListContext ------------------------------------------------------------------

LlamaLangParser::StatementListContext::StatementListContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

std::vector<LlamaLangParser::StatementContext *> LlamaLangParser::StatementListContext::statement()
{
  return getRuleContexts<LlamaLangParser::StatementContext>();
}

LlamaLangParser::StatementContext *LlamaLangParser::StatementListContext::statement(size_t i)
{
  return getRuleContext<LlamaLangParser::StatementContext>(i);
}

std::vector<LlamaLangParser::EosContext *> LlamaLangParser::StatementListContext::eos()
{
  return getRuleContexts<LlamaLangParser::EosContext>();
}

LlamaLangParser::EosContext *LlamaLangParser::StatementListContext::eos(size_t i)
{
  return getRuleContext<LlamaLangParser::EosContext>(i);
}


size_t LlamaLangParser::StatementListContext::getRuleIndex() const
{
  return LlamaLangParser::RuleStatementList;
}


antlrcpp::Any LlamaLangParser::StatementListContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitStatementList(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::StatementListContext *LlamaLangParser::statementList()
{
  StatementListContext *_localctx = _tracker.createInstance<StatementListContext>(_ctx, getState());
  enterRule(_localctx, 20, LlamaLangParser::RuleStatementList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(160);
    _errHandler->sync(this);
    _la = _input->LA(1);

    do {
      setState(157);
      statement();
      setState(158);
      eos();
      setState(162);
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
              ((1ULL << _la) & ((1ULL << LlamaLangParser::RETURN)
                                | (1ULL << LlamaLangParser::IDENTIFIER)
                                | (1ULL << LlamaLangParser::L_PAREN)
                                | (1ULL << LlamaLangParser::L_CURLY)
                                | (1ULL << LlamaLangParser::SEMI)
                                | (1ULL << LlamaLangParser::PLUS_PLUS)
                                | (1ULL << LlamaLangParser::MINUS_MINUS)
                                | (1ULL << LlamaLangParser::EXCLAMATION)
                                | (1ULL << LlamaLangParser::PLUS)
                                | (1ULL << LlamaLangParser::MINUS)
                                | (1ULL << LlamaLangParser::STAR)
                                | (1ULL << LlamaLangParser::AMPERSAND)
                                | (1ULL << LlamaLangParser::DECIMAL_LIT)
                                | (1ULL << LlamaLangParser::OCTAL_LIT)
                                | (1ULL << LlamaLangParser::HEX_LIT)
                                | (1ULL << LlamaLangParser::FLOAT_LIT)
                                | (1ULL << LlamaLangParser::DOUBLE_LIT)
                                | (1ULL << LlamaLangParser::RUNE_LIT)
                                | (1ULL << LlamaLangParser::INTERPRETED_STRING_LIT))) != 0));

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

LlamaLangParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::VarDefContext *LlamaLangParser::StatementContext::varDef()
{
  return getRuleContext<LlamaLangParser::VarDefContext>(0);
}

LlamaLangParser::SimpleStmtContext *LlamaLangParser::StatementContext::simpleStmt()
{
  return getRuleContext<LlamaLangParser::SimpleStmtContext>(0);
}

LlamaLangParser::ReturnStmtContext *LlamaLangParser::StatementContext::returnStmt()
{
  return getRuleContext<LlamaLangParser::ReturnStmtContext>(0);
}

LlamaLangParser::BlockContext *LlamaLangParser::StatementContext::block()
{
  return getRuleContext<LlamaLangParser::BlockContext>(0);
}


size_t LlamaLangParser::StatementContext::getRuleIndex() const
{
  return LlamaLangParser::RuleStatement;
}


antlrcpp::Any LlamaLangParser::StatementContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::StatementContext *LlamaLangParser::statement()
{
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 22, LlamaLangParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(168);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(164);
      varDef();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(165);
      simpleStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(166);
      returnStmt();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(167);
      block();
      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleStmtContext ------------------------------------------------------------------

LlamaLangParser::SimpleStmtContext::SimpleStmtContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::ExpressionStmtContext *LlamaLangParser::SimpleStmtContext::expressionStmt()
{
  return getRuleContext<LlamaLangParser::ExpressionStmtContext>(0);
}

LlamaLangParser::AssignmentContext *LlamaLangParser::SimpleStmtContext::assignment()
{
  return getRuleContext<LlamaLangParser::AssignmentContext>(0);
}

LlamaLangParser::EmptyStmtContext *LlamaLangParser::SimpleStmtContext::emptyStmt()
{
  return getRuleContext<LlamaLangParser::EmptyStmtContext>(0);
}


size_t LlamaLangParser::SimpleStmtContext::getRuleIndex() const
{
  return LlamaLangParser::RuleSimpleStmt;
}


antlrcpp::Any LlamaLangParser::SimpleStmtContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitSimpleStmt(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::SimpleStmtContext *LlamaLangParser::simpleStmt()
{
  SimpleStmtContext *_localctx = _tracker.createInstance<SimpleStmtContext>(_ctx, getState());
  enterRule(_localctx, 24, LlamaLangParser::RuleSimpleStmt);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(173);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(170);
      expressionStmt();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(171);
      assignment();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(172);
      emptyStmt();
      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionStmtContext ------------------------------------------------------------------

LlamaLangParser::ExpressionStmtContext::ExpressionStmtContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::ExpressionContext *LlamaLangParser::ExpressionStmtContext::expression()
{
  return getRuleContext<LlamaLangParser::ExpressionContext>(0);
}


size_t LlamaLangParser::ExpressionStmtContext::getRuleIndex() const
{
  return LlamaLangParser::RuleExpressionStmt;
}


antlrcpp::Any LlamaLangParser::ExpressionStmtContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitExpressionStmt(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ExpressionStmtContext *LlamaLangParser::expressionStmt()
{
  ExpressionStmtContext *_localctx = _tracker.createInstance<ExpressionStmtContext>(_ctx, getState());
  enterRule(_localctx, 26, LlamaLangParser::RuleExpressionStmt);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(175);
    expression(0);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

LlamaLangParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::AssignmentContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}

LlamaLangParser::Assign_opContext *LlamaLangParser::AssignmentContext::assign_op()
{
  return getRuleContext<LlamaLangParser::Assign_opContext>(0);
}

LlamaLangParser::ExpressionListContext *LlamaLangParser::AssignmentContext::expressionList()
{
  return getRuleContext<LlamaLangParser::ExpressionListContext>(0);
}


size_t LlamaLangParser::AssignmentContext::getRuleIndex() const
{
  return LlamaLangParser::RuleAssignment;
}


antlrcpp::Any LlamaLangParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::AssignmentContext *LlamaLangParser::assignment()
{
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 28, LlamaLangParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(177);
    match(LlamaLangParser::IDENTIFIER);
    setState(178);
    assign_op();
    setState(179);
    expressionList();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Assign_opContext ------------------------------------------------------------------

LlamaLangParser::Assign_opContext::Assign_opContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::ASSIGN()
{
  return getToken(LlamaLangParser::ASSIGN, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::PLUS()
{
  return getToken(LlamaLangParser::PLUS, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::MINUS()
{
  return getToken(LlamaLangParser::MINUS, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::OR()
{
  return getToken(LlamaLangParser::OR, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::CARET()
{
  return getToken(LlamaLangParser::CARET, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::STAR()
{
  return getToken(LlamaLangParser::STAR, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::DIV()
{
  return getToken(LlamaLangParser::DIV, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::MOD()
{
  return getToken(LlamaLangParser::MOD, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::LSHIFT()
{
  return getToken(LlamaLangParser::LSHIFT, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::RSHIFT()
{
  return getToken(LlamaLangParser::RSHIFT, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::AMPERSAND()
{
  return getToken(LlamaLangParser::AMPERSAND, 0);
}

tree::TerminalNode *LlamaLangParser::Assign_opContext::BIT_CLEAR()
{
  return getToken(LlamaLangParser::BIT_CLEAR, 0);
}


size_t LlamaLangParser::Assign_opContext::getRuleIndex() const
{
  return LlamaLangParser::RuleAssign_op;
}


antlrcpp::Any LlamaLangParser::Assign_opContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitAssign_op(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::Assign_opContext *LlamaLangParser::assign_op()
{
  Assign_opContext *_localctx = _tracker.createInstance<Assign_opContext>(_ctx, getState());
  enterRule(_localctx, 30, LlamaLangParser::RuleAssign_op);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(182);
    _errHandler->sync(this);

    _la = _input->LA(1);

    if ((((_la & ~ 0x3fULL) == 0) &&
         ((1ULL << _la) & ((1ULL << LlamaLangParser::OR)
                           | (1ULL << LlamaLangParser::DIV)
                           | (1ULL << LlamaLangParser::MOD)
                           | (1ULL << LlamaLangParser::LSHIFT)
                           | (1ULL << LlamaLangParser::RSHIFT)
                           | (1ULL << LlamaLangParser::BIT_CLEAR)
                           | (1ULL << LlamaLangParser::PLUS)
                           | (1ULL << LlamaLangParser::MINUS)
                           | (1ULL << LlamaLangParser::CARET)
                           | (1ULL << LlamaLangParser::STAR)
                           | (1ULL << LlamaLangParser::AMPERSAND))) != 0)) {
      setState(181);
      _la = _input->LA(1);

      if (!((((_la & ~ 0x3fULL) == 0) &&
             ((1ULL << _la) & ((1ULL << LlamaLangParser::OR)
                               | (1ULL << LlamaLangParser::DIV)
                               | (1ULL << LlamaLangParser::MOD)
                               | (1ULL << LlamaLangParser::LSHIFT)
                               | (1ULL << LlamaLangParser::RSHIFT)
                               | (1ULL << LlamaLangParser::BIT_CLEAR)
                               | (1ULL << LlamaLangParser::PLUS)
                               | (1ULL << LlamaLangParser::MINUS)
                               | (1ULL << LlamaLangParser::CARET)
                               | (1ULL << LlamaLangParser::STAR)
                               | (1ULL << LlamaLangParser::AMPERSAND))) != 0))) {
        _errHandler->recoverInline(this);
      } else {
        _errHandler->reportMatch(this);
        consume();
      }
    }

    setState(184);
    match(LlamaLangParser::ASSIGN);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EmptyStmtContext ------------------------------------------------------------------

LlamaLangParser::EmptyStmtContext::EmptyStmtContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::EmptyStmtContext::SEMI()
{
  return getToken(LlamaLangParser::SEMI, 0);
}


size_t LlamaLangParser::EmptyStmtContext::getRuleIndex() const
{
  return LlamaLangParser::RuleEmptyStmt;
}


antlrcpp::Any LlamaLangParser::EmptyStmtContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitEmptyStmt(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::EmptyStmtContext *LlamaLangParser::emptyStmt()
{
  EmptyStmtContext *_localctx = _tracker.createInstance<EmptyStmtContext>(_ctx, getState());
  enterRule(_localctx, 32, LlamaLangParser::RuleEmptyStmt);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(186);
    match(LlamaLangParser::SEMI);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

LlamaLangParser::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::ReturnStmtContext::RETURN()
{
  return getToken(LlamaLangParser::RETURN, 0);
}

LlamaLangParser::ExpressionContext *LlamaLangParser::ReturnStmtContext::expression()
{
  return getRuleContext<LlamaLangParser::ExpressionContext>(0);
}


size_t LlamaLangParser::ReturnStmtContext::getRuleIndex() const
{
  return LlamaLangParser::RuleReturnStmt;
}


antlrcpp::Any LlamaLangParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ReturnStmtContext *LlamaLangParser::returnStmt()
{
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 34, LlamaLangParser::RuleReturnStmt);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(188);
    match(LlamaLangParser::RETURN);
    setState(190);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      setState(189);
      expression(0);
      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Type_Context ------------------------------------------------------------------

LlamaLangParser::Type_Context::Type_Context(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::TypeNameContext *LlamaLangParser::Type_Context::typeName()
{
  return getRuleContext<LlamaLangParser::TypeNameContext>(0);
}

LlamaLangParser::PointerContext *LlamaLangParser::Type_Context::pointer()
{
  return getRuleContext<LlamaLangParser::PointerContext>(0);
}

LlamaLangParser::ArrayContext *LlamaLangParser::Type_Context::array()
{
  return getRuleContext<LlamaLangParser::ArrayContext>(0);
}


size_t LlamaLangParser::Type_Context::getRuleIndex() const
{
  return LlamaLangParser::RuleType_;
}


antlrcpp::Any LlamaLangParser::Type_Context::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitType_(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::Type_Context *LlamaLangParser::type_()
{
  Type_Context *_localctx = _tracker.createInstance<Type_Context>(_ctx, getState());
  enterRule(_localctx, 36, LlamaLangParser::RuleType_);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(195);
    _errHandler->sync(this);

    switch (_input->LA(1)) {
    case LlamaLangParser::IDENTIFIER: {
      enterOuterAlt(_localctx, 1);
      setState(192);
      typeName();
      break;
    }

    case LlamaLangParser::STAR: {
      enterOuterAlt(_localctx, 2);
      setState(193);
      pointer();
      break;
    }

    case LlamaLangParser::L_BRACKET: {
      enterOuterAlt(_localctx, 3);
      setState(194);
      array();
      break;
    }

    default:
      throw NoViableAltException(this);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PointerContext ------------------------------------------------------------------

LlamaLangParser::PointerContext::PointerContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::PointerContext::STAR()
{
  return getToken(LlamaLangParser::STAR, 0);
}

LlamaLangParser::Type_Context *LlamaLangParser::PointerContext::type_()
{
  return getRuleContext<LlamaLangParser::Type_Context>(0);
}


size_t LlamaLangParser::PointerContext::getRuleIndex() const
{
  return LlamaLangParser::RulePointer;
}


antlrcpp::Any LlamaLangParser::PointerContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitPointer(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::PointerContext *LlamaLangParser::pointer()
{
  PointerContext *_localctx = _tracker.createInstance<PointerContext>(_ctx, getState());
  enterRule(_localctx, 38, LlamaLangParser::RulePointer);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(197);
    match(LlamaLangParser::STAR);
    setState(198);
    type_();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayContext ------------------------------------------------------------------

LlamaLangParser::ArrayContext::ArrayContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::ArrayContext::L_BRACKET()
{
  return getToken(LlamaLangParser::L_BRACKET, 0);
}

tree::TerminalNode *LlamaLangParser::ArrayContext::R_BRACKET()
{
  return getToken(LlamaLangParser::R_BRACKET, 0);
}

LlamaLangParser::TypeNameContext *LlamaLangParser::ArrayContext::typeName()
{
  return getRuleContext<LlamaLangParser::TypeNameContext>(0);
}


size_t LlamaLangParser::ArrayContext::getRuleIndex() const
{
  return LlamaLangParser::RuleArray;
}


antlrcpp::Any LlamaLangParser::ArrayContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitArray(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ArrayContext *LlamaLangParser::array()
{
  ArrayContext *_localctx = _tracker.createInstance<ArrayContext>(_ctx, getState());
  enterRule(_localctx, 40, LlamaLangParser::RuleArray);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(200);
    match(LlamaLangParser::L_BRACKET);
    setState(201);
    match(LlamaLangParser::R_BRACKET);
    setState(202);
    typeName();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeNameContext ------------------------------------------------------------------

LlamaLangParser::TypeNameContext::TypeNameContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::TypeNameContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}

LlamaLangParser::QualifiedIdentContext *LlamaLangParser::TypeNameContext::qualifiedIdent()
{
  return getRuleContext<LlamaLangParser::QualifiedIdentContext>(0);
}


size_t LlamaLangParser::TypeNameContext::getRuleIndex() const
{
  return LlamaLangParser::RuleTypeName;
}


antlrcpp::Any LlamaLangParser::TypeNameContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitTypeName(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::TypeNameContext *LlamaLangParser::typeName()
{
  TypeNameContext *_localctx = _tracker.createInstance<TypeNameContext>(_ctx, getState());
  enterRule(_localctx, 42, LlamaLangParser::RuleTypeName);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(206);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(204);
      match(LlamaLangParser::IDENTIFIER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(205);
      qualifiedIdent();
      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignatureContext ------------------------------------------------------------------

LlamaLangParser::SignatureContext::SignatureContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::ParametersContext *LlamaLangParser::SignatureContext::parameters()
{
  return getRuleContext<LlamaLangParser::ParametersContext>(0);
}


size_t LlamaLangParser::SignatureContext::getRuleIndex() const
{
  return LlamaLangParser::RuleSignature;
}


antlrcpp::Any LlamaLangParser::SignatureContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitSignature(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::SignatureContext *LlamaLangParser::signature()
{
  SignatureContext *_localctx = _tracker.createInstance<SignatureContext>(_ctx, getState());
  enterRule(_localctx, 44, LlamaLangParser::RuleSignature);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(208);
    parameters();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ResultContext ------------------------------------------------------------------

LlamaLangParser::ResultContext::ResultContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::ParametersContext *LlamaLangParser::ResultContext::parameters()
{
  return getRuleContext<LlamaLangParser::ParametersContext>(0);
}

LlamaLangParser::Type_Context *LlamaLangParser::ResultContext::type_()
{
  return getRuleContext<LlamaLangParser::Type_Context>(0);
}


size_t LlamaLangParser::ResultContext::getRuleIndex() const
{
  return LlamaLangParser::RuleResult;
}


antlrcpp::Any LlamaLangParser::ResultContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitResult(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ResultContext *LlamaLangParser::result()
{
  ResultContext *_localctx = _tracker.createInstance<ResultContext>(_ctx, getState());
  enterRule(_localctx, 46, LlamaLangParser::RuleResult);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(212);
    _errHandler->sync(this);

    switch (_input->LA(1)) {
    case LlamaLangParser::L_PAREN: {
      enterOuterAlt(_localctx, 1);
      setState(210);
      parameters();
      break;
    }

    case LlamaLangParser::IDENTIFIER:
    case LlamaLangParser::L_BRACKET:
    case LlamaLangParser::STAR: {
      enterOuterAlt(_localctx, 2);
      setState(211);
      type_();
      break;
    }

    default:
      throw NoViableAltException(this);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParametersContext ------------------------------------------------------------------

LlamaLangParser::ParametersContext::ParametersContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::ParametersContext::L_PAREN()
{
  return getToken(LlamaLangParser::L_PAREN, 0);
}

tree::TerminalNode *LlamaLangParser::ParametersContext::R_PAREN()
{
  return getToken(LlamaLangParser::R_PAREN, 0);
}

std::vector<LlamaLangParser::ParameterDeclContext *> LlamaLangParser::ParametersContext::parameterDecl()
{
  return getRuleContexts<LlamaLangParser::ParameterDeclContext>();
}

LlamaLangParser::ParameterDeclContext *LlamaLangParser::ParametersContext::parameterDecl(size_t i)
{
  return getRuleContext<LlamaLangParser::ParameterDeclContext>(i);
}

std::vector<tree::TerminalNode *> LlamaLangParser::ParametersContext::COMMA()
{
  return getTokens(LlamaLangParser::COMMA);
}

tree::TerminalNode *LlamaLangParser::ParametersContext::COMMA(size_t i)
{
  return getToken(LlamaLangParser::COMMA, i);
}


size_t LlamaLangParser::ParametersContext::getRuleIndex() const
{
  return LlamaLangParser::RuleParameters;
}


antlrcpp::Any LlamaLangParser::ParametersContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitParameters(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ParametersContext *LlamaLangParser::parameters()
{
  ParametersContext *_localctx = _tracker.createInstance<ParametersContext>(_ctx, getState());
  enterRule(_localctx, 48, LlamaLangParser::RuleParameters);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(214);
    match(LlamaLangParser::L_PAREN);
    setState(223);
    _errHandler->sync(this);

    _la = _input->LA(1);

    if (_la == LlamaLangParser::IDENTIFIER) {
      setState(215);
      parameterDecl();
      setState(220);
      _errHandler->sync(this);
      _la = _input->LA(1);

      while (_la == LlamaLangParser::COMMA) {
        setState(216);
        match(LlamaLangParser::COMMA);
        setState(217);
        parameterDecl();
        setState(222);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }

    setState(225);
    match(LlamaLangParser::R_PAREN);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterDeclContext ------------------------------------------------------------------

LlamaLangParser::ParameterDeclContext::ParameterDeclContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::ParameterDeclContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}

LlamaLangParser::Type_Context *LlamaLangParser::ParameterDeclContext::type_()
{
  return getRuleContext<LlamaLangParser::Type_Context>(0);
}


size_t LlamaLangParser::ParameterDeclContext::getRuleIndex() const
{
  return LlamaLangParser::RuleParameterDecl;
}


antlrcpp::Any LlamaLangParser::ParameterDeclContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitParameterDecl(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ParameterDeclContext *LlamaLangParser::parameterDecl()
{
  ParameterDeclContext *_localctx = _tracker.createInstance<ParameterDeclContext>(_ctx, getState());
  enterRule(_localctx, 50, LlamaLangParser::RuleParameterDecl);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(227);
    match(LlamaLangParser::IDENTIFIER);
    setState(228);
    type_();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

LlamaLangParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::PrimaryExprContext *LlamaLangParser::ExpressionContext::primaryExpr()
{
  return getRuleContext<LlamaLangParser::PrimaryExprContext>(0);
}

LlamaLangParser::UnaryExprContext *LlamaLangParser::ExpressionContext::unaryExpr()
{
  return getRuleContext<LlamaLangParser::UnaryExprContext>(0);
}

std::vector<LlamaLangParser::ExpressionContext *> LlamaLangParser::ExpressionContext::expression()
{
  return getRuleContexts<LlamaLangParser::ExpressionContext>();
}

LlamaLangParser::ExpressionContext *LlamaLangParser::ExpressionContext::expression(size_t i)
{
  return getRuleContext<LlamaLangParser::ExpressionContext>(i);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::STAR()
{
  return getToken(LlamaLangParser::STAR, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::DIV()
{
  return getToken(LlamaLangParser::DIV, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::MOD()
{
  return getToken(LlamaLangParser::MOD, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::LSHIFT()
{
  return getToken(LlamaLangParser::LSHIFT, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::RSHIFT()
{
  return getToken(LlamaLangParser::RSHIFT, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::AMPERSAND()
{
  return getToken(LlamaLangParser::AMPERSAND, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::BIT_CLEAR()
{
  return getToken(LlamaLangParser::BIT_CLEAR, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::PLUS()
{
  return getToken(LlamaLangParser::PLUS, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::MINUS()
{
  return getToken(LlamaLangParser::MINUS, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::OR()
{
  return getToken(LlamaLangParser::OR, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::CARET()
{
  return getToken(LlamaLangParser::CARET, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::EQUALS()
{
  return getToken(LlamaLangParser::EQUALS, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::NOT_EQUALS()
{
  return getToken(LlamaLangParser::NOT_EQUALS, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::LESS()
{
  return getToken(LlamaLangParser::LESS, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::LESS_OR_EQUALS()
{
  return getToken(LlamaLangParser::LESS_OR_EQUALS, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::GREATER()
{
  return getToken(LlamaLangParser::GREATER, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::GREATER_OR_EQUALS()
{
  return getToken(LlamaLangParser::GREATER_OR_EQUALS, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::LOGICAL_AND()
{
  return getToken(LlamaLangParser::LOGICAL_AND, 0);
}

tree::TerminalNode *LlamaLangParser::ExpressionContext::LOGICAL_OR()
{
  return getToken(LlamaLangParser::LOGICAL_OR, 0);
}


size_t LlamaLangParser::ExpressionContext::getRuleIndex() const
{
  return LlamaLangParser::RuleExpression;
}


antlrcpp::Any LlamaLangParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}


LlamaLangParser::ExpressionContext *LlamaLangParser::expression()
{
  return expression(0);
}

LlamaLangParser::ExpressionContext *LlamaLangParser::expression(int precedence)
{
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LlamaLangParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  LlamaLangParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 52;
  enterRecursionRule(_localctx, 52, LlamaLangParser::RuleExpression, precedence);

  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    unrollRecursionContexts(parentContext);
  });

  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(233);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      setState(231);
      primaryExpr(0);
      break;
    }

    case 2: {
      setState(232);
      unaryExpr();
      break;
    }

    default:
      break;
    }

    _ctx->stop = _input->LT(-1);
    setState(252);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);

    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();

        previousContext = _localctx;
        setState(250);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          _localctx->left = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(235);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");

          setState(236);
          _la = _input->LA(1);

          if (!((((_la & ~ 0x3fULL) == 0) &&
                 ((1ULL << _la) & ((1ULL << LlamaLangParser::DIV)
                                   | (1ULL << LlamaLangParser::MOD)
                                   | (1ULL << LlamaLangParser::LSHIFT)
                                   | (1ULL << LlamaLangParser::RSHIFT)
                                   | (1ULL << LlamaLangParser::BIT_CLEAR)
                                   | (1ULL << LlamaLangParser::STAR)
                                   | (1ULL << LlamaLangParser::AMPERSAND))) != 0))) {
            _errHandler->recoverInline(this);
          } else {
            _errHandler->reportMatch(this);
            consume();
          }

          setState(237);
          dynamic_cast<ExpressionContext *>(_localctx)->right = expression(6);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          _localctx->left = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(238);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");

          setState(239);
          _la = _input->LA(1);

          if (!((((_la & ~ 0x3fULL) == 0) &&
                 ((1ULL << _la) & ((1ULL << LlamaLangParser::OR)
                                   | (1ULL << LlamaLangParser::PLUS)
                                   | (1ULL << LlamaLangParser::MINUS)
                                   | (1ULL << LlamaLangParser::CARET))) != 0))) {
            _errHandler->recoverInline(this);
          } else {
            _errHandler->reportMatch(this);
            consume();
          }

          setState(240);
          dynamic_cast<ExpressionContext *>(_localctx)->right = expression(5);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          _localctx->left = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(241);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");

          setState(242);
          _la = _input->LA(1);

          if (!((((_la & ~ 0x3fULL) == 0) &&
                 ((1ULL << _la) & ((1ULL << LlamaLangParser::EQUALS)
                                   | (1ULL << LlamaLangParser::NOT_EQUALS)
                                   | (1ULL << LlamaLangParser::LESS)
                                   | (1ULL << LlamaLangParser::LESS_OR_EQUALS)
                                   | (1ULL << LlamaLangParser::GREATER)
                                   | (1ULL << LlamaLangParser::GREATER_OR_EQUALS))) != 0))) {
            _errHandler->recoverInline(this);
          } else {
            _errHandler->reportMatch(this);
            consume();
          }

          setState(243);
          dynamic_cast<ExpressionContext *>(_localctx)->right = expression(4);
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          _localctx->left = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(244);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");

          setState(245);
          match(LlamaLangParser::LOGICAL_AND);
          setState(246);
          dynamic_cast<ExpressionContext *>(_localctx)->right = expression(3);
          break;
        }

        case 5: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          _localctx->left = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(247);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");

          setState(248);
          match(LlamaLangParser::LOGICAL_OR);
          setState(249);
          dynamic_cast<ExpressionContext *>(_localctx)->right = expression(2);
          break;
        }

        default:
          break;
        }
      }

      setState(254);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    }
  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PrimaryExprContext ------------------------------------------------------------------

LlamaLangParser::PrimaryExprContext::PrimaryExprContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::OperandContext *LlamaLangParser::PrimaryExprContext::operand()
{
  return getRuleContext<LlamaLangParser::OperandContext>(0);
}

LlamaLangParser::PrimaryExprContext *LlamaLangParser::PrimaryExprContext::primaryExpr()
{
  return getRuleContext<LlamaLangParser::PrimaryExprContext>(0);
}

tree::TerminalNode *LlamaLangParser::PrimaryExprContext::DOT()
{
  return getToken(LlamaLangParser::DOT, 0);
}

tree::TerminalNode *LlamaLangParser::PrimaryExprContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}

LlamaLangParser::ArgumentsContext *LlamaLangParser::PrimaryExprContext::arguments()
{
  return getRuleContext<LlamaLangParser::ArgumentsContext>(0);
}


size_t LlamaLangParser::PrimaryExprContext::getRuleIndex() const
{
  return LlamaLangParser::RulePrimaryExpr;
}


antlrcpp::Any LlamaLangParser::PrimaryExprContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitPrimaryExpr(this);
  else
    return visitor->visitChildren(this);
}


LlamaLangParser::PrimaryExprContext *LlamaLangParser::primaryExpr()
{
  return primaryExpr(0);
}

LlamaLangParser::PrimaryExprContext *LlamaLangParser::primaryExpr(int precedence)
{
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LlamaLangParser::PrimaryExprContext *_localctx = _tracker.createInstance<PrimaryExprContext>(_ctx, parentState);
  LlamaLangParser::PrimaryExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 54;
  enterRecursionRule(_localctx, 54, LlamaLangParser::RulePrimaryExpr, precedence);



#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    unrollRecursionContexts(parentContext);
  });

  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(256);
    operand();
    _ctx->stop = _input->LT(-1);
    setState(266);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);

    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();

        previousContext = _localctx;
        _localctx = _tracker.createInstance<PrimaryExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RulePrimaryExpr);
        setState(258);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");

        setState(262);
        _errHandler->sync(this);

        switch (_input->LA(1)) {
        case LlamaLangParser::DOT: {
          setState(259);
          match(LlamaLangParser::DOT);
          setState(260);
          match(LlamaLangParser::IDENTIFIER);
          break;
        }

        case LlamaLangParser::L_PAREN: {
          setState(261);
          arguments();
          break;
        }

        default:
          throw NoViableAltException(this);
        }
      }

      setState(268);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    }
  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- UnaryExprContext ------------------------------------------------------------------

LlamaLangParser::UnaryExprContext::UnaryExprContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::PrimaryExprContext *LlamaLangParser::UnaryExprContext::primaryExpr()
{
  return getRuleContext<LlamaLangParser::PrimaryExprContext>(0);
}

LlamaLangParser::UnaryOpContext *LlamaLangParser::UnaryExprContext::unaryOp()
{
  return getRuleContext<LlamaLangParser::UnaryOpContext>(0);
}

LlamaLangParser::ExpressionContext *LlamaLangParser::UnaryExprContext::expression()
{
  return getRuleContext<LlamaLangParser::ExpressionContext>(0);
}


size_t LlamaLangParser::UnaryExprContext::getRuleIndex() const
{
  return LlamaLangParser::RuleUnaryExpr;
}


antlrcpp::Any LlamaLangParser::UnaryExprContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitUnaryExpr(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::UnaryExprContext *LlamaLangParser::unaryExpr()
{
  UnaryExprContext *_localctx = _tracker.createInstance<UnaryExprContext>(_ctx, getState());
  enterRule(_localctx, 56, LlamaLangParser::RuleUnaryExpr);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(273);
    _errHandler->sync(this);

    switch (_input->LA(1)) {
    case LlamaLangParser::IDENTIFIER:
    case LlamaLangParser::L_PAREN:
    case LlamaLangParser::DECIMAL_LIT:
    case LlamaLangParser::OCTAL_LIT:
    case LlamaLangParser::HEX_LIT:
    case LlamaLangParser::FLOAT_LIT:
    case LlamaLangParser::DOUBLE_LIT:
    case LlamaLangParser::RUNE_LIT:
    case LlamaLangParser::INTERPRETED_STRING_LIT: {
      enterOuterAlt(_localctx, 1);
      setState(269);
      primaryExpr(0);
      break;
    }

    case LlamaLangParser::PLUS_PLUS:
    case LlamaLangParser::MINUS_MINUS:
    case LlamaLangParser::EXCLAMATION:
    case LlamaLangParser::PLUS:
    case LlamaLangParser::MINUS:
    case LlamaLangParser::STAR:
    case LlamaLangParser::AMPERSAND: {
      enterOuterAlt(_localctx, 2);
      setState(270);
      unaryOp();
      setState(271);
      expression(0);
      break;
    }

    default:
      throw NoViableAltException(this);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OperandContext ------------------------------------------------------------------

LlamaLangParser::OperandContext::OperandContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::LiteralContext *LlamaLangParser::OperandContext::literal()
{
  return getRuleContext<LlamaLangParser::LiteralContext>(0);
}

LlamaLangParser::OperandNameContext *LlamaLangParser::OperandContext::operandName()
{
  return getRuleContext<LlamaLangParser::OperandNameContext>(0);
}

LlamaLangParser::MethodExprContext *LlamaLangParser::OperandContext::methodExpr()
{
  return getRuleContext<LlamaLangParser::MethodExprContext>(0);
}

tree::TerminalNode *LlamaLangParser::OperandContext::L_PAREN()
{
  return getToken(LlamaLangParser::L_PAREN, 0);
}

LlamaLangParser::ExpressionContext *LlamaLangParser::OperandContext::expression()
{
  return getRuleContext<LlamaLangParser::ExpressionContext>(0);
}

tree::TerminalNode *LlamaLangParser::OperandContext::R_PAREN()
{
  return getToken(LlamaLangParser::R_PAREN, 0);
}


size_t LlamaLangParser::OperandContext::getRuleIndex() const
{
  return LlamaLangParser::RuleOperand;
}


antlrcpp::Any LlamaLangParser::OperandContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitOperand(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::OperandContext *LlamaLangParser::operand()
{
  OperandContext *_localctx = _tracker.createInstance<OperandContext>(_ctx, getState());
  enterRule(_localctx, 58, LlamaLangParser::RuleOperand);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(282);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(275);
      literal();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(276);
      operandName();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(277);
      methodExpr();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(278);
      match(LlamaLangParser::L_PAREN);
      setState(279);
      expression(0);
      setState(280);
      match(LlamaLangParser::R_PAREN);
      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryOpContext ------------------------------------------------------------------

LlamaLangParser::UnaryOpContext::UnaryOpContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::UnaryOpContext::PLUS()
{
  return getToken(LlamaLangParser::PLUS, 0);
}

tree::TerminalNode *LlamaLangParser::UnaryOpContext::MINUS()
{
  return getToken(LlamaLangParser::MINUS, 0);
}

tree::TerminalNode *LlamaLangParser::UnaryOpContext::EXCLAMATION()
{
  return getToken(LlamaLangParser::EXCLAMATION, 0);
}

tree::TerminalNode *LlamaLangParser::UnaryOpContext::STAR()
{
  return getToken(LlamaLangParser::STAR, 0);
}

tree::TerminalNode *LlamaLangParser::UnaryOpContext::AMPERSAND()
{
  return getToken(LlamaLangParser::AMPERSAND, 0);
}

tree::TerminalNode *LlamaLangParser::UnaryOpContext::MINUS_MINUS()
{
  return getToken(LlamaLangParser::MINUS_MINUS, 0);
}

tree::TerminalNode *LlamaLangParser::UnaryOpContext::PLUS_PLUS()
{
  return getToken(LlamaLangParser::PLUS_PLUS, 0);
}


size_t LlamaLangParser::UnaryOpContext::getRuleIndex() const
{
  return LlamaLangParser::RuleUnaryOp;
}


antlrcpp::Any LlamaLangParser::UnaryOpContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitUnaryOp(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::UnaryOpContext *LlamaLangParser::unaryOp()
{
  UnaryOpContext *_localctx = _tracker.createInstance<UnaryOpContext>(_ctx, getState());
  enterRule(_localctx, 60, LlamaLangParser::RuleUnaryOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(284);
    _la = _input->LA(1);

    if (!((((_la & ~ 0x3fULL) == 0) &&
           ((1ULL << _la) & ((1ULL << LlamaLangParser::PLUS_PLUS)
                             | (1ULL << LlamaLangParser::MINUS_MINUS)
                             | (1ULL << LlamaLangParser::EXCLAMATION)
                             | (1ULL << LlamaLangParser::PLUS)
                             | (1ULL << LlamaLangParser::MINUS)
                             | (1ULL << LlamaLangParser::STAR)
                             | (1ULL << LlamaLangParser::AMPERSAND))) != 0))) {
      _errHandler->recoverInline(this);
    } else {
      _errHandler->reportMatch(this);
      consume();
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

LlamaLangParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::BasicLitContext *LlamaLangParser::LiteralContext::basicLit()
{
  return getRuleContext<LlamaLangParser::BasicLitContext>(0);
}


size_t LlamaLangParser::LiteralContext::getRuleIndex() const
{
  return LlamaLangParser::RuleLiteral;
}


antlrcpp::Any LlamaLangParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::LiteralContext *LlamaLangParser::literal()
{
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 62, LlamaLangParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(286);
    basicLit();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BasicLitContext ------------------------------------------------------------------

LlamaLangParser::BasicLitContext::BasicLitContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::IntegerContext *LlamaLangParser::BasicLitContext::integer()
{
  return getRuleContext<LlamaLangParser::IntegerContext>(0);
}

LlamaLangParser::String_Context *LlamaLangParser::BasicLitContext::string_()
{
  return getRuleContext<LlamaLangParser::String_Context>(0);
}

LlamaLangParser::FloatingPointContext *LlamaLangParser::BasicLitContext::floatingPoint()
{
  return getRuleContext<LlamaLangParser::FloatingPointContext>(0);
}

tree::TerminalNode *LlamaLangParser::BasicLitContext::RUNE_LIT()
{
  return getToken(LlamaLangParser::RUNE_LIT, 0);
}


size_t LlamaLangParser::BasicLitContext::getRuleIndex() const
{
  return LlamaLangParser::RuleBasicLit;
}


antlrcpp::Any LlamaLangParser::BasicLitContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitBasicLit(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::BasicLitContext *LlamaLangParser::basicLit()
{
  BasicLitContext *_localctx = _tracker.createInstance<BasicLitContext>(_ctx, getState());
  enterRule(_localctx, 64, LlamaLangParser::RuleBasicLit);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(292);
    _errHandler->sync(this);

    switch (_input->LA(1)) {
    case LlamaLangParser::DECIMAL_LIT:
    case LlamaLangParser::OCTAL_LIT:
    case LlamaLangParser::HEX_LIT: {
      enterOuterAlt(_localctx, 1);
      setState(288);
      integer();
      break;
    }

    case LlamaLangParser::INTERPRETED_STRING_LIT: {
      enterOuterAlt(_localctx, 2);
      setState(289);
      string_();
      break;
    }

    case LlamaLangParser::FLOAT_LIT:
    case LlamaLangParser::DOUBLE_LIT: {
      enterOuterAlt(_localctx, 3);
      setState(290);
      floatingPoint();
      break;
    }

    case LlamaLangParser::RUNE_LIT: {
      enterOuterAlt(_localctx, 4);
      setState(291);
      match(LlamaLangParser::RUNE_LIT);
      break;
    }

    default:
      throw NoViableAltException(this);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerContext ------------------------------------------------------------------

LlamaLangParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::IntegerContext::DECIMAL_LIT()
{
  return getToken(LlamaLangParser::DECIMAL_LIT, 0);
}

tree::TerminalNode *LlamaLangParser::IntegerContext::OCTAL_LIT()
{
  return getToken(LlamaLangParser::OCTAL_LIT, 0);
}

tree::TerminalNode *LlamaLangParser::IntegerContext::HEX_LIT()
{
  return getToken(LlamaLangParser::HEX_LIT, 0);
}


size_t LlamaLangParser::IntegerContext::getRuleIndex() const
{
  return LlamaLangParser::RuleInteger;
}


antlrcpp::Any LlamaLangParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::IntegerContext *LlamaLangParser::integer()
{
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 66, LlamaLangParser::RuleInteger);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(294);
    _la = _input->LA(1);

    if (!((((_la & ~ 0x3fULL) == 0) &&
           ((1ULL << _la) & ((1ULL << LlamaLangParser::DECIMAL_LIT)
                             | (1ULL << LlamaLangParser::OCTAL_LIT)
                             | (1ULL << LlamaLangParser::HEX_LIT))) != 0))) {
      _errHandler->recoverInline(this);
    } else {
      _errHandler->reportMatch(this);
      consume();
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FloatingPointContext ------------------------------------------------------------------

LlamaLangParser::FloatingPointContext::FloatingPointContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::FloatingPointContext::FLOAT_LIT()
{
  return getToken(LlamaLangParser::FLOAT_LIT, 0);
}

tree::TerminalNode *LlamaLangParser::FloatingPointContext::DOUBLE_LIT()
{
  return getToken(LlamaLangParser::DOUBLE_LIT, 0);
}


size_t LlamaLangParser::FloatingPointContext::getRuleIndex() const
{
  return LlamaLangParser::RuleFloatingPoint;
}


antlrcpp::Any LlamaLangParser::FloatingPointContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitFloatingPoint(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::FloatingPointContext *LlamaLangParser::floatingPoint()
{
  FloatingPointContext *_localctx = _tracker.createInstance<FloatingPointContext>(_ctx, getState());
  enterRule(_localctx, 68, LlamaLangParser::RuleFloatingPoint);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(296);
    _la = _input->LA(1);

    if (!(_la == LlamaLangParser::FLOAT_LIT

          || _la == LlamaLangParser::DOUBLE_LIT)) {
      _errHandler->recoverInline(this);
    } else {
      _errHandler->reportMatch(this);
      consume();
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OperandNameContext ------------------------------------------------------------------

LlamaLangParser::OperandNameContext::OperandNameContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::OperandNameContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}

LlamaLangParser::QualifiedIdentContext *LlamaLangParser::OperandNameContext::qualifiedIdent()
{
  return getRuleContext<LlamaLangParser::QualifiedIdentContext>(0);
}


size_t LlamaLangParser::OperandNameContext::getRuleIndex() const
{
  return LlamaLangParser::RuleOperandName;
}


antlrcpp::Any LlamaLangParser::OperandNameContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitOperandName(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::OperandNameContext *LlamaLangParser::operandName()
{
  OperandNameContext *_localctx = _tracker.createInstance<OperandNameContext>(_ctx, getState());
  enterRule(_localctx, 70, LlamaLangParser::RuleOperandName);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(300);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(298);
      match(LlamaLangParser::IDENTIFIER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(299);
      qualifiedIdent();
      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QualifiedIdentContext ------------------------------------------------------------------

LlamaLangParser::QualifiedIdentContext::QualifiedIdentContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

std::vector<tree::TerminalNode *> LlamaLangParser::QualifiedIdentContext::IDENTIFIER()
{
  return getTokens(LlamaLangParser::IDENTIFIER);
}

tree::TerminalNode *LlamaLangParser::QualifiedIdentContext::IDENTIFIER(size_t i)
{
  return getToken(LlamaLangParser::IDENTIFIER, i);
}

tree::TerminalNode *LlamaLangParser::QualifiedIdentContext::DOT()
{
  return getToken(LlamaLangParser::DOT, 0);
}


size_t LlamaLangParser::QualifiedIdentContext::getRuleIndex() const
{
  return LlamaLangParser::RuleQualifiedIdent;
}


antlrcpp::Any LlamaLangParser::QualifiedIdentContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitQualifiedIdent(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::QualifiedIdentContext *LlamaLangParser::qualifiedIdent()
{
  QualifiedIdentContext *_localctx = _tracker.createInstance<QualifiedIdentContext>(_ctx, getState());
  enterRule(_localctx, 72, LlamaLangParser::RuleQualifiedIdent);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(302);
    match(LlamaLangParser::IDENTIFIER);
    setState(303);
    match(LlamaLangParser::DOT);
    setState(304);
    match(LlamaLangParser::IDENTIFIER);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralTypeContext ------------------------------------------------------------------

LlamaLangParser::LiteralTypeContext::LiteralTypeContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::TypeNameContext *LlamaLangParser::LiteralTypeContext::typeName()
{
  return getRuleContext<LlamaLangParser::TypeNameContext>(0);
}


size_t LlamaLangParser::LiteralTypeContext::getRuleIndex() const
{
  return LlamaLangParser::RuleLiteralType;
}


antlrcpp::Any LlamaLangParser::LiteralTypeContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitLiteralType(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::LiteralTypeContext *LlamaLangParser::literalType()
{
  LiteralTypeContext *_localctx = _tracker.createInstance<LiteralTypeContext>(_ctx, getState());
  enterRule(_localctx, 74, LlamaLangParser::RuleLiteralType);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(306);
    typeName();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldDeclContext ------------------------------------------------------------------

LlamaLangParser::FieldDeclContext::FieldDeclContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::IdentifierListContext *LlamaLangParser::FieldDeclContext::identifierList()
{
  return getRuleContext<LlamaLangParser::IdentifierListContext>(0);
}

LlamaLangParser::Type_Context *LlamaLangParser::FieldDeclContext::type_()
{
  return getRuleContext<LlamaLangParser::Type_Context>(0);
}

LlamaLangParser::String_Context *LlamaLangParser::FieldDeclContext::string_()
{
  return getRuleContext<LlamaLangParser::String_Context>(0);
}


size_t LlamaLangParser::FieldDeclContext::getRuleIndex() const
{
  return LlamaLangParser::RuleFieldDecl;
}


antlrcpp::Any LlamaLangParser::FieldDeclContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitFieldDecl(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::FieldDeclContext *LlamaLangParser::fieldDecl()
{
  FieldDeclContext *_localctx = _tracker.createInstance<FieldDeclContext>(_ctx, getState());
  enterRule(_localctx, 76, LlamaLangParser::RuleFieldDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(308);
    identifierList();
    setState(309);
    type_();
    setState(311);
    _errHandler->sync(this);

    _la = _input->LA(1);

    if (_la == LlamaLangParser::INTERPRETED_STRING_LIT) {
      setState(310);
      string_();
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- String_Context ------------------------------------------------------------------

LlamaLangParser::String_Context::String_Context(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::String_Context::INTERPRETED_STRING_LIT()
{
  return getToken(LlamaLangParser::INTERPRETED_STRING_LIT, 0);
}


size_t LlamaLangParser::String_Context::getRuleIndex() const
{
  return LlamaLangParser::RuleString_;
}


antlrcpp::Any LlamaLangParser::String_Context::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitString_(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::String_Context *LlamaLangParser::string_()
{
  String_Context *_localctx = _tracker.createInstance<String_Context>(_ctx, getState());
  enterRule(_localctx, 78, LlamaLangParser::RuleString_);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(313);
    match(LlamaLangParser::INTERPRETED_STRING_LIT);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentsContext ------------------------------------------------------------------

LlamaLangParser::ArgumentsContext::ArgumentsContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::ArgumentsContext::L_PAREN()
{
  return getToken(LlamaLangParser::L_PAREN, 0);
}

tree::TerminalNode *LlamaLangParser::ArgumentsContext::R_PAREN()
{
  return getToken(LlamaLangParser::R_PAREN, 0);
}

LlamaLangParser::ExpressionListContext *LlamaLangParser::ArgumentsContext::expressionList()
{
  return getRuleContext<LlamaLangParser::ExpressionListContext>(0);
}

LlamaLangParser::Type_Context *LlamaLangParser::ArgumentsContext::type_()
{
  return getRuleContext<LlamaLangParser::Type_Context>(0);
}

tree::TerminalNode *LlamaLangParser::ArgumentsContext::ELLIPSIS()
{
  return getToken(LlamaLangParser::ELLIPSIS, 0);
}

std::vector<tree::TerminalNode *> LlamaLangParser::ArgumentsContext::COMMA()
{
  return getTokens(LlamaLangParser::COMMA);
}

tree::TerminalNode *LlamaLangParser::ArgumentsContext::COMMA(size_t i)
{
  return getToken(LlamaLangParser::COMMA, i);
}


size_t LlamaLangParser::ArgumentsContext::getRuleIndex() const
{
  return LlamaLangParser::RuleArguments;
}


antlrcpp::Any LlamaLangParser::ArgumentsContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitArguments(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ArgumentsContext *LlamaLangParser::arguments()
{
  ArgumentsContext *_localctx = _tracker.createInstance<ArgumentsContext>(_ctx, getState());
  enterRule(_localctx, 80, LlamaLangParser::RuleArguments);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(315);
    match(LlamaLangParser::L_PAREN);
    setState(330);
    _errHandler->sync(this);

    _la = _input->LA(1);

    if ((((_la & ~ 0x3fULL) == 0) &&
         ((1ULL << _la) & ((1ULL << LlamaLangParser::IDENTIFIER)
                           | (1ULL << LlamaLangParser::L_PAREN)
                           | (1ULL << LlamaLangParser::L_BRACKET)
                           | (1ULL << LlamaLangParser::PLUS_PLUS)
                           | (1ULL << LlamaLangParser::MINUS_MINUS)
                           | (1ULL << LlamaLangParser::EXCLAMATION)
                           | (1ULL << LlamaLangParser::PLUS)
                           | (1ULL << LlamaLangParser::MINUS)
                           | (1ULL << LlamaLangParser::STAR)
                           | (1ULL << LlamaLangParser::AMPERSAND)
                           | (1ULL << LlamaLangParser::DECIMAL_LIT)
                           | (1ULL << LlamaLangParser::OCTAL_LIT)
                           | (1ULL << LlamaLangParser::HEX_LIT)
                           | (1ULL << LlamaLangParser::FLOAT_LIT)
                           | (1ULL << LlamaLangParser::DOUBLE_LIT)
                           | (1ULL << LlamaLangParser::RUNE_LIT)
                           | (1ULL << LlamaLangParser::INTERPRETED_STRING_LIT))) != 0)) {
      setState(322);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
      case 1: {
        setState(316);
        expressionList();
        break;
      }

      case 2: {
        setState(317);
        type_();
        setState(320);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          setState(318);
          match(LlamaLangParser::COMMA);
          setState(319);
          expressionList();
          break;
        }

        default:
          break;
        }

        break;
      }

      default:
        break;
      }

      setState(325);
      _errHandler->sync(this);

      _la = _input->LA(1);

      if (_la == LlamaLangParser::ELLIPSIS) {
        setState(324);
        match(LlamaLangParser::ELLIPSIS);
      }

      setState(328);
      _errHandler->sync(this);

      _la = _input->LA(1);

      if (_la == LlamaLangParser::COMMA) {
        setState(327);
        match(LlamaLangParser::COMMA);
      }
    }

    setState(332);
    match(LlamaLangParser::R_PAREN);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodExprContext ------------------------------------------------------------------

LlamaLangParser::MethodExprContext::MethodExprContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::ReceiverTypeContext *LlamaLangParser::MethodExprContext::receiverType()
{
  return getRuleContext<LlamaLangParser::ReceiverTypeContext>(0);
}

tree::TerminalNode *LlamaLangParser::MethodExprContext::DOT()
{
  return getToken(LlamaLangParser::DOT, 0);
}

tree::TerminalNode *LlamaLangParser::MethodExprContext::IDENTIFIER()
{
  return getToken(LlamaLangParser::IDENTIFIER, 0);
}


size_t LlamaLangParser::MethodExprContext::getRuleIndex() const
{
  return LlamaLangParser::RuleMethodExpr;
}


antlrcpp::Any LlamaLangParser::MethodExprContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitMethodExpr(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::MethodExprContext *LlamaLangParser::methodExpr()
{
  MethodExprContext *_localctx = _tracker.createInstance<MethodExprContext>(_ctx, getState());
  enterRule(_localctx, 82, LlamaLangParser::RuleMethodExpr);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(334);
    receiverType();
    setState(335);
    match(LlamaLangParser::DOT);
    setState(336);
    match(LlamaLangParser::IDENTIFIER);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReceiverTypeContext ------------------------------------------------------------------

LlamaLangParser::ReceiverTypeContext::ReceiverTypeContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

LlamaLangParser::TypeNameContext *LlamaLangParser::ReceiverTypeContext::typeName()
{
  return getRuleContext<LlamaLangParser::TypeNameContext>(0);
}

tree::TerminalNode *LlamaLangParser::ReceiverTypeContext::L_PAREN()
{
  return getToken(LlamaLangParser::L_PAREN, 0);
}

tree::TerminalNode *LlamaLangParser::ReceiverTypeContext::R_PAREN()
{
  return getToken(LlamaLangParser::R_PAREN, 0);
}

tree::TerminalNode *LlamaLangParser::ReceiverTypeContext::STAR()
{
  return getToken(LlamaLangParser::STAR, 0);
}

LlamaLangParser::ReceiverTypeContext *LlamaLangParser::ReceiverTypeContext::receiverType()
{
  return getRuleContext<LlamaLangParser::ReceiverTypeContext>(0);
}


size_t LlamaLangParser::ReceiverTypeContext::getRuleIndex() const
{
  return LlamaLangParser::RuleReceiverType;
}


antlrcpp::Any LlamaLangParser::ReceiverTypeContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitReceiverType(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::ReceiverTypeContext *LlamaLangParser::receiverType()
{
  ReceiverTypeContext *_localctx = _tracker.createInstance<ReceiverTypeContext>(_ctx, getState());
  enterRule(_localctx, 84, LlamaLangParser::RuleReceiverType);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(347);
    _errHandler->sync(this);

    switch (_input->LA(1)) {
    case LlamaLangParser::IDENTIFIER: {
      enterOuterAlt(_localctx, 1);
      setState(338);
      typeName();
      break;
    }

    case LlamaLangParser::L_PAREN: {
      enterOuterAlt(_localctx, 2);
      setState(339);
      match(LlamaLangParser::L_PAREN);
      setState(343);
      _errHandler->sync(this);

      switch (_input->LA(1)) {
      case LlamaLangParser::STAR: {
        setState(340);
        match(LlamaLangParser::STAR);
        setState(341);
        typeName();
        break;
      }

      case LlamaLangParser::IDENTIFIER:
      case LlamaLangParser::L_PAREN: {
        setState(342);
        receiverType();
        break;
      }

      default:
        throw NoViableAltException(this);
      }

      setState(345);
      match(LlamaLangParser::R_PAREN);
      break;
    }

    default:
      throw NoViableAltException(this);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EosContext ------------------------------------------------------------------

LlamaLangParser::EosContext::EosContext(ParserRuleContext *parent, size_t invokingState)
  : LlamaLangParseContext(parent, invokingState)
{
}

tree::TerminalNode *LlamaLangParser::EosContext::SEMI()
{
  return getToken(LlamaLangParser::SEMI, 0);
}

tree::TerminalNode *LlamaLangParser::EosContext::EOF()
{
  return getToken(LlamaLangParser::EOF, 0);
}


size_t LlamaLangParser::EosContext::getRuleIndex() const
{
  return LlamaLangParser::RuleEos;
}


antlrcpp::Any LlamaLangParser::EosContext::accept(tree::ParseTreeVisitor *visitor)
{
  if (auto parserVisitor = dynamic_cast<LlamaLangVisitor *>(visitor))
    return parserVisitor->visitEos(this);
  else
    return visitor->visitChildren(this);
}

LlamaLangParser::EosContext *LlamaLangParser::eos()
{
  EosContext *_localctx = _tracker.createInstance<EosContext>(_ctx, getState());
  enterRule(_localctx, 86, LlamaLangParser::RuleEos);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    setState(353);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(349);
      match(LlamaLangParser::SEMI);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(350);
      match(LlamaLangParser::EOF);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(351);

      if (!(lineTerminatorAhead())) throw FailedPredicateException(this, "lineTerminatorAhead()");

      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(352);

      if (!(checkPreviousTokenText("}"))) throw FailedPredicateException(this, "checkPreviousTokenText(\"}\")");

      break;
    }

    default:
      break;
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool LlamaLangParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex)
{
  switch (ruleIndex) {
  case 26:
    return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  case 27:
    return primaryExprSempred(dynamic_cast<PrimaryExprContext *>(context), predicateIndex);

  case 43:
    return eosSempred(dynamic_cast<EosContext *>(context), predicateIndex);

  default:
    break;
  }

  return true;
}

bool LlamaLangParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex)
{
  switch (predicateIndex) {
  case 0:
    return precpred(_ctx, 5);

  case 1:
    return precpred(_ctx, 4);

  case 2:
    return precpred(_ctx, 3);

  case 3:
    return precpred(_ctx, 2);

  case 4:
    return precpred(_ctx, 1);

  default:
    break;
  }

  return true;
}

bool LlamaLangParser::primaryExprSempred(PrimaryExprContext *_localctx, size_t predicateIndex)
{
  switch (predicateIndex) {
  case 5:
    return precpred(_ctx, 1);

  default:
    break;
  }

  return true;
}

bool LlamaLangParser::eosSempred(EosContext *_localctx, size_t predicateIndex)
{
  switch (predicateIndex) {
  case 6:
    return lineTerminatorAhead();

  case 7:
    return checkPreviousTokenText("}");

  default:
    break;
  }

  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> LlamaLangParser::_decisionToDFA;
atn::PredictionContextCache LlamaLangParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LlamaLangParser::_atn;
std::vector<uint16_t> LlamaLangParser::_serializedATN;

std::vector<std::string> LlamaLangParser::_ruleNames = {
  "sourceFile", "identifierList", "expressionList", "includeDirective",
  "externDirective", "runDirective", "basicDirective", "functionDef", "varDef",
  "block", "statementList", "statement", "simpleStmt", "expressionStmt",
  "assignment", "assign_op", "emptyStmt", "returnStmt", "type_", "pointer",
  "array", "typeName", "signature", "result", "parameters", "parameterDecl",
  "expression", "primaryExpr", "unaryExpr", "operand", "unaryOp", "literal",
  "basicLit", "integer", "floatingPoint", "operandName", "qualifiedIdent",
  "literalType", "fieldDecl", "string_", "arguments", "methodExpr", "receiverType",
  "eos"
};

std::vector<std::string> LlamaLangParser::_literalNames = {
  "", "'include'", "'extern'", "'run'", "'#'", "'func'", "'ret'", "", "'('",
  "')'", "'{'", "'}'", "'['", "']'", "'='", "','", "';'", "':'", "'.'",
  "'++'", "'--'", "':='", "'...'", "", "", "'=='", "'!='", "'<'", "'<='",
  "'>'", "'>='", "'|'", "'/'", "'%'", "'<<'", "'>>'", "'&^'", "'!'", "'+'",
  "'-'", "'^'", "'*'", "'&'"
};

std::vector<std::string> LlamaLangParser::_symbolicNames = {
  "", "", "", "", "DIR_BEGIN", "FUNC", "RETURN", "IDENTIFIER", "L_PAREN",
  "R_PAREN", "L_CURLY", "R_CURLY", "L_BRACKET", "R_BRACKET", "ASSIGN", "COMMA",
  "SEMI", "COLON", "DOT", "PLUS_PLUS", "MINUS_MINUS", "DECLARE_ASSIGN",
  "ELLIPSIS", "LOGICAL_OR", "LOGICAL_AND", "EQUALS", "NOT_EQUALS", "LESS",
  "LESS_OR_EQUALS", "GREATER", "GREATER_OR_EQUALS", "OR", "DIV", "MOD",
  "LSHIFT", "RSHIFT", "BIT_CLEAR", "EXCLAMATION", "PLUS", "MINUS", "CARET",
  "STAR", "AMPERSAND", "DECIMAL_LIT", "OCTAL_LIT", "HEX_LIT", "FLOAT_LIT",
  "DOUBLE_LIT", "RUNE_LIT", "INTERPRETED_STRING_LIT", "WS", "COMMENT", "TERMINATOR",
  "LINE_COMMENT"
};

dfa::Vocabulary LlamaLangParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LlamaLangParser::_tokenNames;

LlamaLangParser::Initializer::Initializer()
{
  for (size_t i = 0; i < _symbolicNames.size(); ++i) {
    std::string name = _vocabulary.getLiteralName(i);

    if (name.empty()) {
      name = _vocabulary.getSymbolicName(i);
    }

    if (name.empty()) {
      _tokenNames.push_back("<INVALID>");
    } else {
      _tokenNames.push_back(name);
    }
  }

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964,
    0x3, 0x37, 0x166, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4,
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7,
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb,
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe,
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4,
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15,
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9,
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b,
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4,
    0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22,
    0x9, 0x22, 0x4, 0x23, 0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9,
    0x25, 0x4, 0x26, 0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28,
    0x4, 0x29, 0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4,
    0x2c, 0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x3, 0x2, 0x7, 0x2, 0x5c, 0xa,
    0x2, 0xc, 0x2, 0xe, 0x2, 0x5f, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2,
    0x3, 0x2, 0x7, 0x2, 0x65, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x68, 0xb, 0x2,
    0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x6f, 0xa,
    0x3, 0xc, 0x3, 0xe, 0x3, 0x72, 0xb, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4,
    0x7, 0x4, 0x77, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x7a, 0xb, 0x4, 0x3, 0x5,
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6,
    0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8,
    0x3, 0x9, 0x5, 0x9, 0x8c, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3,
    0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5,
    0xa, 0x98, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x9c, 0xa, 0xb, 0x3,
    0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x6, 0xc, 0xa3, 0xa, 0xc,
    0xd, 0xc, 0xe, 0xc, 0xa4, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x5,
    0xd, 0xab, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0xb0, 0xa,
    0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10,
    0x3, 0x11, 0x5, 0x11, 0xb9, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12,
    0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0xc1, 0xa, 0x13, 0x3, 0x14,
    0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0xc6, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15,
    0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3,
    0x17, 0x5, 0x17, 0xd1, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3,
    0x19, 0x5, 0x19, 0xd7, 0xa, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3,
    0x1a, 0x7, 0x1a, 0xdd, 0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 0xe0, 0xb, 0x1a,
    0x5, 0x1a, 0xe2, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 0x1b,
    0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0xec, 0xa, 0x1c,
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3,
    0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c,
    0x3, 0x1c, 0x3, 0x1c, 0x7, 0x1c, 0xfd, 0xa, 0x1c, 0xc, 0x1c, 0xe, 0x1c,
    0x100, 0xb, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d,
    0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x109, 0xa, 0x1d, 0x7, 0x1d, 0x10b,
    0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x10e, 0xb, 0x1d, 0x3, 0x1e, 0x3, 0x1e,
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x114, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f,
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x11d,
    0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x22, 0x3,
    0x22, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x127, 0xa, 0x22, 0x3, 0x23,
    0x3, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x12f,
    0xa, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 0x3,
    0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x13a, 0xa, 0x28,
    0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3,
    0x2a, 0x5, 0x2a, 0x143, 0xa, 0x2a, 0x5, 0x2a, 0x145, 0xa, 0x2a, 0x3,
    0x2a, 0x5, 0x2a, 0x148, 0xa, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x14b, 0xa,
    0x2a, 0x5, 0x2a, 0x14d, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2b,
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3,
    0x2c, 0x3, 0x2c, 0x5, 0x2c, 0x15a, 0xa, 0x2c, 0x3, 0x2c, 0x3, 0x2c,
    0x5, 0x2c, 0x15e, 0xa, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d,
    0x5, 0x2d, 0x164, 0xa, 0x2d, 0x3, 0x2d, 0x2, 0x4, 0x36, 0x38, 0x2e,
    0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a,
    0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32,
    0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a,
    0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x2, 0x9, 0x4, 0x2, 0x21,
    0x26, 0x28, 0x2c, 0x4, 0x2, 0x22, 0x26, 0x2b, 0x2c, 0x4, 0x2, 0x21,
    0x21, 0x28, 0x2a, 0x3, 0x2, 0x1b, 0x20, 0x5, 0x2, 0x15, 0x16, 0x27,
    0x29, 0x2b, 0x2c, 0x3, 0x2, 0x2d, 0x2f, 0x3, 0x2, 0x30, 0x31, 0x2, 0x16a,
    0x2, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x4, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6, 0x73,
    0x3, 0x2, 0x2, 0x2, 0x8, 0x7b, 0x3, 0x2, 0x2, 0x2, 0xa, 0x7f, 0x3, 0x2,
    0x2, 0x2, 0xc, 0x83, 0x3, 0x2, 0x2, 0x2, 0xe, 0x87, 0x3, 0x2, 0x2, 0x2,
    0x10, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x12, 0x93, 0x3, 0x2, 0x2, 0x2, 0x14,
    0x99, 0x3, 0x2, 0x2, 0x2, 0x16, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x18, 0xaa,
    0x3, 0x2, 0x2, 0x2, 0x1a, 0xaf, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xb1, 0x3,
    0x2, 0x2, 0x2, 0x1e, 0xb3, 0x3, 0x2, 0x2, 0x2, 0x20, 0xb8, 0x3, 0x2,
    0x2, 0x2, 0x22, 0xbc, 0x3, 0x2, 0x2, 0x2, 0x24, 0xbe, 0x3, 0x2, 0x2,
    0x2, 0x26, 0xc5, 0x3, 0x2, 0x2, 0x2, 0x28, 0xc7, 0x3, 0x2, 0x2, 0x2,
    0x2a, 0xca, 0x3, 0x2, 0x2, 0x2, 0x2c, 0xd0, 0x3, 0x2, 0x2, 0x2, 0x2e,
    0xd2, 0x3, 0x2, 0x2, 0x2, 0x30, 0xd6, 0x3, 0x2, 0x2, 0x2, 0x32, 0xd8,
    0x3, 0x2, 0x2, 0x2, 0x34, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x36, 0xeb, 0x3,
    0x2, 0x2, 0x2, 0x38, 0x101, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x113, 0x3, 0x2,
    0x2, 0x2, 0x3c, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x11e, 0x3, 0x2, 0x2,
    0x2, 0x40, 0x120, 0x3, 0x2, 0x2, 0x2, 0x42, 0x126, 0x3, 0x2, 0x2, 0x2,
    0x44, 0x128, 0x3, 0x2, 0x2, 0x2, 0x46, 0x12a, 0x3, 0x2, 0x2, 0x2, 0x48,
    0x12e, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x130, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x134,
    0x3, 0x2, 0x2, 0x2, 0x4e, 0x136, 0x3, 0x2, 0x2, 0x2, 0x50, 0x13b, 0x3,
    0x2, 0x2, 0x2, 0x52, 0x13d, 0x3, 0x2, 0x2, 0x2, 0x54, 0x150, 0x3, 0x2,
    0x2, 0x2, 0x56, 0x15d, 0x3, 0x2, 0x2, 0x2, 0x58, 0x163, 0x3, 0x2, 0x2,
    0x2, 0x5a, 0x5c, 0x5, 0x8, 0x5, 0x2, 0x5b, 0x5a, 0x3, 0x2, 0x2, 0x2,
    0x5c, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x5d,
    0x5e, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x66, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x5d,
    0x3, 0x2, 0x2, 0x2, 0x60, 0x65, 0x5, 0x10, 0x9, 0x2, 0x61, 0x62, 0x5,
    0x12, 0xa, 0x2, 0x62, 0x63, 0x5, 0x58, 0x2d, 0x2, 0x63, 0x65, 0x3, 0x2,
    0x2, 0x2, 0x64, 0x60, 0x3, 0x2, 0x2, 0x2, 0x64, 0x61, 0x3, 0x2, 0x2,
    0x2, 0x65, 0x68, 0x3, 0x2, 0x2, 0x2, 0x66, 0x64, 0x3, 0x2, 0x2, 0x2,
    0x66, 0x67, 0x3, 0x2, 0x2, 0x2, 0x67, 0x69, 0x3, 0x2, 0x2, 0x2, 0x68,
    0x66, 0x3, 0x2, 0x2, 0x2, 0x69, 0x6a, 0x5, 0x58, 0x2d, 0x2, 0x6a, 0x3,
    0x3, 0x2, 0x2, 0x2, 0x6b, 0x70, 0x7, 0x9, 0x2, 0x2, 0x6c, 0x6d, 0x7,
    0x11, 0x2, 0x2, 0x6d, 0x6f, 0x7, 0x9, 0x2, 0x2, 0x6e, 0x6c, 0x3, 0x2,
    0x2, 0x2, 0x6f, 0x72, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6e, 0x3, 0x2, 0x2,
    0x2, 0x70, 0x71, 0x3, 0x2, 0x2, 0x2, 0x71, 0x5, 0x3, 0x2, 0x2, 0x2,
    0x72, 0x70, 0x3, 0x2, 0x2, 0x2, 0x73, 0x78, 0x5, 0x36, 0x1c, 0x2, 0x74,
    0x75, 0x7, 0x11, 0x2, 0x2, 0x75, 0x77, 0x5, 0x36, 0x1c, 0x2, 0x76, 0x74,
    0x3, 0x2, 0x2, 0x2, 0x77, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x78, 0x76, 0x3,
    0x2, 0x2, 0x2, 0x78, 0x79, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7, 0x3, 0x2,
    0x2, 0x2, 0x7a, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x7c, 0x7, 0x6, 0x2,
    0x2, 0x7c, 0x7d, 0x7, 0x3, 0x2, 0x2, 0x7d, 0x7e, 0x5, 0x50, 0x29, 0x2,
    0x7e, 0x9, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x80, 0x7, 0x6, 0x2, 0x2, 0x80,
    0x81, 0x7, 0x4, 0x2, 0x2, 0x81, 0x82, 0x7, 0x9, 0x2, 0x2, 0x82, 0xb,
    0x3, 0x2, 0x2, 0x2, 0x83, 0x84, 0x7, 0x6, 0x2, 0x2, 0x84, 0x85, 0x7,
    0x5, 0x2, 0x2, 0x85, 0x86, 0x5, 0x38, 0x1d, 0x2, 0x86, 0xd, 0x3, 0x2,
    0x2, 0x2, 0x87, 0x88, 0x7, 0x6, 0x2, 0x2, 0x88, 0x89, 0x7, 0x9, 0x2,
    0x2, 0x89, 0xf, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8c, 0x5, 0xe, 0x8, 0x2,
    0x8b, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8c,
    0x8d, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8e, 0x7, 0x7, 0x2, 0x2, 0x8e, 0x8f,
    0x7, 0x9, 0x2, 0x2, 0x8f, 0x90, 0x5, 0x2e, 0x18, 0x2, 0x90, 0x91, 0x5,
    0x26, 0x14, 0x2, 0x91, 0x92, 0x5, 0x14, 0xb, 0x2, 0x92, 0x11, 0x3, 0x2,
    0x2, 0x2, 0x93, 0x94, 0x7, 0x9, 0x2, 0x2, 0x94, 0x97, 0x5, 0x26, 0x14,
    0x2, 0x95, 0x96, 0x7, 0x10, 0x2, 0x2, 0x96, 0x98, 0x5, 0x6, 0x4, 0x2,
    0x97, 0x95, 0x3, 0x2, 0x2, 0x2, 0x97, 0x98, 0x3, 0x2, 0x2, 0x2, 0x98,
    0x13, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9b, 0x7, 0xc, 0x2, 0x2, 0x9a, 0x9c,
    0x5, 0x16, 0xc, 0x2, 0x9b, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x9c, 0x3,
    0x2, 0x2, 0x2, 0x9c, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9e, 0x7, 0xd,
    0x2, 0x2, 0x9e, 0x15, 0x3, 0x2, 0x2, 0x2, 0x9f, 0xa0, 0x5, 0x18, 0xd,
    0x2, 0xa0, 0xa1, 0x5, 0x58, 0x2d, 0x2, 0xa1, 0xa3, 0x3, 0x2, 0x2, 0x2,
    0xa2, 0x9f, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa4, 0x3, 0x2, 0x2, 0x2, 0xa4,
    0xa2, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa5, 0x17,
    0x3, 0x2, 0x2, 0x2, 0xa6, 0xab, 0x5, 0x12, 0xa, 0x2, 0xa7, 0xab, 0x5,
    0x1a, 0xe, 0x2, 0xa8, 0xab, 0x5, 0x24, 0x13, 0x2, 0xa9, 0xab, 0x5, 0x14,
    0xb, 0x2, 0xaa, 0xa6, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xa7, 0x3, 0x2, 0x2,
    0x2, 0xaa, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xa9, 0x3, 0x2, 0x2, 0x2,
    0xab, 0x19, 0x3, 0x2, 0x2, 0x2, 0xac, 0xb0, 0x5, 0x1c, 0xf, 0x2, 0xad,
    0xb0, 0x5, 0x1e, 0x10, 0x2, 0xae, 0xb0, 0x5, 0x22, 0x12, 0x2, 0xaf,
    0xac, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xad, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xae,
    0x3, 0x2, 0x2, 0x2, 0xb0, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xb1, 0xb2, 0x5,
    0x36, 0x1c, 0x2, 0xb2, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xb3, 0xb4, 0x7, 0x9,
    0x2, 0x2, 0xb4, 0xb5, 0x5, 0x20, 0x11, 0x2, 0xb5, 0xb6, 0x5, 0x6, 0x4,
    0x2, 0xb6, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb9, 0x9, 0x2, 0x2, 0x2,
    0xb8, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb9, 0x3, 0x2, 0x2, 0x2, 0xb9,
    0xba, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbb, 0x7, 0x10, 0x2, 0x2, 0xbb, 0x21,
    0x3, 0x2, 0x2, 0x2, 0xbc, 0xbd, 0x7, 0x12, 0x2, 0x2, 0xbd, 0x23, 0x3,
    0x2, 0x2, 0x2, 0xbe, 0xc0, 0x7, 0x8, 0x2, 0x2, 0xbf, 0xc1, 0x5, 0x36,
    0x1c, 0x2, 0xc0, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc1, 0x3, 0x2, 0x2,
    0x2, 0xc1, 0x25, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc6, 0x5, 0x2c, 0x17, 0x2,
    0xc3, 0xc6, 0x5, 0x28, 0x15, 0x2, 0xc4, 0xc6, 0x5, 0x2a, 0x16, 0x2,
    0xc5, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xc5,
    0xc4, 0x3, 0x2, 0x2, 0x2, 0xc6, 0x27, 0x3, 0x2, 0x2, 0x2, 0xc7, 0xc8,
    0x7, 0x2b, 0x2, 0x2, 0xc8, 0xc9, 0x5, 0x26, 0x14, 0x2, 0xc9, 0x29, 0x3,
    0x2, 0x2, 0x2, 0xca, 0xcb, 0x7, 0xe, 0x2, 0x2, 0xcb, 0xcc, 0x7, 0xf,
    0x2, 0x2, 0xcc, 0xcd, 0x5, 0x2c, 0x17, 0x2, 0xcd, 0x2b, 0x3, 0x2, 0x2,
    0x2, 0xce, 0xd1, 0x7, 0x9, 0x2, 0x2, 0xcf, 0xd1, 0x5, 0x4a, 0x26, 0x2,
    0xd0, 0xce, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xcf, 0x3, 0x2, 0x2, 0x2, 0xd1,
    0x2d, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd3, 0x5, 0x32, 0x1a, 0x2, 0xd3, 0x2f,
    0x3, 0x2, 0x2, 0x2, 0xd4, 0xd7, 0x5, 0x32, 0x1a, 0x2, 0xd5, 0xd7, 0x5,
    0x26, 0x14, 0x2, 0xd6, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd5, 0x3, 0x2,
    0x2, 0x2, 0xd7, 0x31, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xe1, 0x7, 0xa, 0x2,
    0x2, 0xd9, 0xde, 0x5, 0x34, 0x1b, 0x2, 0xda, 0xdb, 0x7, 0x11, 0x2, 0x2,
    0xdb, 0xdd, 0x5, 0x34, 0x1b, 0x2, 0xdc, 0xda, 0x3, 0x2, 0x2, 0x2, 0xdd,
    0xe0, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdf,
    0x3, 0x2, 0x2, 0x2, 0xdf, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xde, 0x3,
    0x2, 0x2, 0x2, 0xe1, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe2, 0x3, 0x2,
    0x2, 0x2, 0xe2, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 0x7, 0xb, 0x2,
    0x2, 0xe4, 0x33, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe6, 0x7, 0x9, 0x2, 0x2,
    0xe6, 0xe7, 0x5, 0x26, 0x14, 0x2, 0xe7, 0x35, 0x3, 0x2, 0x2, 0x2, 0xe8,
    0xe9, 0x8, 0x1c, 0x1, 0x2, 0xe9, 0xec, 0x5, 0x38, 0x1d, 0x2, 0xea, 0xec,
    0x5, 0x3a, 0x1e, 0x2, 0xeb, 0xe8, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xea, 0x3,
    0x2, 0x2, 0x2, 0xec, 0xfe, 0x3, 0x2, 0x2, 0x2, 0xed, 0xee, 0xc, 0x7,
    0x2, 0x2, 0xee, 0xef, 0x9, 0x3, 0x2, 0x2, 0xef, 0xfd, 0x5, 0x36, 0x1c,
    0x8, 0xf0, 0xf1, 0xc, 0x6, 0x2, 0x2, 0xf1, 0xf2, 0x9, 0x4, 0x2, 0x2,
    0xf2, 0xfd, 0x5, 0x36, 0x1c, 0x7, 0xf3, 0xf4, 0xc, 0x5, 0x2, 0x2, 0xf4,
    0xf5, 0x9, 0x5, 0x2, 0x2, 0xf5, 0xfd, 0x5, 0x36, 0x1c, 0x6, 0xf6, 0xf7,
    0xc, 0x4, 0x2, 0x2, 0xf7, 0xf8, 0x7, 0x1a, 0x2, 0x2, 0xf8, 0xfd, 0x5,
    0x36, 0x1c, 0x5, 0xf9, 0xfa, 0xc, 0x3, 0x2, 0x2, 0xfa, 0xfb, 0x7, 0x19,
    0x2, 0x2, 0xfb, 0xfd, 0x5, 0x36, 0x1c, 0x4, 0xfc, 0xed, 0x3, 0x2, 0x2,
    0x2, 0xfc, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xfc, 0xf3, 0x3, 0x2, 0x2, 0x2,
    0xfc, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xfc, 0xf9, 0x3, 0x2, 0x2, 0x2, 0xfd,
    0x100, 0x3, 0x2, 0x2, 0x2, 0xfe, 0xfc, 0x3, 0x2, 0x2, 0x2, 0xfe, 0xff,
    0x3, 0x2, 0x2, 0x2, 0xff, 0x37, 0x3, 0x2, 0x2, 0x2, 0x100, 0xfe, 0x3,
    0x2, 0x2, 0x2, 0x101, 0x102, 0x8, 0x1d, 0x1, 0x2, 0x102, 0x103, 0x5,
    0x3c, 0x1f, 0x2, 0x103, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x104, 0x108, 0xc,
    0x3, 0x2, 0x2, 0x105, 0x106, 0x7, 0x14, 0x2, 0x2, 0x106, 0x109, 0x7,
    0x9, 0x2, 0x2, 0x107, 0x109, 0x5, 0x52, 0x2a, 0x2, 0x108, 0x105, 0x3,
    0x2, 0x2, 0x2, 0x108, 0x107, 0x3, 0x2, 0x2, 0x2, 0x109, 0x10b, 0x3,
    0x2, 0x2, 0x2, 0x10a, 0x104, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x10e, 0x3,
    0x2, 0x2, 0x2, 0x10c, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10c, 0x10d, 0x3,
    0x2, 0x2, 0x2, 0x10d, 0x39, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10c, 0x3, 0x2,
    0x2, 0x2, 0x10f, 0x114, 0x5, 0x38, 0x1d, 0x2, 0x110, 0x111, 0x5, 0x3e,
    0x20, 0x2, 0x111, 0x112, 0x5, 0x36, 0x1c, 0x2, 0x112, 0x114, 0x3, 0x2,
    0x2, 0x2, 0x113, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x113, 0x110, 0x3, 0x2,
    0x2, 0x2, 0x114, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x115, 0x11d, 0x5, 0x40,
    0x21, 0x2, 0x116, 0x11d, 0x5, 0x48, 0x25, 0x2, 0x117, 0x11d, 0x5, 0x54,
    0x2b, 0x2, 0x118, 0x119, 0x7, 0xa, 0x2, 0x2, 0x119, 0x11a, 0x5, 0x36,
    0x1c, 0x2, 0x11a, 0x11b, 0x7, 0xb, 0x2, 0x2, 0x11b, 0x11d, 0x3, 0x2,
    0x2, 0x2, 0x11c, 0x115, 0x3, 0x2, 0x2, 0x2, 0x11c, 0x116, 0x3, 0x2,
    0x2, 0x2, 0x11c, 0x117, 0x3, 0x2, 0x2, 0x2, 0x11c, 0x118, 0x3, 0x2,
    0x2, 0x2, 0x11d, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x11f, 0x9, 0x6, 0x2,
    0x2, 0x11f, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x120, 0x121, 0x5, 0x42, 0x22,
    0x2, 0x121, 0x41, 0x3, 0x2, 0x2, 0x2, 0x122, 0x127, 0x5, 0x44, 0x23,
    0x2, 0x123, 0x127, 0x5, 0x50, 0x29, 0x2, 0x124, 0x127, 0x5, 0x46, 0x24,
    0x2, 0x125, 0x127, 0x7, 0x32, 0x2, 0x2, 0x126, 0x122, 0x3, 0x2, 0x2,
    0x2, 0x126, 0x123, 0x3, 0x2, 0x2, 0x2, 0x126, 0x124, 0x3, 0x2, 0x2,
    0x2, 0x126, 0x125, 0x3, 0x2, 0x2, 0x2, 0x127, 0x43, 0x3, 0x2, 0x2, 0x2,
    0x128, 0x129, 0x9, 0x7, 0x2, 0x2, 0x129, 0x45, 0x3, 0x2, 0x2, 0x2, 0x12a,
    0x12b, 0x9, 0x8, 0x2, 0x2, 0x12b, 0x47, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x12f,
    0x7, 0x9, 0x2, 0x2, 0x12d, 0x12f, 0x5, 0x4a, 0x26, 0x2, 0x12e, 0x12c,
    0x3, 0x2, 0x2, 0x2, 0x12e, 0x12d, 0x3, 0x2, 0x2, 0x2, 0x12f, 0x49, 0x3,
    0x2, 0x2, 0x2, 0x130, 0x131, 0x7, 0x9, 0x2, 0x2, 0x131, 0x132, 0x7,
    0x14, 0x2, 0x2, 0x132, 0x133, 0x7, 0x9, 0x2, 0x2, 0x133, 0x4b, 0x3,
    0x2, 0x2, 0x2, 0x134, 0x135, 0x5, 0x2c, 0x17, 0x2, 0x135, 0x4d, 0x3,
    0x2, 0x2, 0x2, 0x136, 0x137, 0x5, 0x4, 0x3, 0x2, 0x137, 0x139, 0x5,
    0x26, 0x14, 0x2, 0x138, 0x13a, 0x5, 0x50, 0x29, 0x2, 0x139, 0x138, 0x3,
    0x2, 0x2, 0x2, 0x139, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x4f, 0x3, 0x2,
    0x2, 0x2, 0x13b, 0x13c, 0x7, 0x33, 0x2, 0x2, 0x13c, 0x51, 0x3, 0x2,
    0x2, 0x2, 0x13d, 0x14c, 0x7, 0xa, 0x2, 0x2, 0x13e, 0x145, 0x5, 0x6,
    0x4, 0x2, 0x13f, 0x142, 0x5, 0x26, 0x14, 0x2, 0x140, 0x141, 0x7, 0x11,
    0x2, 0x2, 0x141, 0x143, 0x5, 0x6, 0x4, 0x2, 0x142, 0x140, 0x3, 0x2,
    0x2, 0x2, 0x142, 0x143, 0x3, 0x2, 0x2, 0x2, 0x143, 0x145, 0x3, 0x2,
    0x2, 0x2, 0x144, 0x13e, 0x3, 0x2, 0x2, 0x2, 0x144, 0x13f, 0x3, 0x2,
    0x2, 0x2, 0x145, 0x147, 0x3, 0x2, 0x2, 0x2, 0x146, 0x148, 0x7, 0x18,
    0x2, 0x2, 0x147, 0x146, 0x3, 0x2, 0x2, 0x2, 0x147, 0x148, 0x3, 0x2,
    0x2, 0x2, 0x148, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x149, 0x14b, 0x7, 0x11,
    0x2, 0x2, 0x14a, 0x149, 0x3, 0x2, 0x2, 0x2, 0x14a, 0x14b, 0x3, 0x2,
    0x2, 0x2, 0x14b, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14c, 0x144, 0x3, 0x2,
    0x2, 0x2, 0x14c, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14e, 0x3, 0x2,
    0x2, 0x2, 0x14e, 0x14f, 0x7, 0xb, 0x2, 0x2, 0x14f, 0x53, 0x3, 0x2, 0x2,
    0x2, 0x150, 0x151, 0x5, 0x56, 0x2c, 0x2, 0x151, 0x152, 0x7, 0x14, 0x2,
    0x2, 0x152, 0x153, 0x7, 0x9, 0x2, 0x2, 0x153, 0x55, 0x3, 0x2, 0x2, 0x2,
    0x154, 0x15e, 0x5, 0x2c, 0x17, 0x2, 0x155, 0x159, 0x7, 0xa, 0x2, 0x2,
    0x156, 0x157, 0x7, 0x2b, 0x2, 0x2, 0x157, 0x15a, 0x5, 0x2c, 0x17, 0x2,
    0x158, 0x15a, 0x5, 0x56, 0x2c, 0x2, 0x159, 0x156, 0x3, 0x2, 0x2, 0x2,
    0x159, 0x158, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15b, 0x3, 0x2, 0x2, 0x2,
    0x15b, 0x15c, 0x7, 0xb, 0x2, 0x2, 0x15c, 0x15e, 0x3, 0x2, 0x2, 0x2,
    0x15d, 0x154, 0x3, 0x2, 0x2, 0x2, 0x15d, 0x155, 0x3, 0x2, 0x2, 0x2,
    0x15e, 0x57, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x164, 0x7, 0x12, 0x2, 0x2,
    0x160, 0x164, 0x7, 0x2, 0x2, 0x3, 0x161, 0x164, 0x6, 0x2d, 0x8, 0x2,
    0x162, 0x164, 0x6, 0x2d, 0x9, 0x2, 0x163, 0x15f, 0x3, 0x2, 0x2, 0x2,
    0x163, 0x160, 0x3, 0x2, 0x2, 0x2, 0x163, 0x161, 0x3, 0x2, 0x2, 0x2,
    0x163, 0x162, 0x3, 0x2, 0x2, 0x2, 0x164, 0x59, 0x3, 0x2, 0x2, 0x2, 0x26,
    0x5d, 0x64, 0x66, 0x70, 0x78, 0x8b, 0x97, 0x9b, 0xa4, 0xaa, 0xaf, 0xb8,
    0xc0, 0xc5, 0xd0, 0xd6, 0xde, 0xe1, 0xeb, 0xfc, 0xfe, 0x108, 0x10c,
    0x113, 0x11c, 0x126, 0x12e, 0x139, 0x142, 0x144, 0x147, 0x14a, 0x14c,
    0x159, 0x15d, 0x163,
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);

  for (size_t i = 0; i < count; i++) {
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LlamaLangParser::Initializer LlamaLangParser::_init;
