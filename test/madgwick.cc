#include <cmath>
#include <sstream>

#include <scsl/Flags.h>

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
	filter::Madgwickf       mflt;
	const geom::Vector3F    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
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
SimpleAngularOrientationFloatDefaultDT()
{
	filter::Madgwickf       mflt;
	const geom::Vector3F    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaternionf frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const float             delta         = 0.00917; // assume 109 updates per second, as per the paper.
	const float             twentyDegrees = scmp::DegreesToRadiansF(20.0);

	mflt.DeltaT(delta);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro);
	}

	SCTEST_CHECK_EQ(mflt.Orientation(), frame20Deg);

	auto euler = mflt.Euler();
	SCTEST_CHECK_FEQ_EPS(euler[0], twentyDegrees, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[1], 0.0, 0.01);
	SCTEST_CHECK_FEQ_EPS(euler[2], 0.0, 0.01);

	return true;
}


bool
VerifyUpdateWithZeroDeltaTFails()
{
	filter::Madgwickf       mflt;
	const geom::Vector3F    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
	const geom::Quaternionf frame20Deg{0.984808, 0.173648, 0, 0}; // 20° final Orientation.
	const float             twentyDegrees = scmp::DegreesToRadiansF(20.0);

	// The paper specifies a minimum of 109 IMU readings to stabilize; for
	// two seconds, that means 218 updates.
	for (int i = 0; i < 218; i++) {
		mflt.UpdateAngularOrientation(gyro);
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
	filter::Madgwickd       mflt;
	const geom::Vector3D    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
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
	const geom::Vector3F    initialFrame{0, 0, 0};
	filter::Madgwickf       mflt(initialFrame);
	const geom::Vector3F    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
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
	const auto              initialFrame = geom::FloatQuaternionFromEuler({0, 0, 0});
	filter::Madgwickf       mflt(initialFrame);
	const geom::Vector3F    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
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
	const geom::Vector3D    initialFrame{0, 0, 0};
	filter::Madgwickd       mflt(initialFrame);
	const geom::Vector3D    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
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
	const auto              initialFrame = geom::DoubleQuaternionFromEuler({0, 0, 0});
	filter::Madgwickd       mflt(initialFrame);
	const geom::Vector3D    gyro{0.174533, 0.0, 0.0}; // 10° X rotation.
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
	auto quiet = false;
	auto noReport = false;
	auto flags = new scsl::Flags("test_madgwick",
				     "This test validates the Madgwick filter code");
	flags->Register("-n", false, "don't print the report");
	flags->Register("-q", false, "suppress test output");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
	}

	sctest::SimpleSuite	suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	suite.AddTest("SimpleAngularOrientationFloat",
		      SimpleAngularOrientationFloat);
	suite.AddTest("SimpleAngularOrientationFloatDefaultDT",
			SimpleAngularOrientationFloatDefaultDT);
	suite.AddFailingTest("VerifyUpdateWithZeroDeltaTFails",
		      VerifyUpdateWithZeroDeltaTFails);
	suite.AddTest("SimpleAngularOrientationDouble",
		      SimpleAngularOrientationDouble);
	suite.AddTest("SimpleAngularOrientationFloat (inital vector3f)",
		      SimpleAngularOrientation2InitialVector3f);
	suite.AddTest("SimpleAngularOrientationDouble (inital vector3d)",
		      SimpleAngularOrientation2InitialVector3d);
	suite.AddTest("SimpleAngularOrientationFloat (inital MakeQuaternion)",
		      SimpleAngularOrientation2InitialQuaternionf);
	suite.AddTest("SimpleAngularOrientationDouble (inital MakeQuaternion)",
		      SimpleAngularOrientation2InitialQuaterniond);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
