
// Generated from ../../ModuleRetriever.g4 by ANTLR 4.9.1


#include "ModuleRetrieverListener.h"

#include "ModuleRetrieverParser.h"


using namespace antlrcpp;
using namespace llang;
using namespace antlr4;

ModuleRetrieverParser::ModuleRetrieverParser(TokenStream *input) : Parser(input)
{
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ModuleRetrieverParser::~ModuleRetrieverParser()
{
  delete _interpreter;
}

std::string ModuleRetrieverParser::getGrammarFileName() const
{
  return "ModuleRetriever.g4";
}

const std::vector<std::string> &ModuleRetrieverParser::getRuleNames() const
{
  return _ruleNames;
}

dfa::Vocabulary &ModuleRetrieverParser::getVocabulary() const
{
  return _vocabulary;
}


//----------------- ValidSourceContext ------------------------------------------------------------------

ModuleRetrieverParser::ValidSourceContext::ValidSourceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState)
{
}

ModuleRetrieverParser::ModuleDirectiveContext *ModuleRetrieverParser::ValidSourceContext::moduleDirective()
{
  return getRuleContext<ModuleRetrieverParser::ModuleDirectiveContext>(0);
}

std::vector<ModuleRetrieverParser::AnyTokenContext *> ModuleRetrieverParser::ValidSourceContext::anyToken()
{
  return getRuleContexts<ModuleRetrieverParser::AnyTokenContext>();
}

ModuleRetrieverParser::AnyTokenContext *ModuleRetrieverParser::ValidSourceContext::anyToken(size_t i)
{
  return getRuleContext<ModuleRetrieverParser::AnyTokenContext>(i);
}

tree::TerminalNode *ModuleRetrieverParser::ValidSourceContext::EOF()
{
  return getToken(ModuleRetrieverParser::EOF, 0);
}

std::vector<ModuleRetrieverParser::MainDirectiveContext *> ModuleRetrieverParser::ValidSourceContext::mainDirective()
{
  return getRuleContexts<ModuleRetrieverParser::MainDirectiveContext>();
}

ModuleRetrieverParser::MainDirectiveContext *ModuleRetrieverParser::ValidSourceContext::mainDirective(size_t i)
{
  return getRuleContext<ModuleRetrieverParser::MainDirectiveContext>(i);
}


size_t ModuleRetrieverParser::ValidSourceContext::getRuleIndex() const
{
  return ModuleRetrieverParser::RuleValidSource;
}

void ModuleRetrieverParser::ValidSourceContext::enterRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->enterValidSource(this);
}

void ModuleRetrieverParser::ValidSourceContext::exitRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->exitValidSource(this);
}

ModuleRetrieverParser::ValidSourceContext *ModuleRetrieverParser::validSource()
{
  ValidSourceContext *_localctx = _tracker.createInstance<ValidSourceContext>(_ctx, getState());
  enterRule(_localctx, 0, ModuleRetrieverParser::RuleValidSource);
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
    setState(12);
    moduleDirective();
    setState(13);
    anyToken();
    setState(20);
    _errHandler->sync(this);
    _la = _input->LA(1);

    while (_la == ModuleRetrieverParser::HASH

           || _la == ModuleRetrieverParser::FUNC) {
      setState(14);
      mainDirective();
      setState(16);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(15);
        anyToken();
        break;
      }

      default:
        break;
      }

      setState(22);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

    setState(23);
    match(ModuleRetrieverParser::EOF);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ModuleDirectiveContext ------------------------------------------------------------------

ModuleRetrieverParser::ModuleDirectiveContext::ModuleDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode *ModuleRetrieverParser::ModuleDirectiveContext::HASH()
{
  return getToken(ModuleRetrieverParser::HASH, 0);
}

tree::TerminalNode *ModuleRetrieverParser::ModuleDirectiveContext::MODULE()
{
  return getToken(ModuleRetrieverParser::MODULE, 0);
}

tree::TerminalNode *ModuleRetrieverParser::ModuleDirectiveContext::IDENTIFIER()
{
  return getToken(ModuleRetrieverParser::IDENTIFIER, 0);
}


