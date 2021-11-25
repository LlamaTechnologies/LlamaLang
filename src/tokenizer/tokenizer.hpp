#pragma once
#include "../common_defs.hpp"
#include "../utils/file_utils.hpp"
#include "tokenizer_fwd.hpp"

#include <string>

struct Tokenizer {
  std::string fullpath;

  rune curr_rune; // current character
  u64 offset;
  s32 column_minus_one;
  s32 line_count;

  s32 error_count;

  File loaded_file;
};

const TokenizerInitError Tokenizer_Init(Tokenizer &tokenizer, const std::string &in_file_path);

const TokenizerReadError get_next_token(Tokenizer &tokenizer, Token &out_token);
