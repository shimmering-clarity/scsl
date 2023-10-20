///
/// \file include/scmp/geom/Coord2D.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief 2D point and polar coordinate systems.
///
/// Copyright 2017 K. Isom <kyle@imap.cc>
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

#ifndef SCMATH_GEOM_COORD2D_H
#define SCMATH_GEOM_COORD2D_H


#include <cmath>
#include <ostream>
#include <vector>


namespace scmp {
namespace geom {


class Point2D;

class Polar2D;

/// \brief Point2D is a logical grouping of a set of 2D cartesian
///        coordinates.
class Point2D {
public:
	int x, y;

	// A Point2D can be initialised by setting its members to 0, by providing the
	// x and y coordiantes, or through translation from a polar coordinate.
	Point2D();
	Point2D(int _x, int _y);
	Point2D(const Polar2D &);

	std::string ToString();
	void ToPolar(Polar2D &);

	// Rotate rotates the point by theta radians. Alternatively, a rotation
	// can use this point as the centre, with a polar coordinate and a rotation
	// amount (in radians). The latter is used to specify a central point
	// of rotation with vertices specified as polar coordinates from the centre.
	// Both forms take a reference to a Point2D to store the rotated point.
	void Rotate(Point2D &rotated, double theta);
	std::vector<Point2D> Rotate(std::vector<Polar2D>, double);

	// Translate adds this point to the first argument, storing the result in the
	// second argument.
	void Translate(const Point2D &other, Point2D &translated);

	// Distance returns the distance from this point to another.
	int Distance(const Point2D &other);

	Point2D operator+(const Point2D &rhs) const
	{ return Point2D(x + rhs.x, y + rhs.y); }
	Point2D operator-(const Point2D &rhs) const
	{ return Point2D(x - rhs.x, y - rhs.y); }
	Point2D operator*(const int k) const
	{ return Point2D(x * k, y * k); }
	bool operator==(const Point2D &rhs) const;
	bool operator!=(const Point2D &rhs) const
	{ return !(*this == rhs); }
	friend std::ostream &operator<<(std::ostream &outs, const Point2D &pt);
};

// A Polar2D is a 2D polar coordinate, specified in terms of the radius from
// some origin and the angle from the positive X axis of a cartesian coordinate
// system.
class Polar2D {
public:
	double r, theta;

	// A Polar2D can be initialised as a zeroised polar coordinate, by specifying
	// the radius and angle directly, or via conversion from a Point2D.
	Polar2D() : r(0.0), theta(0.0)
	{}
	Polar2D(double _r, double _theta) : r(_r), theta(_theta)
	{}
	Polar2D(const Point2D &);

	std::string ToString();
	void ToPoint(Point2D &);

	// Rotate rotates the polar coordinate by the number of radians, storing the result
	// in the Polar2D argument.
	void Rotate(Polar2D &, double);

	// RotateAround rotates this point about by theta radians, storing the rotated point
	// in result.
	void RotateAround(const Point2D &other, Point2D &result, double tjeta);

	bool operator==(const Polar2D &) const;
	bool operator!=(const Polar2D &rhs) const
	{ return !(*this == rhs); }
	friend std::ostream &operator<<(std::ostream &, const Polar2D &);
};


} // end namespace geom
} // end namespace math
#endif