#ifndef GENERAL_H
#define GENERAL_H

#include <stdlib.h>

/**
Returns the number with the highest value.

@param a first number.
@param b second number.
@return a number of type T.
 */
template <typename T>
inline T max(T const& a, T const& b);

/**
Returns the number with the lowest value.

@param a first number.
@param b second number.
@return a number of type T.
 */
template <typename T>
inline T min(T const& a, T const& b);

/**
Returns a random number between 1 and 0

@return a double.
*/
inline double getSRand(void);

/**
Returns a random number between 0 and x. Number can reach 0, but not x.

@param x random number limit.
@return a number or type T.
*/
template <typename T>
inline T getRand(T const x);

/**
Returns a number between a and b. Does not matter which is greater
or less than which.

@param a lower or upper limit.
@param b lower or upper limit.
@return number of type T.
*/
template <typename T>
inline T getRandRange(T const a, T const b);

#include "general_impl.h"

#endif
