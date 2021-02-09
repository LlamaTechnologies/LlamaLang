// Generated from c:\Users\narbl\Documents\GitHub\LlamaLang-Cpp\LlamaLang.g4 by ANTLR 4.8
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class LlamaLangLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, FUNC=2, RETURN=3, IDENTIFIER=4, L_PAREN=5, R_PAREN=6, L_CURLY=7, 
		R_CURLY=8, L_BRACKET=9, R_BRACKET=10, ASSIGN=11, COMMA=12, SEMI=13, COLON=14, 
		DOT=15, PLUS_PLUS=16, MINUS_MINUS=17, DECLARE_ASSIGN=18, ELLIPSIS=19, 
		LOGICAL_OR=20, LOGICAL_AND=21, EQUALS=22, NOT_EQUALS=23, LESS=24, LESS_OR_EQUALS=25, 
		GREATER=26, GREATER_OR_EQUALS=27, OR=28, DIV=29, MOD=30, LSHIFT=31, RSHIFT=32, 
		BIT_CLEAR=33, EXCLAMATION=34, PLUS=35, MINUS=36, CARET=37, STAR=38, AMPERSAND=39, 
		ARROW=40, DECIMAL_LIT=41, OCTAL_LIT=42, HEX_LIT=43, FLOAT_LIT=44, RUNE_LIT=45, 
		INTERPRETED_STRING_LIT=46, WS=47, COMMENT=48, TERMINATOR=49, LINE_COMMENT=50;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
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
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'<-'", "'func'", "'return'", null, "'('", "')'", "'{'", "'}'", 
			"'['", "']'", "'='", "','", "';'", "':'", "'.'", "'++'", "'--'", "':='", 
			"'...'", "'||'", "'&&'", "'=='", "'!='", "'<'", "'<='", "'>'", "'>='", 
			"'|'", "'/'", "'%'", "'<<'", "'>>'", "'&^'", "'!'", "'+'", "'-'", "'^'", 
			"'*'", "'&'", "'->'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, "FUNC", "RETURN", "IDENTIFIER", "L_PAREN", "R_PAREN", "L_CURLY", 
			"R_CURLY", "L_BRACKET", "R_BRACKET", "ASSIGN", "COMMA", "SEMI", "COLON", 
			"DOT", "PLUS_PLUS", "MINUS_MINUS", "DECLARE_ASSIGN", "ELLIPSIS", "LOGICAL_OR", 
			"LOGICAL_AND", "EQUALS", "NOT_EQUALS", "LESS", "LESS_OR_EQUALS", "GREATER", 
			"GREATER_OR_EQUALS", "OR", "DIV", "MOD", "LSHIFT", "RSHIFT", "BIT_CLEAR", 
			"EXCLAMATION", "PLUS", "MINUS", "CARET", "STAR", "AMPERSAND", "ARROW", 
			"DECIMAL_LIT", "OCTAL_LIT", "HEX_LIT", "FLOAT_LIT", "RUNE_LIT", "INTERPRETED_STRING_LIT", 
			"WS", "COMMENT", "TERMINATOR", "LINE_COMMENT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public LlamaLangLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "LlamaLang.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\64\u0179\b\1\4\2"+
		"\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4"+
		"\13\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22"+
		"\t\22\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31"+
		"\t\31\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t"+
		" \4!\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t"+
		"+\4,\t,\4-\t-\4.\t.\4/\t/\4\60\t\60\4\61\t\61\4\62\t\62\4\63\t\63\4\64"+
		"\t\64\4\65\t\65\4\66\t\66\4\67\t\67\48\t8\49\t9\4:\t:\4;\t;\3\2\3\2\3"+
		"\2\3\3\3\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\7\5\u008a"+
		"\n\5\f\5\16\5\u008d\13\5\3\6\3\6\3\7\3\7\3\b\3\b\3\t\3\t\3\n\3\n\3\13"+
		"\3\13\3\f\3\f\3\r\3\r\3\16\3\16\3\17\3\17\3\20\3\20\3\21\3\21\3\21\3\22"+
		"\3\22\3\22\3\23\3\23\3\23\3\24\3\24\3\24\3\24\3\25\3\25\3\25\3\26\3\26"+
		"\3\26\3\27\3\27\3\27\3\30\3\30\3\30\3\31\3\31\3\32\3\32\3\32\3\33\3\33"+
		"\3\34\3\34\3\34\3\35\3\35\3\36\3\36\3\37\3\37\3 \3 \3 \3!\3!\3!\3\"\3"+
		"\"\3\"\3#\3#\3$\3$\3%\3%\3&\3&\3\'\3\'\3(\3(\3)\3)\3)\3*\3*\7*\u00e8\n"+
		"*\f*\16*\u00eb\13*\3+\3+\7+\u00ef\n+\f+\16+\u00f2\13+\3,\3,\3,\6,\u00f7"+
		"\n,\r,\16,\u00f8\3-\3-\3-\5-\u00fe\n-\3-\5-\u0101\n-\3-\5-\u0104\n-\3"+
		"-\3-\3-\5-\u0109\n-\5-\u010b\n-\3.\3.\3.\5.\u0110\n.\3.\3.\3/\3/\3/\7"+
		"/\u0117\n/\f/\16/\u011a\13/\3/\3/\3\60\6\60\u011f\n\60\r\60\16\60\u0120"+
		"\3\60\3\60\3\61\3\61\3\61\3\61\7\61\u0129\n\61\f\61\16\61\u012c\13\61"+
		"\3\61\3\61\3\61\3\61\3\61\3\62\6\62\u0134\n\62\r\62\16\62\u0135\3\62\3"+
		"\62\3\63\3\63\3\63\3\63\7\63\u013e\n\63\f\63\16\63\u0141\13\63\3\63\3"+
		"\63\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3"+
		"\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\3\64\5\64\u015f"+
		"\n\64\3\65\6\65\u0162\n\65\r\65\16\65\u0163\3\66\3\66\3\67\3\67\38\38"+
		"\58\u016c\n8\38\38\39\39\59\u0172\n9\3:\5:\u0175\n:\3;\5;\u0178\n;\3\u012a"+
		"\2<\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33\17\35"+
		"\20\37\21!\22#\23%\24\'\25)\26+\27-\30/\31\61\32\63\33\65\34\67\359\36"+
		";\37= ?!A\"C#E$G%I&K\'M(O)Q*S+U,W-Y.[/]\60_\61a\62c\63e\64g\2i\2k\2m\2"+
		"o\2q\2s\2u\2\3\2\20\3\2\63;\3\2\62;\4\2ZZzz\4\2\f\f^^\4\2$$^^\4\2\13\13"+
		"\"\"\4\2\f\f\17\17\13\2$$))^^cdhhppttvvxx\3\2\629\5\2\62;CHch\4\2GGgg"+
		"\4\2--//\26\2\62;\u0662\u066b\u06f2\u06fb\u0968\u0971\u09e8\u09f1\u0a68"+
		"\u0a71\u0ae8\u0af1\u0b68\u0b71\u0be9\u0bf1\u0c68\u0c71\u0ce8\u0cf1\u0d68"+
		"\u0d71\u0e52\u0e5b\u0ed2\u0edb\u0f22\u0f2b\u1042\u104b\u136b\u1373\u17e2"+
		"\u17eb\u1812\u181b\uff12\uff1b\u0104\2C\\c|\u00ac\u00ac\u00b7\u00b7\u00bc"+
		"\u00bc\u00c2\u00d8\u00da\u00f8\u00fa\u0221\u0224\u0235\u0252\u02af\u02b2"+
		"\u02ba\u02bd\u02c3\u02d2\u02d3\u02e2\u02e6\u02f0\u02f0\u037c\u037c\u0388"+
		"\u0388\u038a\u038c\u038e\u038e\u0390\u03a3\u03a5\u03d0\u03d2\u03d9\u03dc"+
		"\u03f5\u0402\u0483\u048e\u04c6\u04c9\u04ca\u04cd\u04ce\u04d2\u04f7\u04fa"+
		"\u04fb\u0533\u0558\u055b\u055b\u0563\u0589\u05d2\u05ec\u05f2\u05f4\u0623"+
		"\u063c\u0642\u064c\u0673\u06d5\u06d7\u06d7\u06e7\u06e8\u06fc\u06fe\u0712"+
		"\u0712\u0714\u072e\u0782\u07a7\u0907\u093b\u093f\u093f\u0952\u0952\u095a"+
		"\u0963\u0987\u098e\u0991\u0992\u0995\u09aa\u09ac\u09b2\u09b4\u09b4\u09b8"+
		"\u09bb\u09de\u09df\u09e1\u09e3\u09f2\u09f3\u0a07\u0a0c\u0a11\u0a12\u0a15"+
		"\u0a2a\u0a2c\u0a32\u0a34\u0a35\u0a37\u0a38\u0a3a\u0a3b\u0a5b\u0a5e\u0a60"+
		"\u0a60\u0a74\u0a76\u0a87\u0a8d\u0a8f\u0a8f\u0a91\u0a93\u0a95\u0aaa\u0aac"+
		"\u0ab2\u0ab4\u0ab5\u0ab7\u0abb\u0abf\u0abf\u0ad2\u0ad2\u0ae2\u0ae2\u0b07"+
		"\u0b0e\u0b11\u0b12\u0b15\u0b2a\u0b2c\u0b32\u0b34\u0b35\u0b38\u0b3b\u0b3f"+
		"\u0b3f\u0b5e\u0b5f\u0b61\u0b63\u0b87\u0b8c\u0b90\u0b92\u0b94\u0b97\u0b9b"+
		"\u0b9c\u0b9e\u0b9e\u0ba0\u0ba1\u0ba5\u0ba6\u0baa\u0bac\u0bb0\u0bb7\u0bb9"+
		"\u0bbb\u0c07\u0c0e\u0c10\u0c12\u0c14\u0c2a\u0c2c\u0c35\u0c37\u0c3b\u0c62"+
		"\u0c63\u0c87\u0c8e\u0c90\u0c92\u0c94\u0caa\u0cac\u0cb5\u0cb7\u0cbb\u0ce0"+
		"\u0ce0\u0ce2\u0ce3\u0d07\u0d0e\u0d10\u0d12\u0d14\u0d2a\u0d2c\u0d3b\u0d62"+
		"\u0d63\u0d87\u0d98\u0d9c\u0db3\u0db5\u0dbd\u0dbf\u0dbf\u0dc2\u0dc8\u0e03"+
		"\u0e32\u0e34\u0e35\u0e42\u0e48\u0e83\u0e84\u0e86\u0e86\u0e89\u0e8a\u0e8c"+
		"\u0e8c\u0e8f\u0e8f\u0e96\u0e99\u0e9b\u0ea1\u0ea3\u0ea5\u0ea7\u0ea7\u0ea9"+
		"\u0ea9\u0eac\u0ead\u0eaf\u0eb2\u0eb4\u0eb5\u0ebf\u0ec6\u0ec8\u0ec8\u0ede"+
		"\u0edf\u0f02\u0f02\u0f42\u0f6c\u0f8a\u0f8d\u1002\u1023\u1025\u1029\u102b"+
		"\u102c\u1052\u1057\u10a2\u10c7\u10d2\u10f8\u1102\u115b\u1161\u11a4\u11aa"+
		"\u11fb\u1202\u1208\u120a\u1248\u124a\u124a\u124c\u124f\u1252\u1258\u125a"+
		"\u125a\u125c\u125f\u1262\u1288\u128a\u128a\u128c\u128f\u1292\u12b0\u12b2"+
		"\u12b2\u12b4\u12b7\u12ba\u12c0\u12c2\u12c2\u12c4\u12c7\u12ca\u12d0\u12d2"+
		"\u12d8\u12da\u12f0\u12f2\u1310\u1312\u1312\u1314\u1317\u131a\u1320\u1322"+
		"\u1348\u134a\u135c\u13a2\u13f6\u1403\u1678\u1683\u169c\u16a2\u16ec\u1782"+
		"\u17b5\u1822\u1879\u1882\u18aa\u1e02\u1e9d\u1ea2\u1efb\u1f02\u1f17\u1f1a"+
		"\u1f1f\u1f22\u1f47\u1f4a\u1f4f\u1f52\u1f59\u1f5b\u1f5b\u1f5d\u1f5d\u1f5f"+
		"\u1f5f\u1f61\u1f7f\u1f82\u1fb6\u1fb8\u1fbe\u1fc0\u1fc0\u1fc4\u1fc6\u1fc8"+
		"\u1fce\u1fd2\u1fd5\u1fd8\u1fdd\u1fe2\u1fee\u1ff4\u1ff6\u1ff8\u1ffe\u2081"+
		"\u2081\u2104\u2104\u2109\u2109\u210c\u2115\u2117\u2117\u211b\u211f\u2126"+
		"\u2126\u2128\u2128\u212a\u212a\u212c\u212f\u2131\u2133\u2135\u213b\u2162"+
		"\u2185\u3007\u3009\u3023\u302b\u3033\u3037\u303a\u303c\u3043\u3096\u309f"+
		"\u30a0\u30a3\u30fc\u30fe\u3100\u3107\u312e\u3133\u3190\u31a2\u31b9\u3402"+
		"\u3402\u4db7\u4db7\u4e02\u4e02\u9fa7\u9fa7\ua002\ua48e\uac02\uac02\ud7a5"+
		"\ud7a5\uf902\ufa2f\ufb02\ufb08\ufb15\ufb19\ufb1f\ufb1f\ufb21\ufb2a\ufb2c"+
		"\ufb38\ufb3a\ufb3e\ufb40\ufb40\ufb42\ufb43\ufb45\ufb46\ufb48\ufbb3\ufbd5"+
		"\ufd3f\ufd52\ufd91\ufd94\ufdc9\ufdf2\ufdfd\ufe72\ufe74\ufe76\ufe76\ufe78"+
		"\ufefe\uff23\uff3c\uff43\uff5c\uff68\uffc0\uffc4\uffc9\uffcc\uffd1\uffd4"+
		"\uffd9\uffdc\uffde\2\u0188\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2"+
		"\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2"+
		"\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3"+
		"\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2"+
		"\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67"+
		"\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2"+
		"\2\2\2E\3\2\2\2\2G\3\2\2\2\2I\3\2\2\2\2K\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2"+
		"\2Q\3\2\2\2\2S\3\2\2\2\2U\3\2\2\2\2W\3\2\2\2\2Y\3\2\2\2\2[\3\2\2\2\2]"+
		"\3\2\2\2\2_\3\2\2\2\2a\3\2\2\2\2c\3\2\2\2\2e\3\2\2\2\3w\3\2\2\2\5z\3\2"+
		"\2\2\7\177\3\2\2\2\t\u0086\3\2\2\2\13\u008e\3\2\2\2\r\u0090\3\2\2\2\17"+
		"\u0092\3\2\2\2\21\u0094\3\2\2\2\23\u0096\3\2\2\2\25\u0098\3\2\2\2\27\u009a"+
		"\3\2\2\2\31\u009c\3\2\2\2\33\u009e\3\2\2\2\35\u00a0\3\2\2\2\37\u00a2\3"+
		"\2\2\2!\u00a4\3\2\2\2#\u00a7\3\2\2\2%\u00aa\3\2\2\2\'\u00ad\3\2\2\2)\u00b1"+
		"\3\2\2\2+\u00b4\3\2\2\2-\u00b7\3\2\2\2/\u00ba\3\2\2\2\61\u00bd\3\2\2\2"+
		"\63\u00bf\3\2\2\2\65\u00c2\3\2\2\2\67\u00c4\3\2\2\29\u00c7\3\2\2\2;\u00c9"+
		"\3\2\2\2=\u00cb\3\2\2\2?\u00cd\3\2\2\2A\u00d0\3\2\2\2C\u00d3\3\2\2\2E"+
		"\u00d6\3\2\2\2G\u00d8\3\2\2\2I\u00da\3\2\2\2K\u00dc\3\2\2\2M\u00de\3\2"+
		"\2\2O\u00e0\3\2\2\2Q\u00e2\3\2\2\2S\u00e5\3\2\2\2U\u00ec\3\2\2\2W\u00f3"+
		"\3\2\2\2Y\u010a\3\2\2\2[\u010c\3\2\2\2]\u0113\3\2\2\2_\u011e\3\2\2\2a"+
		"\u0124\3\2\2\2c\u0133\3\2\2\2e\u0139\3\2\2\2g\u0144\3\2\2\2i\u0161\3\2"+
		"\2\2k\u0165\3\2\2\2m\u0167\3\2\2\2o\u0169\3\2\2\2q\u0171\3\2\2\2s\u0174"+
		"\3\2\2\2u\u0177\3\2\2\2wx\7>\2\2xy\7/\2\2y\4\3\2\2\2z{\7h\2\2{|\7w\2\2"+
		"|}\7p\2\2}~\7e\2\2~\6\3\2\2\2\177\u0080\7t\2\2\u0080\u0081\7g\2\2\u0081"+
		"\u0082\7v\2\2\u0082\u0083\7w\2\2\u0083\u0084\7t\2\2\u0084\u0085\7p\2\2"+
		"\u0085\b\3\2\2\2\u0086\u008b\5q9\2\u0087\u008a\5q9\2\u0088\u008a\5s:\2"+
		"\u0089\u0087\3\2\2\2\u0089\u0088\3\2\2\2\u008a\u008d\3\2\2\2\u008b\u0089"+
		"\3\2\2\2\u008b\u008c\3\2\2\2\u008c\n\3\2\2\2\u008d\u008b\3\2\2\2\u008e"+
		"\u008f\7*\2\2\u008f\f\3\2\2\2\u0090\u0091\7+\2\2\u0091\16\3\2\2\2\u0092"+
		"\u0093\7}\2\2\u0093\20\3\2\2\2\u0094\u0095\7\177\2\2\u0095\22\3\2\2\2"+
		"\u0096\u0097\7]\2\2\u0097\24\3\2\2\2\u0098\u0099\7_\2\2\u0099\26\3\2\2"+
		"\2\u009a\u009b\7?\2\2\u009b\30\3\2\2\2\u009c\u009d\7.\2\2\u009d\32\3\2"+
		"\2\2\u009e\u009f\7=\2\2\u009f\34\3\2\2\2\u00a0\u00a1\7<\2\2\u00a1\36\3"+
		"\2\2\2\u00a2\u00a3\7\60\2\2\u00a3 \3\2\2\2\u00a4\u00a5\7-\2\2\u00a5\u00a6"+
		"\7-\2\2\u00a6\"\3\2\2\2\u00a7\u00a8\7/\2\2\u00a8\u00a9\7/\2\2\u00a9$\3"+
		"\2\2\2\u00aa\u00ab\7<\2\2\u00ab\u00ac\7?\2\2\u00ac&\3\2\2\2\u00ad\u00ae"+
		"\7\60\2\2\u00ae\u00af\7\60\2\2\u00af\u00b0\7\60\2\2\u00b0(\3\2\2\2\u00b1"+
		"\u00b2\7~\2\2\u00b2\u00b3\7~\2\2\u00b3*\3\2\2\2\u00b4\u00b5\7(\2\2\u00b5"+
		"\u00b6\7(\2\2\u00b6,\3\2\2\2\u00b7\u00b8\7?\2\2\u00b8\u00b9\7?\2\2\u00b9"+
		".\3\2\2\2\u00ba\u00bb\7#\2\2\u00bb\u00bc\7?\2\2\u00bc\60\3\2\2\2\u00bd"+
		"\u00be\7>\2\2\u00be\62\3\2\2\2\u00bf\u00c0\7>\2\2\u00c0\u00c1\7?\2\2\u00c1"+
		"\64\3\2\2\2\u00c2\u00c3\7@\2\2\u00c3\66\3\2\2\2\u00c4\u00c5\7@\2\2\u00c5"+
		"\u00c6\7?\2\2\u00c68\3\2\2\2\u00c7\u00c8\7~\2\2\u00c8:\3\2\2\2\u00c9\u00ca"+
		"\7\61\2\2\u00ca<\3\2\2\2\u00cb\u00cc\7\'\2\2\u00cc>\3\2\2\2\u00cd\u00ce"+
		"\7>\2\2\u00ce\u00cf\7>\2\2\u00cf@\3\2\2\2\u00d0\u00d1\7@\2\2\u00d1\u00d2"+
		"\7@\2\2\u00d2B\3\2\2\2\u00d3\u00d4\7(\2\2\u00d4\u00d5\7`\2\2\u00d5D\3"+
		"\2\2\2\u00d6\u00d7\7#\2\2\u00d7F\3\2\2\2\u00d8\u00d9\7-\2\2\u00d9H\3\2"+
		"\2\2\u00da\u00db\7/\2\2\u00dbJ\3\2\2\2\u00dc\u00dd\7`\2\2\u00ddL\3\2\2"+
		"\2\u00de\u00df\7,\2\2\u00dfN\3\2\2\2\u00e0\u00e1\7(\2\2\u00e1P\3\2\2\2"+
		"\u00e2\u00e3\7/\2\2\u00e3\u00e4\7@\2\2\u00e4R\3\2\2\2\u00e5\u00e9\t\2"+
		"\2\2\u00e6\u00e8\t\3\2\2\u00e7\u00e6\3\2\2\2\u00e8\u00eb\3\2\2\2\u00e9"+
		"\u00e7\3\2\2\2\u00e9\u00ea\3\2\2\2\u00eaT\3\2\2\2\u00eb\u00e9\3\2\2\2"+
		"\u00ec\u00f0\7\62\2\2\u00ed\u00ef\5k\66\2\u00ee\u00ed\3\2\2\2\u00ef\u00f2"+
		"\3\2\2\2\u00f0\u00ee\3\2\2\2\u00f0\u00f1\3\2\2\2\u00f1V\3\2\2\2\u00f2"+
		"\u00f0\3\2\2\2\u00f3\u00f4\7\62\2\2\u00f4\u00f6\t\4\2\2\u00f5\u00f7\5"+
		"m\67\2\u00f6\u00f5\3\2\2\2\u00f7\u00f8\3\2\2\2\u00f8\u00f6\3\2\2\2\u00f8"+
		"\u00f9\3\2\2\2\u00f9X\3\2\2\2\u00fa\u0103\5i\65\2\u00fb\u00fd\7\60\2\2"+
		"\u00fc\u00fe\5i\65\2\u00fd\u00fc\3\2\2\2\u00fd\u00fe\3\2\2\2\u00fe\u0100"+
		"\3\2\2\2\u00ff\u0101\5o8\2\u0100\u00ff\3\2\2\2\u0100\u0101\3\2\2\2\u0101"+
		"\u0104\3\2\2\2\u0102\u0104\5o8\2\u0103\u00fb\3\2\2\2\u0103\u0102\3\2\2"+
		"\2\u0104\u010b\3\2\2\2\u0105\u0106\7\60\2\2\u0106\u0108\5i\65\2\u0107"+
		"\u0109\5o8\2\u0108\u0107\3\2\2\2\u0108\u0109\3\2\2\2\u0109\u010b\3\2\2"+
		"\2\u010a\u00fa\3\2\2\2\u010a\u0105\3\2\2\2\u010bZ\3\2\2\2\u010c\u010f"+
		"\7)\2\2\u010d\u0110\n\5\2\2\u010e\u0110\5g\64\2\u010f\u010d\3\2\2\2\u010f"+
		"\u010e\3\2\2\2\u0110\u0111\3\2\2\2\u0111\u0112\7)\2\2\u0112\\\3\2\2\2"+
		"\u0113\u0118\7$\2\2\u0114\u0117\n\6\2\2\u0115\u0117\5g\64\2\u0116\u0114"+
		"\3\2\2\2\u0116\u0115\3\2\2\2\u0117\u011a\3\2\2\2\u0118\u0116\3\2\2\2\u0118"+
		"\u0119\3\2\2\2\u0119\u011b\3\2\2\2\u011a\u0118\3\2\2\2\u011b\u011c\7$"+
		"\2\2\u011c^\3\2\2\2\u011d\u011f\t\7\2\2\u011e\u011d\3\2\2\2\u011f\u0120"+
		"\3\2\2\2\u0120\u011e\3\2\2\2\u0120\u0121\3\2\2\2\u0121\u0122\3\2\2\2\u0122"+
		"\u0123\b\60\2\2\u0123`\3\2\2\2\u0124\u0125\7\61\2\2\u0125\u0126\7,\2\2"+
		"\u0126\u012a\3\2\2\2\u0127\u0129\13\2\2\2\u0128\u0127\3\2\2\2\u0129\u012c"+
		"\3\2\2\2\u012a\u012b\3\2\2\2\u012a\u0128\3\2\2\2\u012b\u012d\3\2\2\2\u012c"+
		"\u012a\3\2\2\2\u012d\u012e\7,\2\2\u012e\u012f\7\61\2\2\u012f\u0130\3\2"+
		"\2\2\u0130\u0131\b\61\2\2\u0131b\3\2\2\2\u0132\u0134\t\b\2\2\u0133\u0132"+
		"\3\2\2\2\u0134\u0135\3\2\2\2\u0135\u0133\3\2\2\2\u0135\u0136\3\2\2\2\u0136"+
		"\u0137\3\2\2\2\u0137\u0138\b\62\2\2\u0138d\3\2\2\2\u0139\u013a\7\61\2"+
		"\2\u013a\u013b\7\61\2\2\u013b\u013f\3\2\2\2\u013c\u013e\n\b\2\2\u013d"+
		"\u013c\3\2\2\2\u013e\u0141\3\2\2\2\u013f\u013d\3\2\2\2\u013f\u0140\3\2"+
		"\2\2\u0140\u0142\3\2\2\2\u0141\u013f\3\2\2\2\u0142\u0143\b\63\2\2\u0143"+
		"f\3\2\2\2\u0144\u015e\7^\2\2\u0145\u0146\7w\2\2\u0146\u0147\5m\67\2\u0147"+
		"\u0148\5m\67\2\u0148\u0149\5m\67\2\u0149\u014a\5m\67\2\u014a\u015f\3\2"+
		"\2\2\u014b\u014c\7W\2\2\u014c\u014d\5m\67\2\u014d\u014e\5m\67\2\u014e"+
		"\u014f\5m\67\2\u014f\u0150\5m\67\2\u0150\u0151\5m\67\2\u0151\u0152\5m"+
		"\67\2\u0152\u0153\5m\67\2\u0153\u0154\5m\67\2\u0154\u015f\3\2\2\2\u0155"+
		"\u015f\t\t\2\2\u0156\u0157\5k\66\2\u0157\u0158\5k\66\2\u0158\u0159\5k"+
		"\66\2\u0159\u015f\3\2\2\2\u015a\u015b\7z\2\2\u015b\u015c\5m\67\2\u015c"+
		"\u015d\5m\67\2\u015d\u015f\3\2\2\2\u015e\u0145\3\2\2\2\u015e\u014b\3\2"+
		"\2\2\u015e\u0155\3\2\2\2\u015e\u0156\3\2\2\2\u015e\u015a\3\2\2\2\u015f"+
		"h\3\2\2\2\u0160\u0162\t\3\2\2\u0161\u0160\3\2\2\2\u0162\u0163\3\2\2\2"+
		"\u0163\u0161\3\2\2\2\u0163\u0164\3\2\2\2\u0164j\3\2\2\2\u0165\u0166\t"+
		"\n\2\2\u0166l\3\2\2\2\u0167\u0168\t\13\2\2\u0168n\3\2\2\2\u0169\u016b"+
		"\t\f\2\2\u016a\u016c\t\r\2\2\u016b\u016a\3\2\2\2\u016b\u016c\3\2\2\2\u016c"+
		"\u016d\3\2\2\2\u016d\u016e\5i\65\2\u016ep\3\2\2\2\u016f\u0172\5u;\2\u0170"+
		"\u0172\7a\2\2\u0171\u016f\3\2\2\2\u0171\u0170\3\2\2\2\u0172r\3\2\2\2\u0173"+
		"\u0175\t\16\2\2\u0174\u0173\3\2\2\2\u0175t\3\2\2\2\u0176\u0178\t\17\2"+
		"\2\u0177\u0176\3\2\2\2\u0178v\3\2\2\2\32\2\u0089\u008b\u00e9\u00f0\u00f8"+
		"\u00fd\u0100\u0103\u0108\u010a\u010f\u0116\u0118\u0120\u012a\u0135\u013f"+
		"\u015e\u0163\u016b\u0171\u0174\u0177\3\2\3\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}