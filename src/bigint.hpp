/*
 * Copyright (c) 2017 Andrew Kelley
 *
 * This file is part of zig, which is MIT licensed.
 * See http://opensource.org/licenses/MIT
 */

#pragma once
#include "common_defs.h"
#include <stdint.h>
#include <stddef.h>

struct BigInt {
    size_t digit_count;
    union {
        uint64_t digit;
        uint64_t *digits; // Least significant digit first
    } data;
    bool is_negative;
};

struct Buf;
struct BigFloat;

enum Cmp {
    CmpLT,
    CmpGT,
    CmpEQ,
};

void bigint_init_unsigned(BigInt *dest, uint64_t x);
void bigint_init_signed(BigInt *dest, int64_t x);
void bigint_init_bigint(BigInt *dest, const BigInt *src);
void bigint_deinit(BigInt *bi);

static inline const uint64_t *bigint_ptr(const BigInt *bigint) {
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

Cmp bigint_cmp(const BigInt *op1, const BigInt *op2);

