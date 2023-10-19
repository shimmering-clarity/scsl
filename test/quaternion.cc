#include <cmath>
#include <sstream>

#include <scmp/geom/Quaternion.h>

#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>


using namespace std;
using namespace scmp;
using namespace sctest;


static bool
Quaternion_SelfTest()
{
	geom::Quaternion_SelfTest();
	return true;
}


static bool
Quaterniond_Addition()
{
	geom::Quaterniond	p(geom::Vector4d {3.0, 1.0, -2.0, 1.0});
	geom::Quaterniond	q(geom::Vector4d {2.0, -1.0, 2.0, 3.0});
	geom::Quaterniond	expected(geom::Vector4d{5.0, 0.0, 0.0, 4.0});

	SCTEST_CHECK_EQ(p + q, expected);
	SCTEST_CHECK_EQ(expected - q, p);
	SCTEST_CHECK_NE(expected - q, q); // exercise !=

	return true;
}


static bool
Quaterniond_Conjugate()
{
	geom::Quaterniond	p {2.0, 3.0, 4.0, 5.0};
	geom::Quaterniond	q {2.0, -3.0, -4.0, -5.0};

	SCTEST_CHECK_EQ(p.conjugate(), q);

	return true;
}

static bool
Quaterniond_Euler()
{
	geom::Quaterniond	p = geom::quaterniond(geom::Vector3d{5.037992718099102, 6.212303632611285, 1.7056797335843106}, M_PI/4.0);
	geom::Quaterniond	q = geom::quaterniond_from_euler(p.euler());

	SCTEST_CHECK_EQ(p, q);
	
	return true;
}


static bool
Quaterniond_Identity()
{
	geom::Quaterniond	p {3.0, 1.0, -2.0, 1.0};
	geom::Quaterniond	q;

	SCTEST_CHECK(q.isIdentity());
	SCTEST_CHECK_EQ(p * q, p);
	
	return true;
}


static bool
Quaterniond_Inverse()
{
	geom::Quaterniond	p {2.0, 3.0, 4.0, 5.0};
	geom::Quaterniond	q {0.03704, -0.05556, -0.07407, -0.09259};

	SCTEST_CHECK_EQ(p.inverse(), q);

	return true;
}


static bool
Quaterniond_Norm()
{
	geom::Quaterniond	p {5.563199889674063, 0.9899139811480784, 9.387110042325054, 6.161341707794767};
	double 			norm = 12.57016663729933;

	SCTEST_CHECK_DEQ(p.norm(), norm);

	return true;
}


static bool
Quaterniond_Product()
{
	geom::Quaterniond	p {3.0, 1.0, -2.0, 1.0};
	geom::Quaterniond	q {2.0, -1.0, 2.0, 3.0};
	geom::Quaterniond	expected {8.0, -9.0, -2.0, 11.0};

	SCTEST_CHECK_EQ(p * q, expected);

	return true;
}


static bool
Quaterniond_Rotate()
{
	// This test aims to rotate a vector v using a quaternion.
	// c.f. https://math.stackexchange.com/questions/40164/how-do-you-rotate-a-vector-by-a-unit-quaternion
	// If we assume a standard IMU frame of reference following the
	// right hand rule:
	// + The x axis points toward magnetic north
	// + The y axis points toward magnentic west
	// + The z axis points toward the sky
	// Given a vector pointing due north, rotating by 90º about
	// the y-axis should leave us pointing toward the sky.

	geom::Vector3d		v {1.0, 0.0, 0.0};     // a vector pointed north
	geom::Vector3d		yAxis {0.0, 1.0, 0.0}; // a vector representing the y axis.
	double			angle = M_PI / 2;      // 90º rotation

	// A quaternion representing a 90º rotation about the y axis.
	geom::Quaterniond	p = geom::quaterniond(yAxis, angle);
	geom::Vector3d		vr {0.0, 0.0, 1.0};    // expected rotated vector.

	// A rotation quaternion should be a unit quaternion.
	SCTEST_CHECK(p.isUnitQuaternion());
	SCTEST_CHECK_EQ(p.rotate(v), vr);

	return true;
}


