#include "identity.h"

//#define LIBSYMBOLICCPLUSPLUS

template <> char zero(char) { return static_cast<char>(0); }
template <> char one(char) { return static_cast<char>(1); }

template <> short zero(short) { return static_cast<short>(0); }
template <> short one(short) { return static_cast<short>(1); }

template <> int zero(int) { return static_cast<int>(0); }
template <> int one(int) { return static_cast<int>(1); }

template <> long zero(long) { return static_cast<long>(0); }
template <> long one(long) { return static_cast<long>(1); }

template <> float zero(float) { return static_cast<float>(0.0); }
template <> float one(float) { return static_cast<float>(1.0); }

template <> double zero(double) { return static_cast<double>(0.0); }
template <> double one(double) { return static_cast<double>(1.0); }

//#undef LIBSYMBOLICCPLUSPLUS
//template <class T>
//std::complex<T> zero(std::complex<T>) { return std::complex<T>(zero(T())); }
//template <class T>
//std::complex<T> one(std::complex<T>) { return std::complex<T>(one(T())); }
