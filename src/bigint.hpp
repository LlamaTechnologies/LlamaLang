/*
 * Copyright (c) 2017 Andrew Kelley
 *
 * This file is part of zig, which is MIT licensed.
 * See http://opensource.org/licenses/MIT
 */

#pragma once
#include "common_defs.hpp"

#include <stddef.h> //size_t
#include <stdint.h>

struct BigInt {
  size_t digit_count;
  union {
    u64 digit;
    u64 *digits; // Least significant digit first
  } data;
  bool is_negative;
};

struct Buf;
struct BigFloat;

void bigint_init_unsigned(BigInt *dest, u64 x);
void bigint_init_signed(BigInt *dest, s64 x);
void bigint_init_bigint(BigInt *dest, const BigInt *src);
void bigint_deinit(BigInt *bi);

static inline const u64 *bigint_ptr(const BigInt *bigint) {
  if (bigint->digit_count == 1) {
    return &bigint->data.digit;
  } else {
    return bigint->data.digits;
  }
}

void bigint_add(BigInt *dest, const BigInt *op1, const BigInt *op2);
void bigint_mul(BigInt *dest, const BigInt *op1, const BigInt *op2);

void bigint_shl(BigInt *dest, const BigInt *op1, const BigInt *op2);

void bigint_negate(BigInt *dest, const BigInt *op);

enum Cmp
{
  CmpLT,
  CmpGT,
  CmpEQ,
};

Cmp bigint_cmp(const BigInt *op1, const BigInt *op2);

bool operator==(const BigInt &val1, const BigInt &val2);
