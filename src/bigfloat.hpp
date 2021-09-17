#pragma once
#include "common_defs.hpp"

#include <cstdint>

typedef struct {
  u64 v[2];
} float128_t;

struct BigFloat {
  float128_t value;
  bool overflow;
};
