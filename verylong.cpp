#include "verylong.h"

#include <iostream>
#include <limits>

//#define LIBSYMBOLICCPLUSPLUS

// Class Data

const Verylong Verylong::zero = Verylong("0");

const Verylong Verylong::one = Verylong("1");

const Verylong Verylong::two = Verylong("2");

// Constructors, Destructors and Conversion operators.

Verylong::Verylong(const string &value)
{
    string s = (value.empty()) ? "0" : value;

    vlsign = (s[0] == '-') ? 1 : 0;          // check for negative sign
    if (ispunct(s[0]))                       // if the first character
        vlstr = s.substr(1, s.length() - 1); // is a punctuation mark.
    else
        vlstr = s;
}

Verylong::Verylong(int n)
{
    if (n < 0) {
        vlsign = 1;
        n = (-n);
    } // check for sign and convert the
    else
        vlsign = 0; // number to positive when negative

    if (n > 0)
        while (n >= 1) // extract the number digit by digit and store
        {              // internally
            vlstr = char(n % 10 + '0') + vlstr;
            n /= 10;
        }
    else
        vlstr = string("0"); // else number is zero
}
/*
Verylong::Verylong(const Verylong &rhs)
    : vlstr(rhs.vlstr)
    , vlsign(rhs.vlsign)
{
}

Verylong &Verylong::operator=(const Verylong &rhs)
{
    if (this == &rhs) return *this;
    vlstr = rhs.vlstr;
    vlsign = rhs.vlsign;
    return *this;
}

Verylong::Verylong(Verylong &&rhs) noexcept
    : vlstr{std::move(rhs.vlstr)}
    , vlsign{std::move(rhs.vlsign)}
{
}

Verylong &Verylong::operator=(Verylong &&rhs) noexcept
{
    // if (this == &rhs) return *this;
    vlstr = std::move(rhs.vlstr);
    vlsign = std::move(rhs.vlsign);
    return *this;
}

Verylong::~Verylong() {}
*/
Verylong::operator int() const
{
    const static Verylong max0(std::numeric_limits<int>::max());
    const static Verylong min0(std::numeric_limits<int>::min() + 1);
    if (*this > max0) {
        std::cerr << "Error: Conversion Verylong->integer is not possible" << std::endl;
        return std::numeric_limits<int>::max();
    } else if (*this < min0) {
        std::cerr << "Error: Conversion Verylong->integer is not possible" << std::endl;
        return std::numeric_limits<int>::min();
    }

    string::const_reverse_iterator j = vlstr.rbegin();
    int factor = 1;
    int number = *j - '0';
    for (++j; j != vlstr.rend(); ++j) {
        factor *= 10;
        number += (*j - '0') * factor;
    }
    if (vlsign) return -number;
    return number;
}

Verylong::operator double() const
{
    string::const_reverse_iterator i = vlstr.rbegin();
    double factor = 1.0;
    double sum = double(*i) - '0';
    for (++i; i != vlstr.rend(); ++i) {
        factor *= 10.0;
        sum += double(*i - '0') * factor;
    }
    if (vlsign) return -sum;
    return sum;
}

Verylong::operator string() const
{
    if (vlstr.length() == 0) return string("0");
    return vlstr;
}

// Various member operators

// Unary - operator
Verylong Verylong::operator-() const
{
    Verylong temp(*this);
    if (temp != zero) temp.vlsign = !vlsign;
    return temp;
}

// Prefix increment operator
Verylong Verylong::operator++() { return *this = *this + one; }

// Postfix increment operator
Verylong Verylong::operator++(int)
{
    Verylong result(*this);
    *this = *this + one;
    return result;
}

// Prefix decrement operator
Verylong Verylong::operator--() { return *this = *this - one; }

// Postfix decrement operator
Verylong Verylong::operator--(int)
{
    Verylong result(*this);
    *this = *this - one;
    return result;
}

Verylong Verylong::operator+=(const Verylong &v) { return *this = *this + v; }

Verylong Verylong::operator-=(const Verylong &v) { return *this = *this - v; }

Verylong Verylong::operator*=(const Verylong &v) { return *this = *this * v; }

