#include "tokenizer.hpp"

#include "../utils/file_utils.hpp"
#include "../utils/unicode.hpp"
#include "token.hpp"

#define try_ret(expr, ret_val) \
  if (expr) {                  \
    return ret_val;            \
  }

#define try(expr)    \
  auto value = expr; \
  if (value) {       \
    return value;    \
  }

LL_INTERNAL_FN
const TokenizerInitError filepath_status_to_tokenizer_init_error(const OPEN_FILE_ERROR in_filepath_status) {
  switch (in_filepath_status) {
  case OPEN_FILE_ERROR::OK:
    return TokenizerInitError::NONE;
  case OPEN_FILE_ERROR::NOT_FOUND:
    return TokenizerInitError::NOTEXISTS;
  case OPEN_FILE_ERROR::EMPTY:
    return TokenizerInitError::EMPTY;
  case OPEN_FILE_ERROR::TOO_LARGE:
    return TokenizerInitError::FILETOOLARGE;
  case OPEN_FILE_ERROR::PERMISSION_DENIED:
    return TokenizerInitError::PERMISSION;
  case OPEN_FILE_ERROR::NOT_A_FILE:
  case OPEN_FILE_ERROR::INVALID:
    return TokenizerInitError::INVALID;
  }
}

LL_INTERNAL_FN
void tokenizer_err(Tokenizer &tokenizer, const char *in_msg) {}

LL_INTERNAL_FN
bool advance_to_next_rune(Tokenizer &tokenizer) {
  const FileInput &file_io = get_std_file_input();
  if (tokenizer.curr_rune == '\n') {
    tokenizer.column_minus_one = -1;
    tokenizer.line_count++;
  }

  if (tokenizer.offset < tokenizer.loaded_file.size) {
    rune rune;
    if (file_io.read_rune_at_offset(tokenizer.loaded_file, tokenizer.offset, rune) != READ_FILE_ERROR::OK) {
      tokenizer_err(tokenizer, "Error reading file");
      return true;
    }

    if (rune == 0) {
      tokenizer_err(tokenizer, "Illegal character NUL");
      tokenizer.offset++;
    } else if (rune & UTF_SELF) { // not ASCII
      u8 *rune_str = new u8[5];
      defer({ delete[] rune_str; });

      if (file_io.read_bytes_at_offset(tokenizer.loaded_file, tokenizer.offset, 4, rune_str) != READ_FILE_ERROR::OK) {
        tokenizer_err(tokenizer, "Error reading file");
        return true;
      }
      u64 width = utf8_decode(rune_str, 4, &rune);
      tokenizer.offset += width;
      if (rune == UTF_INVALID_RUNE && width == 1) {
        tokenizer_err(tokenizer, "Illegal UTF-8 encoding");
      } else if (rune == UTF_BOM && tokenizer.offset > 0) {
        tokenizer_err(tokenizer, "Illegal byte order mark");
      }
    } else {
      tokenizer.offset++;
    }
    tokenizer.curr_rune = rune;
    tokenizer.column_minus_one++;
  } else {
    tokenizer.offset = tokenizer.loaded_file.size;
    tokenizer.curr_rune = UTF_EOF;
  }

  return false;
}

LL_INTERNAL_FN
bool tokenizer_skip_whitespace(Tokenizer &tokenizer) {
  for (;;) {
    switch (tokenizer.curr_rune) {
    case '\n':
    case ' ':
    case '\t':
    case '\r':
      try(advance_to_next_rune(tokenizer));
      continue;
    }
    break;
  }

  return false;
}

LL_INTERNAL_FN
bool init_tokenizer_with_data(Tokenizer &tokenizer, const std::string &in_fullpath) {
  tokenizer.fullpath = in_fullpath;
  tokenizer.line_count = 1;

  try(advance_to_next_rune(tokenizer));
  if (tokenizer.curr_rune == UTF_BOM) {
    try(advance_to_next_rune(tokenizer)); // Ignore BOM at file beginning
  }
  return false;
}

const TokenizerInitError Tokenizer_Init(Tokenizer &tokenizer, const std::string &in_file_path) {
  auto file_utils = get_std_file_input();
  std::string fullpath;

  OPEN_FILE_ERROR file_err = file_utils.open_file(in_file_path, FILE_MODE_FLAG::READ, tokenizer.loaded_file, &fullpath);

  TokenizerInitError err = filepath_status_to_tokenizer_init_error(file_err);
  switch (file_err) {
  case OPEN_FILE_ERROR::OK:
    if (init_tokenizer_with_data(tokenizer, fullpath))
      return TokenizerInitError::INVALID;
    break;
  case OPEN_FILE_ERROR::EMPTY:
  case OPEN_FILE_ERROR::TOO_LARGE:
    tokenizer.fullpath = in_file_path;
    tokenizer.line_count = 1;
    break;
  }
  return err;
}

