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
SimpleAngularOrientationFloat()
{
	filter::Madgwickf	mflt;
	const geom::Vector3f    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaternionf frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const float             delta         = 0.00917; // assume 109 updates per second, as per the paper.
	const float             twentyDegrees = scmp::DegreesToRadiansF(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro, delta);
	}

	SCTEST_CHECK_EQ(mflt.Orientation(), frame20Deg);

	auto euler = mflt.Euler();
	SCTEST_CHECK_FEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


bool
SimpleAngularOrientationDouble()
{
	filter::Madgwickd	mflt;
	const geom::Vector3d    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaterniond frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const double            delta         = 0.00917; // assume 109 updates per second, as per the paper.
	const double            twentyDegrees = scmp::DegreesToRadiansD(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro, delta);
	}

	SCTEST_CHECK_EQ(mflt.Orientation(), frame20Deg);

	auto euler = mflt.Euler();
	SCTEST_CHECK_DEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


bool
SimpleAngularOrientation2InitialVector3f()
{
	const geom::Vector3f    initialFrame{0, 0, 0};
	filter::Madgwickf       mflt(initialFrame);
	const geom::Vector3f    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaternionf frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const float             delta         = 0.00917; // assume 109 updates per second, as per the paper.
	const float             twentyDegrees = scmp::DegreesToRadiansF(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro, delta);
	}

	SCTEST_CHECK_EQ(mflt.Orientation(), frame20Deg);

	auto euler = mflt.Euler();
	SCTEST_CHECK_FEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


bool
SimpleAngularOrientation2InitialQuaternionf()
{
	const auto              initialFrame = geom::quaternionf_from_euler({0, 0, 0});
	filter::Madgwickf       mflt(initialFrame);
	const geom::Vector3f    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaternionf frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const float             delta         = 0.00917; // assume 109 updates per second, as per the paper.
	const float             twentyDegrees = scmp::DegreesToRadiansF(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro, delta);
	}

	SCTEST_CHECK_EQ(mflt.Orientation(), frame20Deg);

	auto euler = mflt.Euler();
	SCTEST_CHECK_FEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


bool
SimpleAngularOrientation2InitialVector3d()
{
	const geom::Vector3d    initialFrame{0, 0, 0};
	filter::Madgwickd       mflt(initialFrame);
	const geom::Vector3d    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaterniond frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const double            delta         = 0.00917; // assume 109 updates per second, as per the paper.
	const double            twentyDegrees = scmp::DegreesToRadiansD(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro, delta);
	}

	SCTEST_CHECK_EQ(mflt.Orientation(), frame20Deg);

	auto euler = mflt.Euler();
	SCTEST_CHECK_DEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


bool
SimpleAngularOrientation2InitialQuaterniond()
{
	const auto              initialFrame = geom::quaterniond_from_euler({0, 0, 0});
	filter::Madgwickd	mflt(initialFrame);
	const geom::Vector3d    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaterniond frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const double            delta         = 0.00917; // assume 109 updates per second, as per the paper.
	const double            twentyDegrees = scmp::DegreesToRadiansD(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro, delta);
	}

	SCTEST_CHECK_EQ(mflt.Orientation(), frame20Deg);

	auto euler = mflt.Euler();
	SCTEST_CHECK_DEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_DEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	sctest::SimpleSuite	suite;

	suite.AddTest("SimpleAngularOrientationDouble",
		      SimpleAngularOrientationFloat);
	suite.AddTest("SimpleAngularOrientationDouble",
		      SimpleAngularOrientationDouble);
	suite.AddTest("SimpleAngularOrientationDouble (iniital vector3f)",
		      SimpleAngularOrientation2InitialVector3f);
	suite.AddTest("SimpleAngularOrientationDouble (iniital vector3d)",
		      SimpleAngularOrientation2InitialVector3d);
	suite.AddTest("SimpleAngularOrientationDouble (iniital quaternionf)",
		      SimpleAngularOrientation2InitialQuaternionf);
	suite.AddTest("SimpleAngularOrientationDouble (iniital quaterniond)",
		      SimpleAngularOrientation2InitialQuaterniond);

	auto result = suite.Run();

	std::cout << suite.GetReport() << "\n";
	return result ? 0 : 1;

}
