///
/// \file include/scmp/geom/Coord2D.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief 2D point and polar coordinate systems.
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

#include <cmath>
#include <iostream>
#include <vector>

#include <scmp/Math.h>
#include <scmp/geom/Coord2D.h>
#include <scmp/geom/Orientation.h>
#include <scmp/geom/Vector.h>


// coord2d.cpp contains 2D geometric functions and data structures, such as
// cartesian and polar coordinates and rotations.

// TODO: deprecate Point2D in favour of Vector

namespace scmp {
namespace geom {


//
// Point2D


Point2D::Point2D() : Vector<int, 2>{0, 0}
{

};

Point2D::Point2D(int _x, int _y) : Vector<int, 2>{_x, _y}
{}

Point2D::Point2D(const Polar2D &pol)
    : Vector<int, 2>{static_cast<int>(std::rint(std::cos(pol.Theta()) * pol.R())),
		     static_cast<int>(std::rint(std::sin(pol.Theta()) * pol.R()))}
{}


int
Point2D::X() const
{
	return this->At(0);
}


void
Point2D::X(int _x)
{
	this->Set(BasisX, _x);
}


int
Point2D::Y() const
{
	return this->At(1);
}


void
Point2D::Y(int _y)
{
	this->Set(1, _y);
}


std::ostream &
operator<<(std::ostream &outs, const Point2D &pt)
{
	outs << "(" << std::to_string(pt[0]) << ", " << std::to_string(pt[1]) << ")";
	return outs;
}


std::string
Point2D::ToString()
{
	return "(" + std::to_string(this->X()) + ", " + std::to_string(this->Y()) + ")";
}


void
Point2D::ToPolar(Polar2D &pol)
{
	pol.R(std::sqrt(this->X() * this->X() + this->Y() * this->Y()));
	pol.Theta(std::atan2(this->Y(), this->X()));
}


void
Point2D::Rotate(Point2D &pt, double theta)
{
	Polar2D pol(*this);
	pol.Rotate(pol, theta);
	pol.ToPoint(pt);
}

void
Point2D::Translate(const Point2D &origin, Point2D &translated)
{
	translated.X(origin.X() + this->X());
	translated.Y(origin.Y() + this->Y());
}


std::vector<Point2D>
Point2D::Rotate(std::vector<Polar2D> vertices, double theta)
{
	std::vector<Point2D> rotated;

	for (auto &v: vertices) {
		Point2D p;
		v.RotateAround(*this, p, theta);
		rotated.push_back(p);
	}

	return rotated;
}


int
Point2D::Distance(const Point2D& other) const
{
	auto dx = other.X() - this->X();
	auto dy = other.Y() - this->Y();
	return static_cast<int>(std::rint(std::sqrt(dx * dx + dy * dy)));
}


// Polar2D

Polar2D::Polar2D() : Vector<double, 2>{0.0, 0.0} {};

Polar2D::Polar2D(double _r, double _theta) : Vector<double, 2>{_r, _theta}
{}

Polar2D::Polar2D(const Point2D &pt)
    : Vector<double, 2>{std::sqrt((pt.X() * pt.X()) + (pt.Y() * pt.Y())),
      			std::atan2(pt.Y(), pt.X())}
{}


double
Polar2D::R() const
{
	return this->At(0);
}


void
Polar2D::R(const double _r)
{
	this->Set(0, _r);
}


double
Polar2D::Theta() const
{
	return this->At(1);
}


void
Polar2D::Theta(const double _theta)
{
	this->Set(1, _theta);
}


void
Polar2D::ToPoint(Point2D &pt)
{
	pt.Y(std::rint(std::sin(this->Theta()) * this->R()));
	pt.X(std::rint(std::cos(this->Theta()) * this->R()));
}


std::string
Polar2D::ToString()
{
	return "(" + std::to_string(this->R()) +
	", " + std::to_string(this->Theta()) + ")";
}


void
Polar2D::Rotate(Polar2D &rot, double delta)
{
	rot.R(this->R());
	rot.Theta(RotateRadians(this->Theta(), delta));
}


bool
Polar2D::operator==(const Polar2D &rhs) const
{
	static double eps = 0.0;
	if (eps == 0.0) {
		scmp::DefaultEpsilon(eps);
	}
	return scmp::WithinTolerance(this->R(), rhs.R(), eps) &&
	       scmp::WithinTolerance(this->Theta(), rhs.Theta(), eps);
}


void
Polar2D::RotateAround(const Point2D &origin, Point2D &point, double delta)
{
	Polar2D rot;
	this->Rotate(rot, delta);
	rot.ToPoint(point);
	point.Translate(origin, point);
}


std::ostream &
operator<<(std::ostream &outs, const Polar2D &pol)
{
	outs << "(" << pol.R() << ", " << pol.Theta() << ")";
	return outs;
}


} // end namespace geom
} // end namespace math
