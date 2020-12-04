#ifndef IDENTITY_H
#define IDENTITY_H

template <class T> T zero(T)
{
    static const T z = T() - T();
    return z;
}

template <class T> T one(T)
{
    static T z(zero(T()));
    static const T o = ++z;
    return o;
}

template <> char zero(char);
template <> char one(char);
template <> short zero(short);
template <> short one(short);
template <> int zero(int);
template <> int one(int);
template <> long zero(long);
template <> long one(long);
template <> float zero(float);
template <> float one(float);
template <> double zero(double);
template <> double one(double);

#endif // IDENTITY_H
