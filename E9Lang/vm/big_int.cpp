//
// Created by egor9814 on 02.04.18.
//

#include "math.hpp"

static auto MAX_MAG_LENGTH = INT32_MAX / sizeof(int) + 1;
static auto LONG_MASK = 0xffffffffl;


inline long sarl(long val, long n) {
    __asm__("sar %%rbx, %%rax;":"=a"(val):"a"(val), "b"(n));
    return val;
}

inline int sari(int val, int n) {
    __asm__("sar %%ebx, %%eax;":"=a"(val):"a"(val), "b"(n));
    return val;
}


void destructiveMulAdd(e9lang::math::BigInt::magnitude_t &x, int y, int z) {
    auto ylong = y & LONG_MASK;
    auto zlong = z & LONG_MASK;
    auto len = x.size();

    long product = 0;
    long carry = 0;
    for (auto i = len - 1; i < len; i--) {
        product = ylong * (x[i] & LONG_MASK) + carry;
        x[i] = (int) product;
        carry = sarl(product, 32);
    }

    long sum = (x[len - 1] & LONG_MASK) + zlong;
    x[len - 1] = (int) sum;
    carry = sarl(sum, 32);
    for (auto i = len - 2; i < len; i--) {
        sum = (x[i] & LONG_MASK) + carry;
        x[i] = (int) sum;
        carry = sarl(sum, 32);
    }
}


void reportOverflow() {
    throw "BigInteger would overflow supported range";
}


e9lang::math::BigInt::magnitude_t trustedStripLeadingZeroInts(e9lang::math::BigInt::magnitude_t &val) {
    auto keep = val.begin();
    auto end = val.end();
    for (; keep != end && *keep != 0; keep++);
    e9lang::math::BigInt::magnitude_t result;
    if (keep == val.begin())
        return val;
    for (; keep != end; keep++)
        result.push_back(*keep);
    return result;
}


int compareMagnitude(e9lang::math::BigInt &x, e9lang::math::BigInt &y) {
    auto len1 = x.getMagnitude().size();
    auto len2 = y.getMagnitude().size();
    if (len1 < len2)
        return -1;
    if (len1 > len2)
        return 1;
    for (auto i = 0ul; i < len1; i++) {
        auto a = x.getMagnitude()[i];
        auto b = y.getMagnitude()[i];
        if (a != b)
            return ((a & LONG_MASK) < (b & LONG_MASK) ? -1 : 1);
    }
    return 0;
}

int compare(e9lang::math::BigInt &x, e9lang::math::BigInt &y) {
    if (x.getSignum() == y.getSignum()) {
        switch (x.getSignum()) {
            case 1:
                return compareMagnitude(x, y);
            case -1:
                return compareMagnitude(y, x);
            default:
                return 0;
        }
    }
    return x.getSignum() > y.getSignum() ? 1 : -1;
}


e9lang::math::BigInt::BigInt(const std::string &value) {
    unsigned long cursor = 0, numDigits;
    auto len = value.length();

    if (len == 0)
        throw "Zero length BigInteger";

    int sign = 1;
    auto index1 = value.find('-');
    auto index2 = value.find('+');
    if (index1 >= 0) {
        if (index1 != 0 || index2 >= 0)
            throw "Illegal embedded sign character";
        sign = -1;
        cursor = 1;
    } else if (index2 >= 0) {
        if (index2 != 0)
            throw "Illegal embedded sign character";
        cursor = 1;
    }
    if (cursor == len)
        throw "Zero length BigInteger";

    while (cursor < len && (value[cursor] - '0') == 0)
        cursor++;

    if (cursor == len) {
        signum = 0;
        mag = Zero->mag;
        return;
    }

    numDigits = len - cursor;
    signum = sign;

    auto numBits = ((numDigits * 3402) >> 10) + 1;
    if (numBits + 31 >= (1l << 32)) {
        reportOverflow();
    }
    auto numWords = (int) (numBits + 31);
    numWords = sari(numWords, 5);
    magnitude_t magnitude((size_t) numWords);

    auto firstGroupLen = numDigits % 9;
    if (firstGroupLen == 0)
        firstGroupLen = 9;
    auto group = value.substr(cursor, firstGroupLen);
    cursor += firstGroupLen;
    magnitude[numWords - 1] = std::stoi(group);
    if (magnitude[numWords - 1] < 0)
        throw "Illegal digit";

    auto superRadix = 0x3b9aca00;
    auto groupValue = 0;
    while (cursor < len) {
        group = value.substr(cursor, 9);
        cursor += 9;
        groupValue = std::stoi(group);
        if (groupValue < 0)
            throw "Illegal digit";
        destructiveMulAdd(magnitude, superRadix, groupValue);
    }

    mag = trustedStripLeadingZeroInts(magnitude);
    if (mag.size() >= MAX_MAG_LENGTH) {
        checkRange();
    }
}

