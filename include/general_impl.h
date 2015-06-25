#ifndef GENERAL_IMPL_H
#define GENERAL_IMPL_H

#include <stdlib.h>
#include "general.h"

template <typename T>
inline T max(T const& a, T const& b)
{
	return (a > b ? a : b);
}

template <typename T>
inline T min(T const& a, T const& b)
{
	return (a < b ? a : b);
}

template <typename T>
inline T getRand(T const x)
{
	return (T)((double)(x)*rand() / (RAND_MAX+1.0));
}

template <typename T>
inline T getRandRange(T const a, T const b)
{
	return (T)(getRand(b-a) + a);
}

#endif
