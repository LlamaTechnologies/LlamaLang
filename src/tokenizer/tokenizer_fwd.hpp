#pragma once
#include "token.hpp"

struct Tokenizer;

enum class TokenizerInitError
{
  NONE,
  INVALID,
  NOTEXISTS,
  PERMISSION,
  EMPTY,
  FILETOOLARGE,
};

enum class TokenizerReadError
{
};