e9lang::math::BigInt::BigInt(const e9lang::math::BigInt::magnitude_t &mag, int signum)
        : mag(mag), signum(mag.empty() ? 0 : signum) {
    if (mag.size() >= MAX_MAG_LENGTH) {
        checkRange();
    }
}


e9lang::math::BigInt::magnitude_t add(e9lang::math::BigInt::magnitude_t &x, e9lang::math::BigInt::magnitude_t &y) {
    if (x.size() < y.size()) {
        return add(y, x);
    }

    auto xi = x.size();
    auto yi = y.size();
    e9lang::math::BigInt::magnitude_t result(xi, 0);
    long sum = 0;
    if (yi == 1) {
        sum = (x[--xi] & LONG_MASK) + (y[0] & LONG_MASK);
        result[xi] = (int) sum;
    } else {
        while (yi < y.size()) {
            sum = (x[--xi] & LONG_MASK) + (y[--yi] & LONG_MASK)
                  + (sarl(sum, 32));
            result[xi] = (int) sum;
        }
    }

    bool carry = (sarl(sum, 32) != 0);
    while (xi < x.size() && carry) {
        carry = ((result[--xi] = x[xi] + 1) == 0);
    }

    while (xi < x.size()) {
        result[--xi] = x[xi];
    }

    if (carry) {
        e9lang::math::BigInt::magnitude_t bigger;
        bigger.push_back(1);
        for (auto &i : result) {
            bigger.push_back(i);
        }
        return bigger;
    }
    return result;
}

e9lang::math::BigInt::magnitude_t sub(e9lang::math::BigInt::magnitude_t &x, e9lang::math::BigInt::magnitude_t &y);

e9lang::math::BigInt e9lang::math::BigInt::operator+(const e9lang::math::BigInt &val) {
    if (val.signum == 0)
        return *this;
    if (signum == 0)
        return val;
    if (val.signum == signum) {
        return BigInt(add(mag, val.mag), signum);
    }

    auto cmp = compareMagnitude(*this, val);
    if (cmp == 0)
        return *Zero;
    magnitude_t result = (cmp > 0 ? sub(mag, val.mag) : sub(val.mag, mag));
    result = trustedStripLeadingZeroInts(result);
    return BigInt(result, cmp == signum ? 1 : -1);
}


e9lang::math::BigInt::magnitude_t sub(e9lang::math::BigInt::magnitude_t &x, e9lang::math::BigInt::magnitude_t &y) {
    auto xi = x.size();
    auto yi = y.size();
    e9lang::math::BigInt::magnitude_t result(xi);
    long diff = 0;

    while (--yi < y.size()) {
        diff = (x[--xi] & LONG_MASK) - (y[yi] & LONG_MASK)
               + (diff >> 32);
        result[xi] = (int) diff;
    }

    bool borrow = (diff >> 32 != 0);
    while (xi < x.size() && borrow) {
        borrow = ((result[--xi] = x[xi] - 1) == -1);
    }

    while (xi < x.size()) {
        result[--xi] = x[xi];
    }

    return result;
}

e9lang::math::BigInt e9lang::math::BigInt::operator-(const e9lang::math::BigInt &val) {
    if (val.signum == 0)
        return *this;
    if (signum == 0)
        return operator_negate();
    if (signum != val.signum)
        return BigInt(add(mag, val.mag), signum);

    auto cmp = compareMagnitude(*this, val);
    if (cmp == 0)
        return *Zero;
    magnitude_t result = (cmp > 0 ? sub(mag, val.mag) : sub(val.mag, mag));
    result = trustedStripLeadingZeroInts(result);
    return BigInt(result, cmp == signum ? 1 : -1);
}


int bitCount(int i) {
    // HD, Figure 5-2
    i = i - (sari(i, 1) & 0x55555555);
    i = (i & 0x33333333) + (sari(i, 2) & 0x33333333);
    i = (i + sari(i, 4)) & 0x0f0f0f0f;
    i = i + sari(i, 8);
    i = i + sari(i, 16);
    return i & 0x3f;
}