size_t ModuleRetrieverParser::ModuleDirectiveContext::getRuleIndex() const
{
  return ModuleRetrieverParser::RuleModuleDirective;
}

void ModuleRetrieverParser::ModuleDirectiveContext::enterRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->enterModuleDirective(this);
}

void ModuleRetrieverParser::ModuleDirectiveContext::exitRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->exitModuleDirective(this);
}

ModuleRetrieverParser::ModuleDirectiveContext *ModuleRetrieverParser::moduleDirective()
{
  ModuleDirectiveContext *_localctx = _tracker.createInstance<ModuleDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 2, ModuleRetrieverParser::RuleModuleDirective);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(25);
    match(ModuleRetrieverParser::HASH);
    setState(26);
    match(ModuleRetrieverParser::MODULE);
    setState(27);
    match(ModuleRetrieverParser::IDENTIFIER);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MainDirectiveContext ------------------------------------------------------------------

ModuleRetrieverParser::MainDirectiveContext::MainDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode *ModuleRetrieverParser::MainDirectiveContext::LPAREN()
{
  return getToken(ModuleRetrieverParser::LPAREN, 0);
}

tree::TerminalNode *ModuleRetrieverParser::MainDirectiveContext::RPAREN()
{
  return getToken(ModuleRetrieverParser::RPAREN, 0);
}

ModuleRetrieverParser::ParameterListContext *ModuleRetrieverParser::MainDirectiveContext::parameterList()
{
  return getRuleContext<ModuleRetrieverParser::ParameterListContext>(0);
}

tree::TerminalNode *ModuleRetrieverParser::MainDirectiveContext::HASH()
{
  return getToken(ModuleRetrieverParser::HASH, 0);
}

tree::TerminalNode *ModuleRetrieverParser::MainDirectiveContext::MAIN()
{
  return getToken(ModuleRetrieverParser::MAIN, 0);
}

tree::TerminalNode *ModuleRetrieverParser::MainDirectiveContext::FUNC()
{
  return getToken(ModuleRetrieverParser::FUNC, 0);
}

tree::TerminalNode *ModuleRetrieverParser::MainDirectiveContext::IDENTIFIER()
{
  return getToken(ModuleRetrieverParser::IDENTIFIER, 0);
}


size_t ModuleRetrieverParser::MainDirectiveContext::getRuleIndex() const
{
  return ModuleRetrieverParser::RuleMainDirective;
}

void ModuleRetrieverParser::MainDirectiveContext::enterRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->enterMainDirective(this);
}

void ModuleRetrieverParser::MainDirectiveContext::exitRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->exitMainDirective(this);
}

ModuleRetrieverParser::MainDirectiveContext *ModuleRetrieverParser::mainDirective()
{
  MainDirectiveContext *_localctx = _tracker.createInstance<MainDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 4, ModuleRetrieverParser::RuleMainDirective);
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
    setState(35);
    _errHandler->sync(this);

    switch (_input->LA(1)) {
    case ModuleRetrieverParser::HASH: {
      setState(29);
      match(ModuleRetrieverParser::HASH);
      setState(30);
      match(ModuleRetrieverParser::MAIN);
      setState(31);
      match(ModuleRetrieverParser::FUNC);
      setState(32);
      match(ModuleRetrieverParser::IDENTIFIER);
      break;
    }

    case ModuleRetrieverParser::FUNC: {
      setState(33);
      match(ModuleRetrieverParser::FUNC);
      setState(34);
      match(ModuleRetrieverParser::MAIN);
      break;
    }

    default:
      throw NoViableAltException(this);
    }

    setState(37);
    match(ModuleRetrieverParser::LPAREN);
    setState(39);
    _errHandler->sync(this);

    _la = _input->LA(1);

    if (_la == ModuleRetrieverParser::IDENTIFIER) {
      setState(38);
      parameterList();
    }

    setState(41);
    match(ModuleRetrieverParser::RPAREN);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterListContext ------------------------------------------------------------------

ModuleRetrieverParser::ParameterListContext::ParameterListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState)
{
}