Verylong Verylong::operator/=(const Verylong &v) { return *this = *this / v; }

Verylong Verylong::operator%=(const Verylong &v) { return *this = *this % v; }

// Various friendship operators and functions.

Verylong operator+(const Verylong &u, const Verylong &v)
{
    if (u.vlsign ^ v.vlsign) {
        if (u.vlsign == 0)
            return u - abs(v);
        else
            return v - abs(u);
    }

    string temp;
    string tmp(temp);
    char carry = 0;
    string::const_reverse_iterator j;
    string::const_reverse_iterator k;
    for (j = u.vlstr.rbegin(), k = v.vlstr.rbegin(); j != u.vlstr.rend() || k != v.vlstr.rend();) {
        const char d1 = (j == u.vlstr.rend()) ? 0 : *(j++) - '0'; // get digit
        const char d2 = (k == v.vlstr.rend()) ? 0 : *(k++) - '0'; // get digit
        char digitsum = d1 + d2 + carry;                          // add digits
        carry = (digitsum >= 10) ? 1 : 0;
        digitsum -= 10 * carry;
        tmp += char(digitsum + '0');
    }
    temp.assign(tmp.rbegin(), tmp.rend());
    if (carry) temp = '1' + temp; // if carry at end, last digit is 1
    if (u.vlsign) temp = '-' + temp;
    return Verylong(temp);
}

Verylong operator-(const Verylong &u, const Verylong &v)
{
    if (u.vlsign ^ v.vlsign) {
        if (u.vlsign == 0)
            return u + abs(v);
        else
            return -(v + abs(u));
    }
    Verylong w;
    Verylong y;
    int negative;
    if (u.vlsign == 0) { // both u,v are positive
        if (u < v) {
            w = v;
            y = u;
            negative = 1;
        } else {
            w = u;
            y = v;
            negative = 0;
        }
    } else { // both u,v are negative
        if (u < v) {
            w = u;
            y = v;
            negative = 1;
        } else {
            w = v;
            y = u;
            negative = 0;
        }
    }

    string temp;
    string tmp(temp);
    char borrow = 0;
    string::reverse_iterator i;
    string::reverse_iterator j;
    for (i = w.vlstr.rbegin(), j = y.vlstr.rbegin(); i != w.vlstr.rend() || j != y.vlstr.rend();) {
        const char d1 = (i == w.vlstr.rend()) ? 0 : *(i++) - '0';
        const char d2 = (j == y.vlstr.rend()) ? 0 : *(j++) - '0';
        char d = d1 - d2 - borrow;
        borrow = (d < 0) ? 1 : 0;
        d += 10 * borrow;
        tmp += char(d + '0');
    }
    temp.assign(tmp.rbegin(), tmp.rend());

    while (temp[0] == '0') temp = temp.substr(1);
    if (negative) temp = '-' + temp;
    return Verylong(temp);
}

Verylong operator*(const Verylong &u, const Verylong &v)
{
    Verylong pprod("1");
    Verylong tempsum("0");
    string::const_reverse_iterator r = v.vlstr.rbegin();

    for (int j = 0; r != v.vlstr.rend(); ++j, ++r) {
        const int digit = *r - '0'; // extract a digit
        pprod = u.multdigit(digit); // multiplied by the digit
        pprod = pprod.mult10(j);    // "adds" suitable zeros behind
        tempsum = tempsum + pprod;  // result added to tempsum
    }
    tempsum.vlsign = u.vlsign ^ v.vlsign; // to determine sign
    return tempsum;
}

