//
// Created by egor9814 on 03.04.18.
//

#include "math.hpp"

e9lang::math::MutableBigInt::MutableBigInt()
        : val(1, 0), len(0) {}

e9lang::math::MutableBigInt::MutableBigInt(int value)
        : val(1, value), len(1) {}

e9lang::math::MutableBigInt::MutableBigInt(const e9lang::math::BigInt::magnitude_t &mag)
        : val(mag), len(mag.size()) {}

e9lang::math::MutableBigInt::MutableBigInt(const e9lang::math::BigInt &value)
        : val(const_cast<BigInt&>(value).getMagnitude()), len(val.size()) {}

e9lang::math::MutableBigInt::MutableBigInt(const e9lang::math::MutableBigInt &value)
        : len(value.len) {
    auto it = value.val.begin();
    for(auto i = 0ul; i < value.offset; i++, it++);
    for(auto i = 0ul; i < len; i++)
        val.push_back(value.val[i]);
}

void e9lang::math::MutableBigInt::ones(size_t n) {
    if(n > val.size())
        val = std::move(BigInt::magnitude_t(n, -1));
    else
        for(auto &i : val)
            i = -1;
    offset = 0;
    len = n;
}

e9lang::math::BigInt::magnitude_t e9lang::math::MutableBigInt::getMagnitude() {
    if(offset > 0 || val.size() != len){
        BigInt::magnitude_t result;
        auto it = val.begin();
        for(auto i = 0ul; i < offset; i++, it++);
        for(auto i = 0ul; i < len; i++, it++)
            result.push_back(*it);
    }
    return val;
}

long e9lang::math::MutableBigInt::toLong() {
    if(len == 0)
        return 0;
    auto d = val[offset] & 0xffffffffl;
    return len == 2 ? d << 32 | (val[offset+1] & 0xffffffffl) : d;
}

e9lang::math::BigInt e9lang::math::MutableBigInt::toBigInt(int sign) {
    if(len == 0 || sign == 0)
        return *BigInt::Zero;
    return BigInt(getMagnitude(), sign);
}

e9lang::math::BigInt e9lang::math::MutableBigInt::toBigInt() {
    normalize();
    return toBigInt(isZero() ? 0 : 1);
}

long e9lang::math::MutableBigInt::toComactValue(int sign) {
    if(len == 0 || sign == 0)
        return 0;
    auto mag = getMagnitude();
    auto len = mag.size();
    auto d = mag[0];

    if(len > 2 || (d < 0 && len == 2))
        return INT64_MIN;
    auto v = len == 2 ? ((mag[1] & 0xffffffffl) | (d & 0xffffffffl) << 32) : d & 0xffffffffl;
    return sign == -1 ? -v : v;
}

void e9lang::math::MutableBigInt::clear() {
    offset = len = 0;
    for(auto &i : val)
        i = 0;
}

void e9lang::math::MutableBigInt::reset() {
    offset = len = 0;
}

int e9lang::math::MutableBigInt::compare(const e9lang::math::MutableBigInt &x) {
    auto xlen = x.len;
    if(len < xlen)
        return -1;
    if(len > xlen)
        return 1;

    for(auto i = offset, j = x.offset; i < len + offset; i++, j++){
        int x1 = val[i] + 0x80000000;
        int x2 = x.val[j] + 0x8000000;
        if(x1 < x2)
            return -1;
        if(x1 > x2)
            return 1;
    }
    return 0;
}

int e9lang::math::MutableBigInt::compareShifted(const e9lang::math::MutableBigInt &x, size_t ints) {
    auto xlen = x.len;
    auto ylen = len - ints;
    if(ylen < xlen)
        return -1;
    if(ylen > xlen)
        return 1;

    for(auto i = offset, j = x.offset; i < ylen + offset; i++, j++){
        int x1 = val[i] + 0x80000000;
        int x2 = x.val[j] + 0x8000000;
        if(x1 < x2)
            return -1;
        if(x1 > x2)
            return 1;
    }
    return 0;
}