static bool
Quaterniond_ShortestSLERP()
{
	// Our starting point is an Orientation that is yawed 45° - our
	// Orientation is pointed π/4 radians in the X axis.
	geom::Quaterniond	p {0.92388, 0.382683, 0, 0};
	// Our ending point is an Orientation that is yawed -45° - or
	// pointed -π/4 radians in the X axis.
	geom::Quaterniond	q {0.92388, -0.382683, 0, 0};
	// The halfway point should be oriented midway about the X axis. It turns
	// out this is an identity quaternion.
	geom::Quaterniond	r;

	SCTEST_CHECK_EQ(geom::ShortestSLERP(p, q, 0.0), p);
	SCTEST_CHECK_EQ(geom::ShortestSLERP(p, q, 1.0), q);
	SCTEST_CHECK_EQ(geom::ShortestSLERP(p, q, 0.5), r);

	return true;
}


static bool
Quaterniond_ShortestSLERP2()
{
	// Start with an Orientation pointing forward, all Euler angles
	// set to 0.
	geom::Quaterniond	start {1.0, 0.0, 0.0, 0.0};
	// The goal is to end up face up, or 90º pitch (still facing forward).
	geom::Quaterniond	end {0.707107, 0, -0.707107, 0};
	// Halfway to the endpoint should be a 45º pitch.
	geom::Quaterniond	halfway {0.92388, 0, -0.382683, 0};
	// 2/3 of the way should be 60º pitch.
	geom::Quaterniond	twoThirds {0.866025, 0, -0.5, 0};

	SCTEST_CHECK_EQ(ShortestSLERP(start, end, 0.0), start);
	SCTEST_CHECK_EQ(ShortestSLERP(start, end, 1.0), end);
	SCTEST_CHECK_EQ(ShortestSLERP(start, end, 0.5), halfway);
	SCTEST_CHECK_EQ(ShortestSLERP(start, end, 2.0/3.0), twoThirds);

	return true;
}


static bool
Quaterniond_Unit()
{
	geom::Quaterniond	q {0.0, 0.5773502691896258, 0.5773502691896258, 0.5773502691896258};

	SCTEST_CHECK(q.isUnitQuaternion());

	return true;
}


static bool
Quaterniond_UtilityCreator()
{
	geom::Vector3d		v {1.0, 1.0, 1.0};
	double			w = M_PI;
	geom::Quaterniond	p = geom::quaterniond(v, w);
	geom::Quaterniond	q {0.0, 0.5773502691896258, 0.5773502691896258, 0.5773502691896258};

	SCTEST_CHECK_EQ(p, q);

	return true;
}


static bool
Quaternionf_Addition()
{
	geom::Quaternionf	p {3.0, 1.0, -2.0, 1.0};
	geom::Quaternionf	q {2.0, -1.0, 2.0, 3.0};
	geom::Quaternionf	expected {5.0, 0.0, 0.0, 4.0};

	SCTEST_CHECK_EQ(p + q, expected);
	SCTEST_CHECK_EQ(expected - q, p);
	SCTEST_CHECK_NE(expected - q, q); // exercise !=

	return true;
}


static bool
Quaternionf_Conjugate()
{
	geom::Quaternionf	p {2.0, 3.0, 4.0, 5.0};
	geom::Quaternionf	q {2.0, -3.0, -4.0, -5.0};

	SCTEST_CHECK_EQ(p.conjugate(), q);

	return true;
}


static bool
Quaternionf_Euler()
{
	geom::Quaternionf	p = geom::quaternionf(geom::Vector3f{5.037992718099102, 6.212303632611285, 1.7056797335843106}, M_PI/4.0);
	geom::Quaternionf	q = geom::quaternionf_from_euler(p.euler());

	SCTEST_CHECK_EQ(p, q);

	return true;
}


static bool
Quaternionf_Identity()
{
	geom::Quaternionf	p {1.0, 3.0, 1.0, -2.0};
	geom::Quaternionf	q;

	SCTEST_CHECK_EQ(p * q, p);

	return true;
}