//  This algorithm is the long division algorithm.
Verylong operator/(const Verylong &u, const Verylong &v)
{
    if (v == Verylong::zero) {
        std::cerr << "Error : division by zero" << std::endl;
        return Verylong::zero;
    }

    const Verylong w = abs(u);
    const Verylong y = abs(v);
    if (w < y) return Verylong::zero;
    const int len = u.vlstr.length() - v.vlstr.length();
    Verylong c = Verylong(w.vlstr.substr(0, w.vlstr.length() - len));

    Verylong b;
    Verylong d;
    Verylong quotient = Verylong::zero;
    for (int i = 0; i <= len; i++) {
        quotient = quotient.mult10(1);
        b = d = Verylong::zero; // initialize b and d to 0
        while (b < c) {
            b = b + y;
            d = d + Verylong::one;
        }
        if (c < b) // if b>c, then
        {          // we have added one count too many
            b = b - y;
            d = d - Verylong::one;
        }
        quotient = quotient + d; // add to the quotient
        if (i < len) {
            // partial remainder * 10 and add to next digit
            c = (c - b).mult10(1);
            c += Verylong(w.vlstr[w.vlstr.length() - len + i] - '0');
        }
    }
    quotient.vlsign = u.vlsign ^ v.vlsign; // to determine sign
    return quotient;
}

Verylong operator%(const Verylong &u, const Verylong &v) { return (u - v * (u / v)); }

int operator==(const Verylong &u, const Verylong &v) { return (u.vlsign == v.vlsign && u.vlstr == v.vlstr); }

int operator!=(const Verylong &u, const Verylong &v) { return !(u == v); }

int operator<(const Verylong &u, const Verylong &v)
{
    if (u.vlsign < v.vlsign)
        return 0;
    else if (u.vlsign > v.vlsign)
        return 1;
    // exclusive or (^) to determine sign
    if (u.vlstr.length() < v.vlstr.length())
        return (1 ^ u.vlsign);
    else if (u.vlstr.length() > v.vlstr.length())
        return (0 ^ u.vlsign);
    return (u.vlstr < v.vlstr && !u.vlsign) || (u.vlstr > v.vlstr && u.vlsign);
}

int operator<=(const Verylong &u, const Verylong &v) { return (u < v || u == v); }

int operator>(const Verylong &u, const Verylong &v) { return (!(u < v) && u != v); }

int operator>=(const Verylong &u, const Verylong &v) { return (u > v || u == v); }

// Calculate the absolute value of a number
Verylong abs(const Verylong &v)
{
    Verylong u(v);
    if (u.vlsign) u.vlsign = 0;
    return u;
}

// Calculate the integer square root of a number
// based on the formula (a+b)^2 = a^2 + 2ab + b^2
Verylong sqrt(const Verylong &v)
{
    // if v is negative, error is reported
    if (v.vlsign) {
        std::cerr << "NaN" << std::endl;
        return Verylong::zero;
    }

    const int len1 = v.vlstr.length() + 1;
    const int num = len1 >> 1;

    Verylong digitsum;
    string w(v.vlstr);
    int k = 0;
    // segment the number 2 digits by 2 digits
    if (v.vlstr.length() % 2)
        digitsum = Verylong(w[k++] - '0');
    else {
        digitsum = Verylong((w[k] - '0') * 10 + w[k + 1] - '0');
        k += 2;
    }

    Verylong z;
    Verylong sum;
    string temp;
    // find the first digit of the integer square root
    sum = z = Verylong(int(sqrt(double(digitsum))));
    // store partial result
    temp = char(int(z) + '0');
    digitsum = digitsum - z * z;

    Verylong y;
    Verylong tempsum;
    for (int j = 1; j < num; j++) {
        // get next digit from the number
        digitsum = digitsum.mult10(1) + Verylong(w[k++] - '0');
        y = z + z; // 2*a
        z = digitsum / y;
        tempsum = digitsum.mult10(1) + Verylong(w[k++] - '0');
        digitsum = -y * z.mult10(1) + tempsum - z * z;
        // decrease z by 1 and re-calculate when it is over-estimated.
        while (digitsum < Verylong::zero) {
            --z;
            digitsum = -y * z.mult10(1) + tempsum - z * z;
        }
        temp += char(int(z) + '0');  // store partial result
        z = sum = sum.mult10(1) + z; // update value of the partial result
    }
    Verylong result(temp);
    return result;
}

