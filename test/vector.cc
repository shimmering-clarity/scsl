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
#include <sstream>

#include <scmp/geom/Vector.h>
#include <sctest/SimpleSuite.h>
#include <sctest/Checks.h>


using namespace scmp;
using namespace sctest;
using namespace std;


static bool
Vector3Miscellaneous_ExtractionOperator3d()
{
	geom::Vector3d	vec {1.0, 2.0, 3.0};
	stringstream	vecBuffer;

	vecBuffer << vec;
	SCTEST_CHECK_EQ(vecBuffer.str(), "<1, 2, 3>");

	return true;
}

static bool
Vector3Miscellaneous_ExtractionOperator3f()
{
	geom::Vector3f	vec {1.0, 2.0, 3.0};
	stringstream	vecBuffer;

	vecBuffer << vec;
	SCTEST_CHECK_EQ(vecBuffer.str(), "<1, 2, 3>");
	return true;
}


static bool
Vector3Miscellaneous_SetEpsilon() {
	geom::Vector3f	a {1.0, 1.0, 1.0};
	geom::Vector3f	b;

	a.setEpsilon(1.1);
	SCTEST_CHECK_EQ(a, b);
	return true;
}


static bool
Vector3FloatTests_Magnitude()
{
	geom::Vector3f	v3f {1.0, -2.0, 3.0};
	const float	expected = 3.74165738677394;

	SCTEST_CHECK_FEQ(v3f.magnitude(), expected);

	return true;
}


static bool
Vector3FloatTests_Equality()
{
	geom::Vector3f	a {1.0, 2.0, 3.0};
	geom::Vector3f	b {1.0, 2.0, 3.0};
	geom::Vector3f	c {1.0, 2.0, 1.0};

	SCTEST_CHECK_EQ(a, b);
	SCTEST_CHECK_EQ(b, a);
	SCTEST_CHECK_NE(a, c);
	SCTEST_CHECK_NE(b, c);

	return true;
}


