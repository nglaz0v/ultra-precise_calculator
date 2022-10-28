/*
    SymbolicC++ : An object oriented computer algebra system written in C++

    Copyright (C) 2008 Yorick Hardy and Willi-Hans Steeb

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef VERYLONG_H
#define VERYLONG_H

#include "identity.h"
#include <cmath>
#include <string>

using std::string;

//! Very Long Integer Class
class Verylong final {
private:
    // Data Fields
    string vlstr; // The string is stored in reverse order.
    int vlsign;   // Sign of Verylong: +=>0; -=>1

    // Private member functions
    Verylong multdigit(int) const;
    Verylong mult10(int) const;

public:
    // Constructors and destructor
    explicit Verylong(const string & = "0");
    /*explicit*/ Verylong(int);
    ~Verylong() = default;

    Verylong(const Verylong &) = default;
    Verylong &operator=(const Verylong &) = default;

    Verylong(Verylong &&) noexcept = default;
    Verylong &operator=(Verylong &&) noexcept = default;

    // Conversion operators
    operator int() const;
    operator double() const;
    operator string() const;

    // Arithmetic operators and Relational operators
    Verylong operator-() const; // negate  operator
    Verylong operator++();      // prefix  increment operator
    Verylong operator++(int);   // postfix increment operator
    Verylong operator--();      // prefix  decrement operator
    Verylong operator--(int);   // postfix decrement operator

    Verylong operator+=(const Verylong &);
    Verylong operator-=(const Verylong &);
    Verylong operator*=(const Verylong &);
    Verylong operator/=(const Verylong &);
    Verylong operator%=(const Verylong &);

    friend Verylong operator+(const Verylong &, const Verylong &);
    friend Verylong operator-(const Verylong &, const Verylong &);
    friend Verylong operator*(const Verylong &, const Verylong &);
    friend Verylong operator/(const Verylong &, const Verylong &);
    friend Verylong operator%(const Verylong &, const Verylong &);

    friend int operator==(const Verylong &, const Verylong &);
    friend int operator!=(const Verylong &, const Verylong &);
    friend int operator<(const Verylong &, const Verylong &);
    friend int operator<=(const Verylong &, const Verylong &);
    friend int operator>(const Verylong &, const Verylong &);
    friend int operator>=(const Verylong &, const Verylong &);

    // Other functions
    friend Verylong abs(const Verylong &);
    friend Verylong sqrt(const Verylong &);
    friend Verylong pow(const Verylong &, const Verylong &);
    friend double div(const Verylong &, const Verylong &);

    // Class Data
    static const Verylong zero;
    static const Verylong one;
    static const Verylong two;

    // I/O stream functions
    friend std::ostream &operator<<(std::ostream &, const Verylong &);
    friend std::istream &operator>>(std::istream &, Verylong &);
};

#endif // VERYLONG_H
