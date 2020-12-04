#include "identity.h"

//#define LIBSYMBOLICCPLUSPLUS

template <> char zero(char /*unused*/) { return static_cast<char>(0); }
template <> char one(char /*unused*/) { return static_cast<char>(1); }

template <> short zero(short /*unused*/) { return static_cast<short>(0); }
template <> short one(short /*unused*/) { return static_cast<short>(1); }

template <> int zero(int /*unused*/) { return static_cast<int>(0); }
template <> int one(int /*unused*/) { return static_cast<int>(1); }

template <> long zero(long /*unused*/) { return static_cast<long>(0); }
template <> long one(long /*unused*/) { return static_cast<long>(1); }

template <> float zero(float /*unused*/) { return static_cast<float>(0.0); }
template <> float one(float /*unused*/) { return static_cast<float>(1.0); }

template <> double zero(double /*unused*/) { return static_cast<double>(0.0); }
template <> double one(double /*unused*/) { return static_cast<double>(1.0); }

//#undef LIBSYMBOLICCPLUSPLUS
// template <class T>
// std::complex<T> zero(std::complex<T>) { return std::complex<T>(zero(T())); }
// template <class T>
// std::complex<T> one(std::complex<T>) { return std::complex<T>(one(T())); }