int numberOfTrailingZeros(int i) {
    // HD, Figure 5-14
    int y;
    if (i == 0) return 32;
    int n = 31;
    y = i << 16;
    if (y != 0) {
        n = n - 16;
        i = y;
    }
    y = i << 8;
    if (y != 0) {
        n = n - 8;
        i = y;
    }
    y = i << 4;
    if (y != 0) {
        n = n - 4;
        i = y;
    }
    y = i << 2;
    if (y != 0) {
        n = n - 2;
        i = y;
    }
    return n - sari(i << 1, 31);
}

e9lang::math::BigInt::magnitude_t shiftLeft(e9lang::math::BigInt::magnitude_t &x, int);

e9lang::math::BigInt mul(e9lang::math::BigInt::magnitude_t &x, int y, int sign) {
    if (bitCount(y) == 1)
        return e9lang::math::BigInt(shiftLeft(x, numberOfTrailingZeros(y)), sign);

    auto xlen = x.size();
    e9lang::math::BigInt::magnitude_t result(xlen + 1, 0);
    long carry = 0;
    long yl = y & LONG_MASK;
    auto rstart = result.size() - 1;
    for (auto i = xlen - 1; i < xlen; i--) {
        auto product = (x[i] & LONG_MASK) * yl + carry;
        result[rstart--] = (int) product;
        carry = sarl(product, 32);
    }
    if (carry == 0l) {
        result.erase(result.begin());
    } else {
        result[rstart] = (int) carry;
    }
    return e9lang::math::BigInt(result, sign);
}

e9lang::math::BigInt::magnitude_t mul(e9lang::math::BigInt::magnitude_t &x, size_t xlen,
                                      e9lang::math::BigInt::magnitude_t &y, size_t ylen) {
    auto xi = xlen - 1;
    auto yi = ylen - 1;

    e9lang::math::BigInt::magnitude_t result(xlen + ylen);

    long carry = 0;
    for (auto j = yi, k = yi + 1 + xi; j < ylen; j--, k--) {
        auto product = (y[j] & LONG_MASK) * (x[xi] & LONG_MASK) + carry;
        result[k] = (int) product;
        carry = sarl(product, 32);
    }
    result[xi] = (int) carry;

    for (auto i = xi - 1; i < xlen; i--) {
        carry = 0;
        for (auto j = yi, k = yi + 1 + i; j < ylen; j--, k--) {
            auto product = (y[j] & LONG_MASK) * (x[i] & LONG_MASK)
                           + (result[k] & LONG_MASK) + carry;
            result[k] = (int) product;
            carry = sarl(product, 32);
        }
        result[i] = (int) carry;
    }
    return result;
}

e9lang::math::BigInt getLower(e9lang::math::BigInt &x, size_t n) {
    auto len = x.getMagnitude().size();
    if (len <= n)
        return x.abs();

    e9lang::math::BigInt::magnitude_t result;
    auto it = x.getMagnitude().end();
    for (auto i = 0; i < n; i++, it--);
    for (auto end = x.getMagnitude().end(); it != end; it++) {
        result.push_back(*it);
    }
    return e9lang::math::BigInt(trustedStripLeadingZeroInts(result), 1);
}

e9lang::math::BigInt getUpper(e9lang::math::BigInt &x, size_t n) {
    auto len = x.getMagnitude().size();
    if (len <= n)
        return *e9lang::math::BigInt::Zero;

    auto upperLen = len - n;
    e9lang::math::BigInt::magnitude_t result;
    auto it = x.getMagnitude().begin();
    for (auto i = 0ul; i < upperLen; i++, it++)
        result.push_back(*it);
    return e9lang::math::BigInt(trustedStripLeadingZeroInts(result), 1);
}

e9lang::math::BigInt getToomSlice(e9lang::math::BigInt &x,
                                  size_t lowerSize, size_t upperSize, size_t slice, size_t fullsize) {
    size_t start, end, sliceSize, len, offset;
    len = x.getMagnitude().size();
    offset = fullsize - len;

    if (slice == 0) {
        start = 0 - offset;
        end = upperSize - 1 - offset;
    } else {
        start = upperSize + (slice - 1) * lowerSize - offset;
        end = start + lowerSize - 1;
    }

    if (start >= -1ul) {
        start = 0;
    }
    if (end >= -1ul) {
        return *e9lang::math::BigInt::Zero;
    }

    sliceSize = (end - start) + 1;
    if (sliceSize > -1ul) {
        return *e9lang::math::BigInt::Zero;
    }

    if (start == 0 && sliceSize >= len) {
        return x.abs();
    }

    e9lang::math::BigInt::magnitude_t result;
    auto it = x.getMagnitude().begin();
    for (auto i = 0ul; i < start; i++, it++);
    for (auto i = 0ul; i < sliceSize; i++, it++)
        result.push_back(*it);

    return e9lang::math::BigInt(trustedStripLeadingZeroInts(result), 1);
}