std::vector<ModuleRetrieverParser::ParameterDeclContext *> ModuleRetrieverParser::ParameterListContext::parameterDecl()
{
  return getRuleContexts<ModuleRetrieverParser::ParameterDeclContext>();
}

ModuleRetrieverParser::ParameterDeclContext *ModuleRetrieverParser::ParameterListContext::parameterDecl(size_t i)
{
  return getRuleContext<ModuleRetrieverParser::ParameterDeclContext>(i);
}

std::vector<tree::TerminalNode *> ModuleRetrieverParser::ParameterListContext::COMMA()
{
  return getTokens(ModuleRetrieverParser::COMMA);
}

tree::TerminalNode *ModuleRetrieverParser::ParameterListContext::COMMA(size_t i)
{
  return getToken(ModuleRetrieverParser::COMMA, i);
}


size_t ModuleRetrieverParser::ParameterListContext::getRuleIndex() const
{
  return ModuleRetrieverParser::RuleParameterList;
}

void ModuleRetrieverParser::ParameterListContext::enterRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->enterParameterList(this);
}

void ModuleRetrieverParser::ParameterListContext::exitRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->exitParameterList(this);
}

ModuleRetrieverParser::ParameterListContext *ModuleRetrieverParser::parameterList()
{
  ParameterListContext *_localctx = _tracker.createInstance<ParameterListContext>(_ctx, getState());
  enterRule(_localctx, 6, ModuleRetrieverParser::RuleParameterList);
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
    setState(43);
    parameterDecl();
    setState(48);
    _errHandler->sync(this);
    _la = _input->LA(1);

    while (_la == ModuleRetrieverParser::COMMA) {
      setState(44);
      match(ModuleRetrieverParser::COMMA);
      setState(45);
      parameterDecl();
      setState(50);
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

//----------------- ParameterDeclContext ------------------------------------------------------------------

ModuleRetrieverParser::ParameterDeclContext::ParameterDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState)
{
}

std::vector<tree::TerminalNode *> ModuleRetrieverParser::ParameterDeclContext::IDENTIFIER()
{
  return getTokens(ModuleRetrieverParser::IDENTIFIER);
}

tree::TerminalNode *ModuleRetrieverParser::ParameterDeclContext::IDENTIFIER(size_t i)
{
  return getToken(ModuleRetrieverParser::IDENTIFIER, i);
}

tree::TerminalNode *ModuleRetrieverParser::ParameterDeclContext::LBRACKET()
{
  return getToken(ModuleRetrieverParser::LBRACKET, 0);
}

tree::TerminalNode *ModuleRetrieverParser::ParameterDeclContext::RBRACKET()
{
  return getToken(ModuleRetrieverParser::RBRACKET, 0);
}


size_t ModuleRetrieverParser::ParameterDeclContext::getRuleIndex() const
{
  return ModuleRetrieverParser::RuleParameterDecl;
}

void ModuleRetrieverParser::ParameterDeclContext::enterRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->enterParameterDecl(this);
}

void ModuleRetrieverParser::ParameterDeclContext::exitRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->exitParameterDecl(this);
}

