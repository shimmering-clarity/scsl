#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <vector>

#include <scccl/math/math.h>


namespace scmath {


std::vector<int>	
Die(int m, int n)
{
	std::uniform_int_distribution<>	die(1, n);

	std::random_device	rd;
	std::vector<int>	dice;
	int			i = 0;

	for (i = 0; i < m; i++) {
		dice.push_back(die(rd));
	}

	return dice;
}


int
BestDie(int k, int m, int n)
{
	auto dice = Die(m, n);

	if (k < m) {
		std::sort(dice.begin(), dice.end(), std::greater<int>());
		dice.resize(static_cast<size_t>(k));
	}

	return std::accumulate(dice.begin(), dice.end(), 0);
}


int
DieTotal(int m, int n)
{
	std::uniform_int_distribution<>	die(1, n);

	std::random_device	rd;
	int			i = 0, total = 0;

	for (i = 0; i < m; i++) {
		total += die(rd);
	}

	return total;
}


float
RadiansToDegreesF(float rads)
{
	return rads * (180.0 / M_PI);
}


double
RadiansToDegreesD(double rads)
{
	return rads * (180.0 / M_PI);
}


float
DegreesToRadiansF(float degrees)
{
	return degrees * M_PI / 180.0;
}


double
DegreesToRadiansD(double degrees)
{
	return degrees * M_PI / 180.0;
}


double
RotateRadians(double theta0, double theta1)
{
	auto	dtheta = theta0 + theta1;

	if (dtheta > M_PI) {
		dtheta -= MAX_RADIAN;
	} else if (dtheta < -M_PI) {
		dtheta += MAX_RADIAN;
	}

	if ((dtheta < -M_PI) || (dtheta > M_PI)) {
		return RotateRadians(dtheta, 0);
	}

	return dtheta;
}




const double Epsilon_double = 0.0001;
const float  Epsilon_float = 0.0001;


void
DefaultEpsilon(double &epsilon)
{
	epsilon = Epsilon_double;
}


void
DefaultEpsilon(float &epsilon)
{
	epsilon = Epsilon_float;
}


} // namespace math

