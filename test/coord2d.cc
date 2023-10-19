//
// Project: scccl
// File: test/math/geom2d_test.cpp
// Author: Kyle Isom
// Date: 2017-06-05
//
// geom2d_test runs a set of unit tests on the 2D parts of the
// math::geom namespace.
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
#include <iostream>
#include <vector>

#include <scmp/Math.h>
#include <scmp/geom/Coord2D.h>
#include <sctest/SimpleSuite.h>
#include <sctest/Checks.h>

using namespace scmp::geom;
using namespace sctest;


#define CHECK_ROTATE(theta, expected)	if (!scmp::WithinTolerance(scmp::RotateRadians((double)theta, 0), (double)expected, (double)0.0001)) { \
	std::cerr << "Expected " << theta << " to wrap to " << expected << std::endl; \
	std::cerr << "    have " << scmp::RotateRadians(theta, 0) << std::endl; \
	return false; \
}

static bool
geom_validate_angular_rotation(void)
{
		CHECK_ROTATE(0, 0);
		CHECK_ROTATE(M_PI/4, M_PI/4);
		CHECK_ROTATE(M_PI/2, M_PI/2);
		CHECK_ROTATE(3 * M_PI / 4,  3 * M_PI / 4);
		CHECK_ROTATE(M_PI, M_PI);
		CHECK_ROTATE(5 * M_PI / 4, -3 * M_PI / 4);
		CHECK_ROTATE(3 * M_PI / 2, -(M_PI / 2));
		CHECK_ROTATE(7 * M_PI / 4, -(M_PI / 4));
		CHECK_ROTATE(4 * M_PI, 0)

		return true;
}

static bool
geom_conversion_identities(void)
{
	Point2D	points[4] = {
		Point2D(1, 0),
		Point2D(0, 1),
		Point2D(-1, 0),
		Point2D(0, -1)
	};

	Polar2D	polars[4] = {
		Polar2D(1, 0),
		Polar2D(1, scmp::DegreesToRadiansD(90)),
		Polar2D(1, scmp::DegreesToRadiansD(180)),
		Polar2D(1, scmp::DegreesToRadiansD(-90)),
	};

	for (auto i = 0; i < 4; i++) {
		Polar2D	pol(points[i]);
		if (pol != polars[i]) {
			std::cerr << "! measured value outside tolerance (" << i << ")" << std::endl;
			std::cerr << "  " << points[i] << " → " << pol << " ← " << polars[i] << std::endl; 
			return false;
		}

		Point2D	pt(pol);
		SCTEST_CHECK(pt == points[i]);
	}

	return true;
}

static bool
geom_verify_basic_properties(void)
{
	Point2D p1(1, 1);
	Point2D p2(2, 2);
	Point2D p3(3, 3);

	SCTEST_CHECK((p1 + p2) == p3);
	SCTEST_CHECK((p3 - p2) == p1);

	// commutative
	SCTEST_CHECK((p1 + p2) == (p2 + p1));
	SCTEST_CHECK((p1 + p3) == (p3 + p1));
	SCTEST_CHECK((p2 + p3) == (p3 + p2));

	// associative
	SCTEST_CHECK(((p1 + p2) + p3) == (p1 + (p2 + p3)));

	// transitive
	Point2D	p4(1, 1);
	Point2D p5(1, 1);
	SCTEST_CHECK(p1 == p4);
	SCTEST_CHECK(p4 == p5);
	SCTEST_CHECK(p1 == p5);

	// scaling
	Point2D p6(2, 3);
	Point2D p7(8, 12);
	SCTEST_CHECK((p6 * 4) == p7);
	return true;
}

static bool
geom_compare_point2d(void)
{
	Point2D	p1(1, 1);
	Point2D	p2(1, 1);
	Point2D	p3(0, 1);

	SCTEST_CHECK(p1 == p2);
	SCTEST_CHECK_FALSE(p2 == p3);
	return true;
}

static bool
geom_rotate_point2d(void)
{
	Point2D vertices[4] = {	
		Point2D(1, 0),  // θ = 0	
		Point2D(0, 1),  // θ = π/2
		Point2D(-1, 0), // θ = π
		Point2D(0, -1)  // θ = 3π/2
	};

	Point2D	vertex;
	vertices[0].Rotate(vertex, 1.5708);

	if (vertex != vertices[1]) {
		std::cerr << "expected: " << vertices[1] << std::endl;
		std::cerr << "    have: " << vertex << std::endl;
		return false;
	}
	return true;
}

static bool
geom_rotate_points_about_origin(void)
{
	Point2D	origin(3, 3);
	double	theta = 0;

	std::vector<Polar2D> vertices {
		Polar2D(2, 0),
		Polar2D(1.41421, 2.35619),
		Polar2D(1.41421, -2.35619)		
	};

	// expected coordinates with no rotation
	std::vector<Point2D> rotated0 {
		Point2D(5, 3),
		Point2D(2, 4),
		Point2D(2, 2)
	};

	auto rotated = origin.Rotate(vertices, theta);
	for (auto i = 0; i < 3; i++) {
		SCTEST_CHECK(rotated.at(i) == rotated0.at(i));
	}

	// expected after 90° rotation
	theta = scmp::DegreesToRadiansD(90);
	std::vector<Point2D> rotated90 {
		Point2D(3, 5),
		Point2D(2, 2),
		Point2D(4, 2)
	};

	rotated = origin.Rotate(vertices, theta);
	for (auto i = 0; i < 3; i++) {
		SCTEST_CHECK(rotated.at(i) == rotated90.at(i));
	}

	// expected after 180° rotation
	theta = scmp::DegreesToRadiansD(180);
	std::vector<Point2D> rotated180 {
		Point2D(1, 3),
		Point2D(4, 2),
		Point2D(4, 4)
	};

	rotated = origin.Rotate(vertices, theta);
	for (auto i = 0; i < 3; i++) {
		SCTEST_CHECK(rotated.at(i) == rotated180.at(i));
	}

	return true;
}

int
main(void)
{
	SimpleSuite	ts;
	ts.AddTest("geom_validate_angular_rotation", geom_validate_angular_rotation);
	ts.AddTest("geom_conversion_identities", geom_conversion_identities);
	ts.AddTest("geom_verify_basic_properties", geom_verify_basic_properties);
	ts.AddTest("geom_compare_point2d", geom_compare_point2d);
	ts.AddTest("geom_rotate_point2d", geom_rotate_point2d);
	ts.AddTest("geom_rotate_points_about_origin", geom_rotate_points_about_origin);
	
	if (ts.Run()) {
		std::cout << "OK" << std::endl;
		return 0;
	}
	else {
		auto r = ts.GetReport();
		std::cerr << r.Failing << "/" << r.Total << " tests failed." << std::endl;
		return 1;
	}
}
