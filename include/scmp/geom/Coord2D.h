///
/// \file include/scmp/geom/Coord2D.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief 2D point and polar coordinate systems.
///
/// \section COPYRIGHT
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

/// \brief Point2D is a cartesian (X,Y) pairing.
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

/// \brief Polar2D is a pairing of a radius r and angle θ from some
///        reference point; in this library, it is assumed to be the
///        Cartesian origin (0, 0).
class Polar2D : public Vector<double, 2> {
public:
	/// A Polar2D can be initialised as a zeroised polar coordinate, by specifying
	/// the radius and Angle directly, or via conversion from a Point2D.

	/// \brief Construct a zero polar coordinate.
	Polar2D();

	/// \brief Construct a polar coordinate from a radius and
	///        angle.
	///
	/// \param _r A radius
	/// \param _theta An angle
	Polar2D(double _r, double _theta);

	/// \brief Construct a polar coordinate from a point.
	///
	/// This construct uses the origin (0,0) as the reference point.
	///
	/// \param point A 2D Cartesian point.
	Polar2D(const Point2D& point);

	/// \brief Return the radius component of this coordinate.
	double R() const;

	/// \brief Set the radius component of this coordinate.
	void R(const double _r);

	/// \brief Return the angle component of this coordinate.
	double Theta() const;

	/// \brief Set the angle component of this coordinate.
	void Theta(const double _theta);

	/// \brief Return the coordinate in string form.
	std::string ToString();

	/// \brief Construct a Point2D representing this Polar2D.
	void ToPoint(Point2D &point);

	/// \brief Rotate polar coordinate by some angle.
	///
	/// \param rotated The rotated Polar2D will be stored in this
	///        coordinate.
	/// \param delta The angle to rotate by.
	void Rotate(Polar2D &rotated, double delta);

	/// \brief Rotate this polar coordinate around a 2D point.
	///
	/// \param other The reference point.
	/// \param result The point where the result will stored.
	/// \param delta The angle to rotate by.
	void RotateAround(const Point2D &other, Point2D &result, double delta);

	friend std::ostream &operator<<(std::ostream &, const Polar2D &);
};


} // end namespace geom
} // end namespace math
#endif