static bool
Vector3FloatTests_Addition()
{
	geom::Vector3f	a {1.0, 2.0, 3.0};
	geom::Vector3f	b {4.0, 5.0, 6.0};
	geom::Vector3f	expected {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(a+b, expected);

	return true;
}


static bool
Vector3FloatTests_Subtraction()
{
	geom::Vector3f	a {1.0, 2.0, 3.0};
	geom::Vector3f	b {4.0, 5.0, 6.0};
	geom::Vector3f	c {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(c-b, a);

	return true;
}


static bool
Vector3FloatTests_ScalarMultiplication()
{
	geom::Vector3f	a {1.0, 2.0, 3.0};
	geom::Vector3f	expected {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(a * 3.0, expected);

	return true;
}


static bool
Vector3FloatTests_ScalarDivision()
{
	geom::Vector3f	a {1.0, 2.0, 3.0};
	geom::Vector3f	b {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(b / 3.0, a);

	return true;
}


static bool
Vector3FloatTests_DotProduct()
{
	geom::Vector3f	a {1.0, 2.0, 3.0};
	geom::Vector3f	b {4.0, 5.0, 6.0};

	SCTEST_CHECK_FEQ(a * b, (float)32.0);

	return true;
}

static bool
Vector3FloatTests_UnitVector()
{
	// Test values randomly generated and calculated with numpy.
	geom::Vector3f	vec3 {5.320264018493507, 5.6541812891273935, 1.9233435162644652};
	geom::Vector3f	unit {0.6651669556972103, 0.7069150218815566, 0.24046636539587804};
	geom::Vector3f	unit2;

	SCTEST_CHECK_EQ(vec3.unitVector(), unit);
	SCTEST_CHECK_FALSE(vec3.isUnitVector());
	SCTEST_CHECK(unit.isUnitVector());
	SCTEST_CHECK(unit2.isUnitVector());

	return true;
}

static bool
Vector3FloatTests_Angle()
{
	geom::Vector3f	a {0.3977933061361172, 8.053980094436525, 8.1287759943773};
	geom::Vector3f	b {9.817895298608196, 4.034166890407462, 4.37628316513266};
	geom::Vector3f	c {7.35, 0.221, 5.188};
	geom::Vector3f	d {2.751, 8.259, 3.985};

	SCTEST_CHECK_FEQ(a.angle(b), (float)0.9914540426033251);
	if (!scmp::WithinTolerance(c.angle(d), (float)1.052, (float)0.001)) {
		return false;
	}

	return true;
}


static bool
Vector3FloatTests_ParallelOrthogonalVectors()
{
	geom::Vector3f	a {-2.029, 9.97, 4.172};
	geom::Vector3f	b {-9.231, -6.639, -7.245};
	geom::Vector3f	c {-2.328, -7.284, -1.214};
	geom::Vector3f	d {-1.821, 1.072, -2.94};
	geom::Vector3f	e {-2.0, 1.0, 3.0};
	geom::Vector3f	f {-6.0, 3.0, 9.0};
	geom::Vector3f	zeroVector {0.0, 0.0, 0.0};

	SCTEST_CHECK_FALSE(a.isParallel(b));
	SCTEST_CHECK_FALSE(a.isOrthogonal(b));

	SCTEST_CHECK_FALSE(c.isParallel(d));
	SCTEST_CHECK(c.isOrthogonal(d));

	SCTEST_CHECK(e.isParallel(f));
	SCTEST_CHECK_FALSE(e.isOrthogonal(f));

	SCTEST_CHECK(zeroVector.isZero());
	SCTEST_CHECK(c.isParallel(zeroVector));
	SCTEST_CHECK(c.isOrthogonal(zeroVector));

	return true;
}


static bool
Vector3FloatTests_Projections()
{
	geom::Vector3f	a {4.866769214609107, 6.2356222686140566, 9.140878417029711};
	geom::Vector3f	b {6.135533104801077, 8.757851406697895, 0.6738031370548048};
	geom::Vector3f	c {4.843812341655318, 6.9140509888133055, 0.5319465962229454};
	geom::Vector3f	d {0.02295687295378901, -0.6784287201992489, 8.608931820806765};

	SCTEST_CHECK_EQ(a.projectParallel(b), c);
	SCTEST_CHECK_EQ(a.projectOrthogonal(b), d);

	return true;
}


static bool
Vector3FloatTests_CrossProduct()
{
	geom::Vector3f	a {8.462, 7.893, -8.187};
	geom::Vector3f	b {6.984, -5.975, 4.778};
	geom::Vector3f	c {-11.2046, -97.6094, -105.685};

	c.setEpsilon(0.001);
	SCTEST_CHECK_EQ(c, a.cross(b));

	return true;
}


static bool
Vector3DoubleTests_Magnitude()
{
	geom::Vector3d	v3d{1.0, -2.0, 3.0};
	const double	expected = 3.74165738677394;
	
	SCTEST_CHECK_DEQ(v3d.magnitude(), expected);
	return true;
}


static bool
Vector3DoubleTests_Equality()
{
	geom::Vector3d	a {1.0, 2.0, 3.0};
	geom::Vector3d	b {1.0, 2.0, 3.0};
	geom::Vector3d	c {1.0, 2.0, 1.0};

	SCTEST_CHECK_EQ(a, b);
	SCTEST_CHECK_EQ(b, a);
	SCTEST_CHECK_NE(a, c);
	SCTEST_CHECK_NE(b, c);

	return true;
}


static bool
Vector3DoubleTests_Addition()
{
	geom::Vector3d	a {1.0, 2.0, 3.0};
	geom::Vector3d	b {4.0, 5.0, 6.0};
	geom::Vector3d	expected {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(a+b, expected);

	return true;
}


static bool
Vector3DoubleTests_Subtraction()
{
	geom::Vector3d	a {1.0, 2.0, 3.0};
	geom::Vector3d	b {4.0, 5.0, 6.0};
	geom::Vector3d	c {5.0, 7.0, 9.0};

	SCTEST_CHECK_EQ(c-b, a);

	return true;
}


static bool
Vector3DoubleTests_ScalarMultiplication()
{
	geom::Vector3d	a {1.0, 2.0, 3.0};
	geom::Vector3d	expected {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(a * 3.0, expected);

	return true;
}


static bool
Vector3DoubleTests_ScalarDivision()
{
	geom::Vector3d	a {1.0, 2.0, 3.0};
	geom::Vector3d	b {3.0, 6.0, 9.0};

	SCTEST_CHECK_EQ(b / 3.0, a);

	return true;
}


static bool
Vector3DoubleTests_DotProduct()
{
	geom::Vector3d	a {1.0, 2.0, 3.0};
	geom::Vector3d	b {4.0, 5.0, 6.0};

	SCTEST_CHECK_DEQ(a * b, 32.0);

	return true;
}


static bool
Vector3DoubleTests_UnitVector()
{
	// Test values randomly generated and calculated with numpy.
	geom::Vector3d	vec3 {5.320264018493507, 5.6541812891273935, 1.9233435162644652};
	geom::Vector3d	unit {0.6651669556972103, 0.7069150218815566, 0.24046636539587804};
	geom::Vector3d	unit2;

	SCTEST_CHECK_EQ(vec3.unitVector(), unit);
	SCTEST_CHECK_FALSE(vec3.isUnitVector());
	SCTEST_CHECK(unit.isUnitVector());
	SCTEST_CHECK(unit2.isUnitVector());

	return true;
}


static bool
Vector3DoubleTests_Angle()
{
	geom::Vector3d	a {0.3977933061361172, 8.053980094436525, 8.1287759943773};
	geom::Vector3d	b {9.817895298608196, 4.034166890407462, 4.37628316513266};
	geom::Vector3d	c {7.35, 0.221, 5.188};
	geom::Vector3d	d {2.751, 8.259, 3.985};

	SCTEST_CHECK_DEQ(a.angle(b), 0.9914540426033251);
	if (!scmp::WithinTolerance(c.angle(d), (double)1.052, (double)0.001)) {
		return false;
	}

	return true;
}


static bool
Vector3DoubleTests_ParallelOrthogonalVectors()
{
	geom::Vector3d	a {-2.029, 9.97, 4.172};
	geom::Vector3d	b {-9.231, -6.639, -7.245};
	geom::Vector3d	c {-2.328, -7.284, -1.214};
	geom::Vector3d	d {-1.821, 1.072, -2.94};
	geom::Vector3d	e {-2.0, 1.0, 3.0};
	geom::Vector3d	f {-6.0, 3.0, 9.0};
	geom::Vector3d	zeroVector {0.0, 0.0, 0.0};

	SCTEST_CHECK_FALSE(a.isParallel(b));
	SCTEST_CHECK_FALSE(a.isOrthogonal(b));

	SCTEST_CHECK_FALSE(c.isParallel(d));
	SCTEST_CHECK(c.isOrthogonal(d));

	SCTEST_CHECK(e.isParallel(f));
	SCTEST_CHECK_FALSE(e.isOrthogonal(f));

	SCTEST_CHECK(zeroVector.isZero());
	SCTEST_CHECK(c.isParallel(zeroVector));
	SCTEST_CHECK(c.isOrthogonal(zeroVector));

	return true;
}


static bool
Vector3DoubleTests_Projections()
{
	geom::Vector3d	a {4.866769214609107, 6.2356222686140566, 9.140878417029711};
	geom::Vector3d	b {6.135533104801077, 8.757851406697895, 0.6738031370548048};
	geom::Vector3d	c {4.843812341655318, 6.9140509888133055, 0.5319465962229454};
	geom::Vector3d	d {0.02295687295378901, -0.6784287201992489, 8.608931820806765};

	SCTEST_CHECK_EQ(a.projectParallel(b), c);
	SCTEST_CHECK_EQ(a.projectOrthogonal(b), d);

	return true;
}


static bool
Vector3DoubleTests_CrossProduct()
{
	geom::Vector3d	a {8.462, 7.893, -8.187};
	geom::Vector3d	b {6.984, -5.975, 4.778};
	geom::Vector3d	c {-11.2046, -97.6094, -105.685};

	c.setEpsilon(0.001); // double trouble
	SCTEST_CHECK_EQ(c, a.cross(b));

	return true;
}


int
main()
{
	SimpleSuite suite;
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
	auto result = suite.Run();

	std::cout << suite.GetReport() << "\n";
	return result ? 0 : 1;
}
