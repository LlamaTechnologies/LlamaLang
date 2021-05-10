/*
 * Copyright (c) 2017 Andrew Kelley
 *
 * This file is part of zig, which is MIT licensed.
 * See http://opensource.org/licenses/MIT
 */

#include "bigint.hpp"

#include <limits>
#include <algorithm>
#include <cassert>

static void bigint_normalize(BigInt *dest) {
    const uint64_t *digits = bigint_ptr(dest);

    size_t last_nonzero_digit = SIZE_MAX;
    for (size_t i = 0; i < dest->digit_count; i += 1) {
        uint64_t digit = digits[i];
        if (digit != 0) {
            last_nonzero_digit = i;
        }
    }
    if (last_nonzero_digit == SIZE_MAX) {
        dest->is_negative = false;
        dest->digit_count = 0;
    } else {
        dest->digit_count = last_nonzero_digit + 1;
        if (last_nonzero_digit == 0) {
            dest->data.digit = digits[0];
        }
    }
}

void bigint_init_unsigned(BigInt *dest, uint64_t x) {
    if (x == 0) {
        dest->digit_count = 0;
        dest->is_negative = false;
        return;
    }
    dest->digit_count = 1;
    dest->data.digit = x;
    dest->is_negative = false;
}

void bigint_init_signed(BigInt *dest, int64_t x) {
    if (x >= 0) {
        return bigint_init_unsigned(dest, x);
    }
    dest->is_negative = true;
    dest->digit_count = 1;
    dest->data.digit = ((uint64_t)(-(x + 1))) + 1;
}

void bigint_init_data(BigInt *dest, const uint64_t *digits, size_t digit_count, bool is_negative) {
    if (digit_count == 0) {
        return bigint_init_unsigned(dest, 0);
    } else if (digit_count == 1) {
        dest->digit_count = 1;
        dest->data.digit = digits[0];
        dest->is_negative = is_negative;
        bigint_normalize(dest);
        return;
    }

    dest->digit_count = digit_count;
    dest->is_negative = is_negative;
    dest->data.digits = (uint64_t*) malloc(digit_count + sizeof(uint64_t));
    memcpy(dest->data.digits, digits, sizeof(uint64_t) * digit_count);

    bigint_normalize(dest);
}

void bigint_init_bigint(BigInt *dest, const BigInt *src) {
    if (src->digit_count == 0) {
        return bigint_init_unsigned(dest, 0);
    } else if (src->digit_count == 1) {
        dest->digit_count = 1;
        dest->data.digit = src->data.digit;
        dest->is_negative = src->is_negative;
        return;
    }
    dest->is_negative = src->is_negative;
    dest->digit_count = src->digit_count;
    dest->data.digits = (uint64_t*)malloc(dest->digit_count + sizeof(uint64_t));
    memcpy(dest->data.digits, src->data.digits, sizeof(uint64_t) * dest->digit_count);
}

void bigint_deinit(BigInt *bi) {
    if (bi->digit_count > 1)
        free(bi->data.digits);
}

static uint64_t bigint_as_unsigned(const BigInt* bigint) {
    assert(!bigint->is_negative);
    if (bigint->digit_count == 0) {
        return 0;
    }
    else if (bigint->digit_count == 1) {
        return bigint->data.digit;
    }
    else {
        UNREACHEABLE;
    }
}


#if defined(_MSC_VER)
static bool add_u64_overflow(uint64_t op1, uint64_t op2, uint64_t *result) {
   *result = op1 + op2;
   return *result < op1 || *result < op2;
}

static bool sub_u64_overflow(uint64_t op1, uint64_t op2, uint64_t *result) {
   *result = op1 - op2;
   return *result > op1;
}

bool mul_u64_overflow(uint64_t op1, uint64_t op2, uint64_t *result) {
    *result = op1 * op2;

    if (op1 == 0 || op2 == 0)
        return false;

    if (op1 > UINT64_MAX / op2)
        return true;

    if (op2 > UINT64_MAX / op1)
        return true;

    return false;
}
#else
static bool add_u64_overflow(uint64_t op1, uint64_t op2, uint64_t *result) {
    return __builtin_uaddll_overflow((unsigned long long)op1, (unsigned long long)op2,
            (unsigned long long *)result);
}

static bool sub_u64_overflow(uint64_t op1, uint64_t op2, uint64_t *result) {
    return __builtin_usubll_overflow((unsigned long long)op1, (unsigned long long)op2,
            (unsigned long long *)result);
}

bool mul_u64_overflow(uint64_t op1, uint64_t op2, uint64_t *result) {
    return __builtin_umulll_overflow((unsigned long long)op1, (unsigned long long)op2,
            (unsigned long long *)result);
}
#endif

