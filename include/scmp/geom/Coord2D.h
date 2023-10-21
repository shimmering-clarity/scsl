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

#include <scmp/geom/Vector.h>


namespace scmp {
namespace geom {


class Point2D;

class Polar2D;

/// \brief Point2D is a logical grouping of a set of 2D cartesian
///        coordinates.
class Point2D : public Vector<int, 2> {
public:
	/// \brief A Point2D defaults to (0,0).
	Point2D();

	/// \brief Initialize a Point2D At (_x, _y).
	Point2D(int _x, int _y);

	/// \brief Initialize a Point2D from a Polar2D coordinate.
	Point2D(const Polar2D &pol);

	/// \brief Return the X component of the point.
	int X() const;

	/// \brief Set the X component of the point.
	void X(int _x);

	/// \brief Return the Y component of the point.
	int Y() const;

	/// Set the Y component of the point.
	void Y(int _y);

	/// \brief ToString returns a string in the format (x,y).
	std::string ToString();

	/// \brief ToPolar converts the Point2D to a polar coordinate
	///        in-place.
	void ToPolar(Polar2D &);

	/// \brief Rotate rotates the point by theta radians.
	///
	/// \param rotated Stores the rotated point.
	/// \param theta The Angle (in radians) to Rotate the point.
	void Rotate(Point2D& rotated, double theta);

	/// \brief Rotate this point around a series of vertices.
	///
	/// \param vertices A series of vertices to Rotate this point around.
	/// \param theta The Angle to Rotate by.
	/// \return A series of rotated points.
	std::vector<Point2D> Rotate(std::vector<Polar2D> vertices, double theta);

	/// \brief Translate adds this point to the first argument,
	///        storing the result in the second argument.
	///
	/// \param other The point to translate by.
	/// \param translated The point to store the translation in.
	void Translate(const Point2D &other, Point2D &translated);

	/// \brief Distance returns the distance from this point to another.
	int Distance(const Point2D &other) const;

	friend std::ostream &operator<<(std::ostream &outs, const Point2D &pt);
};

// A Polar2D is a 2D polar coordinate, specified in terms of the radius from
// some origin and the Angle from the positive X Axis of a cartesian coordinate
// system.
class Polar2D : public Vector<double, 2> {
public:
	// A Polar2D can be initialised as a zeroised polar coordinate, by specifying
	// the radius and Angle directly, or via conversion from a Point2D.
	Polar2D();
	Polar2D(double _r, double _theta);
	Polar2D(const Point2D &);

	double R() const;
	void R(const double _r);

	double Theta() const;
	void Theta(const double _theta);

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