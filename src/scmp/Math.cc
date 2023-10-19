///
/// \file Math.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2020-02-26
/// \brief Mathematical convience functions.
///
/// Copyright 2020 K. Isom <kyle@imap.cc>
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

#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <vector>

#include <scmp/Math.h>


namespace scmp {


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


static constexpr double Epsilon_double = 0.0001;
static constexpr float  Epsilon_float = 0.0001;


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

