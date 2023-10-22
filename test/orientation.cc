#include <scsl/Flags.h>

#include <scmp/Math.h>
#include <scmp/geom/Vector.h>
#include <scmp/geom/Orientation.h>

#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>


using namespace std;
using namespace scmp;
using namespace sctest;


namespace {


bool
UnitConversions_RadiansToDegreesF()
{
	for (int i = 0; i < 360; i++) {
		auto rads = scmp::DegreesToRadiansF(i);
		auto deg = scmp::RadiansToDegreesF(rads);

		SCTEST_CHECK_FEQ(deg, static_cast<float>(i));
	}

	return true;
}


bool
UnitConversions_RadiansToDegreesD()
{
	for (int i = 0; i < 360; i++) {
		auto deg = static_cast<double>(i);
		SCTEST_CHECK_DEQ(scmp::RadiansToDegreesD(scmp::DegreesToRadiansD(deg)), deg);
	}

	return true;
}


bool
Orientation2f_Heading()
{
	geom::Vector2F const a{2.0, 2.0};

	SCTEST_CHECK_FEQ(geom::Heading2F(a), scmp::DegreesToRadiansF(45));

	return true;
}


bool
Orientation3f_Heading()
{
	geom::Vector3F const a{2.0, 2.0, 2.0};

	SCTEST_CHECK_FEQ(geom::Heading3F(a), scmp::DegreesToRadiansF(45));

	return true;
}


bool
Orientation2d_Heading()
{
	geom::Vector2D const a{2.0, 2.0};

	return scmp::WithinTolerance(geom::Heading2D(a), scmp::DegreesToRadiansD(45), 0.000001) != 0.0;
}


bool
Orientation3d_Heading()
{
	geom::Vector3D const a{2.0, 2.0, 2.0};

	return scmp::WithinTolerance(geom::Heading3D(a), scmp::DegreesToRadiansD(45), 0.000001) != 0.0;
}


} // anonymous namespace


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet = false;
	auto *flags = new scsl::Flags("test_orientation",
				     "This test validates various orientation-related components in scmp.");
	flags->Register("-n", false, "don't print the report");
	flags->Register("-q", false, "suppress test output");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
		exit(1);
	}

	SimpleSuite suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	suite.AddTest("UnitConversions_RadiansToDegreesF", UnitConversions_RadiansToDegreesF);
	suite.AddTest("UnitConversions_RadiansToDegreesD", UnitConversions_RadiansToDegreesD);
	suite.AddTest("Orientation2f_Heading", Orientation2f_Heading);
	suite.AddTest("Orientation3f_Heading", Orientation3f_Heading);
	suite.AddTest("Orientation2d_Heading", Orientation2d_Heading);
	suite.AddTest("Orientation3d_Heading", Orientation3d_Heading);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