e9lang::math::BigInt exactDivideBy3(const e9lang::math::BigInt &num){
    auto x = const_cast<e9lang::math::BigInt*>(&num);

    auto len = x->getMagnitude().size();
    e9lang::math::BigInt::magnitude_t result(len, 0);
    long y, w, q, borrow = 0;
    for(auto i = len-1; i < len; i--){
        y = (x->getMagnitude()[i] & LONG_MASK);
        w = y - borrow;
        borrow = borrow > y ? 1 : 0;

        q = (w * 0xaaaaaaabl) & LONG_MASK;
        result[i] = (int)q;

        if(q >= 0x55555556l){
            borrow++;
            if(q >= 0xaaaaaaabl)
                borrow++;
        }
    }
    return e9lang::math::BigInt(trustedStripLeadingZeroInts(result), x->getSignum());
}

e9lang::math::BigInt e9lang::math::BigInt::operator*(const e9lang::math::BigInt &val) {
    if (signum == 0 || val.signum == 0)
        return *Zero;

    auto xlen = mag.size();

    if (&val == this && xlen > 20)
        return square();

    auto ylen = val.mag.size();

    if ((xlen < 80) || (ylen < 80)) {
        auto resultSign = signum == val.signum ? 1 : -1;
        if (ylen == 1)
            return mul(mag, val.mag[0], resultSign);
        if (xlen == 1)
            return mul(val.mag, mag[0], resultSign);
        auto result = mul(mag, xlen, val.mag, ylen);
        result = trustedStripLeadingZeroInts(result);
        return BigInt(result, resultSign);
    } else {
        if ((xlen < 240) && (ylen < 240)) {
            auto half = (std::max(xlen, ylen) + 1) / 2;

            auto xl = getLower(*this, half);
            auto xh = getUpper(*this, half);
            auto yl = getLower(const_cast<BigInt &>(val), half);
            auto yh = getUpper(const_cast<BigInt &>(val), half);

            auto p1 = xh * yh;
            auto p2 = xl * yl;

            auto p3 = (xh + xl) * (yh + yl);

            BigInt shift(std::to_string(32 * half));
            auto result = (p1 << shift) + ((p3 - p1 - p2) << shift) + p2;

            if (signum != val.signum) {
                return result.operator_negate();
            } else {
                return result;
            }
        } else {
            auto largest = std::max(xlen, ylen);

            auto k = (largest + 2) / 3;

            auto r = largest - 2 * k;

            auto x0 = getToomSlice(*this, k, r, 2, largest);
            auto x1 = getToomSlice(*this, k, r, 1, largest);
            auto x2 = getToomSlice(*this, k, r, 0, largest);
            auto y0 = getToomSlice(const_cast<BigInt &>(val), k, r, 2, largest);
            auto y1 = getToomSlice(const_cast<BigInt &>(val), k, r, 1, largest);
            auto y2 = getToomSlice(const_cast<BigInt &>(val), k, r, 0, largest);

            auto v0 = x0 * y0;
            auto dx1 = x2 + x0;
            auto dy1 = y2 + y0;
            auto vm1 = (dx1 - x1) * (dy1 - y1);
            dx1 = dx1 + x1;
            dy1 = dy1 + y1;
            auto v1 = dx1 * dy1;
            auto v2 = (((dx1 + x2) << *One) - x0) * (((dy1 + y2) << *One) - y0);
            auto vinf = x2 * y2;

            auto t2 = exactDivideBy3(v2 - vm1);
            auto tm1 = (v1 - vm1) >> *One;
            auto t1 = v1 - v0;
            t2 = (t2 - t1) >> *One;
            t1 = t1 - tm1 - vinf;
            t2 = t2 - (vinf << *One);
            tm1 = tm1 - t2;

            BigInt ss(std::to_string(k * 32));
            auto result = (((((((vinf << ss) + t2) << ss) + t1) << ss) + tm1) << ss) + v0;

            if (signum != val.signum)
                return result.operator_negate();
            else
                return result;
        }
    }
}


