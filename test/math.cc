///
/// \file test/math.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-20
/// \brief Unit tests for math functions.
///
/// Arena defines a memory management backend for pre-allocating memory.
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
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

#include <iostream>

#include <scsl/Flags.h>
#include <scmp/Math.h>
#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>


namespace {


bool
BestDie()
{
	// Theoretically, this could fail. The odds of that happening
	// with 100 die and a proper RNG is 0.99999998792533,
	// practically 100%. At 1000 die, it's virtually guaranteed.
	auto	n = scmp::BestDie(1000, 6, 1);

	SCTEST_CHECK_EQ(n, 6);

	return true;
}


bool
DieTotal()
{
	auto	n = scmp::DieTotal(100, 6);

	SCTEST_CHECK_GEQ(n, 100);
	SCTEST_CHECK_LEQ(n, 600);

	return true;
}


bool
WithinToleranceFloat()
{
	float x = 0.1235;
	float y = 0.1236;
	float eps = 0.0;
	float expected = 0.1234;

	scmp::DefaultEpsilon(eps);

	SCTEST_CHECK_FEQ_EPS(x, expected, eps);
	SCTEST_CHECK_FNE_EPS(y, expected, eps);

	return true;
}


bool
WithinToleranceDouble()
{
	double x = 0.12348;
	double y = 0.1236;
	double eps = 0.0;
	double expected = 0.12345;

	scmp::DefaultEpsilon(eps);

	SCTEST_CHECK_DEQ_EPS(x, expected, eps);
	SCTEST_CHECK_DNE_EPS(y, expected, eps);

	return true;
}


bool
RotateRadians()
{
	double theta0 = 0.0;
	double theta1 = scmp::PI_D;

	auto rotated = scmp::RotateRadians(theta0, theta1);
	SCTEST_CHECK_DEQ(rotated, theta1);

	rotated = scmp::RotateRadians(rotated, theta1);
	SCTEST_CHECK_DEQ(rotated, theta0);

	theta1 = scmp::PI_D * 3 / 2;
	rotated = scmp::RotateRadians(theta0, theta1);
	SCTEST_CHECK_DEQ(rotated, -scmp::PI_D / 2);

	rotated = scmp::RotateRadians(rotated, theta1);
	SCTEST_CHECK_DEQ(rotated, scmp::PI_D);

	return true;
}


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

	sctest::SimpleSuite suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	suite.AddTest("BestDie", BestDie);
	suite.AddTest("DieTotal", DieTotal);
	suite.AddTest("WithinToleranceFloat", WithinToleranceFloat);
	suite.AddTest("WithinToleranceDouble", WithinToleranceDouble);
	suite.AddTest("RotateRadians", RotateRadians);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
