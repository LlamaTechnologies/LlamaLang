#pragma once
#include <cstdint>

typedef struct { uint64_t v[2]; } float128_t;

struct BigFloat {
    float128_t value;
    bool overflow;
};