static bool
Quaternionf_Inverse()
{
	geom::Quaternionf	p {2.0, 3.0, 4.0, 5.0};
	geom::Quaternionf	q {0.03704, -0.05556, -0.07407, -0.09259};

	SCTEST_CHECK_EQ(p.inverse(), q);
	
	return true;
}


static bool
Quaternionf_Norm()
{
	geom::Quaternionf	p {0.9899139811480784, 9.387110042325054, 6.161341707794767, 5.563199889674063};
	float 			norm = 12.57016663729933;

	SCTEST_CHECK_FEQ(p.norm(), norm);
	
	return true;
}


static bool
Quaternionf_Product()
{
	geom::Quaternionf	p {3.0, 1.0, -2.0, 1.0};
	geom::Quaternionf	q {2.0, -1.0, 2.0, 3.0};
	geom::Quaternionf	expected {8.0, -9.0, -2.0, 11.0};

	SCTEST_CHECK_EQ(p * q, expected);

	return true;
}


static bool
Quaternionf_Rotate()
{
	geom::Vector3f		v {1.0, 0.0, 0.0};
	geom::Vector3f		yAxis {0.0, 1.0, 0.0};
	float			angle = M_PI / 2;

	geom::Quaternionf	p = geom::quaternionf(yAxis, angle); 
	geom::Vector3f		vr {0.0, 0.0, 1.0};

	SCTEST_CHECK(p.isUnitQuaternion());
	SCTEST_CHECK_EQ(p.rotate(v), vr);

	return true;
}


static bool
Quaternionf_ShortestSLERP()
{
	// Our starting point is an Orientation that is yawed 45° - our
	// Orientation is pointed π/4 radians in the X axis.
	geom::Quaternionf	p {0.92388, 0.382683, 0, 0};
	// Our ending point is an Orientation that is yawed -45° - or
	// pointed -π/4 radians in the X axis.
	geom::Quaternionf	q {0.92388, -0.382683, 0, 0};
	// The halfway point should be oriented midway about the X axis. It turns
	// out this is an identity quaternion.
	geom::Quaternionf	r;

	SCTEST_CHECK_EQ(geom::ShortestSLERP(p, q, (float)0.0), p);
	SCTEST_CHECK_EQ(geom::ShortestSLERP(p, q, (float)1.0), q);
	SCTEST_CHECK_EQ(geom::ShortestSLERP(p, q, (float)0.5), r);

	return true;
}


static bool
Quaternionf_ShortestSLERP2()
{
	// Start with an Orientation pointing forward, all Euler angles
	// set to 0.
	geom::Quaternionf	start {1.0, 0.0, 0.0, 0.0};
	// The goal is to end up face up, or 90º pitch (still facing forward).
	geom::Quaternionf	end {0.707107, 0, -0.707107, 0};
	// Halfway to the endpoint should be a 45º pitch.
	geom::Quaternionf	halfway {0.92388, 0, -0.382683, 0};
	// 2/3 of the way should be 60º pitch.
	geom::Quaternionf	twoThirds {0.866025, 0, -0.5, 0};

	SCTEST_CHECK_EQ(ShortestSLERP(start, end, (float)0.0), start);
	SCTEST_CHECK_EQ(ShortestSLERP(start, end, (float)1.0), end);
	SCTEST_CHECK_EQ(ShortestSLERP(start, end, (float)0.5), halfway);
	SCTEST_CHECK_EQ(ShortestSLERP(start, end, (float)(2.0/3.0)), twoThirds);

	return true;
}


static bool
Quaternionf_Unit()
{
	geom::Quaternionf	q {0.0, 0.5773502691896258, 0.5773502691896258, 0.5773502691896258};

	SCTEST_CHECK(q.isUnitQuaternion());

	return true;
}


static bool
Quaternionf_UtilityCreator()
{
	geom::Vector3f		v {1.0, 1.0, 1.0};
	float			w = M_PI;
	geom::Quaternionf	p = geom::quaternionf(v, w);
	geom::Quaternionf	q {0.0, 0.5773502691896258, 0.5773502691896258, 0.5773502691896258};

	SCTEST_CHECK_EQ(p, q);

	return true;
}


