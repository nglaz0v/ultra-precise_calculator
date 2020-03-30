#ifndef VERYLONG_H
#define VERYLONG_H

#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include "identity.h"

using std::string;

//! Very Long Integer Class
class Verylong
{
   private:
      // Data Fields
      string vlstr;     // The string is stored in reverse order.
      int    vlsign;    // Sign of Verylong: +=>0; -=>1

      // Private member functions
      Verylong multdigit(int) const;
      Verylong mult10(int) const;

   public:
      // Constructors and destructor
      Verylong(const string& = "0");
      Verylong(int);
      Verylong(const Verylong&);
      ~Verylong();

      // Conversion operators
      operator int () const;
      operator double () const;
      operator string () const;

      // Arithmetic operators and Relational operators
      const Verylong & operator = (const Verylong&);  // assignment operator
      Verylong operator - () const;     // negate  operator
      Verylong operator ++ ();          // prefix  increment operator
      Verylong operator ++ (int);       // postfix increment operator
      Verylong operator -- ();          // prefix  decrement operator
      Verylong operator -- (int);       // postfix decrement operator

      Verylong operator += (const Verylong&);
      Verylong operator -= (const Verylong&);
      Verylong operator *= (const Verylong&);
      Verylong operator /= (const Verylong&);
      Verylong operator %= (const Verylong&);

      friend Verylong operator + (const Verylong&,const Verylong&);
      friend Verylong operator - (const Verylong&,const Verylong&);
      friend Verylong operator * (const Verylong&,const Verylong&);
      friend Verylong operator / (const Verylong&,const Verylong&);
      friend Verylong operator % (const Verylong&,const Verylong&);

      friend int operator == (const Verylong&,const Verylong&);
      friend int operator != (const Verylong&,const Verylong&);
      friend int operator <  (const Verylong&,const Verylong&);
      friend int operator <= (const Verylong&,const Verylong&);
      friend int operator >  (const Verylong&,const Verylong&);
      friend int operator >= (const Verylong&,const Verylong&);

      // Other functions
      friend Verylong abs(const Verylong&);
      friend Verylong sqrt(const Verylong&);
      friend Verylong pow(const Verylong&,const Verylong&);
      friend double div(const Verylong&,const Verylong&);

      // Class Data
      static const Verylong zero;
      static const Verylong one;
      static const Verylong two;

      // I/O stream functions
      friend std::ostream & operator << (std::ostream&, const Verylong&);
      friend std::istream & operator >> (std::istream&, Verylong&);
};

#endif // VERYLONG_H
