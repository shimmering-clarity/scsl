#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>

#include <scmp/Math.h>
#include <scmp/geom/Vector.h>
#include <scmp/geom/Orientation.h>

using namespace std;
using namespace scmp;
using namespace sctest;


static bool
UnitConversions_RadiansToDegreesF()
{
	for (int i = 0; i < 360; i++) {
		auto	deg = static_cast<float>(i);
		SCTEST_CHECK_FEQ(scmp::RadiansToDegreesF(scmp::DegreesToRadiansF(deg)), deg);
	}

	return true;
}


static bool
UnitConversions_RadiansToDegreesD()
{
	for (int i = 0; i < 360; i++) {
		auto	deg = static_cast<double>(i);
		SCTEST_CHECK_DEQ(scmp::RadiansToDegreesD(scmp::DegreesToRadiansD(deg)), deg);
	}

	return true;
}


static bool
Orientation2f_Heading()
{
	geom::Vector2f	a {2.0, 2.0};

	SCTEST_CHECK_FEQ(geom::Heading2f(a), scmp::DegreesToRadiansF(45));

	return true;
}


static bool
Orientation3f_Heading()
{
	geom::Vector3f	a {2.0, 2.0, 2.0};

	SCTEST_CHECK_FEQ(geom::Heading3f(a), scmp::DegreesToRadiansF(45));

	return true;
}


static bool
Orientation2d_Heading()
{
	geom::Vector2d	a {2.0, 2.0};

	 return scmp::WithinTolerance(geom::Heading2d(a), scmp::DegreesToRadiansD(45), 0.000001);
}


static bool
Orientation3d_Heading()
{
	geom::Vector3d	a {2.0, 2.0, 2.0};

	return scmp::WithinTolerance(geom::Heading3d(a), scmp::DegreesToRadiansD(45), 0.000001);
}


int
main(void)
{
	SimpleSuite	ts;

	ts.AddTest("UnitConversions_RadiansToDegreesF", UnitConversions_RadiansToDegreesF);
	ts.AddTest("UnitConversions_RadiansToDegreesD", UnitConversions_RadiansToDegreesD);
	ts.AddTest("Orientation2f_Heading", Orientation2f_Heading);
	ts.AddTest("Orientation3f_Heading", Orientation3f_Heading);
	ts.AddTest("Orientation2d_Heading", Orientation2d_Heading);
	ts.AddTest("Orientation3d_Heading", Orientation3d_Heading);

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