void bigint_add(BigInt *dest, const BigInt *op1, const BigInt *op2) {
    if (op1->digit_count == 0) {
        return bigint_init_bigint(dest, op2);
    }
    if (op2->digit_count == 0) {
        return bigint_init_bigint(dest, op1);
    }
    if (op1->is_negative == op2->is_negative) {
        dest->is_negative = op1->is_negative;

        const uint64_t *op1_digits = bigint_ptr(op1);
        const uint64_t *op2_digits = bigint_ptr(op2);
        bool overflow = add_u64_overflow(op1_digits[0], op2_digits[0], &dest->data.digit);
        if (overflow == 0 && op1->digit_count == 1 && op2->digit_count == 1) {
            dest->digit_count = 1;
            bigint_normalize(dest);
            return;
        }
        size_t i = 1;
        uint64_t first_digit = dest->data.digit;
        dest->data.digits = (uint64_t*) malloc(sizeof(uint64_t) + std::max(op1->digit_count, op2->digit_count) + 1);
        dest->data.digits[0] = first_digit;

        for (;;) {
            bool found_digit = false;
            uint64_t x = overflow;
            overflow = 0;

            if (i < op1->digit_count) {
                found_digit = true;
                uint64_t digit = op1_digits[i];
                overflow += add_u64_overflow(x, digit, &x);
            }

            if (i < op2->digit_count) {
                found_digit = true;
                uint64_t digit = op2_digits[i];
                overflow += add_u64_overflow(x, digit, &x);
            }

            dest->data.digits[i] = x;
            i += 1;

            if (!found_digit) {
                dest->digit_count = i;
                bigint_normalize(dest);
                return;
            }
        }
    }
    const BigInt *op_pos;
    const BigInt *op_neg;
    if (op1->is_negative) {
        op_neg = op1;
        op_pos = op2;
    } else {
        op_pos = op1;
        op_neg = op2;
    }

    BigInt op_neg_abs = {0};
    bigint_negate(&op_neg_abs, op_neg);
    const BigInt *bigger_op;
    const BigInt *smaller_op;
    switch (bigint_cmp(op_pos, &op_neg_abs)) {
        case CmpEQ:
            bigint_init_unsigned(dest, 0);
            return;
        case CmpLT:
            bigger_op = &op_neg_abs;
            smaller_op = op_pos;
            dest->is_negative = true;
            break;
        case CmpGT:
            bigger_op = op_pos;
            smaller_op = &op_neg_abs;
            dest->is_negative = false;
            break;
    }
    const uint64_t *bigger_op_digits = bigint_ptr(bigger_op);
    const uint64_t *smaller_op_digits = bigint_ptr(smaller_op);
    uint64_t overflow = sub_u64_overflow(bigger_op_digits[0], smaller_op_digits[0], &dest->data.digit);
    if (overflow == 0 && bigger_op->digit_count == 1 && smaller_op->digit_count == 1) {
        dest->digit_count = 1;
        bigint_normalize(dest);
        return;
    }
    uint64_t first_digit = dest->data.digit;
    dest->data.digits = (uint64_t*)malloc(sizeof(uint64_t) + bigger_op->digit_count);
    dest->data.digits[0] = first_digit;
    size_t i = 1;

    for (;;) {
        bool found_digit = false;
        uint64_t x = bigger_op_digits[i];
        uint64_t prev_overflow = overflow;
        overflow = 0;

        if (i < smaller_op->digit_count) {
            found_digit = true;
            uint64_t digit = smaller_op_digits[i];
            overflow += sub_u64_overflow(x, digit, &x);
        }
        if (sub_u64_overflow(x, prev_overflow, &x)) {
            found_digit = true;
            overflow += 1;
        }
        dest->data.digits[i] = x;
        i += 1;

        if (!found_digit || i >= bigger_op->digit_count)
            break;
    }
    assert(overflow == 0);
    dest->digit_count = i;
    bigint_normalize(dest);
}

static void mul_overflow(uint64_t op1, uint64_t op2, uint64_t *lo, uint64_t *hi) {
    uint64_t u1 = (op1 & 0xffffffff);
    uint64_t v1 = (op2 & 0xffffffff);
    uint64_t t = (u1 * v1);
    uint64_t w3 = (t & 0xffffffff);
    uint64_t k = (t >> 32);

    op1 >>= 32;
    t = (op1 * v1) + k;
    k = (t & 0xffffffff);
    uint64_t w1 = (t >> 32);

    op2 >>= 32;
    t = (u1 * op2) + k;
    k = (t >> 32);

    *hi = (op1 * op2) + w1 + k;
    *lo = (t << 32) + w3;
}

static void mul_scalar(BigInt *dest, const BigInt *op, uint64_t scalar) {
    bigint_init_unsigned(dest, 0);

    BigInt bi_64;
    bigint_init_unsigned(&bi_64, 64);

    const uint64_t *op_digits = bigint_ptr(op);
    size_t i = op->digit_count - 1;

    for (;;) {
        BigInt shifted;
        bigint_shl(&shifted, dest, &bi_64);

        uint64_t result_scalar;
        uint64_t carry_scalar;
        mul_overflow(scalar, op_digits[i], &result_scalar, &carry_scalar);

        BigInt result;
        bigint_init_unsigned(&result, result_scalar);

        BigInt carry;
        bigint_init_unsigned(&carry, carry_scalar);

        BigInt carry_shifted;
        bigint_shl(&carry_shifted, &carry, &bi_64);

        BigInt tmp;
        bigint_add(&tmp, &shifted, &carry_shifted);

        bigint_add(dest, &tmp, &result);

        if (i == 0) {
            break;
        }
        i -= 1;
    }
}

