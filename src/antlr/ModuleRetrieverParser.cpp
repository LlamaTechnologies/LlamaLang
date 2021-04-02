
// Generated from ../../ModuleRetriever.g4 by ANTLR 4.9.1


#include "ModuleRetrieverListener.h"

#include "ModuleRetrieverParser.h"


using namespace antlrcpp;
using namespace llang;
using namespace antlr4;

ModuleRetrieverParser::ModuleRetrieverParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ModuleRetrieverParser::~ModuleRetrieverParser() {
  delete _interpreter;
}

std::string ModuleRetrieverParser::getGrammarFileName() const {
  return "ModuleRetriever.g4";
}

const std::vector<std::string>& ModuleRetrieverParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& ModuleRetrieverParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ValidSourceContext ------------------------------------------------------------------

ModuleRetrieverParser::ValidSourceContext::ValidSourceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModuleRetrieverParser::ModuleDirectiveContext* ModuleRetrieverParser::ValidSourceContext::moduleDirective() {
  return getRuleContext<ModuleRetrieverParser::ModuleDirectiveContext>(0);
}

ModuleRetrieverParser::AnyTokenContext* ModuleRetrieverParser::ValidSourceContext::anyToken() {
  return getRuleContext<ModuleRetrieverParser::AnyTokenContext>(0);
}

tree::TerminalNode* ModuleRetrieverParser::ValidSourceContext::EOF() {
  return getToken(ModuleRetrieverParser::EOF, 0);
}

std::vector<ModuleRetrieverParser::IncludeDirectiveContext *> ModuleRetrieverParser::ValidSourceContext::includeDirective() {
  return getRuleContexts<ModuleRetrieverParser::IncludeDirectiveContext>();
}

ModuleRetrieverParser::IncludeDirectiveContext* ModuleRetrieverParser::ValidSourceContext::includeDirective(size_t i) {
  return getRuleContext<ModuleRetrieverParser::IncludeDirectiveContext>(i);
}


size_t ModuleRetrieverParser::ValidSourceContext::getRuleIndex() const {
  return ModuleRetrieverParser::RuleValidSource;
}

void ModuleRetrieverParser::ValidSourceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValidSource(this);
}

void ModuleRetrieverParser::ValidSourceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValidSource(this);
}

ModuleRetrieverParser::ValidSourceContext* ModuleRetrieverParser::validSource() {
  ValidSourceContext *_localctx = _tracker.createInstance<ValidSourceContext>(_ctx, getState());
  enterRule(_localctx, 0, ModuleRetrieverParser::RuleValidSource);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(8);
    moduleDirective();
    setState(12);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(9);
        includeDirective(); 
      }
      setState(14);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    }
    setState(15);
    anyToken();
    setState(16);
    match(ModuleRetrieverParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ModuleDirectiveContext ------------------------------------------------------------------

ModuleRetrieverParser::ModuleDirectiveContext::ModuleDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModuleRetrieverParser::ModuleDirectiveContext::HASH() {
  return getToken(ModuleRetrieverParser::HASH, 0);
}

tree::TerminalNode* ModuleRetrieverParser::ModuleDirectiveContext::MODULE() {
  return getToken(ModuleRetrieverParser::MODULE, 0);
}

tree::TerminalNode* ModuleRetrieverParser::ModuleDirectiveContext::IDENTIFIER() {
  return getToken(ModuleRetrieverParser::IDENTIFIER, 0);
}


size_t ModuleRetrieverParser::ModuleDirectiveContext::getRuleIndex() const {
  return ModuleRetrieverParser::RuleModuleDirective;
}

void ModuleRetrieverParser::ModuleDirectiveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterModuleDirective(this);
}

void ModuleRetrieverParser::ModuleDirectiveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitModuleDirective(this);
}

ModuleRetrieverParser::ModuleDirectiveContext* ModuleRetrieverParser::moduleDirective() {
  ModuleDirectiveContext *_localctx = _tracker.createInstance<ModuleDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 2, ModuleRetrieverParser::RuleModuleDirective);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(18);
    match(ModuleRetrieverParser::HASH);
    setState(19);
    match(ModuleRetrieverParser::MODULE);
    setState(20);
    match(ModuleRetrieverParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncludeDirectiveContext ------------------------------------------------------------------

ModuleRetrieverParser::IncludeDirectiveContext::IncludeDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModuleRetrieverParser::IncludeDirectiveContext::HASH() {
  return getToken(ModuleRetrieverParser::HASH, 0);
}

tree::TerminalNode* ModuleRetrieverParser::IncludeDirectiveContext::INCLUDE() {
  return getToken(ModuleRetrieverParser::INCLUDE, 0);
}

tree::TerminalNode* ModuleRetrieverParser::IncludeDirectiveContext::IDENTIFIER() {
  return getToken(ModuleRetrieverParser::IDENTIFIER, 0);
}


size_t ModuleRetrieverParser::IncludeDirectiveContext::getRuleIndex() const {
  return ModuleRetrieverParser::RuleIncludeDirective;
}

void ModuleRetrieverParser::IncludeDirectiveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIncludeDirective(this);
}

