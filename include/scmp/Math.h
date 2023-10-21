///
/// \file include/scmp/Math.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Common math functions.
///
/// Arena defines a memory management backend for pre-allocating memory.
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that
/// the above copyright notice and this permission notice appear in all /// copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
/// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
/// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
/// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
/// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
/// PERFORMANCE OF THIS SOFTWARE.
///

#ifndef SCCCL_MATH_H
#define SCCCL_MATH_H

#include <cmath>
#include <vector>

namespace scmp {


/// MAX_RADIAN is a precomputed 2 * M_PI.
constexpr double MAX_RADIAN = 2 * M_PI;
constexpr double MIN_RADIAN = -2 * M_PI;
constexpr double PI_D = 3.141592653589793;


/// Roll m die of n sides, returning a vector of the dice.
std::vector<int>	Die(int m, int n);

/// Roll m die of n sides, returning the total of the die.
int			DieTotal(int m, int n);

/// Roll m die of n sides, and take the total of the top k die.
int			BestDie(int k, int m, int n);


/// \brief Convert radians to degrees.
///
/// \param rads the Angle in radians
/// \return the Angle in degrees.
float	RadiansToDegreesF(float rads);

/// \brief Convert radians to degrees.
///
/// \param rads the Angle in radians
/// \return the Angle in degrees.
double	RadiansToDegreesD(double rads);

/// \brief Convert degrees to radians.
///
/// \param degrees the Angle in degrees
/// \return the Angle in radians.
float	DegreesToRadiansF(float degrees);

/// \brief Convert degrees to radians.
///
/// \param degrees the Angle in degrees
/// \return the Angle in radians.
double	DegreesToRadiansD(double degrees);

/// \brief RotateRadians rotates theta0 by theta1 radians, wrapping
/// 	   the result to MIN_RADIAN <= result <= MAX_RADIAN.
/// 
/// \param theta0 
/// \param theta1 
/// \return 
double	RotateRadians(double theta0, double theta1);

/// \brief Get the default epsilon value.
///
/// \param epsilon The variable to store the epsilon value in.
void	DefaultEpsilon(double &epsilon);

/// \brief Get the default epsilon value.
///
/// \param epsilon The variable to store the epsilon value in.
void	DefaultEpsilon(float &epsilon);


/// \brief Get the default epsilon for integer types.
///
/// \param epsilon The variable to store the epsilon value in.
void	DefaultEpsilon(int& epsilon);


/// \brief Return whether the two values of type T are equal to within
/// 	   some tolerance.
///
/// \tparam T The type of value
/// \param a A value of type T used as the left-hand side of an
/// 	   equality check.
/// \param b A value of type T used as the right-hand side of an
/// 	   equality check.
/// \param epsilon The tolerance value.
/// \return Whether the two values are "close enough" to be considered
/// 	    equal.
template <typename T>
static T
WithinTolerance(T a, T b, T epsilon)
{
	return std::abs(a - b) <= epsilon;
}


} // namespace scmp


#endif //SCCCL_MATH_H