void bigint_mul(BigInt *dest, const BigInt *op1, const BigInt *op2) {
    if (op1->digit_count == 0 || op2->digit_count == 0) {
        return bigint_init_unsigned(dest, 0);
    }
    const uint64_t *op1_digits = bigint_ptr(op1);
    const uint64_t *op2_digits = bigint_ptr(op2);

    uint64_t carry;
    mul_overflow(op1_digits[0], op2_digits[0], &dest->data.digit, &carry);
    if (carry == 0 && op1->digit_count == 1 && op2->digit_count == 1) {
        dest->is_negative = (op1->is_negative != op2->is_negative);
        dest->digit_count = 1;
        bigint_normalize(dest);
        return;
    }

    bigint_init_unsigned(dest, 0);

    BigInt bi_64;
    bigint_init_unsigned(&bi_64, 64);

    size_t i = op2->digit_count - 1;
    for (;;) {
        BigInt shifted;
        bigint_shl(&shifted, dest, &bi_64);

        BigInt scalar_result;
        mul_scalar(&scalar_result, op1, op2_digits[i]);

        bigint_add(dest, &scalar_result, &shifted);

        if (i == 0) {
            break;
        }
        i -= 1;
    }

    dest->is_negative = (op1->is_negative != op2->is_negative);
    bigint_normalize(dest);
}

void bigint_shl(BigInt *dest, const BigInt *op1, const BigInt *op2) {
    assert(!op2->is_negative);

    if (op2->digit_count == 0) {
        bigint_init_bigint(dest, op1);
        return;
    }

    if (op1->digit_count == 0) {
        bigint_init_unsigned(dest, 0);
        return;
    }

    if (op2->digit_count != 1) {
        panic("TODO shift left by amount greater than 64 bit integer");
    }

    const uint64_t *op1_digits = bigint_ptr(op1);
    uint64_t shift_amt = bigint_as_unsigned(op2);

    if (op1->digit_count == 1 && shift_amt < 64) {
        dest->data.digit = op1_digits[0] << shift_amt;
        if (dest->data.digit > op1_digits[0]) {
            dest->digit_count = 1;
            dest->is_negative = op1->is_negative;
            return;
        }
    }

    uint64_t digit_shift_count = shift_amt / 64;
    uint64_t leftover_shift_count = shift_amt % 64;

    dest->data.digits = (uint64_t*)malloc(sizeof(uint64_t) + op1->digit_count + digit_shift_count + 1);
    dest->digit_count = digit_shift_count;
    uint64_t carry = 0;
    for (size_t i = 0; i < op1->digit_count; i += 1) {
        uint64_t digit = op1_digits[i];
        dest->data.digits[dest->digit_count] = carry | (digit << leftover_shift_count);
        dest->digit_count += 1;
        if (leftover_shift_count > 0) {
            carry = digit >> (64 - leftover_shift_count);
        } else {
            carry = 0;
        }
    }
    dest->data.digits[dest->digit_count] = carry;
    dest->digit_count += 1;
    dest->is_negative = op1->is_negative;
    bigint_normalize(dest);
}

void bigint_negate(BigInt *dest, const BigInt *op) {
    bigint_init_bigint(dest, op);
    dest->is_negative = !dest->is_negative;
    bigint_normalize(dest);
}

Cmp bigint_cmp(const BigInt *op1, const BigInt *op2) {
    if (op1->is_negative && !op2->is_negative) {
        return CmpLT;
    } else if (!op1->is_negative && op2->is_negative) {
        return CmpGT;
    } else if (op1->digit_count > op2->digit_count) {
        return op1->is_negative ? CmpLT : CmpGT;
    } else if (op2->digit_count > op1->digit_count) {
        return op1->is_negative ? CmpGT : CmpLT;
    } else if (op1->digit_count == 0) {
        return CmpEQ;
    }
    const uint64_t *op1_digits = bigint_ptr(op1);
    const uint64_t *op2_digits = bigint_ptr(op2);
    for (size_t i = op1->digit_count - 1; ;) {
        uint64_t op1_digit = op1_digits[i];
        uint64_t op2_digit = op2_digits[i];

        if (op1_digit > op2_digit) {
            return op1->is_negative ? CmpLT : CmpGT;
        }
        if (op1_digit < op2_digit) {
            return op1->is_negative ? CmpGT : CmpLT;
        }

        if (i == 0) {
            return CmpEQ;
        }
        i -= 1;
    }
}

