///
/// \file tset/coord2d.cc
/// \author K.Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Unit tests on 2D geometry code.
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

#include <array>
#include <iostream>
#include <vector>

#include <scsl/Flags.h>
#include <scmp/Math.h>
#include <scmp/geom/Coord2D.h>
#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>

using namespace scmp::geom;
using namespace sctest;


namespace {
#define CHECK_ROTATE(theta, expected)	if (!scmp::WithinTolerance(scmp::RotateRadians((double)theta, 0), (double)expected, (double)0.0001)) { \
	std::cerr << "Expected " << theta << " to wrap to " << expected << "\n"; \
	std::cerr << "    have " << scmp::RotateRadians(theta, 0) << "\n"; \
	return false; \
}


bool
geomValidateAngularRotation()
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


bool
geomConversionIdentities()
{
	const std::array<Point2D,4> 	points = {
		Point2D(1, 0),
		Point2D(0, 1),
		Point2D(-1, 0),
		Point2D(0, -1)
	};

	const std::array<Polar2D,4>	polars = {
		Polar2D(1, 0),
		Polar2D(1, scmp::DegreesToRadiansD(90)),
		Polar2D(1, scmp::DegreesToRadiansD(180)),
		Polar2D(1, scmp::DegreesToRadiansD(-90)),
	};

	for (auto i = 0; i < 4; i++) {
		const Polar2D	pol(points.at(i));
		if (pol != polars.at(i)) {
			std::cerr << "! measured value outside tolerance ("
				  << i << ")\n";
			std::cerr << "  " << points.at(i) << " → " << pol
				  << " ← " << polars.at(i) << "\n";
			return false;
		}

		const Point2D point(pol);
		SCTEST_CHECK(point == points.at(i));
	}

	return true;
}


bool
geomVerifyBasicProperties()
{
	const Point2D pt1(1, 1);
	const Point2D pt2(2, 2);
	const Point2D pt3(3, 3);

	SCTEST_CHECK((pt1 + pt2) == pt3);
	SCTEST_CHECK((pt3 - pt2) == pt1);

	// commutative
	SCTEST_CHECK((pt1 + pt2) == (pt2 + pt1));
	SCTEST_CHECK((pt1 + pt3) == (pt3 + pt1));
	SCTEST_CHECK((pt2 + pt3) == (pt3 + pt2));

	// associative
	SCTEST_CHECK(((pt1 + pt2) + pt3) == (pt1 + (pt2 + pt3)));

	// transitive
	const Point2D pt4(1, 1);
	const Point2D pt5(1, 1);
	SCTEST_CHECK(pt1 == pt4);
	SCTEST_CHECK(pt4 == pt5);
	SCTEST_CHECK(pt1 == pt5);

	// scaling
	const Point2D pt6(2, 3);
	const Point2D pt7(8, 12);
	SCTEST_CHECK((pt6 * 4) == pt7);
	return true;
}


bool
geomComparePoint2D()
{
	const Point2D pt1(1, 1);
	const Point2D pt2(1, 1);
	const Point2D pt3(0, 1);

	SCTEST_CHECK(pt1 == pt2);
	SCTEST_CHECK_FALSE(pt2 == pt3);
	return true;
}


bool
geomRotatePoint2D()
{
	std::array<Point2D, 4> vertices = {
		Point2D(1, 0),  // θ = 0	
		Point2D(0, 1),  // θ = π/2
		Point2D(-1, 0), // θ = π
		Point2D(0, -1)  // θ = 3π/2
	};

	for (auto i = 0; i < 4; i++) {
		auto first    = i % 4;
		auto expected = (i + 1) % 4;

		Point2D vertex;
		vertices.at(first).Rotate(vertex, 1.5708);

		if (vertex != vertices.at(expected)) {
			std::cerr << "expected: " << expected << "\n";
			std::cerr << "    have: " << vertex << "\n";
			return false;
		}
	}
	return true;
}


bool
geomRotatePointsAboutOrigin()
{
	Point2D	origin(3, 3);
	double	theta = 0;

	const std::vector<Polar2D> vertices {
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


bool
pointDistances()
{
	const Point2D	origin;
	const Point2D	y2(0, 2);
	const Point2D	x2(2, 0);
	const int 	dist2 = 2;
	const int 	dist10 = 10;
	const Point2D	deg45{8, 6};

	SCTEST_CHECK_EQ(y2.Distance(origin), dist2);
	SCTEST_CHECK_EQ(x2.Distance(origin), dist2);
	SCTEST_CHECK_EQ(deg45.Distance(origin), dist10);

	return true;
};


} // anonymous namespace


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet = false;
	auto flags = new scsl::Flags("test_orientation",
				     "This test validates various orientation-related components in scmp.");
	flags->Register("-n", false, "don't print the report");
	flags->Register("-q", false, "suppress test output");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
	}

	SimpleSuite suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	suite.AddTest("geomValidateAngularRotation", geomValidateAngularRotation);
	suite.AddTest("geomConversionIdentities", geomConversionIdentities);
	suite.AddTest("geomVerifyBasicProperties", geomVerifyBasicProperties);
	suite.AddTest("geomComparePoint2D", geomComparePoint2D);
	suite.AddTest("geomRotatePoint2D", geomRotatePoint2D);
	suite.AddTest("geomRotatePointsAboutOrigin", geomRotatePointsAboutOrigin);
	suite.AddTest("pointDistances", pointDistances);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
