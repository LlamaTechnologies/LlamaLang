
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1


#include "LlamaLangLexer.h"


using namespace antlr4;

using namespace llang;

LlamaLangLexer::LlamaLangLexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LlamaLangLexer::~LlamaLangLexer() {
  delete _interpreter;
}

std::string LlamaLangLexer::getGrammarFileName() const {
  return "LlamaLang.g4";
}

const std::vector<std::string>& LlamaLangLexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& LlamaLangLexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& LlamaLangLexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& LlamaLangLexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& LlamaLangLexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> LlamaLangLexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& LlamaLangLexer::getATN() const {
  return _atn;
}




// Static vars and initialization.
std::vector<dfa::DFA> LlamaLangLexer::_decisionToDFA;
atn::PredictionContextCache LlamaLangLexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LlamaLangLexer::_atn;
std::vector<uint16_t> LlamaLangLexer::_serializedATN;

std::vector<std::string> LlamaLangLexer::_ruleNames = {
  "T__0", "FUNC", "RETURN", "IDENTIFIER", "L_PAREN", "R_PAREN", "L_CURLY", 
  "R_CURLY", "L_BRACKET", "R_BRACKET", "ASSIGN", "COMMA", "SEMI", "COLON", 
  "DOT", "PLUS_PLUS", "MINUS_MINUS", "DECLARE_ASSIGN", "ELLIPSIS", "LOGICAL_OR", 
  "LOGICAL_AND", "EQUALS", "NOT_EQUALS", "LESS", "LESS_OR_EQUALS", "GREATER", 
  "GREATER_OR_EQUALS", "OR", "DIV", "MOD", "LSHIFT", "RSHIFT", "BIT_CLEAR", 
  "EXCLAMATION", "PLUS", "MINUS", "CARET", "STAR", "AMPERSAND", "ARROW", 
  "DECIMAL_LIT", "OCTAL_LIT", "HEX_LIT", "FLOAT_LIT", "RUNE_LIT", "INTERPRETED_STRING_LIT", 
  "WS", "COMMENT", "TERMINATOR", "LINE_COMMENT", "ESCAPED_VALUE", "DECIMALS", 
  "OCTAL_DIGIT", "HEX_DIGIT", "EXPONENT", "LETTER", "UNICODE_DIGIT", "UNICODE_LETTER"
};

std::vector<std::string> LlamaLangLexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> LlamaLangLexer::_modeNames = {
  "DEFAULT_MODE"
};

std::vector<std::string> LlamaLangLexer::_literalNames = {
  "", "'<-'", "'func'", "'return'", "", "'('", "')'", "'{'", "'}'", "'['", 
  "']'", "'='", "','", "';'", "':'", "'.'", "'++'", "'--'", "':='", "'...'", 
  "'||'", "'&&'", "'=='", "'!='", "'<'", "'<='", "'>'", "'>='", "'|'", "'/'", 
  "'%'", "'<<'", "'>>'", "'&^'", "'!'", "'+'", "'-'", "'^'", "'*'", "'&'", 
  "'->'"
};

std::vector<std::string> LlamaLangLexer::_symbolicNames = {
  "", "", "FUNC", "RETURN", "IDENTIFIER", "L_PAREN", "R_PAREN", "L_CURLY", 
  "R_CURLY", "L_BRACKET", "R_BRACKET", "ASSIGN", "COMMA", "SEMI", "COLON", 
  "DOT", "PLUS_PLUS", "MINUS_MINUS", "DECLARE_ASSIGN", "ELLIPSIS", "LOGICAL_OR", 
  "LOGICAL_AND", "EQUALS", "NOT_EQUALS", "LESS", "LESS_OR_EQUALS", "GREATER", 
  "GREATER_OR_EQUALS", "OR", "DIV", "MOD", "LSHIFT", "RSHIFT", "BIT_CLEAR", 
  "EXCLAMATION", "PLUS", "MINUS", "CARET", "STAR", "AMPERSAND", "ARROW", 
  "DECIMAL_LIT", "OCTAL_LIT", "HEX_LIT", "FLOAT_LIT", "RUNE_LIT", "INTERPRETED_STRING_LIT", 
  "WS", "COMMENT", "TERMINATOR", "LINE_COMMENT"
};

dfa::Vocabulary LlamaLangLexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LlamaLangLexer::_tokenNames;

LlamaLangLexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
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
    0x2, 0x34, 0x179, 0x8, 0x1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 
    0x4, 0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 
    0x9, 0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
    0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 
    0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 
    0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 
    0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 
    0x18, 0x9, 0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 
    0x9, 0x1b, 0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 
    0x1e, 0x4, 0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 
    0x4, 0x22, 0x9, 0x22, 0x4, 0x23, 0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 
    0x25, 0x9, 0x25, 0x4, 0x26, 0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 
    0x9, 0x28, 0x4, 0x29, 0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 
    0x2b, 0x4, 0x2c, 0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 
    0x4, 0x2f, 0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 
    0x32, 0x9, 0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 
    0x9, 0x35, 0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x4, 0x38, 0x9, 
    0x38, 0x4, 0x39, 0x9, 0x39, 0x4, 0x3a, 0x9, 0x3a, 0x4, 0x3b, 0x9, 0x3b, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x8a, 0xa, 0x5, 0xc, 
    0x5, 0xe, 0x5, 0x8d, 0xb, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 
    0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x3, 
    0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 
    0x25, 0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 0x28, 
    0x3, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x7, 
    0x2a, 0xe8, 0xa, 0x2a, 0xc, 0x2a, 0xe, 0x2a, 0xeb, 0xb, 0x2a, 0x3, 0x2b, 
    0x3, 0x2b, 0x7, 0x2b, 0xef, 0xa, 0x2b, 0xc, 0x2b, 0xe, 0x2b, 0xf2, 0xb, 
    0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x6, 0x2c, 0xf7, 0xa, 0x2c, 0xd, 
    0x2c, 0xe, 0x2c, 0xf8, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0xfe, 
    0xa, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x101, 0xa, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 
    0x104, 0xa, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x109, 
    0xa, 0x2d, 0x5, 0x2d, 0x10b, 0xa, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 
    0x5, 0x2e, 0x110, 0xa, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 
    0x3, 0x2f, 0x7, 0x2f, 0x117, 0xa, 0x2f, 0xc, 0x2f, 0xe, 0x2f, 0x11a, 
    0xb, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x30, 0x6, 0x30, 0x11f, 0xa, 0x30, 
    0xd, 0x30, 0xe, 0x30, 0x120, 0x3, 0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 
    0x3, 0x31, 0x3, 0x31, 0x7, 0x31, 0x129, 0xa, 0x31, 0xc, 0x31, 0xe, 0x31, 
    0x12c, 0xb, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
    0x3, 0x32, 0x6, 0x32, 0x134, 0xa, 0x32, 0xd, 0x32, 0xe, 0x32, 0x135, 
    0x3, 0x32, 0x3, 0x32, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x7, 
    0x33, 0x13e, 0xa, 0x33, 0xc, 0x33, 0xe, 0x33, 0x141, 0xb, 0x33, 0x3, 
    0x33, 0x3, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 
    0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 
    0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 
    0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 
    0x34, 0x3, 0x34, 0x5, 0x34, 0x15f, 0xa, 0x34, 0x3, 0x35, 0x6, 0x35, 
    0x162, 0xa, 0x35, 0xd, 0x35, 0xe, 0x35, 0x163, 0x3, 0x36, 0x3, 0x36, 
    0x3, 0x37, 0x3, 0x37, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x16c, 0xa, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x39, 0x3, 0x39, 0x5, 0x39, 0x172, 0xa, 0x39, 
    0x3, 0x3a, 0x5, 0x3a, 0x175, 0xa, 0x3a, 0x3, 0x3b, 0x5, 0x3b, 0x178, 
    0xa, 0x3b, 0x3, 0x12a, 0x2, 0x3c, 0x3, 0x3, 0x5, 0x4, 0x7, 0x5, 0x9, 
    0x6, 0xb, 0x7, 0xd, 0x8, 0xf, 0x9, 0x11, 0xa, 0x13, 0xb, 0x15, 0xc, 
    0x17, 0xd, 0x19, 0xe, 0x1b, 0xf, 0x1d, 0x10, 0x1f, 0x11, 0x21, 0x12, 
    0x23, 0x13, 0x25, 0x14, 0x27, 0x15, 0x29, 0x16, 0x2b, 0x17, 0x2d, 0x18, 
    0x2f, 0x19, 0x31, 0x1a, 0x33, 0x1b, 0x35, 0x1c, 0x37, 0x1d, 0x39, 0x1e, 
    0x3b, 0x1f, 0x3d, 0x20, 0x3f, 0x21, 0x41, 0x22, 0x43, 0x23, 0x45, 0x24, 
    0x47, 0x25, 0x49, 0x26, 0x4b, 0x27, 0x4d, 0x28, 0x4f, 0x29, 0x51, 0x2a, 
    0x53, 0x2b, 0x55, 0x2c, 0x57, 0x2d, 0x59, 0x2e, 0x5b, 0x2f, 0x5d, 0x30, 
    0x5f, 0x31, 0x61, 0x32, 0x63, 0x33, 0x65, 0x34, 0x67, 0x2, 0x69, 0x2, 
    0x6b, 0x2, 0x6d, 0x2, 0x6f, 0x2, 0x71, 0x2, 0x73, 0x2, 0x75, 0x2, 0x3, 
    0x2, 0x10, 0x3, 0x2, 0x33, 0x3b, 0x3, 0x2, 0x32, 0x3b, 0x4, 0x2, 0x5a, 
    0x5a, 0x7a, 0x7a, 0x4, 0x2, 0xc, 0xc, 0x5e, 0x5e, 0x4, 0x2, 0x24, 0x24, 
    0x5e, 0x5e, 0x4, 0x2, 0xb, 0xb, 0x22, 0x22, 0x4, 0x2, 0xc, 0xc, 0xf, 
    0xf, 0xb, 0x2, 0x24, 0x24, 0x29, 0x29, 0x5e, 0x5e, 0x63, 0x64, 0x68, 
    0x68, 0x70, 0x70, 0x74, 0x74, 0x76, 0x76, 0x78, 0x78, 0x3, 0x2, 0x32, 
    0x39, 0x5, 0x2, 0x32, 0x3b, 0x43, 0x48, 0x63, 0x68, 0x4, 0x2, 0x47, 
    0x47, 0x67, 0x67, 0x4, 0x2, 0x2d, 0x2d, 0x2f, 0x2f, 0x16, 0x2, 0x32, 
    0x3b, 0x662, 0x66b, 0x6f2, 0x6fb, 0x968, 0x971, 0x9e8, 0x9f1, 0xa68, 
    0xa71, 0xae8, 0xaf1, 0xb68, 0xb71, 0xbe9, 0xbf1, 0xc68, 0xc71, 0xce8, 
    0xcf1, 0xd68, 0xd71, 0xe52, 0xe5b, 0xed2, 0xedb, 0xf22, 0xf2b, 0x1042, 
    0x104b, 0x136b, 0x1373, 0x17e2, 0x17eb, 0x1812, 0x181b, 0xff12, 0xff1b, 
    0x104, 0x2, 0x43, 0x5c, 0x63, 0x7c, 0xac, 0xac, 0xb7, 0xb7, 0xbc, 0xbc, 
    0xc2, 0xd8, 0xda, 0xf8, 0xfa, 0x221, 0x224, 0x235, 0x252, 0x2af, 0x2b2, 
    0x2ba, 0x2bd, 0x2c3, 0x2d2, 0x2d3, 0x2e2, 0x2e6, 0x2f0, 0x2f0, 0x37c, 
    0x37c, 0x388, 0x388, 0x38a, 0x38c, 0x38e, 0x38e, 0x390, 0x3a3, 0x3a5, 
    0x3d0, 0x3d2, 0x3d9, 0x3dc, 0x3f5, 0x402, 0x483, 0x48e, 0x4c6, 0x4c9, 
    0x4ca, 0x4cd, 0x4ce, 0x4d2, 0x4f7, 0x4fa, 0x4fb, 0x533, 0x558, 0x55b, 
    0x55b, 0x563, 0x589, 0x5d2, 0x5ec, 0x5f2, 0x5f4, 0x623, 0x63c, 0x642, 
    0x64c, 0x673, 0x6d5, 0x6d7, 0x6d7, 0x6e7, 0x6e8, 0x6fc, 0x6fe, 0x712, 
    0x712, 0x714, 0x72e, 0x782, 0x7a7, 0x907, 0x93b, 0x93f, 0x93f, 0x952, 
    0x952, 0x95a, 0x963, 0x987, 0x98e, 0x991, 0x992, 0x995, 0x9aa, 0x9ac, 
    0x9b2, 0x9b4, 0x9b4, 0x9b8, 0x9bb, 0x9de, 0x9df, 0x9e1, 0x9e3, 0x9f2, 
    0x9f3, 0xa07, 0xa0c, 0xa11, 0xa12, 0xa15, 0xa2a, 0xa2c, 0xa32, 0xa34, 
    0xa35, 0xa37, 0xa38, 0xa3a, 0xa3b, 0xa5b, 0xa5e, 0xa60, 0xa60, 0xa74, 
    0xa76, 0xa87, 0xa8d, 0xa8f, 0xa8f, 0xa91, 0xa93, 0xa95, 0xaaa, 0xaac, 
    0xab2, 0xab4, 0xab5, 0xab7, 0xabb, 0xabf, 0xabf, 0xad2, 0xad2, 0xae2, 
    0xae2, 0xb07, 0xb0e, 0xb11, 0xb12, 0xb15, 0xb2a, 0xb2c, 0xb32, 0xb34, 
    0xb35, 0xb38, 0xb3b, 0xb3f, 0xb3f, 0xb5e, 0xb5f, 0xb61, 0xb63, 0xb87, 
    0xb8c, 0xb90, 0xb92, 0xb94, 0xb97, 0xb9b, 0xb9c, 0xb9e, 0xb9e, 0xba0, 
    0xba1, 0xba5, 0xba6, 0xbaa, 0xbac, 0xbb0, 0xbb7, 0xbb9, 0xbbb, 0xc07, 
    0xc0e, 0xc10, 0xc12, 0xc14, 0xc2a, 0xc2c, 0xc35, 0xc37, 0xc3b, 0xc62, 
    0xc63, 0xc87, 0xc8e, 0xc90, 0xc92, 0xc94, 0xcaa, 0xcac, 0xcb5, 0xcb7, 
    0xcbb, 0xce0, 0xce0, 0xce2, 0xce3, 0xd07, 0xd0e, 0xd10, 0xd12, 0xd14, 
    0xd2a, 0xd2c, 0xd3b, 0xd62, 0xd63, 0xd87, 0xd98, 0xd9c, 0xdb3, 0xdb5, 
    0xdbd, 0xdbf, 0xdbf, 0xdc2, 0xdc8, 0xe03, 0xe32, 0xe34, 0xe35, 0xe42, 
    0xe48, 0xe83, 0xe84, 0xe86, 0xe86, 0xe89, 0xe8a, 0xe8c, 0xe8c, 0xe8f, 
    0xe8f, 0xe96, 0xe99, 0xe9b, 0xea1, 0xea3, 0xea5, 0xea7, 0xea7, 0xea9, 
    0xea9, 0xeac, 0xead, 0xeaf, 0xeb2, 0xeb4, 0xeb5, 0xebf, 0xec6, 0xec8, 
    0xec8, 0xede, 0xedf, 0xf02, 0xf02, 0xf42, 0xf6c, 0xf8a, 0xf8d, 0x1002, 
    0x1023, 0x1025, 0x1029, 0x102b, 0x102c, 0x1052, 0x1057, 0x10a2, 0x10c7, 
    0x10d2, 0x10f8, 0x1102, 0x115b, 0x1161, 0x11a4, 0x11aa, 0x11fb, 0x1202, 
    0x1208, 0x120a, 0x1248, 0x124a, 0x124a, 0x124c, 0x124f, 0x1252, 0x1258, 
    0x125a, 0x125a, 0x125c, 0x125f, 0x1262, 0x1288, 0x128a, 0x128a, 0x128c, 
    0x128f, 0x1292, 0x12b0, 0x12b2, 0x12b2, 0x12b4, 0x12b7, 0x12ba, 0x12c0, 
    0x12c2, 0x12c2, 0x12c4, 0x12c7, 0x12ca, 0x12d0, 0x12d2, 0x12d8, 0x12da, 
    0x12f0, 0x12f2, 0x1310, 0x1312, 0x1312, 0x1314, 0x1317, 0x131a, 0x1320, 
    0x1322, 0x1348, 0x134a, 0x135c, 0x13a2, 0x13f6, 0x1403, 0x1678, 0x1683, 
    0x169c, 0x16a2, 0x16ec, 0x1782, 0x17b5, 0x1822, 0x1879, 0x1882, 0x18aa, 
    0x1e02, 0x1e9d, 0x1ea2, 0x1efb, 0x1f02, 0x1f17, 0x1f1a, 0x1f1f, 0x1f22, 
    0x1f47, 0x1f4a, 0x1f4f, 0x1f52, 0x1f59, 0x1f5b, 0x1f5b, 0x1f5d, 0x1f5d, 
    0x1f5f, 0x1f5f, 0x1f61, 0x1f7f, 0x1f82, 0x1fb6, 0x1fb8, 0x1fbe, 0x1fc0, 
    0x1fc0, 0x1fc4, 0x1fc6, 0x1fc8, 0x1fce, 0x1fd2, 0x1fd5, 0x1fd8, 0x1fdd, 
    0x1fe2, 0x1fee, 0x1ff4, 0x1ff6, 0x1ff8, 0x1ffe, 0x2081, 0x2081, 0x2104, 
    0x2104, 0x2109, 0x2109, 0x210c, 0x2115, 0x2117, 0x2117, 0x211b, 0x211f, 
    0x2126, 0x2126, 0x2128, 0x2128, 0x212a, 0x212a, 0x212c, 0x212f, 0x2131, 
    0x2133, 0x2135, 0x213b, 0x2162, 0x2185, 0x3007, 0x3009, 0x3023, 0x302b, 
    0x3033, 0x3037, 0x303a, 0x303c, 0x3043, 0x3096, 0x309f, 0x30a0, 0x30a3, 
    0x30fc, 0x30fe, 0x3100, 0x3107, 0x312e, 0x3133, 0x3190, 0x31a2, 0x31b9, 
    0x3402, 0x3402, 0x4db7, 0x4db7, 0x4e02, 0x4e02, 0x9fa7, 0x9fa7, 0xa002, 
    0xa48e, 0xac02, 0xac02, 0xd7a5, 0xd7a5, 0xf902, 0xfa2f, 0xfb02, 0xfb08, 
    0xfb15, 0xfb19, 0xfb1f, 0xfb1f, 0xfb21, 0xfb2a, 0xfb2c, 0xfb38, 0xfb3a, 
    0xfb3e, 0xfb40, 0xfb40, 0xfb42, 0xfb43, 0xfb45, 0xfb46, 0xfb48, 0xfbb3, 
    0xfbd5, 0xfd3f, 0xfd52, 0xfd91, 0xfd94, 0xfdc9, 0xfdf2, 0xfdfd, 0xfe72, 
    0xfe74, 0xfe76, 0xfe76, 0xfe78, 0xfefe, 0xff23, 0xff3c, 0xff43, 0xff5c, 
    0xff68, 0xffc0, 0xffc4, 0xffc9, 0xffcc, 0xffd1, 0xffd4, 0xffd9, 0xffdc, 
    0xffde, 0x2, 0x188, 0x2, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2, 0x5, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x9, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0xb, 0x3, 0x2, 0x2, 0x2, 0x2, 0xd, 0x3, 0x2, 0x2, 0x2, 0x2, 0xf, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x11, 0x3, 0x2, 0x2, 0x2, 0x2, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x15, 0x3, 0x2, 0x2, 0x2, 0x2, 0x17, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x19, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1d, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x2, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x23, 0x3, 0x2, 0x2, 0x2, 0x2, 0x25, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x27, 0x3, 0x2, 0x2, 0x2, 0x2, 0x29, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2b, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2f, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x31, 0x3, 0x2, 0x2, 0x2, 0x2, 0x33, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x35, 0x3, 0x2, 0x2, 0x2, 0x2, 0x37, 0x3, 0x2, 0x2, 0x2, 0x2, 0x39, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x2, 0x3d, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x2, 0x41, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x43, 0x3, 0x2, 0x2, 0x2, 0x2, 0x45, 0x3, 0x2, 0x2, 0x2, 0x2, 0x47, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x49, 0x3, 0x2, 0x2, 0x2, 0x2, 0x4b, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x2, 0x4f, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x51, 0x3, 0x2, 0x2, 0x2, 0x2, 0x53, 0x3, 0x2, 0x2, 0x2, 0x2, 0x55, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x57, 0x3, 0x2, 0x2, 0x2, 0x2, 0x59, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x2, 0x5d, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x2, 0x61, 0x3, 0x2, 0x2, 0x2, 0x2, 0x63, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x65, 0x3, 0x2, 0x2, 0x2, 0x3, 0x77, 0x3, 0x2, 
    0x2, 0x2, 0x5, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7, 0x7f, 0x3, 0x2, 0x2, 0x2, 
    0x9, 0x86, 0x3, 0x2, 0x2, 0x2, 0xb, 0x8e, 0x3, 0x2, 0x2, 0x2, 0xd, 0x90, 
    0x3, 0x2, 0x2, 0x2, 0xf, 0x92, 0x3, 0x2, 0x2, 0x2, 0x11, 0x94, 0x3, 
    0x2, 0x2, 0x2, 0x13, 0x96, 0x3, 0x2, 0x2, 0x2, 0x15, 0x98, 0x3, 0x2, 
    0x2, 0x2, 0x17, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x19, 0x9c, 0x3, 0x2, 0x2, 
    0x2, 0x1b, 0x9e, 0x3, 0x2, 0x2, 0x2, 0x1d, 0xa0, 0x3, 0x2, 0x2, 0x2, 
    0x1f, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x21, 0xa4, 0x3, 0x2, 0x2, 0x2, 0x23, 
    0xa7, 0x3, 0x2, 0x2, 0x2, 0x25, 0xaa, 0x3, 0x2, 0x2, 0x2, 0x27, 0xad, 
    0x3, 0x2, 0x2, 0x2, 0x29, 0xb1, 0x3, 0x2, 0x2, 0x2, 0x2b, 0xb4, 0x3, 
    0x2, 0x2, 0x2, 0x2d, 0xb7, 0x3, 0x2, 0x2, 0x2, 0x2f, 0xba, 0x3, 0x2, 
    0x2, 0x2, 0x31, 0xbd, 0x3, 0x2, 0x2, 0x2, 0x33, 0xbf, 0x3, 0x2, 0x2, 
    0x2, 0x35, 0xc2, 0x3, 0x2, 0x2, 0x2, 0x37, 0xc4, 0x3, 0x2, 0x2, 0x2, 
    0x39, 0xc7, 0x3, 0x2, 0x2, 0x2, 0x3b, 0xc9, 0x3, 0x2, 0x2, 0x2, 0x3d, 
    0xcb, 0x3, 0x2, 0x2, 0x2, 0x3f, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x41, 0xd0, 
    0x3, 0x2, 0x2, 0x2, 0x43, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x45, 0xd6, 0x3, 
    0x2, 0x2, 0x2, 0x47, 0xd8, 0x3, 0x2, 0x2, 0x2, 0x49, 0xda, 0x3, 0x2, 
    0x2, 0x2, 0x4b, 0xdc, 0x3, 0x2, 0x2, 0x2, 0x4d, 0xde, 0x3, 0x2, 0x2, 
    0x2, 0x4f, 0xe0, 0x3, 0x2, 0x2, 0x2, 0x51, 0xe2, 0x3, 0x2, 0x2, 0x2, 
    0x53, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x55, 0xec, 0x3, 0x2, 0x2, 0x2, 0x57, 
    0xf3, 0x3, 0x2, 0x2, 0x2, 0x59, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x10c, 
    0x3, 0x2, 0x2, 0x2, 0x5d, 0x113, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x11e, 0x3, 
    0x2, 0x2, 0x2, 0x61, 0x124, 0x3, 0x2, 0x2, 0x2, 0x63, 0x133, 0x3, 0x2, 
    0x2, 0x2, 0x65, 0x139, 0x3, 0x2, 0x2, 0x2, 0x67, 0x144, 0x3, 0x2, 0x2, 
    0x2, 0x69, 0x161, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x165, 0x3, 0x2, 0x2, 0x2, 
    0x6d, 0x167, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x169, 0x3, 0x2, 0x2, 0x2, 0x71, 
    0x171, 0x3, 0x2, 0x2, 0x2, 0x73, 0x174, 0x3, 0x2, 0x2, 0x2, 0x75, 0x177, 
    0x3, 0x2, 0x2, 0x2, 0x77, 0x78, 0x7, 0x3e, 0x2, 0x2, 0x78, 0x79, 0x7, 
    0x2f, 0x2, 0x2, 0x79, 0x4, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7b, 0x7, 0x68, 
    0x2, 0x2, 0x7b, 0x7c, 0x7, 0x77, 0x2, 0x2, 0x7c, 0x7d, 0x7, 0x70, 0x2, 
    0x2, 0x7d, 0x7e, 0x7, 0x65, 0x2, 0x2, 0x7e, 0x6, 0x3, 0x2, 0x2, 0x2, 
    0x7f, 0x80, 0x7, 0x74, 0x2, 0x2, 0x80, 0x81, 0x7, 0x67, 0x2, 0x2, 0x81, 
    0x82, 0x7, 0x76, 0x2, 0x2, 0x82, 0x83, 0x7, 0x77, 0x2, 0x2, 0x83, 0x84, 
    0x7, 0x74, 0x2, 0x2, 0x84, 0x85, 0x7, 0x70, 0x2, 0x2, 0x85, 0x8, 0x3, 
    0x2, 0x2, 0x2, 0x86, 0x8b, 0x5, 0x71, 0x39, 0x2, 0x87, 0x8a, 0x5, 0x71, 
    0x39, 0x2, 0x88, 0x8a, 0x5, 0x73, 0x3a, 0x2, 0x89, 0x87, 0x3, 0x2, 0x2, 
    0x2, 0x89, 0x88, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8d, 0x3, 0x2, 0x2, 0x2, 
    0x8b, 0x89, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8c, 
    0xa, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8f, 
    0x7, 0x2a, 0x2, 0x2, 0x8f, 0xc, 0x3, 0x2, 0x2, 0x2, 0x90, 0x91, 0x7, 
    0x2b, 0x2, 0x2, 0x91, 0xe, 0x3, 0x2, 0x2, 0x2, 0x92, 0x93, 0x7, 0x7d, 
    0x2, 0x2, 0x93, 0x10, 0x3, 0x2, 0x2, 0x2, 0x94, 0x95, 0x7, 0x7f, 0x2, 
    0x2, 0x95, 0x12, 0x3, 0x2, 0x2, 0x2, 0x96, 0x97, 0x7, 0x5d, 0x2, 0x2, 
    0x97, 0x14, 0x3, 0x2, 0x2, 0x2, 0x98, 0x99, 0x7, 0x5f, 0x2, 0x2, 0x99, 
    0x16, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9b, 0x7, 0x3f, 0x2, 0x2, 0x9b, 0x18, 
    0x3, 0x2, 0x2, 0x2, 0x9c, 0x9d, 0x7, 0x2e, 0x2, 0x2, 0x9d, 0x1a, 0x3, 
    0x2, 0x2, 0x2, 0x9e, 0x9f, 0x7, 0x3d, 0x2, 0x2, 0x9f, 0x1c, 0x3, 0x2, 
    0x2, 0x2, 0xa0, 0xa1, 0x7, 0x3c, 0x2, 0x2, 0xa1, 0x1e, 0x3, 0x2, 0x2, 
    0x2, 0xa2, 0xa3, 0x7, 0x30, 0x2, 0x2, 0xa3, 0x20, 0x3, 0x2, 0x2, 0x2, 
    0xa4, 0xa5, 0x7, 0x2d, 0x2, 0x2, 0xa5, 0xa6, 0x7, 0x2d, 0x2, 0x2, 0xa6, 
    0x22, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x7, 0x2f, 0x2, 0x2, 0xa8, 0xa9, 
    0x7, 0x2f, 0x2, 0x2, 0xa9, 0x24, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xab, 0x7, 
    0x3c, 0x2, 0x2, 0xab, 0xac, 0x7, 0x3f, 0x2, 0x2, 0xac, 0x26, 0x3, 0x2, 
    0x2, 0x2, 0xad, 0xae, 0x7, 0x30, 0x2, 0x2, 0xae, 0xaf, 0x7, 0x30, 0x2, 
    0x2, 0xaf, 0xb0, 0x7, 0x30, 0x2, 0x2, 0xb0, 0x28, 0x3, 0x2, 0x2, 0x2, 
    0xb1, 0xb2, 0x7, 0x7e, 0x2, 0x2, 0xb2, 0xb3, 0x7, 0x7e, 0x2, 0x2, 0xb3, 
    0x2a, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb5, 0x7, 0x28, 0x2, 0x2, 0xb5, 0xb6, 
    0x7, 0x28, 0x2, 0x2, 0xb6, 0x2c, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb8, 0x7, 
    0x3f, 0x2, 0x2, 0xb8, 0xb9, 0x7, 0x3f, 0x2, 0x2, 0xb9, 0x2e, 0x3, 0x2, 
    0x2, 0x2, 0xba, 0xbb, 0x7, 0x23, 0x2, 0x2, 0xbb, 0xbc, 0x7, 0x3f, 0x2, 
    0x2, 0xbc, 0x30, 0x3, 0x2, 0x2, 0x2, 0xbd, 0xbe, 0x7, 0x3e, 0x2, 0x2, 
    0xbe, 0x32, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc0, 0x7, 0x3e, 0x2, 0x2, 0xc0, 
    0xc1, 0x7, 0x3f, 0x2, 0x2, 0xc1, 0x34, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc3, 
    0x7, 0x40, 0x2, 0x2, 0xc3, 0x36, 0x3, 0x2, 0x2, 0x2, 0xc4, 0xc5, 0x7, 
    0x40, 0x2, 0x2, 0xc5, 0xc6, 0x7, 0x3f, 0x2, 0x2, 0xc6, 0x38, 0x3, 0x2, 
    0x2, 0x2, 0xc7, 0xc8, 0x7, 0x7e, 0x2, 0x2, 0xc8, 0x3a, 0x3, 0x2, 0x2, 
    0x2, 0xc9, 0xca, 0x7, 0x31, 0x2, 0x2, 0xca, 0x3c, 0x3, 0x2, 0x2, 0x2, 
    0xcb, 0xcc, 0x7, 0x27, 0x2, 0x2, 0xcc, 0x3e, 0x3, 0x2, 0x2, 0x2, 0xcd, 
    0xce, 0x7, 0x3e, 0x2, 0x2, 0xce, 0xcf, 0x7, 0x3e, 0x2, 0x2, 0xcf, 0x40, 
    0x3, 0x2, 0x2, 0x2, 0xd0, 0xd1, 0x7, 0x40, 0x2, 0x2, 0xd1, 0xd2, 0x7, 
    0x40, 0x2, 0x2, 0xd2, 0x42, 0x3, 0x2, 0x2, 0x2, 0xd3, 0xd4, 0x7, 0x28, 
    0x2, 0x2, 0xd4, 0xd5, 0x7, 0x60, 0x2, 0x2, 0xd5, 0x44, 0x3, 0x2, 0x2, 
    0x2, 0xd6, 0xd7, 0x7, 0x23, 0x2, 0x2, 0xd7, 0x46, 0x3, 0x2, 0x2, 0x2, 
    0xd8, 0xd9, 0x7, 0x2d, 0x2, 0x2, 0xd9, 0x48, 0x3, 0x2, 0x2, 0x2, 0xda, 
    0xdb, 0x7, 0x2f, 0x2, 0x2, 0xdb, 0x4a, 0x3, 0x2, 0x2, 0x2, 0xdc, 0xdd, 
    0x7, 0x60, 0x2, 0x2, 0xdd, 0x4c, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdf, 0x7, 
    0x2c, 0x2, 0x2, 0xdf, 0x4e, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x7, 0x28, 
    0x2, 0x2, 0xe1, 0x50, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe3, 0x7, 0x2f, 0x2, 
    0x2, 0xe3, 0xe4, 0x7, 0x40, 0x2, 0x2, 0xe4, 0x52, 0x3, 0x2, 0x2, 0x2, 
    0xe5, 0xe9, 0x9, 0x2, 0x2, 0x2, 0xe6, 0xe8, 0x9, 0x3, 0x2, 0x2, 0xe7, 
    0xe6, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xeb, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xe7, 
    0x3, 0x2, 0x2, 0x2, 0xe9, 0xea, 0x3, 0x2, 0x2, 0x2, 0xea, 0x54, 0x3, 
    0x2, 0x2, 0x2, 0xeb, 0xe9, 0x3, 0x2, 0x2, 0x2, 0xec, 0xf0, 0x7, 0x32, 
    0x2, 0x2, 0xed, 0xef, 0x5, 0x6b, 0x36, 0x2, 0xee, 0xed, 0x3, 0x2, 0x2, 
    0x2, 0xef, 0xf2, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xee, 0x3, 0x2, 0x2, 0x2, 
    0xf0, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xf1, 0x56, 0x3, 0x2, 0x2, 0x2, 0xf2, 
    0xf0, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf4, 0x7, 0x32, 0x2, 0x2, 0xf4, 0xf6, 
    0x9, 0x4, 0x2, 0x2, 0xf5, 0xf7, 0x5, 0x6d, 0x37, 0x2, 0xf6, 0xf5, 0x3, 
    0x2, 0x2, 0x2, 0xf7, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xf6, 0x3, 0x2, 
    0x2, 0x2, 0xf8, 0xf9, 0x3, 0x2, 0x2, 0x2, 0xf9, 0x58, 0x3, 0x2, 0x2, 
    0x2, 0xfa, 0x103, 0x5, 0x69, 0x35, 0x2, 0xfb, 0xfd, 0x7, 0x30, 0x2, 
    0x2, 0xfc, 0xfe, 0x5, 0x69, 0x35, 0x2, 0xfd, 0xfc, 0x3, 0x2, 0x2, 0x2, 
    0xfd, 0xfe, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x100, 0x3, 0x2, 0x2, 0x2, 0xff, 
    0x101, 0x5, 0x6f, 0x38, 0x2, 0x100, 0xff, 0x3, 0x2, 0x2, 0x2, 0x100, 
    0x101, 0x3, 0x2, 0x2, 0x2, 0x101, 0x104, 0x3, 0x2, 0x2, 0x2, 0x102, 
    0x104, 0x5, 0x6f, 0x38, 0x2, 0x103, 0xfb, 0x3, 0x2, 0x2, 0x2, 0x103, 
    0x102, 0x3, 0x2, 0x2, 0x2, 0x104, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x105, 
    0x106, 0x7, 0x30, 0x2, 0x2, 0x106, 0x108, 0x5, 0x69, 0x35, 0x2, 0x107, 
    0x109, 0x5, 0x6f, 0x38, 0x2, 0x108, 0x107, 0x3, 0x2, 0x2, 0x2, 0x108, 
    0x109, 0x3, 0x2, 0x2, 0x2, 0x109, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x10a, 
    0xfa, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x105, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x5a, 
    0x3, 0x2, 0x2, 0x2, 0x10c, 0x10f, 0x7, 0x29, 0x2, 0x2, 0x10d, 0x110, 
    0xa, 0x5, 0x2, 0x2, 0x10e, 0x110, 0x5, 0x67, 0x34, 0x2, 0x10f, 0x10d, 
    0x3, 0x2, 0x2, 0x2, 0x10f, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x110, 0x111, 
    0x3, 0x2, 0x2, 0x2, 0x111, 0x112, 0x7, 0x29, 0x2, 0x2, 0x112, 0x5c, 
    0x3, 0x2, 0x2, 0x2, 0x113, 0x118, 0x7, 0x24, 0x2, 0x2, 0x114, 0x117, 
    0xa, 0x6, 0x2, 0x2, 0x115, 0x117, 0x5, 0x67, 0x34, 0x2, 0x116, 0x114, 
    0x3, 0x2, 0x2, 0x2, 0x116, 0x115, 0x3, 0x2, 0x2, 0x2, 0x117, 0x11a, 
    0x3, 0x2, 0x2, 0x2, 0x118, 0x116, 0x3, 0x2, 0x2, 0x2, 0x118, 0x119, 
    0x3, 0x2, 0x2, 0x2, 0x119, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x118, 
    0x3, 0x2, 0x2, 0x2, 0x11b, 0x11c, 0x7, 0x24, 0x2, 0x2, 0x11c, 0x5e, 
    0x3, 0x2, 0x2, 0x2, 0x11d, 0x11f, 0x9, 0x7, 0x2, 0x2, 0x11e, 0x11d, 
    0x3, 0x2, 0x2, 0x2, 0x11f, 0x120, 0x3, 0x2, 0x2, 0x2, 0x120, 0x11e, 
    0x3, 0x2, 0x2, 0x2, 0x120, 0x121, 0x3, 0x2, 0x2, 0x2, 0x121, 0x122, 
    0x3, 0x2, 0x2, 0x2, 0x122, 0x123, 0x8, 0x30, 0x2, 0x2, 0x123, 0x60, 
    0x3, 0x2, 0x2, 0x2, 0x124, 0x125, 0x7, 0x31, 0x2, 0x2, 0x125, 0x126, 
    0x7, 0x2c, 0x2, 0x2, 0x126, 0x12a, 0x3, 0x2, 0x2, 0x2, 0x127, 0x129, 
    0xb, 0x2, 0x2, 0x2, 0x128, 0x127, 0x3, 0x2, 0x2, 0x2, 0x129, 0x12c, 
    0x3, 0x2, 0x2, 0x2, 0x12a, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x12a, 0x128, 
    0x3, 0x2, 0x2, 0x2, 0x12b, 0x12d, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x12a, 
    0x3, 0x2, 0x2, 0x2, 0x12d, 0x12e, 0x7, 0x2c, 0x2, 0x2, 0x12e, 0x12f, 
    0x7, 0x31, 0x2, 0x2, 0x12f, 0x130, 0x3, 0x2, 0x2, 0x2, 0x130, 0x131, 
    0x8, 0x31, 0x2, 0x2, 0x131, 0x62, 0x3, 0x2, 0x2, 0x2, 0x132, 0x134, 
    0x9, 0x8, 0x2, 0x2, 0x133, 0x132, 0x3, 0x2, 0x2, 0x2, 0x134, 0x135, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x133, 0x3, 0x2, 0x2, 0x2, 0x135, 0x136, 
    0x3, 0x2, 0x2, 0x2, 0x136, 0x137, 0x3, 0x2, 0x2, 0x2, 0x137, 0x138, 
    0x8, 0x32, 0x2, 0x2, 0x138, 0x64, 0x3, 0x2, 0x2, 0x2, 0x139, 0x13a, 
    0x7, 0x31, 0x2, 0x2, 0x13a, 0x13b, 0x7, 0x31, 0x2, 0x2, 0x13b, 0x13f, 
    0x3, 0x2, 0x2, 0x2, 0x13c, 0x13e, 0xa, 0x8, 0x2, 0x2, 0x13d, 0x13c, 
    0x3, 0x2, 0x2, 0x2, 0x13e, 0x141, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x13d, 
    0x3, 0x2, 0x2, 0x2, 0x13f, 0x140, 0x3, 0x2, 0x2, 0x2, 0x140, 0x142, 
    0x3, 0x2, 0x2, 0x2, 0x141, 0x13f, 0x3, 0x2, 0x2, 0x2, 0x142, 0x143, 
    0x8, 0x33, 0x2, 0x2, 0x143, 0x66, 0x3, 0x2, 0x2, 0x2, 0x144, 0x15e, 
    0x7, 0x5e, 0x2, 0x2, 0x145, 0x146, 0x7, 0x77, 0x2, 0x2, 0x146, 0x147, 
    0x5, 0x6d, 0x37, 0x2, 0x147, 0x148, 0x5, 0x6d, 0x37, 0x2, 0x148, 0x149, 
    0x5, 0x6d, 0x37, 0x2, 0x149, 0x14a, 0x5, 0x6d, 0x37, 0x2, 0x14a, 0x15f, 
    0x3, 0x2, 0x2, 0x2, 0x14b, 0x14c, 0x7, 0x57, 0x2, 0x2, 0x14c, 0x14d, 
    0x5, 0x6d, 0x37, 0x2, 0x14d, 0x14e, 0x5, 0x6d, 0x37, 0x2, 0x14e, 0x14f, 
    0x5, 0x6d, 0x37, 0x2, 0x14f, 0x150, 0x5, 0x6d, 0x37, 0x2, 0x150, 0x151, 
    0x5, 0x6d, 0x37, 0x2, 0x151, 0x152, 0x5, 0x6d, 0x37, 0x2, 0x152, 0x153, 
    0x5, 0x6d, 0x37, 0x2, 0x153, 0x154, 0x5, 0x6d, 0x37, 0x2, 0x154, 0x15f, 
    0x3, 0x2, 0x2, 0x2, 0x155, 0x15f, 0x9, 0x9, 0x2, 0x2, 0x156, 0x157, 
    0x5, 0x6b, 0x36, 0x2, 0x157, 0x158, 0x5, 0x6b, 0x36, 0x2, 0x158, 0x159, 
    0x5, 0x6b, 0x36, 0x2, 0x159, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15b, 
    0x7, 0x7a, 0x2, 0x2, 0x15b, 0x15c, 0x5, 0x6d, 0x37, 0x2, 0x15c, 0x15d, 
    0x5, 0x6d, 0x37, 0x2, 0x15d, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x145, 
    0x3, 0x2, 0x2, 0x2, 0x15e, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x155, 
    0x3, 0x2, 0x2, 0x2, 0x15e, 0x156, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x15a, 
    0x3, 0x2, 0x2, 0x2, 0x15f, 0x68, 0x3, 0x2, 0x2, 0x2, 0x160, 0x162, 0x9, 
    0x3, 0x2, 0x2, 0x161, 0x160, 0x3, 0x2, 0x2, 0x2, 0x162, 0x163, 0x3, 
    0x2, 0x2, 0x2, 0x163, 0x161, 0x3, 0x2, 0x2, 0x2, 0x163, 0x164, 0x3, 
    0x2, 0x2, 0x2, 0x164, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x165, 0x166, 0x9, 0xa, 
    0x2, 0x2, 0x166, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x167, 0x168, 0x9, 0xb, 0x2, 
    0x2, 0x168, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x169, 0x16b, 0x9, 0xc, 0x2, 0x2, 
    0x16a, 0x16c, 0x9, 0xd, 0x2, 0x2, 0x16b, 0x16a, 0x3, 0x2, 0x2, 0x2, 
    0x16b, 0x16c, 0x3, 0x2, 0x2, 0x2, 0x16c, 0x16d, 0x3, 0x2, 0x2, 0x2, 
    0x16d, 0x16e, 0x5, 0x69, 0x35, 0x2, 0x16e, 0x70, 0x3, 0x2, 0x2, 0x2, 
    0x16f, 0x172, 0x5, 0x75, 0x3b, 0x2, 0x170, 0x172, 0x7, 0x61, 0x2, 0x2, 
    0x171, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x171, 0x170, 0x3, 0x2, 0x2, 0x2, 
    0x172, 0x72, 0x3, 0x2, 0x2, 0x2, 0x173, 0x175, 0x9, 0xe, 0x2, 0x2, 0x174, 
    0x173, 0x3, 0x2, 0x2, 0x2, 0x175, 0x74, 0x3, 0x2, 0x2, 0x2, 0x176, 0x178, 
    0x9, 0xf, 0x2, 0x2, 0x177, 0x176, 0x3, 0x2, 0x2, 0x2, 0x178, 0x76, 0x3, 
    0x2, 0x2, 0x2, 0x1a, 0x2, 0x89, 0x8b, 0xe9, 0xf0, 0xf8, 0xfd, 0x100, 
    0x103, 0x108, 0x10a, 0x10f, 0x116, 0x118, 0x120, 0x12a, 0x135, 0x13f, 
    0x15e, 0x163, 0x16b, 0x171, 0x174, 0x177, 0x3, 0x2, 0x3, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LlamaLangLexer::Initializer LlamaLangLexer::_init;
