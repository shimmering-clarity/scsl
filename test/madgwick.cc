#include <cmath>
#include <sstream>

#include <scmp/geom/Vector.h>
#include <scmp/geom/Quaternion.h>
#include <scmp/Math.h>
#include <scmp/filter/Madgwick.h>

#include <sctest/Assert.h>
#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>

using namespace std;
using namespace scmp;


bool
SimpleAngularOrientation()
{
	filter::Madgwickd mf;
	geom::Vector3d    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	geom::Quaterniond frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	double            delta         = 0.00917; // assume 109 updates per second, as per the paper.
	double            twentyDegrees = scmp::DegreesToRadiansD(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mf.UpdateAngularOrientation(gyro, delta);
	}

	SCTEST_CHECK_EQ(mf.Orientation(), frame20Deg);

	auto euler = mf.Euler();
	SCTEST_CHECK_DEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


int
main(int argc, char **argv)
{
	sctest::SimpleSuite	suite;

	suite.AddTest("SimpleAngularOrientation", SimpleAngularOrientation);
	auto result = suite.Run();

	if (suite.IsReportReady()) {
		auto report = suite.GetReport();
		std::cout << report.Failing << " / " << report.Total;
		std::cout << " tests failed.\n";
	}

	if (result) {
		return 0;
	}
	else {
		return 1;
	}
}