int mulAdd(e9lang::math::BigInt::magnitude_t &out, e9lang::math::BigInt::magnitude_t &in,
           size_t offset, size_t len, int k){
    auto kl = k & LONG_MASK;
    auto carry = 0l;

    offset = out.size() - offset - 1;
    for(auto j=len-1; j < len; j--){
        auto product = (in[j] & LONG_MASK) * kl + (out[offset] & LONG_MASK) + carry;
        out[offset--] = (int)product;
        carry = sarl(product, 32);
    }
    return (int)carry;
}

int addOne(e9lang::math::BigInt::magnitude_t &x, size_t offset, size_t len, int carry){
    offset = x.size() - offset - len - 1;
    auto t = (x[offset] & LONG_MASK) + (carry & LONG_MASK);

    x[offset] = (int)t;
    if(sarl(t, 32) == 0){
        return 0;
    }
    while (--len < len){
        if(--offset == -1ul){
            return 1;
        } else {
            x[offset]++;
            if(x[offset] != 0)
                return 0;
        }
    }
    return 1;
}

void primitiveLeftShift(e9lang::math::BigInt::magnitude_t &x, size_t len, int n){
    if(len == 0 || n == 0)
        return;

    auto n2 = 32 - n;
    for(auto i = 0ul, c=x[i], m=i+len-1; i < m; i++){
        auto b = c;
        c = x[i+1];
        x[i] = (b << n) | sari(c, n2);
    }
    x[len-1] <<= n;
}

e9lang::math::BigInt::magnitude_t squareToLen(e9lang::math::BigInt::magnitude_t &mag, size_t len){
    auto rlen = len << 1;
    e9lang::math::BigInt::magnitude_t r;

    int lastProductLowWord = 0;
    for(auto j = 0ul, i = 0ul; j < len; j++){
        auto piece = (mag[j] & LONG_MASK);
        auto product = piece * piece;
        r[i++] = (lastProductLowWord << 31) | (int)(sarl(product, 33));
        r[i++] = (int)(sarl(product, 1));
        lastProductLowWord = (int)product;
    }

    for(auto i = len, offset = 1ul; i > 0; i--, offset += 2){
        auto t = mag[i-1];
        t = mulAdd(r, mag, offset, i-1, t);
        addOne(r, offset-1, i, t);
    }

    primitiveLeftShift(r, rlen, 1);
    r[rlen-1] |= mag[len-1] & 1;

    return r;
}

e9lang::math::BigInt e9lang::math::BigInt::square() {
    if(signum == 0)
        return *Zero;
    auto len = mag.size();

    if(len < 128){
        auto result = squareToLen(mag, len);
        return BigInt(trustedStripLeadingZeroInts(result), 1);
    } else {
        if(len < 216){
            auto half = (len+1) / 2;
            auto xl = getLower(*this, half);
            auto xh = getUpper(*this, half);

            auto xls = xl.square();
            auto xhs = xh.square();

            BigInt half32(std::to_string(half * 32));
            return (xhs << half32 + ((xl + xh).square() - (xhs + xls))) << half32 + xls;
        } else {
            auto k = (len+2)/3;
            auto r = len - 2*k;

            auto a0 = getToomSlice(*this, k, r, 2, len);
            auto a1 = getToomSlice(*this, k, r, 1, len);
            auto a2 = getToomSlice(*this, k, r, 0, len);

            auto v0 = a0.square();
            auto da1 = a2 + a0;
            auto vm1 = (da1 - a1).square();
            da1 = da1 + a1;
            auto v1 = da1.square();
            auto vinf = a2.square();
            auto v2 = ((da1 + a2) << *One - a0).square();

            auto t2 = exactDivideBy3(v2 - vm1);
            auto tm1 = (v1 - vm1) >> *One;
            auto t1 = v1 - v0;
            t2 = (t2 - t1) >> *One;
            t1 = t1 - tm1 - vinf;
            t2 = t2 - (vinf << *One);
            tm1 = tm1 - t2;

            BigInt ss(std::to_string(k*32));
            return ((((vinf << ss) + t2) << ss + t1) << ss + tm1) << ss + v0;
        }
    }
}


e9lang::math::BigInt e9lang::math::BigInt::operator/(const e9lang::math::BigInt &val) {
    if(val.mag.size() < 80 || mag.size() - val.mag.size() < 40){

    }
}