// Raise a number X to a power of degree
Verylong pow(const Verylong &X, const Verylong &degree)
{
    Verylong N(degree);
    if (N == Verylong::zero) return Verylong::one;
    if (N < Verylong::zero) return Verylong::zero;

    Verylong Y("1");
    Verylong x(X);
    while (true) {
        if (N % Verylong::two != Verylong::zero) {
            Y = Y * x;
            N = N / Verylong::two;
            if (N == Verylong::zero) return Y;
        } else
            N = N / Verylong::two;
        x = x * x;
    }
}

// Double division function
double div(const Verylong &u, const Verylong &v)
{
    // number of significant digits
    const static int decno = std::numeric_limits<double>::digits;

    if (v == Verylong::zero) {
        std::cerr << "ERROR : Division by zero" << std::endl;
        return 0.0;
    }
    if (u == Verylong::zero) return 0.0;

    Verylong w = abs(u);
    const Verylong y = abs(v);
    double qqscale = 1.0;
    while (w < y) {
        w = w.mult10(1);
        qqscale *= 0.1;
    }

    const int len = w.vlstr.length() - y.vlstr.length();
    string temp = w.vlstr.substr(0, w.vlstr.length() - len);
    Verylong c = Verylong(temp);

    Verylong b;
    double qq = 0.0;
    for (int i = 0; i <= len; i++) {
        qq *= 10.0;
        b = Verylong::zero;
        int d = 0; // initialize b and d to 0
        while (b < c) {
            b += y;
            d += 1;
        }

        if (c < b) {
            b -= y;
            d -= 1;
        }                      // if b>c, then we have added one too many
        qq += double(d);       // add to the quotient
        c = (c - b).mult10(1); // the partial remainder * 10
        if (i < len)           // and add to next digit
            c += Verylong(w.vlstr[w.vlstr.length() - len + i] - '0');
    }
    qq *= qqscale;
    int count = 0;

    while (c != Verylong::zero && count < decno) {
        qqscale *= 0.1;
        b = Verylong::zero;
        int d = 0; // initialize b and d to 0
        while (b < c) {
            b += y;
            d += 1;
        }
        if (c < b) {
            b -= y;
            d -= 1;
        } // if b>c, then we have added one too many
        qq += double(d) * qqscale;
        c = (c - b).mult10(1);
        count++;
    }
    if (u.vlsign ^ v.vlsign) qq *= (-1.0); // check for the sign
    return qq;
}

std::ostream &operator<<(std::ostream &s, const Verylong &v)
{
    if (v.vlstr.length() > 0) {
        if (v.vlsign) s << "-";
        s << v.vlstr;
    } else
        s << "0";
    return s;
}

std::istream &operator>>(std::istream &s, Verylong &v)
{
    string temp(10000, ' ');
    s >> temp;
    v = Verylong(temp);
    return s;
}

// Private member functions: multdigit(), mult10().

// Multiply this Verylong number by num
Verylong Verylong::multdigit(int num) const
{
    if (num) {
        int carry = 0;
        string temp;
        string tmp(temp);
        for (auto r = vlstr.rbegin(); r != vlstr.rend(); r++) {
            const int d1 = *r - '0';          // get digit and multiplied by
            int digitprod = d1 * num + carry; // that digit plus carry
            if (digitprod >= 10) {            // if there's a new carry,
                carry = digitprod / 10;       // carry is high digit
                digitprod -= carry * 10;      // result is low digit
            } else {
                carry = 0; // otherwise carry is 0
            }
            tmp += char(digitprod + '0'); // insert char in string
        }
        temp.assign(tmp.rbegin(), tmp.rend());
        if (carry) temp = char(carry + '0') + temp; // if carry at end,
        Verylong result(temp);
        return result;
    } else
        return zero;
}

// Multiply this Verylong number by 10*num
Verylong Verylong::mult10(int num) const
{
    if (*this != zero) {
        string temp;
        for (int j = 0; j < num; j++) temp += '0';
        Verylong result(vlstr + temp);
        if (vlsign) result = -result;
        return result;
    } else
        return zero;
}

template <> Verylong zero(Verylong /*unused*/) { return Verylong::zero; }
template <> Verylong one(Verylong /*unused*/) { return Verylong::one; }

//#undef LIBSYMBOLICCPLUSPLUS
// template <> Verylong zero(Verylong);
// template <> Verylong one(Verylong);