static bool
QuaternionMiscellaneous_SanityChecks()
{
	geom::Vector4d		q {4.0, 1.0, 2.0, 3.0};
	geom::Vector3d		v {1.0, 2.0, 3.0};
	double 			w = 4.0;
	geom::Quaterniond	p(q);
	geom::Quaterniond	u = p.unitQuaternion();

	SCTEST_CHECK_EQ(p.axis(), v);
	SCTEST_CHECK_DEQ(p.angle(), w);
	SCTEST_CHECK(u.isUnitQuaternion());

	return true;
}


static bool
QuaternionMiscellaneous_OutputStream()
{
	geom::Quaternionf	p {4.0, 1.0, 2.0, 3.0};
	geom::Quaterniond	q {4.0, 1.0, 2.0, 3.0};
	stringstream		ss;

	ss << p;
	SCTEST_CHECK_EQ(ss.str(), "4 + <1, 2, 3>");
	ss.str("");

	ss << q;
	SCTEST_CHECK_EQ(ss.str(), "4 + <1, 2, 3>");

	return true;
}


static bool
QuaternionMiscellanous_InitializerConstructor()
{
	geom::Quaternionf	p {1.0, 1.0, 1.0, 1.0};
	geom::Quaternionf	q(geom::Vector4f {1.0, 1.0, 1.0, 1.0});

	SCTEST_CHECK_EQ(p, q);
	SCTEST_CHECK_FEQ(p.norm(), (float)2.0);

	return true;
}


int
main(void)
{
	SimpleSuite	ts;

	ts.AddTest("Quaternion_SelfTest", Quaternion_SelfTest);
	ts.AddTest("QuaternionMiscellanous_InitializerConstructor",
		QuaternionMiscellanous_InitializerConstructor);
	ts.AddTest("QuaternionMiscellaneous_SanityChecks",
		   QuaternionMiscellaneous_SanityChecks);
	ts.AddTest("QuaternionMiscellaneous_OutputStream",
		   QuaternionMiscellaneous_OutputStream);

	ts.AddTest("Quaterniond_Addition", Quaterniond_Addition);
	ts.AddTest("Quaterniond_Conjugate", Quaterniond_Conjugate);
	ts.AddTest("Quaterniond_Euler", Quaterniond_Euler);
	ts.AddTest("Quaterniond_Identity", Quaterniond_Identity);
	ts.AddTest("Quaterniond_Inverse", Quaterniond_Inverse);
	ts.AddTest("Quaterniond_Norm", Quaterniond_Norm);
	ts.AddTest("Quaterniond_Product", Quaterniond_Product);
	ts.AddTest("Quaterniond_Rotate", Quaterniond_Rotate);
	ts.AddTest("Quaterniond_ShortestSLERP", Quaterniond_ShortestSLERP);
	ts.AddTest("Quaterniond_ShortestSLERP2", Quaterniond_ShortestSLERP2);
	ts.AddTest("Quaterniond_Unit", Quaterniond_Unit);
	ts.AddTest("Quaterniond_UtilityCreator", Quaterniond_UtilityCreator);

	ts.AddTest("Quaternionf_Addition", Quaternionf_Addition);
	ts.AddTest("Quaternionf_Conjugate", Quaternionf_Conjugate);
	ts.AddTest("Quaternionf_Euler", Quaternionf_Euler);
	ts.AddTest("Quaternionf_Identity", Quaternionf_Identity);
	ts.AddTest("Quaternionf_Inverse", Quaternionf_Inverse);
	ts.AddTest("Quaternionf_Norm", Quaternionf_Norm);
	ts.AddTest("Quaternionf_Product", Quaternionf_Product);
	ts.AddTest("Quaternionf_Rotate", Quaternionf_Rotate);
	ts.AddTest("Quaternionf_ShortestSLERP", Quaternionf_ShortestSLERP);
	ts.AddTest("Quaternionf_ShortestSLERP2", Quaternionf_ShortestSLERP2);
	ts.AddTest("Quaternionf_Unit", Quaternionf_Unit);
	ts.AddTest("Quaternionf_UtilityCreator", Quaternionf_UtilityCreator);

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
