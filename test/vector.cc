//
// Project: scccl
// File: test/math/geom2d_test.cpp
// Author: Kyle Isom
// Date: 2020-02-19
//
// vector runs a set of unit tests on the vector parts of the
// math::geom namespace.
//
// Copyright 2020 Kyle Isom <kyle@imap.cc>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License At
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <iostream>
#include <sstream>

#include <scmp/geom/Vector.h>
#include <sctest/SimpleSuite.h>
#include <sctest/Checks.h>
#include "scsl/Flags.h"


using namespace scmp;
using namespace sctest;
using namespace std;


static bool
Vector3Miscellaneous_ExtractionOperator3d()
{
	geom::Vector3D vec {1.0, 2.0, 3.0};
	stringstream   vecBuffer;

	vecBuffer << vec;
	SCTEST_CHECK_EQ(vecBuffer.str(), "<1, 2, 3>");

	return true;
}

static bool
Vector3Miscellaneous_ExtractionOperator3f()
{
	geom::Vector3F vec {1.0, 2.0, 3.0};
	stringstream   vecBuffer;

	vecBuffer << vec;
	SCTEST_CHECK_EQ(vecBuffer.str(), "<1, 2, 3>");
	return true;
}


static bool
Vector3Miscellaneous_SetEpsilon() {
	geom::Vector3F a {1.0, 1.0, 1.0};
	geom::Vector3F b;

	a.SetEpsilon(1.1);
	SCTEST_CHECK_EQ(a, b);
	return true;
}


static bool
Vector3FloatTests_Magnitude()
{
	geom::Vector3F v3f {1.0, -2.0, 3.0};
	const float    expected = 3.74165738677394;

	SCTEST_CHECK_FEQ(v3f.Magnitude(), expected);

	return true;
}


static bool
Vector3FloatTests_Equality()
{
	geom::Vector3F a {1.0, 2.0, 3.0};
	geom::Vector3F b {1.0, 2.0, 3.0};
	geom::Vector3F c {1.0, 2.0, 1.0};

	SCTEST_CHECK_EQ(a, b);
	SCTEST_CHECK_EQ(b, a);
	SCTEST_CHECK_NE(a, c);
	SCTEST_CHECK_NE(b, c);

	return true;
}


