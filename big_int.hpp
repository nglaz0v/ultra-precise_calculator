#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include "bitset_arithmetic.hpp" // Recipe 11.20
#include <bitset>
#include <iterator>
#include <sstream>
#include <vector>
template <unsigned int N> class BigInt {
    using self = BigInt;

public:
    BigInt()
        : bits()
    {
    }
    BigInt(unsigned long x)
    {
        int n = 0;
        while (x) {
            bits[n++] = x & 0x1;
            x >>= 1;
        }
    }
    explicit BigInt(const std::bitset<N> &x)
        : bits(x)
    {
    }
    explicit BigInt(const std::string &x)
        : BigInt(std::stoul(x))
    {
    }
    ~BigInt() = default;
    BigInt(const BigInt &) = default;
    BigInt &operator=(const BigInt &) = default;
    BigInt(BigInt &&) noexcept = default;
    BigInt &operator=(BigInt &&) noexcept = default;
    // public functions
    bool operator[](int n) const { return bits[n]; }
    unsigned long toUlong() const { return bits.to_ulong(); }
    operator std::string() const
    {
        auto x = *this;
        if (x == 0) { return std::string("0"); }
        std::vector<int> v;
        while (x > 0) {
            v.push_back((x % 10).toUlong());
            x /= 10;
        }
        std::stringstream str;
        copy(v.rbegin(), v.rend(), std::ostream_iterator<int>(str, ""));
        return str.str();
    }
    // operators
    self &operator<<=(unsigned int n)
    {
        bits <<= n;
        return *this;
    }
    self &operator>>=(unsigned int n)
    {
        bits >>= n;
        return *this;
    }
    self operator++(int)
    {
        self i = *this;
        operator++();
        return i;
    }
    self operator--(int)
    {
        self i = *this;
        operator--();
        return i;
    }
    self &operator++()
    {
        bool carry = false;
        bits[0] = fullAdder(bits[0], 1, carry);
        for (int i = 1; i < N; i++) { bits[i] = fullAdder(bits[i], 0, carry); }
        return *this;
    }
    self &operator--()
    {
        bool borrow = false;
        bits[0] = fullSubtractor(bits[0], 1, borrow);
        for (int i = 1; i < N; i++) { bits[i] = fullSubtractor(bits[i], 0, borrow); }
        return *this;
    }
    self &operator+=(const self &x)
    {
        bitsetAdd(bits, x.bits);
        return *this;
    }
    self &operator-=(const self &x)
    {
        bitsetSubtract(bits, x.bits);
        return *this;
    }
    self &operator*=(const self &x)
    {
        bitsetMultiply(bits, x.bits);
        return *this;
    }
    self &operator/=(const self &x)
    {
        std::bitset<N> tmp;
        bitsetDivide(bits, x.bits, bits, tmp);
        return *this;
    }
    self &operator%=(const self &x)
    {
        std::bitset<N> tmp;
        bitsetDivide(bits, x.bits, tmp, bits);
        return *this;
    }
    self operator~() const { return ~bits; }
    self &operator&=(self x)
    {
        bits &= x.bits;
        return *this;
    }
    self &operator|=(self x)
    {
        bits |= x.bits;
        return *this;
    }
    self &operator^=(self x)
    {
        bits ^= x.bits;
        return *this;
    }
    self operator-() const { return self(~bits) + self(1); }
    // friend functions
    friend self operator<<(self x, unsigned int n) { return x <<= n; }
    friend self operator>>(self x, unsigned int n) { return x >>= n; }
    friend self operator+(self x, const self &y) { return x += y; }
    friend self operator-(self x, const self &y) { return x -= y; }
    friend self operator*(self x, const self &y) { return x *= y; }
    friend self operator/(self x, const self &y) { return x /= y; }
    friend self operator%(self x, const self &y) { return x %= y; }
    friend self operator^(self x, const self &y) { return x ^= y; }
    friend self operator&(self x, const self &y) { return x &= y; }
    friend self operator|(self x, const self &y) { return x |= y; }
    // comparison operators
    friend bool operator==(const self &x, const self &y) { return x.bits == y.bits; }
    friend bool operator!=(const self &x, const self &y) { return x.bits != y.bits; }
    friend bool operator>(const self &x, const self &y) { return bitsetGt(x.bits, y.bits); }
    friend bool operator<(const self &x, const self &y) { return bitsetLt(x.bits, y.bits); }
    friend bool operator>=(const self &x, const self &y) { return bitsetGtEq(x.bits, y.bits); }
    friend bool operator<=(const self &x, const self &y) { return bitsetLtEq(x.bits, y.bits); }

private:
    std::bitset<N> bits;
};

#endif // BIG_INT_HPP
