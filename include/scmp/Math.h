/// math.h provides certain useful mathematical functions.

#ifndef SCCCL_MATH_H
#define SCCCL_MATH_H

#include <cmath>
#include <vector>

namespace scmp {


// MAX_RADIAN is a precomputed 2 * M_PI, and MIN_RADIAN is -2 * M_PI.
constexpr double	MAX_RADIAN = 2 * M_PI;
constexpr double MIN_RADIAN = -2 * M_PI;
constexpr double POS_HALF_RADIAN = M_PI / 2;
constexpr double NEG_HALF_RADIAN = -(M_PI / 2);


/// Roll m die of n sides, returning a vector of the dice.
std::vector<int>	Die(int m, int n);
/// Roll m die of n sides, returning the total of the die.
int			DieTotal(int m, int n);
/// Roll m die of n sides, and take the total of the top k die.
int			BestDie(int k, int m, int n);


/// Convert radians to degrees.
/// @param rads the angle in radians
/// @return the angle in degrees.
float	RadiansToDegreesF(float rads);

/// Convert radians to degrees.
/// @param rads the angle in radians
/// @return the angle in degrees.
double	RadiansToDegreesD(double rads);

/// Convert degrees to radians.
/// @param degrees the angle in degrees
/// @return the angle in radians.
float	DegreesToRadiansF(float degrees);

/// Convert degrees to radians.
/// @param degrees the angle in degrees
/// @return the angle in radians.
double	DegreesToRadiansD(double degrees);

/// RotateRadians rotates theta0 by theta1 radians, wrapping the result to
/// MIN_RADIAN <= result <= MAX_RADIAN.
double	RotateRadians(double theta0, double theta1);

/// Get the default epsilon value.
/// @param epsilon The variable to store the epsilon value in.
void	DefaultEpsilon(double &epsilon);

/// Get the default epsilon value.
/// @param epsilon The variable to store the epsilon value in.
void	DefaultEpsilon(float &epsilon);


/// Return whether the two values of type T are equal to within some tolerance.
/// @tparam T The type of value
/// @param a A value of type T used as the left-hand side of an equality check.
/// @param b A value of type T used as the right-hand side of an equality check.
/// @param epsilon The tolerance value.
/// @return Whether the two values are "close enough" to be considered equal.
template <typename T>
static T
WithinTolerance(T a, T b, T epsilon)
{
	return std::abs(a - b) < epsilon;
}


} // namespace scmp


#endif //SCCCL_MATH_H