const TokenizerReadError get_next_token(Tokenizer &tokenizer, Token &out_token) {
  try_ret(tokenizer_skip_whitespace(tokenizer), TokenizerReadError::INVALID);

  out_token.kind = TokenId::ERROR;
  out_token.string.text = tokenizer.curr;
  out_token.string.len = 1;
  out_token.pos.file_id = tokenizer.curr_file_id;
  out_token.pos.line = tokenizer.line_count;
  out_token.pos.offset = cast(s32)(tokenizer.curr - tokenizer.start);
  out_token.pos.column = tokenizer.column_minus_one + 1;

  TokenPos current_pos = out_token.pos;

  rune curr_rune = tokenizer.curr_rune;
  if (rune_is_letter(curr_rune)) {
    out_token.kind = Token_Ident;
    while (rune_is_letter_or_digit(tokenizer.curr_rune)) { advance_to_next_rune(t); }

    out_token.string.len = tokenizer.curr - out_token.string.text;

    // NOTE(bill): Heavily optimize to make it faster to find keywords
    if (1 < out_token.string.len && out_token.string.len <= max_keyword_size && keyword_indices[out_token.string.len]) {
      u32 hash = keyword_hash(out_token.string.text, out_token.string.len);
      u32 index = hash & KEYWORD_HASH_TABLE_MASK;
      KeywordHashEntry *entry = &keyword_hash_table[index];
      if (entry->kind != Token_Invalid && entry->hash == hash) {
        if (str_eq(entry->text, out_token.string)) {
          out_token.kind = entry->kind;
          if (out_token.kind == Token_not_in && entry->text == "notin") {
            syntax_warning(*token, "'notin' is deprecated in favour of 'not_in'");
          }
        }
      }
    }

    goto semicolon_check;
  } else {
    switch (curr_rune) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      scan_number_to_token(t, token, false);
      goto semicolon_check;
    }

    advance_to_next_rune(t);
    switch (curr_rune) {
    case GB_RUNE_EOF:
      out_token.kind = Token_EOF;
      if (tokenizer.insert_semicolon) {
        tokenizer.insert_semicolon = false; // EOF consumed
        out_token.string = str_lit("\n");
        out_token.kind = Token_Semicolon;
        return;
      }
      break;

    case '\n':
      tokenizer.insert_semicolon = false;
      out_token.string = str_lit("\n");
      out_token.kind = Token_Semicolon;
      return;

    case '\\':
      tokenizer.insert_semicolon = false;
      tokenizer_get_token(t, token);
      if (out_token.pos.line == current_pos.line) {
        tokenizer_err(t, token_pos_add_column(current_pos), "Expected a newline after \\");
      }
      // NOTE(bill): tokenizer_get_token has been called already, return early
      return;

    case '\'': // rune Literal
    {
      out_token.kind = Token_Rune;
      rune quote = curr_rune;
      bool valid = true;
      s32 n = 0, success;
      for (;;) {
        rune r = tokenizer.curr_rune;
        if (r == '\n' || r < 0) {
          tokenizer_err(t, "rune literal not terminated");
          break;
        }
        advance_to_next_rune(t);
        if (r == quote) {
          break;
        }
        n++;
        if (r == '\\') {
          if (!scan_escape(t)) {
            valid = false;
          }
        }
      }

      // TODO(bill): Better Error Handling
      if (valid && n != 1) {
        tokenizer_err(t, "Invalid rune literal");
      }
      out_token.string.len = tokenizer.curr - out_token.string.text;
      goto semicolon_check;
    } break;

    case '`': // Raw String Literal
    case '"': // String Literal
    {
      bool has_carriage_return = false;
      s32 success;
      rune quote = curr_rune;
      out_token.kind = Token_String;
      if (curr_rune == '"') {
        for (;;) {
          rune r = tokenizer.curr_rune;
          if (r == '\n' || r < 0) {
            tokenizer_err(t, "String literal not terminated");
            break;
          }
          advance_to_next_rune(t);
          if (r == quote) {
            break;
          }
          if (r == '\\') {
            scan_escape(t);
          }
        }
      } else {
        for (;;) {
          rune r = tokenizer.curr_rune;
          if (r < 0) {
            tokenizer_err(t, "String literal not terminated");
            break;
          }
          advance_to_next_rune(t);
          if (r == quote) {
            break;
          }
          if (r == '\r') {
            has_carriage_return = true;
          }
        }
      }
      out_token.string.len = tokenizer.curr - out_token.string.text;
      goto semicolon_check;
    } break;

    case '.':
      out_token.kind = Token_Period;
      switch (tokenizer.curr_rune) {
      case '.':
        advance_to_next_rune(t);
        out_token.kind = Token_Ellipsis;
        if (tokenizer.curr_rune == '<') {
          advance_to_next_rune(t);
          out_token.kind = Token_RangeHalf;
        } else if (tokenizer.curr_rune == '=') {
          advance_to_next_rune(t);
          out_token.kind = Token_RangeFull;
        }
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        scan_number_to_token(t, token, true);
        break;
      }
      break;
    case '@':
      out_token.kind = Token_At;
      break;
    case '$':
      out_token.kind = Token_Dollar;
      break;
    case '?':
      out_token.kind = Token_Question;
      break;
    case '^':
      out_token.kind = Token_Pointer;
      break;
    case ';':
      out_token.kind = Token_Semicolon;
      break;
    case ',':
      out_token.kind = Token_Comma;
      break;
    case ':':
      out_token.kind = Token_Colon;
      break;
    case '(':
      out_token.kind = Token_OpenParen;
      break;
    case ')':
      out_token.kind = Token_CloseParen;
      break;
    case '[':
      out_token.kind = Token_OpenBracket;
      break;
    case ']':
      out_token.kind = Token_CloseBracket;
      break;
    case '{':
      out_token.kind = Token_OpenBrace;
      break;
    case '}':
      out_token.kind = Token_CloseBrace;
      break;
    case '%':
      out_token.kind = Token_Mod;
      switch (tokenizer.curr_rune) {
      case '=':
        advance_to_next_rune(t);
        out_token.kind = Token_ModEq;
        break;
      case '%':
        out_token.kind = Token_ModMod;
        advance_to_next_rune(t);
        if (tokenizer.curr_rune == '=') {
          out_token.kind = Token_ModModEq;
          advance_to_next_rune(t);
        }
        break;
      }
      break;

    case '*':
      out_token.kind = Token_Mul;
      if (tokenizer.curr_rune == '=') {
        advance_to_next_rune(t);
        out_token.kind = Token_MulEq;
      }
      break;
    case '=':
      out_token.kind = Token_Eq;
      if (tokenizer.curr_rune == '=') {
        advance_to_next_rune(t);
        out_token.kind = Token_CmpEq;
      }
      break;
    case '~':
      out_token.kind = Token_Xor;
      if (tokenizer.curr_rune == '=') {
        advance_to_next_rune(t);
        out_token.kind = Token_XorEq;
      }
      break;
    case '!':
      out_token.kind = Token_Not;
      if (tokenizer.curr_rune == '=') {
        advance_to_next_rune(t);
        out_token.kind = Token_NotEq;
      }
      break;
    case '+':
      out_token.kind = Token_Add;
      switch (tokenizer.curr_rune) {
      case '=':
        advance_to_next_rune(t);
        out_token.kind = Token_AddEq;
        break;
      case '+':
        advance_to_next_rune(t);
        out_token.kind = Token_Increment;
        break;
      }
      break;
    case '-':
      out_token.kind = Token_Sub;
      switch (tokenizer.curr_rune) {
      case '=':
        advance_to_next_rune(t);
        out_token.kind = Token_SubEq;
        break;
      case '-':
        advance_to_next_rune(t);
        out_token.kind = Token_Decrement;
        if (tokenizer.curr_rune == '-') {
          advance_to_next_rune(t);
          out_token.kind = Token_Undef;
        }
        break;
      case '>':
        advance_to_next_rune(t);
        out_token.kind = Token_ArrowRight;
        break;
      }
      break;
    case '#':
      out_token.kind = Token_Hash;
      if (tokenizer.curr_rune == '!') {
        out_token.kind = Token_Comment;
        tokenizer_skip_line(t);
      }
      break;
    case '/':
      out_token.kind = Token_Quo;
      switch (tokenizer.curr_rune) {
      case '/':
        out_token.kind = Token_Comment;
        tokenizer_skip_line(t);
        break;
      case '*':
        out_token.kind = Token_Comment;
        advance_to_next_rune(t);
        for (isize comment_scope = 1; comment_scope > 0; /**/) {
          if (tokenizer.curr_rune == GB_RUNE_EOF) {
            break;
          } else if (tokenizer.curr_rune == '/') {
            advance_to_next_rune(t);
            if (tokenizer.curr_rune == '*') {
              advance_to_next_rune(t);
              comment_scope++;
            }
          } else if (tokenizer.curr_rune == '*') {
            advance_to_next_rune(t);
            if (tokenizer.curr_rune == '/') {
              advance_to_next_rune(t);
              comment_scope--;
            }
          } else {
            advance_to_next_rune(t);
          }
        }
        break;
      case '=':
        advance_to_next_rune(t);
        out_token.kind = Token_QuoEq;
        break;
      }
      break;
    case '<':
      out_token.kind = Token_Lt;
      switch (tokenizer.curr_rune) {
      case '=':
        out_token.kind = Token_LtEq;
        advance_to_next_rune(t);
        break;
      case '<':
        out_token.kind = Token_Shl;
        advance_to_next_rune(t);
        if (tokenizer.curr_rune == '=') {
          out_token.kind = Token_ShlEq;
          advance_to_next_rune(t);
        }
        break;
      }
      break;
    case '>':
      out_token.kind = Token_Gt;
      switch (tokenizer.curr_rune) {
      case '=':
        out_token.kind = Token_GtEq;
        advance_to_next_rune(t);
        break;
      case '>':
        out_token.kind = Token_Shr;
        advance_to_next_rune(t);
        if (tokenizer.curr_rune == '=') {
          out_token.kind = Token_ShrEq;
          advance_to_next_rune(t);
        }
        break;
      }
      break;
    case '&':
      out_token.kind = Token_And;
      switch (tokenizer.curr_rune) {
      case '~':
        out_token.kind = Token_AndNot;
        advance_to_next_rune(t);
        if (tokenizer.curr_rune == '=') {
          out_token.kind = Token_AndNotEq;
          advance_to_next_rune(t);
        }
        break;
      case '=':
        out_token.kind = Token_AndEq;
        advance_to_next_rune(t);
        break;
      case '&':
        out_token.kind = Token_CmpAnd;
        advance_to_next_rune(t);
        if (tokenizer.curr_rune == '=') {
          out_token.kind = Token_CmpAndEq;
          advance_to_next_rune(t);
        }
        break;
      }
      break;
    case '|':
      out_token.kind = Token_Or;
      switch (tokenizer.curr_rune) {
      case '=':
        out_token.kind = Token_OrEq;
        advance_to_next_rune(t);
        break;
      case '|':
        out_token.kind = Token_CmpOr;
        advance_to_next_rune(t);
        if (tokenizer.curr_rune == '=') {
          out_token.kind = Token_CmpOrEq;
          advance_to_next_rune(t);
        }
        break;
      }
      break;
    default:
      out_token.kind = Token_Invalid;
      if (curr_rune != GB_RUNE_BOM) {
        u8 str[4] = {};
        int len = cast(int) gb_utf8_encode_rune(str, curr_rune);
        tokenizer_err(t, "Illegal character: %.*s (%d) ", len, str, curr_rune);
      }
      break;
    }
  }

  out_token.string.len = tokenizer.curr - out_token.string.text;

semicolon_check:;
  switch (out_token.kind) {
  case Token_Invalid:
  case Token_Comment:
    // Preserve insert_semicolon info
    break;
  case Token_Ident:
  case Token_context:
  case Token_typeid:
  case Token_break:
  case Token_continue:
  case Token_fallthrough:
  case Token_return:
  case Token_or_return:
    /*fallthrough*/
  case Token_Integer:
  case Token_Float:
  case Token_Imag:
  case Token_Rune:
  case Token_String:
  case Token_Undef:
    /*fallthrough*/
  case Token_Question:
  case Token_Pointer:
  case Token_CloseParen:
  case Token_CloseBracket:
  case Token_CloseBrace:
    /*fallthrough*/
  case Token_Increment:
  case Token_Decrement:
    /*fallthrough*/
    tokenizer.insert_semicolon = true;
    break;
  default:
    tokenizer.insert_semicolon = false;
    break;
  }

  return TokenizerReadError::NONE;
}
