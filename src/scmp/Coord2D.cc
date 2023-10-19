
//
// Project: scccl
// File: src/math/geom2d.cpp
// Author: Kyle Isom
// Date: 2017-06-05
// Namespace: math::geom
//
// geom2d.cpp contains the implementation of 2D geometry in the math::geom
// namespace.
//
// Copyright 2017 Kyle Isom <kyle@imap.cc>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <cmath>
#include <iostream>
#include <vector>

#include <scmp/Math.h>
#include <scmp/geom/Coord2D.h>


// coord2d.cpp contains 2D geometric functions and data structures, such as
// cartesian and polar coordinates and rotations.

// TODO: deprecate Point2D in favour of Vector

namespace scmp {
namespace geom {


//
// Point2D

Point2D::Point2D(const Polar2D &pol)
    : x(std::rint(std::cos(pol.theta) * pol.r)),
      y(std::rint(std::sin(pol.theta) * pol.r)) {}


std::ostream&
operator<<(std::ostream& outs, const Point2D& pt)
{
	outs << "(" << std::to_string(pt.x) << ", " << std::to_string(pt.y) << ")";
	return outs;
}


std::string
Point2D::ToString()
{
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}


void
Point2D::ToPolar(Polar2D& pol)
{
	pol.r = std::sqrt((x * x) + (y * y));
	pol.theta = std::atan2(y, x);
}


void
Point2D::Rotate(Point2D& pt, double theta)
{
	Polar2D	pol(*this);
	pol.Rotate(pol, theta);
	pol.ToPoint(pt);
}


bool
Point2D::operator==(const Point2D& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}


void
Point2D::Translate(const Point2D& origin, Point2D &translated)
{
	translated.x = origin.x + x;
	translated.y = origin.y + y;
}


std::vector<Point2D>
Point2D::Rotate(std::vector<Polar2D> vertices, double theta)
{
	std::vector<Point2D>	rotated;

	for (auto v : vertices) {
		Point2D	p;
		v.RotateAround(*this, p, theta);
		rotated.push_back(p)	;
	}

	return rotated;
}


int
Point2D::Distance(const Point2D& other)
{
	auto dx = other.x - x;
	auto dy = other.y - y;
	return std::sqrt(dx * dx + dy + dy);
}


// Polar2D

Polar2D::Polar2D(const Point2D &pt)
    : r(std::sqrt((pt.x * pt.x) + (pt.y * pt.y))),
      theta(std::atan2(pt.y, pt.x)) {}


void
Polar2D::ToPoint(Point2D& pt)
{
	pt.y = std::rint(std::sin(theta) * r);
	pt.x = std::rint(std::cos(theta) * r);
}


std::string
Polar2D::ToString()
{
	return "(" + std::to_string(r) + ", " + std::to_string(theta) + ")";
}


void
Polar2D::Rotate(Polar2D& rot, double delta)
{
	rot.r = r;
	rot.theta = RotateRadians(theta, delta);
}


bool
Polar2D::operator==(const Polar2D& rhs) const
{
	static double eps = 0.0;
	if (eps == 0.0) {
		scmp::DefaultEpsilon(eps);
	}
	return scmp::WithinTolerance(r, rhs.r, eps) &&
	       scmp::WithinTolerance(theta, rhs.theta, eps);
}


void
Polar2D::RotateAround(const Point2D &origin, Point2D &point, double delta)
{
	Polar2D	rot;
	this->Rotate(rot, delta);
	rot.ToPoint(point);	
	point.Translate(origin, point);
}


std::ostream&
operator<<(std::ostream& outs, const Polar2D& pol)
{
	outs << "(" << pol.r << ", " << pol.theta << ")";
	return outs;
}


} // end namespace geom
} // end namespace math