static bool
Vector3FloatTests_Addition()
{
	geom::Vector3F a {1.0, 2.0, 3.0};
	geom::Vector3F b {4.0, 5.0, 6.0};
	geom::Vector3F expected {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(a+b, expected);

	return true;
}


static bool
Vector3FloatTests_Subtraction()
{
	geom::Vector3F a {1.0, 2.0, 3.0};
	geom::Vector3F b {4.0, 5.0, 6.0};
	geom::Vector3F c {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(c-b, a);

	return true;
}


static bool
Vector3FloatTests_ScalarMultiplication()
{
	geom::Vector3F a {1.0, 2.0, 3.0};
	geom::Vector3F expected {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(a * 3.0, expected);

	return true;
}


static bool
Vector3FloatTests_ScalarDivision()
{
	geom::Vector3F a {1.0, 2.0, 3.0};
	geom::Vector3F b {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(b / 3.0, a);

	return true;
}


static bool
Vector3FloatTests_DotProduct()
{
	geom::Vector3F a {1.0, 2.0, 3.0};
	geom::Vector3F b {4.0, 5.0, 6.0};

	SCTEST_CHECK_FEQ(a * b, (float)32.0);

	return true;
}

static bool
Vector3FloatTests_UnitVector()
{
	// Test values randomly generated and calculated with numpy.
	geom::Vector3F vec3 {5.320264018493507, 5.6541812891273935, 1.9233435162644652};
	geom::Vector3F unit {0.6651669556972103, 0.7069150218815566, 0.24046636539587804};
	geom::Vector3F unit2;

	SCTEST_CHECK_EQ(vec3.UnitVector(), unit);
	SCTEST_CHECK_FALSE(vec3.IsUnitVector());
	SCTEST_CHECK(unit.IsUnitVector());
	SCTEST_CHECK(unit2.IsUnitVector());

	return true;
}

static bool
Vector3FloatTests_Angle()
{
	geom::Vector3F a {0.3977933061361172, 8.053980094436525, 8.1287759943773};
	geom::Vector3F b {9.817895298608196, 4.034166890407462, 4.37628316513266};
	geom::Vector3F c {7.35, 0.221, 5.188};
	geom::Vector3F d {2.751, 8.259, 3.985};

	SCTEST_CHECK_FEQ(a.Angle(b), (float)0.9914540426033251);
	if (!scmp::WithinTolerance(c.Angle(d), (float)1.052, (float)0.001)) {
		return false;
	}

	return true;
}


static bool
Vector3FloatTests_ParallelOrthogonalVectors()
{
	geom::Vector3F a {-2.029, 9.97, 4.172};
	geom::Vector3F b {-9.231, -6.639, -7.245};
	geom::Vector3F c {-2.328, -7.284, -1.214};
	geom::Vector3F d {-1.821, 1.072, -2.94};
	geom::Vector3F e {-2.0, 1.0, 3.0};
	geom::Vector3F f {-6.0, 3.0, 9.0};
	geom::Vector3F zeroVector {0.0, 0.0, 0.0};

	SCTEST_CHECK_FALSE(a.IsParallel(b));
	SCTEST_CHECK_FALSE(a.IsOrthogonal(b));

	SCTEST_CHECK_FALSE(c.IsParallel(d));
	SCTEST_CHECK(c.IsOrthogonal(d));

	SCTEST_CHECK(e.IsParallel(f));
	SCTEST_CHECK_FALSE(e.IsOrthogonal(f));

	SCTEST_CHECK(zeroVector.IsZero());
	SCTEST_CHECK(c.IsParallel(zeroVector));
	SCTEST_CHECK(c.IsOrthogonal(zeroVector));

	return true;
}


static bool
Vector3FloatTests_Projections()
{
	geom::Vector3F a {4.866769214609107, 6.2356222686140566, 9.140878417029711};
	geom::Vector3F b {6.135533104801077, 8.757851406697895, 0.6738031370548048};
	geom::Vector3F c {4.843812341655318, 6.9140509888133055, 0.5319465962229454};
	geom::Vector3F d {0.02295687295378901, -0.6784287201992489, 8.608931820806765};

	SCTEST_CHECK_EQ(a.ProjectParallel(b), c);
	SCTEST_CHECK_EQ(a.ProjectOrthogonal(b), d);

	return true;
}


static bool
Vector3FloatTests_CrossProduct()
{
	geom::Vector3F a {8.462, 7.893, -8.187};
	geom::Vector3F b {6.984, -5.975, 4.778};
	geom::Vector3F c {-11.2046, -97.6094, -105.685};

	c.SetEpsilon(0.001);
	SCTEST_CHECK_EQ(c, a.Cross(b));

	return true;
}


static bool
Vector3DoubleTests_Magnitude()
{
	geom::Vector3D v3d{1.0, -2.0, 3.0};
	const double   expected = 3.74165738677394;
	
	SCTEST_CHECK_DEQ(v3d.Magnitude(), expected);
	return true;
}


static bool
Vector3DoubleTests_Equality()
{
	geom::Vector3D a {1.0, 2.0, 3.0};
	geom::Vector3D b {1.0, 2.0, 3.0};
	geom::Vector3D c {1.0, 2.0, 1.0};

	SCTEST_CHECK_EQ(a, b);
	SCTEST_CHECK_EQ(b, a);
	SCTEST_CHECK_NE(a, c);
	SCTEST_CHECK_NE(b, c);

	return true;
}


static bool
Vector3DoubleTests_Addition()
{
	geom::Vector3D a {1.0, 2.0, 3.0};
	geom::Vector3D b {4.0, 5.0, 6.0};
	geom::Vector3D expected {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(a+b, expected);

	return true;
}


static bool
Vector3DoubleTests_Subtraction()
{
	geom::Vector3D a {1.0, 2.0, 3.0};
	geom::Vector3D b {4.0, 5.0, 6.0};
	geom::Vector3D c {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(c-b, a);

	return true;
}


static bool
Vector3DoubleTests_ScalarMultiplication()
{
	geom::Vector3D a {1.0, 2.0, 3.0};
	geom::Vector3D expected {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(a * 3.0, expected);

	return true;
}


static bool
Vector3DoubleTests_ScalarDivision()
{
	geom::Vector3D a {1.0, 2.0, 3.0};
	geom::Vector3D b {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(b / 3.0, a);

	return true;
}


static bool
Vector3DoubleTests_DotProduct()
{
	geom::Vector3D a {1.0, 2.0, 3.0};
	geom::Vector3D b {4.0, 5.0, 6.0};

	SCTEST_CHECK_DEQ(a * b, 32.0);

	return true;
}


static bool
Vector3DoubleTests_UnitVector()
{
	// Test values randomly generated and calculated with numpy.
	geom::Vector3D vec3 {5.320264018493507, 5.6541812891273935, 1.9233435162644652};
	geom::Vector3D unit {0.6651669556972103, 0.7069150218815566, 0.24046636539587804};
	geom::Vector3D unit2;

	SCTEST_CHECK_EQ(vec3.UnitVector(), unit);
	SCTEST_CHECK_FALSE(vec3.IsUnitVector());
	SCTEST_CHECK(unit.IsUnitVector());
	SCTEST_CHECK(unit2.IsUnitVector());

	return true;
}


static bool
Vector3DoubleTests_Angle()
{
	geom::Vector3D a {0.3977933061361172, 8.053980094436525, 8.1287759943773};
	geom::Vector3D b {9.817895298608196, 4.034166890407462, 4.37628316513266};
	geom::Vector3D c {7.35, 0.221, 5.188};
	geom::Vector3D d {2.751, 8.259, 3.985};

	SCTEST_CHECK_DEQ(a.Angle(b), 0.9914540426033251);
	if (!scmp::WithinTolerance(c.Angle(d), (double)1.052, (double)0.001)) {
		return false;
	}

	return true;
}


static bool
Vector3DoubleTests_ParallelOrthogonalVectors()
{
	geom::Vector3D a {-2.029, 9.97, 4.172};
	geom::Vector3D b {-9.231, -6.639, -7.245};
	geom::Vector3D c {-2.328, -7.284, -1.214};
	geom::Vector3D d {-1.821, 1.072, -2.94};
	geom::Vector3D e {-2.0, 1.0, 3.0};
	geom::Vector3D f {-6.0, 3.0, 9.0};
	geom::Vector3D zeroVector {0.0, 0.0, 0.0};

	SCTEST_CHECK_FALSE(a.IsParallel(b));
	SCTEST_CHECK_FALSE(a.IsOrthogonal(b));

	SCTEST_CHECK_FALSE(c.IsParallel(d));
	SCTEST_CHECK(c.IsOrthogonal(d));

	SCTEST_CHECK(e.IsParallel(f));
	SCTEST_CHECK_FALSE(e.IsOrthogonal(f));

	SCTEST_CHECK(zeroVector.IsZero());
	SCTEST_CHECK(c.IsParallel(zeroVector));
	SCTEST_CHECK(c.IsOrthogonal(zeroVector));

	return true;
}


static bool
Vector3DoubleTests_Projections()
{
	geom::Vector3D a {4.866769214609107, 6.2356222686140566, 9.140878417029711};
	geom::Vector3D b {6.135533104801077, 8.757851406697895, 0.6738031370548048};
	geom::Vector3D c {4.843812341655318, 6.9140509888133055, 0.5319465962229454};
	geom::Vector3D d {0.02295687295378901, -0.6784287201992489, 8.608931820806765};

	SCTEST_CHECK_EQ(a.ProjectParallel(b), c);
	SCTEST_CHECK_EQ(a.ProjectOrthogonal(b), d);

	return true;
}


static bool
Vector3DoubleTests_CrossProduct()
{
	geom::Vector3D a {8.462, 7.893, -8.187};
	geom::Vector3D b {6.984, -5.975, 4.778};
	geom::Vector3D c {-11.2046, -97.6094, -105.685};

	c.SetEpsilon(0.001); // double trouble
	SCTEST_CHECK_EQ(c, a.Cross(b));

	return true;
}


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet = false;
	auto flags = new scsl::Flags("test_vector",
				     "This test validates the vector implementation.");
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

	suite.AddTest("Vector3Miscellaneous_ExtractionOperator3d",
		      Vector3Miscellaneous_ExtractionOperator3d);
	suite.AddTest("Vector3Miscellaneous_ExtractionOperator3f",
		      Vector3Miscellaneous_ExtractionOperator3f);
	suite.AddTest("Vector3Miscellaneous_SetEpsilon",
		      Vector3Miscellaneous_SetEpsilon);
	suite.AddTest("Vector3FloatTests_Magnitude",
		      Vector3FloatTests_Magnitude);
	suite.AddTest("Vector3FloatTests_Equality",
		      Vector3FloatTests_Equality);
	suite.AddTest("Vector3FloatTests_Addition",
		      Vector3FloatTests_Addition);
	suite.AddTest("Vector3FloatTests_Subtraction",
		      Vector3FloatTests_Subtraction);
	suite.AddTest("Vector3FloatTests_ScalarMultiplication",
		      Vector3FloatTests_ScalarMultiplication);
	suite.AddTest("Vector3FloatTests_ScalarDivision",
		      Vector3FloatTests_ScalarDivision);
	suite.AddTest("Vector3FloatTests_DotProduct",
		      Vector3FloatTests_DotProduct);
	suite.AddTest("Vector3FloatTests_UnitVector",
		      Vector3FloatTests_UnitVector);
	suite.AddTest("Vector3FloatTests_Angle",
		      Vector3FloatTests_Angle);
	suite.AddTest("Vector3FloatTests_ParallelOrthogonalVectors",
		      Vector3FloatTests_ParallelOrthogonalVectors);
	suite.AddTest("Vector3FloatTests_Projections",
		      Vector3FloatTests_Projections);
	suite.AddTest("Vector3FloatTests_CrossProduct",
		      Vector3FloatTests_CrossProduct);
	suite.AddTest("Vector3DoubleTests_Magnitude",
		      Vector3DoubleTests_Magnitude);
	suite.AddTest("Vector3DoubleTests_Equality",
		      Vector3DoubleTests_Equality);
	suite.AddTest("Vector3DoubleTests_Addition",
		      Vector3DoubleTests_Addition);
	suite.AddTest("Vector3DoubleTests_Subtraction",
		      Vector3DoubleTests_Subtraction);
	suite.AddTest("Vector3DoubleTests_ScalarMultiplication",
		      Vector3DoubleTests_ScalarMultiplication);
	suite.AddTest("Vector3DoubleTests_ScalarDivision",
		      Vector3DoubleTests_ScalarDivision);
	suite.AddTest("Vector3DoubleTests_DotProduct",
		      Vector3DoubleTests_DotProduct);
	suite.AddTest("Vector3DoubleTests_UnitVector",
		      Vector3DoubleTests_UnitVector);
	suite.AddTest("Vector3DoubleTests_Angle",
		      Vector3DoubleTests_Angle);
	suite.AddTest("Vector3DoubleTests_ParallelOrthogonalVectors",
		      Vector3DoubleTests_ParallelOrthogonalVectors);
	suite.AddTest("Vector3DoubleTests_Projections",
		      Vector3DoubleTests_Projections);
	suite.AddTest("Vector3DoubleTests_CrossProduct",
		      Vector3DoubleTests_CrossProduct);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