void ModuleRetrieverParser::IncludeDirectiveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIncludeDirective(this);
}

ModuleRetrieverParser::IncludeDirectiveContext* ModuleRetrieverParser::includeDirective() {
  IncludeDirectiveContext *_localctx = _tracker.createInstance<IncludeDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 4, ModuleRetrieverParser::RuleIncludeDirective);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(22);
    match(ModuleRetrieverParser::HASH);
    setState(23);
    match(ModuleRetrieverParser::INCLUDE);
    setState(24);
    match(ModuleRetrieverParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnyTokenContext ------------------------------------------------------------------

ModuleRetrieverParser::AnyTokenContext::AnyTokenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModuleRetrieverParser::AnyTokenContext::getRuleIndex() const {
  return ModuleRetrieverParser::RuleAnyToken;
}

void ModuleRetrieverParser::AnyTokenContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnyToken(this);
}

void ModuleRetrieverParser::AnyTokenContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ModuleRetrieverListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnyToken(this);
}

ModuleRetrieverParser::AnyTokenContext* ModuleRetrieverParser::anyToken() {
  AnyTokenContext *_localctx = _tracker.createInstance<AnyTokenContext>(_ctx, getState());
  enterRule(_localctx, 6, ModuleRetrieverParser::RuleAnyToken);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(29);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModuleRetrieverParser::HASH)
      | (1ULL << ModuleRetrieverParser::MODULE)
      | (1ULL << ModuleRetrieverParser::INCLUDE)
      | (1ULL << ModuleRetrieverParser::IDENTIFIER)
      | (1ULL << ModuleRetrieverParser::WS)
      | (1ULL << ModuleRetrieverParser::UNICODE_WS)
      | (1ULL << ModuleRetrieverParser::COMMENT)
      | (1ULL << ModuleRetrieverParser::LINE_COMMENT))) != 0)) {
      setState(26);
      matchWildcard();
      setState(31);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
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
  "validSource", "moduleDirective", "includeDirective", "anyToken"
};

std::vector<std::string> ModuleRetrieverParser::_literalNames = {
  "", "'#'", "'module'", "'include'"
};

std::vector<std::string> ModuleRetrieverParser::_symbolicNames = {
  "", "HASH", "MODULE", "INCLUDE", "IDENTIFIER", "WS", "UNICODE_WS", "COMMENT", 
  "LINE_COMMENT"
};

dfa::Vocabulary ModuleRetrieverParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ModuleRetrieverParser::_tokenNames;

ModuleRetrieverParser::Initializer::Initializer() {
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
    0x3, 0xa, 0x23, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0xd, 0xa, 0x2, 
    0xc, 0x2, 0xe, 0x2, 0x10, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x5, 0x7, 0x5, 0x1e, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x21, 0xb, 
    0x5, 0x3, 0x5, 0x2, 0x2, 0x6, 0x2, 0x4, 0x6, 0x8, 0x2, 0x2, 0x2, 0x20, 
    0x2, 0xa, 0x3, 0x2, 0x2, 0x2, 0x4, 0x14, 0x3, 0x2, 0x2, 0x2, 0x6, 0x18, 
    0x3, 0x2, 0x2, 0x2, 0x8, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xa, 0xe, 0x5, 0x4, 
    0x3, 0x2, 0xb, 0xd, 0x5, 0x6, 0x4, 0x2, 0xc, 0xb, 0x3, 0x2, 0x2, 0x2, 
    0xd, 0x10, 0x3, 0x2, 0x2, 0x2, 0xe, 0xc, 0x3, 0x2, 0x2, 0x2, 0xe, 0xf, 
    0x3, 0x2, 0x2, 0x2, 0xf, 0x11, 0x3, 0x2, 0x2, 0x2, 0x10, 0xe, 0x3, 0x2, 
    0x2, 0x2, 0x11, 0x12, 0x5, 0x8, 0x5, 0x2, 0x12, 0x13, 0x7, 0x2, 0x2, 
    0x3, 0x13, 0x3, 0x3, 0x2, 0x2, 0x2, 0x14, 0x15, 0x7, 0x3, 0x2, 0x2, 
    0x15, 0x16, 0x7, 0x4, 0x2, 0x2, 0x16, 0x17, 0x7, 0x6, 0x2, 0x2, 0x17, 
    0x5, 0x3, 0x2, 0x2, 0x2, 0x18, 0x19, 0x7, 0x3, 0x2, 0x2, 0x19, 0x1a, 
    0x7, 0x5, 0x2, 0x2, 0x1a, 0x1b, 0x7, 0x6, 0x2, 0x2, 0x1b, 0x7, 0x3, 
    0x2, 0x2, 0x2, 0x1c, 0x1e, 0xb, 0x2, 0x2, 0x2, 0x1d, 0x1c, 0x3, 0x2, 
    0x2, 0x2, 0x1e, 0x21, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x1d, 0x3, 0x2, 0x2, 
    0x2, 0x1f, 0x20, 0x3, 0x2, 0x2, 0x2, 0x20, 0x9, 0x3, 0x2, 0x2, 0x2, 
    0x21, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x4, 0xe, 0x1f, 
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