ModuleRetrieverParser::ParameterDeclContext *ModuleRetrieverParser::parameterDecl()
{
  ParameterDeclContext *_localctx = _tracker.createInstance<ParameterDeclContext>(_ctx, getState());
  enterRule(_localctx, 8, ModuleRetrieverParser::RuleParameterDecl);

#if __cplusplus > 201703L
  auto onExit = finally([ =, this] {
#else
  auto onExit = finally([ = ] {
#endif
    exitRule();
  });

  try {
    enterOuterAlt(_localctx, 1);
    setState(51);
    match(ModuleRetrieverParser::IDENTIFIER);
    setState(55);
    _errHandler->sync(this);

    switch (_input->LA(1)) {
    case ModuleRetrieverParser::LBRACKET: {
      setState(52);
      match(ModuleRetrieverParser::LBRACKET);
      setState(53);
      match(ModuleRetrieverParser::RBRACKET);
      break;
    }

    case ModuleRetrieverParser::T__0: {
      setState(54);
      match(ModuleRetrieverParser::T__0);
      break;
    }

    case ModuleRetrieverParser::IDENTIFIER: {
      break;
    }

    default:
      break;
    }

    setState(57);
    match(ModuleRetrieverParser::IDENTIFIER);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnyTokenContext ------------------------------------------------------------------

ModuleRetrieverParser::AnyTokenContext::AnyTokenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState)
{
}


size_t ModuleRetrieverParser::AnyTokenContext::getRuleIndex() const
{
  return ModuleRetrieverParser::RuleAnyToken;
}

void ModuleRetrieverParser::AnyTokenContext::enterRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->enterAnyToken(this);
}

void ModuleRetrieverParser::AnyTokenContext::exitRule(tree::ParseTreeListener *listener)
{
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);

  if (parserListener != nullptr)
    parserListener->exitAnyToken(this);
}

ModuleRetrieverParser::AnyTokenContext *ModuleRetrieverParser::anyToken()
{
  AnyTokenContext *_localctx = _tracker.createInstance<AnyTokenContext>(_ctx, getState());
  enterRule(_localctx, 10, ModuleRetrieverParser::RuleAnyToken);

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
    setState(62);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);

    while (alt != 1 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1 + 1) {
        setState(59);
        matchWildcard();
      }

      setState(64);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> ModuleRetrieverParser::_decisionToDFA;
atn::PredictionContextCache ModuleRetrieverParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN ModuleRetrieverParser::_atn;
std::vector<uint16_t> ModuleRetrieverParser::_serializedATN;

std::vector<std::string> ModuleRetrieverParser::_ruleNames = {
  "validSource", "moduleDirective", "mainDirective", "parameterList", "parameterDecl",
  "anyToken"
};

std::vector<std::string> ModuleRetrieverParser::_literalNames = {
  "", "'*'", "'#'", "'module'", "'include'", "'main'", "'func'", "'('",
  "')'", "'['", "']'", "','"
};

std::vector<std::string> ModuleRetrieverParser::_symbolicNames = {
  "", "", "HASH", "MODULE", "INCLUDE", "MAIN", "FUNC", "LPAREN", "RPAREN",
  "LBRACKET", "RBRACKET", "COMMA", "IDENTIFIER", "WS", "UNICODE_WS", "COMMENT",
  "LINE_COMMENT", "OTHER"
};

dfa::Vocabulary ModuleRetrieverParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ModuleRetrieverParser::_tokenNames;

ModuleRetrieverParser::Initializer::Initializer()
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
    0x3, 0x13, 0x44, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9,
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x3,
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x13, 0xa, 0x2, 0x7, 0x2,
    0x15, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x18, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2,
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4,
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x26, 0xa, 0x4, 0x3, 0x4, 0x3,
    0x4, 0x5, 0x4, 0x2a, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5,
    0x3, 0x5, 0x7, 0x5, 0x31, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x34, 0xb, 0x5,
    0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x3a, 0xa, 0x6, 0x3,
    0x6, 0x3, 0x6, 0x3, 0x7, 0x7, 0x7, 0x3f, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7,
    0x42, 0xb, 0x7, 0x3, 0x7, 0x3, 0x40, 0x2, 0x8, 0x2, 0x4, 0x6, 0x8, 0xa,
    0xc, 0x2, 0x2, 0x2, 0x45, 0x2, 0xe, 0x3, 0x2, 0x2, 0x2, 0x4, 0x1b, 0x3,
    0x2, 0x2, 0x2, 0x6, 0x25, 0x3, 0x2, 0x2, 0x2, 0x8, 0x2d, 0x3, 0x2, 0x2,
    0x2, 0xa, 0x35, 0x3, 0x2, 0x2, 0x2, 0xc, 0x40, 0x3, 0x2, 0x2, 0x2, 0xe,
    0xf, 0x5, 0x4, 0x3, 0x2, 0xf, 0x16, 0x5, 0xc, 0x7, 0x2, 0x10, 0x12,
    0x5, 0x6, 0x4, 0x2, 0x11, 0x13, 0x5, 0xc, 0x7, 0x2, 0x12, 0x11, 0x3,
    0x2, 0x2, 0x2, 0x12, 0x13, 0x3, 0x2, 0x2, 0x2, 0x13, 0x15, 0x3, 0x2,
    0x2, 0x2, 0x14, 0x10, 0x3, 0x2, 0x2, 0x2, 0x15, 0x18, 0x3, 0x2, 0x2,
    0x2, 0x16, 0x14, 0x3, 0x2, 0x2, 0x2, 0x16, 0x17, 0x3, 0x2, 0x2, 0x2,
    0x17, 0x19, 0x3, 0x2, 0x2, 0x2, 0x18, 0x16, 0x3, 0x2, 0x2, 0x2, 0x19,
    0x1a, 0x7, 0x2, 0x2, 0x3, 0x1a, 0x3, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x1c,
    0x7, 0x4, 0x2, 0x2, 0x1c, 0x1d, 0x7, 0x5, 0x2, 0x2, 0x1d, 0x1e, 0x7,
    0xe, 0x2, 0x2, 0x1e, 0x5, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x20, 0x7, 0x4,
    0x2, 0x2, 0x20, 0x21, 0x7, 0x7, 0x2, 0x2, 0x21, 0x22, 0x7, 0x8, 0x2,
    0x2, 0x22, 0x26, 0x7, 0xe, 0x2, 0x2, 0x23, 0x24, 0x7, 0x8, 0x2, 0x2,
    0x24, 0x26, 0x7, 0x7, 0x2, 0x2, 0x25, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x25,
    0x23, 0x3, 0x2, 0x2, 0x2, 0x26, 0x27, 0x3, 0x2, 0x2, 0x2, 0x27, 0x29,
    0x7, 0x9, 0x2, 0x2, 0x28, 0x2a, 0x5, 0x8, 0x5, 0x2, 0x29, 0x28, 0x3,
    0x2, 0x2, 0x2, 0x29, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2b, 0x3, 0x2,
    0x2, 0x2, 0x2b, 0x2c, 0x7, 0xa, 0x2, 0x2, 0x2c, 0x7, 0x3, 0x2, 0x2,
    0x2, 0x2d, 0x32, 0x5, 0xa, 0x6, 0x2, 0x2e, 0x2f, 0x7, 0xd, 0x2, 0x2,
    0x2f, 0x31, 0x5, 0xa, 0x6, 0x2, 0x30, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x31,
    0x34, 0x3, 0x2, 0x2, 0x2, 0x32, 0x30, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33,
    0x3, 0x2, 0x2, 0x2, 0x33, 0x9, 0x3, 0x2, 0x2, 0x2, 0x34, 0x32, 0x3,
    0x2, 0x2, 0x2, 0x35, 0x39, 0x7, 0xe, 0x2, 0x2, 0x36, 0x37, 0x7, 0xb,
    0x2, 0x2, 0x37, 0x3a, 0x7, 0xc, 0x2, 0x2, 0x38, 0x3a, 0x7, 0x3, 0x2,
    0x2, 0x39, 0x36, 0x3, 0x2, 0x2, 0x2, 0x39, 0x38, 0x3, 0x2, 0x2, 0x2,
    0x39, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3b,
    0x3c, 0x7, 0xe, 0x2, 0x2, 0x3c, 0xb, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3f,
    0xb, 0x2, 0x2, 0x2, 0x3e, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x42, 0x3,
    0x2, 0x2, 0x2, 0x40, 0x41, 0x3, 0x2, 0x2, 0x2, 0x40, 0x3e, 0x3, 0x2,
    0x2, 0x2, 0x41, 0xd, 0x3, 0x2, 0x2, 0x2, 0x42, 0x40, 0x3, 0x2, 0x2,
    0x2, 0x9, 0x12, 0x16, 0x25, 0x29, 0x32, 0x39, 0x40,
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);

  for (size_t i = 0; i < count; i++) {
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

ModuleRetrieverParser::Initializer ModuleRetrieverParser::_init;
